#pragma once

#include "textwidget.h"

#include <QWidget>
#include <QGridLayout>
#include <QLabel>
#include <QListWidget>
#include <QMessageBox>
#include <QClipboard>


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
    public slots:
        void addTextToList();
        void copyItem();
        void deleteItem();
        void cutItem();
        void pasteItem();
    private:
        QListWidget* stack;
        TextWidget* textField;
};