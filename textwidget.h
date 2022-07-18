#pragma once

#include <QWidget>
#include <QApplication>
#include <QGridLayout>
#include <QTextEdit>
#include <QKeyEvent>

class NoteGrid;

class TextWidget : public QTextEdit {

    Q_OBJECT

    public:
        TextWidget(QWidget *parent = nullptr);
    protected:
        void focusInEvent(QFocusEvent* event);
        void keyPressEvent(QKeyEvent* event);
    private:
        NoteGrid* notegrid;
};
