#pragma once

#include "chalkboardwidget.h"
#include "chalkboardtext.h"

#include <QWidget>
#include <QMainWindow>
#include <QWindow>
#include <QApplication>
#include <QStackedLayout>
#include <QTextEdit>
#include <QKeyEvent>
#include <QMenuBar>

class NoteGrid;
class ChalkboardWidget;

class Chalkboard : public QMainWindow {

    Q_OBJECT

    public:
        Chalkboard(NoteGrid *notegrid_pass, QWidget *parent = nullptr);
    private slots:
        void closeWindow();
        void toggleAlwaysOnTop();
    private:
        NoteGrid* notegrid;
        ChalkboardWidget* chalkboardwidget;
        QAction* always_on_top_cbox;
};
