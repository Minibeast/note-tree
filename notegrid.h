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
        void exitEdit();
        QList<QString> getList();
        int getListCount();
        bool isDirty;
        TextWidget* textField;
        NoteTree* notetree;
        void setSavedFile();
        void checkChanges();
    public slots:
        void addTextToList();
        void copyItem();
        void deleteItem();
        void cutItem();
        void pasteItem();
        void increaseFontSize();
        void decreaseFontSize();
        void resetZoom();
        void increasePaddingSize();
        void decreasePaddingSize();
        void resetPadding();
        void editItem();
    private:
        int getItemFromStack(QListWidgetItem *item);
        QListWidget* stack;
        int listIndex;
        QString cachedText;
        QString originalText;
        QList<QString> savedFile;
        int stackFontSize;
        int paddingSize;
        QString style;
};
