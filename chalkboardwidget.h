#pragma once

#include "chalkboardtext.h"

#include <QWidget>
#include <QWindow>
#include <QApplication>
#include <QStackedLayout>

class NoteGrid;
class ChalkboardTextWidget;

class ChalkboardWidget : public QWidget {

    Q_OBJECT

    public:
        ChalkboardWidget(NoteGrid *notegrid_pass, QWidget *parent = nullptr);
        void addChalkboardToList();
    private:
        QString getText();
        void clearText();
        NoteGrid* notegrid;
        ChalkboardTextWidget* textbox;

};
