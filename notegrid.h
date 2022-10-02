#pragma once

#include "textwidget.h"

#include <QWidget>
#include <QGridLayout>
#include <QLabel>
#include <QListWidget>
#include <QMessageBox>
#include <QClipboard>

class NoteTree;

enum class HeaderType {
    None, h1, h2
};

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
        void itemDoubleClicked();
        void checkChanges();
    private:
        int getItemFromStack(QListWidgetItem *item);
        HeaderType getHeaderTypeFromItem(QListWidgetItem *item);
        QString convertItemToPlainText(QListWidgetItem *item);
        void updateHeaderFontSize();
        QString findURL(QString item);
        QListWidget* stack;
        int listIndex;
        QString cachedText;
        QString originalText;
        QList<QString> savedFile;
        int stackFontSize;
        int paddingSize;
        QString style;
};
