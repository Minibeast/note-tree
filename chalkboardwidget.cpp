#include "chalkboardwidget.h"
#include "notegrid.h"

ChalkboardWidget::ChalkboardWidget(NoteGrid *notegrid_pass, QWidget *parent) : QWidget(parent) {
    notegrid = notegrid_pass;

    auto* grid = new QStackedLayout(this);
    textbox = new ChalkboardTextWidget(notegrid, this);
    grid->addWidget(textbox);
    setLayout(grid);
}

QString ChalkboardWidget::getText() {
    return textbox->toPlainText();
}

void ChalkboardWidget::clearText() {
    textbox->clear();
}

void ChalkboardWidget::addChalkboardToList() {
    auto text = getText();
    notegrid->addItemToList(text);
    clearText();
}
