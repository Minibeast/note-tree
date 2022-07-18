#include "textwidget.h"
#include "notegrid.h"

TextWidget::TextWidget(QWidget *parent) : QTextEdit(parent) {
    notegrid = reinterpret_cast<NoteGrid*>(parent);
    this->setAcceptRichText(false);
}

void TextWidget::focusInEvent(QFocusEvent* event) {
    notegrid->unselectList();
    QTextEdit::focusInEvent(event);
}

void TextWidget::keyPressEvent(QKeyEvent* event) {
    if (event->key() == Qt::Key_Return && !((event->modifiers() & Qt::ShiftModifier) == Qt::ShiftModifier))
        notegrid->addTextToList();
    else
        QTextEdit::keyPressEvent(event);
}
