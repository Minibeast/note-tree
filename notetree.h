#pragma once

#include "notegrid.h"
#include "chalkboard.h"

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
#include <QStandardPaths>
#include <QInputDialog>
#include <QMimeData>


class NoteTree : public QMainWindow {

    Q_OBJECT

    public:
        NoteTree(QWidget *parent = nullptr);
        void markSaved();
        void updateStatusBar();
        QSettings* settings;
   public slots:
        void quitMainWindow();
   private slots:
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
        void newWindow();
        void addToFavorites();
        void openFavorite(QString file);
        void removeFavorite(QString file);
        void changeEditColor();
        void toggleAlwaysOnTop();
        void showChalkboard();
        void addDirectoryToFavorites();
        void openFolderLocation(QString folder);
    protected:
        void closeEvent(QCloseEvent *event);
        void dropEvent(QDropEvent *event);
        void dragEnterEvent(QDragEnterEvent *event);
    private:
        void saveFile(bool saveAs);
        void updateWindowTitle(QString title = QString());
        QString visibleFilePath(QString path = nullptr);
        bool checkDirty();
        void updateRecentItemsMenu(bool setSettingsList = true);
        void updateFavoritesMenu(bool setSettingsList = true);
        QString filePath;
        NoteGrid* notegrid;
        QAction* view_statusbar;
        QAction* statusbar_showcount;
        QAction* statusbar_showpath;
        QAction* clear_recent_items;
        QAction* add_to_favorites;
        QAction* add_folder_to_favorites;
        QAction* always_on_top_cbox;
        QMenu *recentItemsGroup;
        QMenu *favorites_menu;
        QStringList recentItems;
        QStringList favorites;
        Chalkboard *chalkboard;

        QMessageBox aboutwindow;
        QMessageBox fileNotFound;
        QMessageBox saveChanges;
        QMessageBox notTextFile;
};
