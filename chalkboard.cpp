#include "chalkboard.h"
#include "notegrid.h"
#include "notetree.h"

Chalkboard::Chalkboard(NoteGrid *notegrid_pass, QWidget *parent) : QMainWindow(parent) {
    notegrid = notegrid_pass;
    chalkboardwidget = new ChalkboardWidget(notegrid, this);
    NoteTree *notetree = notegrid->notetree;

    setWindowTitle("Note Tree | Chalkboard");
    this->resize(400, 450);

    auto *quit = new QAction("&Quit", this);
    quit->setShortcut(QKeySequence(QKeySequence::Quit));
    auto *close_window = new QAction("&Close", this);
    close_window->setShortcut(QKeySequence(QKeySequence::Close));
    always_on_top_cbox = new QAction("Always on Top");
    always_on_top_cbox->setCheckable(true);
    always_on_top_cbox->setChecked(true);
    always_on_top_cbox->setShortcut(QKeySequence("Ctrl+Shift+T"));
    toggleAlwaysOnTop();

    QMenu *file = menuBar()->addMenu("&File");
    QMenu *view = menuBar()->addMenu("&View");

    file->addAction(close_window);
    file->addAction(quit);
    view->addAction(always_on_top_cbox);

    connect(quit, &QAction::triggered, notetree, &NoteTree::quitMainWindow);
    connect(close_window, SIGNAL(triggered()), this, SLOT(closeWindow()));
    connect(always_on_top_cbox, SIGNAL(triggered()), this, SLOT(toggleAlwaysOnTop()));


    setCentralWidget(chalkboardwidget);
}

void Chalkboard::closeWindow() {
    this->close();
}

void Chalkboard::toggleAlwaysOnTop() {
    bool isVisible = this->isVisible();
    Qt::WindowFlags flags;
    flags = this->windowFlags();
    this->hide();
    if (always_on_top_cbox->isChecked())
        this->setWindowFlags(flags | Qt::WindowStaysOnTopHint);
    else
        this->setWindowFlags(flags & ~Qt::WindowStaysOnTopHint);
    if (isVisible)
        this->show();
}
