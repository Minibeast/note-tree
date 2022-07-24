#pragma once

#include "textwidget.h"

#include <QWidget>
#include <QGridLayout>
#include <QLabel>
#include <QListWidget>
#include <QMessageBox>
#include <QClipboard>

class NoteTree;

class NoteGrid : public QWidget {
    
    Q_OBJECT

    public:
        NoteGrid(QWidget *parent = nullptr);
        QString getTextFieldContents();
        void clearTextFieldContents();
        void addItemToList(QString text);
        void unselectList();
        void clearList();
        QList<QString> getList();
        bool isDirty;
        TextWidget* textField;
    public slots:
        void addTextToList();
        void copyItem();
        void deleteItem();
        void cutItem();
        void pasteItem();
        void increaseFontSize();
        void decreaseFontSize();
        void resetZoom();
    private:
        QListWidget* stack;
        NoteTree* notetree;
        int stackFontSize;
};
