#pragma once

#include "notegrid.h"

#include <QMainWindow>
#include <QApplication>
#include <QMenu>
#include <QMenuBar>
#include <QKeyEvent>
#include <QGridLayout>
#include <QTextEdit>
#include <QShortcut>
#include <QMessageBox>
#include <QFile>
#include <QTextStream>
#include <QDir>
#include <QSettings>
#include <QFileDialog>
#include <QTimer>
#include <QSettings>
#include <QSysInfo>
#include <QStatusBar>
#include <QDesktopServices>
#include <QProcess>


class NoteTree : public QMainWindow {

    Q_OBJECT

    public:
        NoteTree(QWidget *parent = nullptr);
        void markSaved();
        QSettings* settings;
   private slots:
        void quitMainWindow();
        void saveFileSlot();
        void saveAsFileSlot();
        void openFile(QString filename = nullptr);
        void appStarting();
        bool closeFile();
        void showAboutWindow();
        void toggleStatusBar();
        void openFileLocation();
        void openRecentFile();
        void clearRecentItems();
    protected:
        void closeEvent(QCloseEvent *event);
    private:
        void saveFile(bool saveAs);
        void updateWindowTitle(QString title = QString());
        bool checkDirty();
        void updateRecentItemsMenu(bool setSettingsList);
        QString filePath;
        NoteGrid* notegrid;
        QAction* view_statusbar;
        QAction* clear_recent_items;
        QMenu *recentItemsGroup;
        QStringList recentItems;
};
