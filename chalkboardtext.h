#pragma once

#include "textwidget.h"

#include <QWidget>
#include <QApplication>
#include <QTextEdit>
#include <QKeyEvent>

class NoteGrid;
class ChalkboardWidget;

class ChalkboardTextWidget : public TextWidget {

    Q_OBJECT

    public:
        ChalkboardTextWidget(NoteGrid *notegrid_pass, QWidget *parent = nullptr);
    protected:
        void keyPressEvent(QKeyEvent* event);
    private:
        NoteGrid *notegrid;
        ChalkboardWidget *chalkboard;
};
