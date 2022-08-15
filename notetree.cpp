#include "notetree.h"

#define _STR(x) #x
#define STRINGIFY(x)  _STR(x)

NoteTree::NoteTree(QWidget *parent) : QMainWindow(parent) {
    settings = new QSettings("Mini's Applications", "Note Tree");
    this->restoreGeometry(settings->value("view/geometry").toByteArray());
    notegrid = new NoteGrid(this);


    aboutwindow.setText("About Note Tree");
    aboutwindow.setInformativeText("Git Commit: " + QString(STRINGIFY(GIT_VERSION)) + "\nOS: " + QSysInfo::prettyProductName() + " (" + QSysInfo::currentCpuArchitecture() + ")\n\nMade by Mini / Amy");
    fileNotFound.setText("The file that was attempted to open does not exist.");
    fileNotFound.setIcon(QMessageBox::Critical);
    saveChanges.setText("The document has been modified.");
    saveChanges.setInformativeText("Do you want to save your changes?");
    saveChanges.setStandardButtons(QMessageBox::Save | QMessageBox::Discard | QMessageBox::Cancel);
    saveChanges.setDefaultButton(QMessageBox::Save);
    saveChanges.setIcon(QMessageBox::Warning);
    notTextFile.setText("Something went wrong with reading/writing to the file.");
    notTextFile.setIcon(QMessageBox::Critical);

    auto *quit = new QAction("&Quit", this);
    quit->setShortcut(QKeySequence(QKeySequence::Quit));
    auto *new_file = new QAction("&Close File", this);
    new_file->setShortcut(QKeySequence(QKeySequence::Close));
    auto *open_file = new QAction("&Open", this);
    open_file->setShortcut(QKeySequence(QKeySequence::Open));
    auto *save_file = new QAction("&Save", this);
    save_file->setShortcut(QKeySequence(QKeySequence::Save));
    auto *save_file_as = new QAction("Save &As", this);
    save_file_as->setShortcut(QKeySequence(QKeySequence::SaveAs));
    auto *new_line = new QAction("New &Line", this);
    //new_line->setShortcut(QKeySequence("Return"));
    auto *copy = new QAction("&Copy", this);
    copy->setShortcut(QKeySequence(QKeySequence::Copy));
    auto *cut = new QAction("C&ut", this);
    cut->setShortcut(QKeySequence(QKeySequence::Cut));
    auto *paste = new QAction("&Paste", this);
    paste->setShortcut(QKeySequence(QKeySequence::Paste));
    auto *delete_item = new QAction("&Delete", this);
    delete_item->setShortcut(QKeySequence(QKeySequence::Delete));
    auto *about_item = new QAction("&About", this);
    about_item->setShortcut(QKeySequence(QKeySequence::HelpContents));
    auto *zoom_in = new QAction("Zoom &In", this);
    zoom_in->setShortcut(QKeySequence("Ctrl+="));
    auto *zoom_out = new QAction("Zoom &Out", this);
    zoom_out->setShortcut(QKeySequence(QKeySequence::ZoomOut));
    auto *reset_zoom = new QAction("Reset Zoom", this);
    reset_zoom->setShortcut(QKeySequence("Ctrl+0"));
    view_statusbar = new QAction("View &Status Bar", this);
    view_statusbar->setShortcut(QKeySequence("Ctrl+/"));
    view_statusbar->setCheckable(true);
    view_statusbar->setChecked(settings->value("view/statusbar", true).toBool());
    statusbar_showcount = new QAction("Show Item Counter", this);
    statusbar_showcount->setCheckable(true);
    statusbar_showcount->setChecked(settings->value("view/statusbar_showcount", true).toBool());
    statusbar_showpath = new QAction("Show File Path", this);
    statusbar_showpath->setCheckable(true);
    statusbar_showpath->setChecked(settings->value("view/statusbar_showpath", true).toBool());
    toggleStatusBar();
    auto *open_file_location = new QAction("Open File Location", this);
    open_file_location->setShortcut(QKeySequence("Ctrl+Shift+O"));
    auto *new_window = new QAction("New Window", this);
    new_window->setShortcut(QKeySequence(QKeySequence::New));
    add_to_favorites = new QAction("Add to Favorites");
    add_to_favorites->setShortcut(QKeySequence("Ctrl+Shift+F"));
    auto *padding_in = new QAction("Increase Text Padding");
    auto *padding_out = new QAction("Decrease Text Padding");
    auto *padding_reset = new QAction("Reset Text Padding");
    auto *edit_item = new QAction("Edit Item");
    edit_item->setShortcut(QKeySequence("Ctrl+E"));
    auto *set_edit_color = new QAction("Set Edit Color");
    always_on_top_cbox = new QAction("Always on Top");
    always_on_top_cbox->setCheckable(true);
    always_on_top_cbox->setChecked(settings->value("view/alwaysontop", false).toBool());
    toggleAlwaysOnTop();

    filePath = "";
    this->updateWindowTitle();

    QMenu *file = menuBar()->addMenu("&File");
    QMenu *edit = menuBar()->addMenu("&Edit");
    QMenu *view = menuBar()->addMenu("&View");
    favorites_menu = menuBar()->addMenu("&Favorites");
    QMenu *help = menuBar()->addMenu("&Help");

    file->addAction(new_window);
    file->addAction(open_file);
    file->addAction(open_file_location);
    recentItemsGroup = file->addMenu("Recent Items");
    recentItems = settings->value("files/recentItems").toStringList();
    clear_recent_items = new QAction("Clear Recent Items");
    connect(clear_recent_items, SIGNAL(triggered()), this, SLOT(clearRecentItems()));
    updateRecentItemsMenu(false); // No reason to set the list when nothing gets modified.
    file->addAction(save_file);
    file->addAction(save_file_as);
    file->addAction(new_file);
    file->addSeparator();
    file->addAction(quit);

    edit->addAction(cut);
    edit->addAction(copy);
    edit->addAction(paste);
    edit->addSeparator();
    edit->addAction(edit_item);
    edit->addAction(set_edit_color);
    edit->addSeparator();
    edit->addAction(new_line);
    edit->addAction(delete_item);

    QMenu *statusbar_menu = view->addMenu("Status Bar");
    statusbar_menu->addAction(view_statusbar);
    statusbar_menu->addSeparator();
    statusbar_menu->addAction(statusbar_showpath);
    statusbar_menu->addAction(statusbar_showcount);

    QMenu *padding_menu = view->addMenu("Text Padding");
    padding_menu->addAction(padding_in);
    padding_menu->addAction(padding_out);
    padding_menu->addAction(padding_reset);

    view->addAction(always_on_top_cbox);
    view->addSeparator();
    view->addAction(zoom_in);
    view->addAction(zoom_out);
    view->addAction(reset_zoom);

    favorites = settings->value("files/favorites").toStringList();
    updateFavoritesMenu(false);

    help->addAction(about_item);

    setCentralWidget(notegrid);

    QTimer::singleShot(0, this, SIGNAL(appStarting()));

    connect(quit, SIGNAL(triggered()), this, SLOT(quitMainWindow()));
    connect(new_file, SIGNAL(triggered()), this, SLOT(closeFile()));
    connect(save_file, SIGNAL(triggered()), this, SLOT(saveFileSlot()));
    connect(new_line, &QAction::triggered, notegrid, &NoteGrid::addTextToList);
    connect(copy, &QAction::triggered, notegrid, &NoteGrid::copyItem);
    connect(delete_item, &QAction::triggered, notegrid, &NoteGrid::deleteItem);
    connect(cut, &QAction::triggered, notegrid, &NoteGrid::cutItem);
    connect(paste, &QAction::triggered, notegrid, &NoteGrid::pasteItem);
    connect(open_file, SIGNAL(triggered()), this, SLOT(openFile()));
    connect(save_file_as, SIGNAL(triggered()), this, SLOT(saveAsFileSlot()));
    connect(about_item, SIGNAL(triggered()), this, SLOT(showAboutWindow()));
    connect(zoom_in, &QAction::triggered, notegrid, &NoteGrid::increaseFontSize);
    connect(zoom_out, &QAction::triggered, notegrid, &NoteGrid::decreaseFontSize);
    connect(zoom_in, &QAction::triggered, notegrid->textField, &TextWidget::increaseFontSize);
    connect(zoom_out, &QAction::triggered, notegrid->textField, &TextWidget::decreaseFontSize);
    connect(reset_zoom, &QAction::triggered, notegrid, &NoteGrid::resetZoom);
    connect(reset_zoom, &QAction::triggered, notegrid->textField, &TextWidget::resetZoom);
    connect(view_statusbar, SIGNAL(triggered()), this, SLOT(toggleStatusBar()));
    connect(statusbar_showpath, SIGNAL(triggered()), this, SLOT(toggleStatusBar()));
    connect(statusbar_showcount, SIGNAL(triggered()), this, SLOT(toggleStatusBar()));
    connect(open_file_location, SIGNAL(triggered()), this, SLOT(openFileLocation()));
    connect(new_window, SIGNAL(triggered()), this, SLOT(newWindow()));
    connect(add_to_favorites, SIGNAL(triggered()), this, SLOT(addToFavorites()));
    connect(padding_in, &QAction::triggered, notegrid, &NoteGrid::increasePaddingSize);
    connect(padding_out, &QAction::triggered, notegrid, &NoteGrid::decreasePaddingSize);
    connect(padding_reset, &QAction::triggered, notegrid, &NoteGrid::resetPadding);
    connect(edit_item, &QAction::triggered, notegrid, &NoteGrid::editItem);
    connect(set_edit_color, SIGNAL(triggered()), this, SLOT(changeEditColor()));
    connect(always_on_top_cbox, SIGNAL(triggered()), this, SLOT(toggleAlwaysOnTop()));
}

void NoteTree::newWindow() {
    QProcess::startDetached(QFileInfo(QCoreApplication::applicationFilePath()).absoluteFilePath(), QStringList());
}

void NoteTree::appStarting() {
    QString lastFilePath = settings->value("files/lastOpened", "").toString();
    if (!lastFilePath.isEmpty()) { openFile(lastFilePath); }
}

void NoteTree::showAboutWindow() {
    aboutwindow.exec();
}

void NoteTree::updateRecentItemsMenu(bool setSettingsList) {
    recentItemsGroup->clear();
    recentItems.removeDuplicates();
    for (int i = 0; i < recentItems.length() && i < 10; i++) {
        if (recentItems[i] == filePath) {
            recentItems.removeAt(i);
            continue;
        }
        auto *action = new QAction(visibleFilePath(recentItems[i]));
        recentItemsGroup->addAction(action);
        connect(action, SIGNAL(triggered()), this, SLOT(openRecentFile()));
    }
    while (recentItems.length() > 10) { // Clear the list if it gets bigger than 10. While loop for a total catch all in case some code doesn't keep it balanced correctly.
        recentItems.removeLast();
    }
    recentItemsGroup->addSeparator();
    recentItemsGroup->addAction(clear_recent_items);
    if (setSettingsList)
        settings->setValue("files/recentItems", recentItems);
}

void NoteTree::updateFavoritesMenu(bool setSettingsList) {
    favorites_menu->clear();
    favorites.removeDuplicates();
    favorites_menu->addAction(add_to_favorites);
    favorites_menu->addSeparator();
    for (int i = 0; i < favorites.length(); i++) {
        auto *action = new QMenu(visibleFilePath(favorites[i]));
        auto *open_action = new QAction("Open File");
        if (i < 9)
            open_action->setShortcut(QKeySequence("Ctrl+Shift+" + QString::number(i + 1)));
        else if (i == 9)
            open_action->setShortcut(QKeySequence("Ctrl+Shift+0"));
        connect(open_action, &QAction::triggered, this, [this, i]{ openFavorite(favorites[i]); });
        auto *remove_action = new QAction("Remove from Favorites");
        connect(remove_action, &QAction::triggered, this, [this, i]{ removeFavorite(favorites[i]); });
        action->addAction(open_action);
        action->addAction(remove_action);
        favorites_menu->addMenu(action);
    }

    if (setSettingsList)
        settings->setValue("files/favorites", favorites);
}

void NoteTree::addToFavorites() {
    if (filePath.isEmpty()) return;
    if (favorites.contains(filePath)) return;
    favorites.append(filePath);
    updateFavoritesMenu();
}

void NoteTree::openFavorite(QString file) {
    if (file == filePath) return;
    openFile(file);
}

void NoteTree::removeFavorite(QString file) {
    favorites.removeAll(file);
    updateFavoritesMenu();
}

void NoteTree::changeEditColor() {
    QString text = QInputDialog::getText(this,"Change 'Edit Item' Background","Any valid color in CSS.", QLineEdit::Normal, settings->value("edit/editcolor", "#696db8").toString());
    if (text.isEmpty()) return;
    settings->setValue("edit/editcolor", text);
    if (notegrid->textField->getEditStyle())
        notegrid->textField->setEditStyle(true);
}

void NoteTree::clearRecentItems() {
    recentItems.clear();
    updateRecentItemsMenu();
}

void NoteTree::openRecentFile() {
    QAction *action = qobject_cast<QAction *>(sender());
    if (action) {
        openFile(action->text().replace("~", QStandardPaths::writableLocation(QStandardPaths::HomeLocation)));
    }
}

void NoteTree::toggleStatusBar() {
    if (view_statusbar->isChecked()) {
        updateStatusBar();
        statusBar()->show();
    }
    else
        statusBar()->hide();
    settings->setValue("view/statusbar", view_statusbar->isChecked());
    settings->setValue("view/statusbar_showpath", statusbar_showpath->isChecked());
    settings->setValue("view/statusbar_showcount", statusbar_showcount->isChecked());
}

void NoteTree::toggleAlwaysOnTop() {
    Qt::WindowFlags flags;
    flags = this->windowFlags();
    this->hide();
    if (always_on_top_cbox->isChecked())
        this->setWindowFlags(flags | Qt::WindowStaysOnTopHint);
    else
        this->setWindowFlags(flags & ~Qt::WindowStaysOnTopHint);
    this->show();

    settings->setValue("view/alwaysontop", always_on_top_cbox->isChecked());
}

QString NoteTree::visibleFilePath(QString path) {
    QString tempPath = filePath;
    if (!path.isNull())
        tempPath = path;
#ifndef Q_OS_WIN // Windows has no common shorthand for a user's home folder.
    return tempPath.replace(QStandardPaths::writableLocation(QStandardPaths::HomeLocation), "~");
#else
    return tempPath;
#endif
}

bool NoteTree::closeFile() {
    if (checkDirty()) { return true; }
    notegrid->clearList();
    if (!filePath.isEmpty())
        recentItems.insert(0, filePath);
    filePath = "";
    updateRecentItemsMenu();
    settings->setValue("files/lastOpened", "");
    this->updateWindowTitle();
    notegrid->clearTextFieldContents();
    return false;
}


void NoteTree::saveFile(bool saveAs) {
    QString filename;
    if (filePath.isEmpty() || saveAs)
        filename = QFileDialog::getSaveFileName(this, "Save File");
    else
        filename = filePath;

    if (saveAs)
        recentItems.insert(0, filePath);

    filePath = filename;

    if (saveAs)
        updateRecentItemsMenu();

    QFile file(filename);
    QFileInfo fileInfo(file.fileName());
    this->updateWindowTitle(fileInfo.fileName());

    if (!file.open(QIODevice::WriteOnly | QIODevice::Text)) {
        notTextFile.setInformativeText("Make sure the current user has the permission to read/write to " + visibleFilePath(filePath));
        notTextFile.exec();
        return;
    }
    
    QList<QString> itemList = notegrid->getList();
    QTextStream out(&file);

    foreach (QString item, itemList) {
        item = item.replace("\n\n", "\n,\n");
        out << item << Qt::endl << Qt::endl;
    }
    notegrid->setSavedFile();
    settings->setValue("files/lastOpened", filePath);
}

void NoteTree::saveFileSlot() {
    saveFile(false);
}

void NoteTree::saveAsFileSlot() {
    saveFile(true);
}

void NoteTree::openFileLocation() {
    /* https://stackoverflow.com/a/46019091
     Doesn't have Linux support because file browsers are funny. That final catch at the end is "good enough" for Linux users.
     TODO: add something in for GNOME (nautilus) or KDE (dolphin).
     */
    if (filePath.isEmpty()) { return; }
    QString path = filePath;
    QFileInfo info(path);
    #if defined(Q_OS_WIN)
        QStringList args;
        if (!info.isDir())
            args << "/select,";
        args << QDir::toNativeSeparators(path);
        if (QProcess::startDetached("explorer", args))
            return;
    #elif defined(Q_OS_MAC)
        QStringList args;
        args << "-e";
        args << "tell application \"Finder\"";
        args << "-e";
        args << "activate";
        args << "-e";
        args << "select POSIX file \"" + path + "\"";
        args << "-e";
        args << "end tell";
        args << "-e";
        args << "return";
        if (!QProcess::execute("/usr/bin/osascript", args))
            return;
    #endif
        QDesktopServices::openUrl(QUrl::fromLocalFile(info.isDir()? path : info.path()));
}

void NoteTree::openFile(QString filename) {
    if (filename.isEmpty()) {
        filename = QFileDialog::getOpenFileName(this, "Open File");
        if (filename.isEmpty()) { return; }
    }
    QFile file(filename);
    if (!file.exists()) {
        fileNotFound.setInformativeText(visibleFilePath(filename) + " could not be found.");
        fileNotFound.exec();
        if (recentItems.contains(filename)) {
            recentItems.removeAll(filename); // Removes all instances of that filename from recent items, the most likely place to get deleted files.
            updateRecentItemsMenu();
        }
        return;
    }
    if (closeFile()) { return; }
    QFileInfo fileInfo(file.fileName());
    filePath = filename;


    if (!file.open(QIODevice::ReadOnly | QIODevice::Text)) {
        notTextFile.setInformativeText("Make sure the current user has the permission to read/write to " + visibleFilePath(filePath));
        notTextFile.exec();
        filePath = "";
        return;
    }

    QTextStream in(&file);
    QString line;
    QString item = "";
    while (!(line = in.readLine()).isNull())
    {
        if (line.isEmpty() && !item.isEmpty()) {
            item = item.replace("\n,\n", "\n\n");
            notegrid->addItemToList(item);
            item = "";
        } else {
            item += line + "\n";
        }
    }
    if (!item.isEmpty()) {
        item = item.replace("\n,\n", "\n\n");
        notegrid->addItemToList(item);
    }
    notegrid->setSavedFile();
    updateRecentItemsMenu();
    this->updateWindowTitle(fileInfo.fileName());
    settings->setValue("files/lastOpened", filePath);
}

void NoteTree::updateWindowTitle(QString title) {
    this->setWindowTitle("Note Tree" + (title.isNull() ? "" : " : " + title));
    updateStatusBar();
}

void NoteTree::updateStatusBar() {
    if (!filePath.isEmpty() && statusbar_showpath->isChecked() && statusbar_showcount->isChecked())
        this->statusBar()->showMessage(QString::number(notegrid->getListCount()) + " : " + visibleFilePath(filePath));
    else if (statusbar_showcount->isChecked())
        this->statusBar()->showMessage(QString::number(notegrid->getList().count()));
    else if (!filePath.isEmpty() && statusbar_showpath->isChecked() && !statusbar_showcount->isChecked())
        this->statusBar()->showMessage(visibleFilePath(filePath));
    else
        this->statusBar()->showMessage("");
}

void NoteTree::markSaved() {
    if (notegrid->isDirty && !this->windowTitle().endsWith("*")) {
        // If dirty and doesn't have star, apply star.
        this->setWindowTitle(this->windowTitle() + "*");
    } else if (!notegrid->isDirty && this->windowTitle().endsWith("*")) {
        // If has star but is not dirty, remove it.
        QString window_title = this->windowTitle();
        window_title.chop(1);
        this->setWindowTitle(window_title);
    }
}

void NoteTree::quitMainWindow() {
    this->close();
}

bool NoteTree::checkDirty() {
    if (!notegrid->isDirty) { return false; }
    int ret = saveChanges.exec();

    switch (ret) {
        case QMessageBox::Save:
            saveFile(false);
            return false;
        case QMessageBox::Discard:
            notegrid->isDirty = false;
            markSaved();
            return false;
        case QMessageBox::Cancel:
            return true;
        default:
            return true;
    }
}

void NoteTree::closeEvent(QCloseEvent *event) {
    if (this->checkDirty())
        event->ignore();
    else {
        settings->setValue("view/geometry", saveGeometry());
        QMainWindow::closeEvent(event);
    }
}
