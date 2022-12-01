#include "notegrid.h"
#include "notetree.h"

NoteGrid::NoteGrid(QWidget *parent) : QWidget(parent) {
    style = "QListWidget { background-color: transparent; font-size: %1px; } QListWidget::item { padding-bottom: %2px; }";
    notetree = reinterpret_cast<NoteTree*>(parent);
    textField = new TextWidget(this);
    auto *grid = new QGridLayout(this);
    stack = new QListWidget(this);
    stack->setMovement(QListView::Free);
    stack->setWordWrap(true);
    stack->setDragDropMode(QAbstractItemView::InternalMove);
    stackFontSize = notetree->settings->value("view/stackFont", 12).toInt();
    paddingSize = notetree->settings->value("view/paddingSize", 2).toInt();
    stack->setStyleSheet(style.arg(QString::number(stackFontSize), QString::number(paddingSize)));

    isDirty = false;
    listIndex = -1;
    
    grid->addWidget(stack, 0, 0);
    grid->addWidget(textField, 1, 0);

    grid->setRowStretch(0, 3);
    grid->setRowStretch(1, 1);

    setLayout(grid);
    notetree->connect(stack, &QListWidget::itemDoubleClicked, this, &NoteGrid::itemDoubleClicked);
    notetree->connect(stack->model(), &QAbstractItemModel::rowsMoved, this, &NoteGrid::checkChanges);
}

QString NoteGrid::findURL(QString item) {
    QStringList stringList = item.split(" ");
    // Regex is dumb.
    for (int i = 0; i < stringList.length(); i++) {
        QString stringItem = stringList[i];

        // Handling for spaces in file/folder names. Spaces need to be escaped due to link checking with split().
        // What we've learned today kids is to never use spaces in anything!
        while (stringItem.endsWith("\\") && i < stringList.length() - 1) {
            stringItem.chop(1);
            stringItem += " " + stringList[i+1];
            i++;
        }

        if (stringItem.startsWith("https://") || stringItem.startsWith("ftp://") || stringItem.startsWith("http://")
        || stringItem.startsWith("mailto:") || stringItem.startsWith("smb://") || stringItem.startsWith("file://")) {
            return stringItem;
        } else if (stringItem.startsWith("<") && stringItem.endsWith(">")) {
            QString filePath = notetree->getRelativePath();
            filePath += stringItem.mid(1, stringItem.length() - 2);
            filePath = "file://" + filePath;
            return filePath;
        }
    }
    return "";
}

void NoteGrid::itemDoubleClicked() {
    auto items = stack->selectedItems();
    if (items.length() > 0) {
        auto item = items[0];
        int itemIndex = getItemFromStack(item);
        if (itemIndex != -1) { // This should NEVER fail, but if it does...
            QString text = convertItemToPlainText(item);
            QString url = findURL(text);
            if (!url.isEmpty()) QDesktopServices::openUrl(url);
            else editItem();
        }
    }
}

void NoteGrid::checkChanges() {
    isDirty = savedFile != getList();
    notetree->autosaveFile();
    notetree->markSaved();
}

void NoteGrid::setSavedFile() {
    savedFile = getList();
    isDirty = false;
    notetree->markSaved();
}

QString NoteGrid::getTextFieldContents() {
    return textField->toPlainText();
}

void NoteGrid::addItemToList(QString text) {
    text = text.trimmed();
    text = unicodeFormatting(text);
    if (text.isEmpty()) {
        if (textField->getEditStyle()) {
            listIndex = -1;
            textField->setEditStyle(false);
            cachedText = nullptr;
            clearTextFieldContents();
        }
        return;
    }
    auto *item = new QListWidgetItem(text);

    QFont font = item->font();
    if (text.startsWith("# ")) {
        font.setWeight(QFont::Bold);
        font.setPointSize(13 + (stackFontSize / 2) + 10);
        item->setFont(font);
        item->setText(text.mid(2));
    } else if (text.startsWith("## ")) {
        font.setItalic(true);
        font.setPointSize(13 + (stackFontSize / 2) + 5);
        item->setFont(font);
        item->setText(text.mid(3));
    } else if (text.startsWith("### ")) {
        font.setWeight(QFont::Bold);
        item->setFont(font);
        item->setText(text.mid(4));
    } else if (text.startsWith("#### ")) {
        font.setItalic(true);
        item->setFont(font);
        item->setText(text.mid(5));
    }

    if (listIndex != -1) {
        stack->insertItem(listIndex, item);
        listIndex = -1;
        textField->setEditStyle(false);
        cachedText = nullptr;
    }
    else
        stack->addItem(item);
    checkChanges();
    stack->scrollToItem(item);
    notetree->updateStatusBar();
}

HeaderType NoteGrid::getHeaderTypeFromItem(QListWidgetItem *item) {
    QFont font = item->font();
    if (font.weight() == QFont::Bold && font.pointSize() != 13) { return HeaderType::h1; }
    else if (font.italic() && font.pointSize() != 13) { return HeaderType::h2; }
    else if (font.weight() == QFont::Bold) { return HeaderType::h3; }
    else if (font.italic()) { return HeaderType::h4; }
    else { return HeaderType::None; }
}

QString NoteGrid::convertItemToPlainText(QListWidgetItem *item) {
    QString result = item->text();
    result = result.replace("→", "->");
    result = result.replace("✓", ":check:");
    result = result.replace("--------------", "_line_");
    HeaderType header = getHeaderTypeFromItem(item);
    if (header == HeaderType::h1) { return "# " + result; }
    else if (header == HeaderType::h2) { return "## " + result; }
    else if (header == HeaderType::h3) { return "### " + result; }
    else if (header == HeaderType::h4) { return "#### " + result; }
    else { return result; }
}

QString NoteGrid::unicodeFormatting(QString item) {
    QString result;
    result = item.replace("->", "→");
    result = item.replace(":check:", "✓");
    result = item.replace("_line_", "--------------");
    return result;
}

void NoteGrid::clearTextFieldContents() {
    textField->clear();
}

void NoteGrid::unselectList() {
    stack->selectionModel()->clear();
}

void NoteGrid::updateHeaderFontSize() {
    for (int i = 0; i < stack->count(); i++) {
        HeaderType header = getHeaderTypeFromItem(stack->item(i));
        QFont font = stack->item(i)->font();
        if (header == HeaderType::h1) {
            font.setPointSize(13 + (stackFontSize / 2) + 10);
            stack->item(i)->setFont(font);
        } else if (header == HeaderType::h2) {
            font.setPointSize(13 + (stackFontSize / 2) + 5);
            stack->item(i)->setFont(font);
        }
    }
}

void NoteGrid::increaseFontSize() {
    stackFontSize += 2;
    if (stackFontSize >= 100) { stackFontSize -= 2; return; }
    updateHeaderFontSize();
    notetree->settings->setValue("view/stackFont", stackFontSize);
    stack->setStyleSheet(style.arg(QString::number(stackFontSize), QString::number(paddingSize)));
}

void NoteGrid::decreaseFontSize() {
    stackFontSize -= 2;
    if (stackFontSize <= 0) { stackFontSize += 2; return; }
    updateHeaderFontSize();
    notetree->settings->setValue("view/stackFont", stackFontSize);
    stack->setStyleSheet(style.arg(QString::number(stackFontSize), QString::number(paddingSize)));
}

void NoteGrid::resetZoom() {
    stackFontSize = 12;
    updateHeaderFontSize();
    notetree->settings->setValue("view/stackFont", stackFontSize);
    stack->setStyleSheet(style.arg(QString::number(stackFontSize), QString::number(paddingSize)));
}

void NoteGrid::increasePaddingSize() {
    paddingSize += 2;
    if (paddingSize >= 50) { paddingSize -= 2; return; }
    notetree->settings->setValue("view/paddingSize", paddingSize);
    stack->setStyleSheet(style.arg(QString::number(stackFontSize), QString::number(paddingSize)));
}

void NoteGrid::decreasePaddingSize() {
    paddingSize -= 2;
    if (paddingSize <= 0) { paddingSize += 2; return; }
    notetree->settings->setValue("view/paddingSize", paddingSize);
    stack->setStyleSheet(style.arg(QString::number(stackFontSize), QString::number(paddingSize)));
}

void NoteGrid::resetPadding() {
    paddingSize = 2;
    notetree->settings->setValue("view/paddingSize", paddingSize);
    stack->setStyleSheet(style.arg(QString::number(stackFontSize), QString::number(paddingSize)));
}

void NoteGrid::copyItem() {
    auto items = stack->selectedItems();
    auto clipboard = QGuiApplication::clipboard();
    foreach(QListWidgetItem* item, items){
        clipboard->setText(convertItemToPlainText(item));
    }
}

void NoteGrid::cutItem() {
    copyItem();
    deleteItem();
}

void NoteGrid::pasteItem() {
    auto clipboard = QGuiApplication::clipboard();
    if (textField->hasFocus())
        textField->insertPlainText(clipboard->text());
    else
        addItemToList(clipboard->text());

}

void NoteGrid::deleteItem() {
    auto items = stack->selectedItems();
    foreach(QListWidgetItem* item, items){
        stack->removeItemWidget(item);
        delete item; // Qt documentation warnings you to destroy item to effectively remove it from QListWidget.
    }
    checkChanges();
    notetree->updateStatusBar();
}

void NoteGrid::clearList() {
    stack->clear();
}

QList<QString> NoteGrid::getList() {
    QList<QString> list;
    for (int i = 0; i < stack->count(); i++) {
        list.append(convertItemToPlainText(stack->item(i)));
    }
    return list;
}

int NoteGrid::getListCount() {
    return stack->count();
}

void NoteGrid::addTextToList() {
    auto text = this->getTextFieldContents();
    this->addItemToList(text);
    this->clearTextFieldContents();
}

int NoteGrid::getItemFromStack(QListWidgetItem *item) {
    for (int i = 0; i < stack->count(); i++)
        if (stack->item(i) == item)
            return i;
    return -1;
}

void NoteGrid::editItem() {
    if (textField->getEditStyle() && !cachedText.isNull())
        addItemToList(cachedText);
    auto items = stack->selectedItems();
    if (items.length() > 0) {
        auto item = items[0];
        listIndex = getItemFromStack(item);
        if (listIndex != -1) { // This should NEVER fail, but if it does...
            originalText = getTextFieldContents();
            textField->setText(convertItemToPlainText(item));
            cachedText = convertItemToPlainText(item);
            deleteItem();
            textField->setEditStyle(true);
        }
    }
}

void NoteGrid::exitEdit() {
    if (!cachedText.isNull())
        addItemToList(cachedText);
    clearTextFieldContents();
    if (!originalText.isEmpty()) {
        textField->setText(originalText);
        originalText = nullptr;
    }
    textField->setEditStyle(false);
}
