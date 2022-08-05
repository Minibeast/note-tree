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
    
    grid->addWidget(stack, 0, 0);
    grid->addWidget(textField, 1, 0);

    grid->setRowStretch(0, 3);
    grid->setRowStretch(1, 1);

    setLayout(grid);
}

QString NoteGrid::getTextFieldContents() {
    return textField->toPlainText();
}

void NoteGrid::addItemToList(QString text) {
    text = text.trimmed();
    if (text.isEmpty()) { return; }
    isDirty = true;
    notetree->markSaved();
    auto *item = new QListWidgetItem(text);
    stack->addItem(item);
    notetree->updateStatusBar();
}

void NoteGrid::clearTextFieldContents() {
    textField->clear();
}

void NoteGrid::unselectList() {
    stack->selectionModel()->clear();
}

void NoteGrid::increaseFontSize() {
    stackFontSize += 2;
    if (stackFontSize >= 100) { stackFontSize -= 2; return; }
    notetree->settings->setValue("view/stackFont", stackFontSize);
    stack->setStyleSheet(style.arg(QString::number(stackFontSize), QString::number(paddingSize)));
}

void NoteGrid::decreaseFontSize() {
    stackFontSize -= 2;
    if (stackFontSize <= 0) { stackFontSize += 2; return; }
    notetree->settings->setValue("view/stackFont", stackFontSize);
    stack->setStyleSheet(style.arg(QString::number(stackFontSize), QString::number(paddingSize)));
}

void NoteGrid::resetZoom() {
    stackFontSize = 12;
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
        clipboard->setText(item->text());
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
        isDirty = true;
        notetree->markSaved();
        delete item; // Qt documentation warnings you to destroy item to effectively remove it from QListWidget.
    }
    notetree->updateStatusBar();
}

void NoteGrid::clearList() {
    stack->clear();
}

QList<QString> NoteGrid::getList() {
    QList<QString> list;
    for (int i = 0; i < stack->count(); i++) {
        list.append(stack->item(i)->text());
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
