#include "notetree.h"

#define _STR(x) #x
#define STRINGIFY(x)  _STR(x)

NoteTree::NoteTree(QWidget *parent) : QMainWindow(parent) {
    settings = new QSettings("Mini's Applications", "Note Tree");

    auto *quit = new QAction("&Quit", this);
    quit->setShortcut(QKeySequence(QKeySequence::Quit));
    auto *new_file = new QAction("&New", this);
    new_file->setShortcut(QKeySequence(QKeySequence::New));
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
    view_statusbar = new QAction("View &Statusbar", this);
    view_statusbar->setShortcut(QKeySequence("Ctrl+/"));
    view_statusbar->setCheckable(true);
    view_statusbar->setChecked(settings->value("view/statusbar", true).toBool());
    toggleStatusBar();
    auto *open_file_location = new QAction("Open File Location", this);
    open_file_location->setShortcut(QKeySequence("Ctrl+Shift+O"));

    notegrid = new NoteGrid(this);
    filePath = "";
    this->updateWindowTitle();

    QMenu *file = menuBar()->addMenu("&File");
    QMenu *edit = menuBar()->addMenu("&Edit");
    QMenu *view = menuBar()->addMenu("&View");
    QMenu *help = menuBar()->addMenu("&Help");

    file->addAction(new_file);
    file->addAction(open_file);
    file->addAction(open_file_location);
    file->addAction(save_file);
    file->addAction(save_file_as);
    file->addSeparator();
    file->addAction(quit);

    edit->addAction(cut);
    edit->addAction(copy);
    edit->addAction(paste);
    edit->addSeparator();
    edit->addAction(new_line);
    edit->addAction(delete_item);

    view->addAction(zoom_in);
    view->addAction(zoom_out);
    view->addAction(reset_zoom);
    view->addSeparator();
    view->addAction(view_statusbar);

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
    connect(open_file_location, SIGNAL(triggered()), this, SLOT(openFileLocation()));
}

void NoteTree::appStarting() {
    QString lastFilePath = settings->value("files/lastOpened", "").toString();
    if (!lastFilePath.isEmpty()) { openFile(lastFilePath); }
}

void NoteTree::showAboutWindow() {
    QMessageBox aboutwindow;
    aboutwindow.setText("About Note Tree");
    aboutwindow.setInformativeText("Git Commit: " + QString(STRINGIFY(GIT_VERSION)) + "\nOS: " + QSysInfo::kernelType() + " " + QSysInfo::kernelVersion() + "\n\nMade by Mini / Amy");
    aboutwindow.exec();
}

void NoteTree::toggleStatusBar() {
    if (view_statusbar->isChecked())
        statusBar()->show();
    else
        statusBar()->hide();
    settings->setValue("view/statusbar", view_statusbar->isChecked());
}

bool NoteTree::closeFile() {
    if (checkDirty()) { return true; }
    notegrid->clearList();
    filePath = "";
    settings->setValue("files/lastOpened", "");
    this->updateWindowTitle();
    notegrid->clearTextFieldContents();
    return false;
}


void NoteTree::saveFile(bool saveAs) {
    QString filename;
    if (filePath.isEmpty() || saveAs)
        filename = QFileDialog::getSaveFileName(this, "Save File", "notes/");
    else
        filename = filePath;

    filePath = filename;

    QFile file(filename);
    QFileInfo fileInfo(file.fileName());
    this->updateWindowTitle(fileInfo.fileName());

    if (!file.open(QIODevice::WriteOnly | QIODevice::Text))
        return;
    
    QList<QString> itemList = notegrid->getList();
    QTextStream out(&file);

    foreach (QString item, itemList) {
        item = item.replace("\n\n", "\n,\n");
        out << item << Qt::endl << Qt::endl;
    }
    notegrid->isDirty = false;
    markSaved();
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
        filename = QFileDialog::getOpenFileName(this, "Open File", "notes/");
        if (filename.isEmpty()) { return; }
    }
    if (closeFile()) { return; }
    QFile file(filename);
    QFileInfo fileInfo(file.fileName());
    filePath = filename;
    this->updateWindowTitle(fileInfo.fileName());

    if (!file.open(QIODevice::ReadOnly | QIODevice::Text))
        return;

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
        notegrid->addItemToList(item);
    }
    notegrid->isDirty = false;
    markSaved();
    settings->setValue("files/lastOpened", filePath);
}

void NoteTree::createNewFile() {
    QMessageBox msgBox;
    msgBox.setText("Create New Document.");
    msgBox.setInformativeText("Do you want to create a new document?");
    msgBox.setStandardButtons(QMessageBox::Ok | QMessageBox::Cancel);
    msgBox.setDefaultButton(QMessageBox::Ok);
    int ret = msgBox.exec();

    switch (ret) {
        case QMessageBox::Ok:
            return;
        case QMessageBox::Cancel:
            return;
        default:
            return;
    }
}

void NoteTree::updateWindowTitle(QString title) {
    this->setWindowTitle("Note Tree" + (title.isNull() ? "" : " : " + title));
    if (!filePath.isNull())
        this->statusBar()->showMessage(filePath);
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
    QMessageBox msgBox;
    msgBox.setText("The document has been modified.");
    msgBox.setInformativeText("Do you want to save your changes?");
    msgBox.setStandardButtons(QMessageBox::Save | QMessageBox::Discard | QMessageBox::Cancel);
    msgBox.setDefaultButton(QMessageBox::Save);
    int ret = msgBox.exec();

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
    if (this->checkDirty()) { event->ignore(); }
    else // lol
    QMainWindow::closeEvent(event);
}
