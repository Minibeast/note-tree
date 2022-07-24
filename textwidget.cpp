#include "textwidget.h"
#include "notegrid.h"
#include "notetree.h"

TextWidget::TextWidget(QWidget *parent) : QTextEdit(parent) {
    notegrid = reinterpret_cast<NoteGrid*>(parent);
    this->setAcceptRichText(false);
    textFontSize = notegrid->notetree->settings->value("view/textFont", 12).toInt(); // TODO: Add to settings.
    this->setStyleSheet("font-size: " + QString::number(textFontSize) + "px");
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

void TextWidget::increaseFontSize() {
    textFontSize += 2;
    if (textFontSize >= 100) { textFontSize -= 2; return; }
    notegrid->notetree->settings->setValue("view/textFont", textFontSize);
    this->setStyleSheet("font-size: " + QString::number(textFontSize) + "px;");
}

void TextWidget::decreaseFontSize() {
    textFontSize -= 2;
    if (textFontSize <= 0) { textFontSize += 2; return; }
    notegrid->notetree->settings->setValue("view/textFont", textFontSize);
    this->setStyleSheet("font-size: " + QString::number(textFontSize) + "px;");
}

void TextWidget::resetZoom() {
    textFontSize = 12;
    this->setStyleSheet("font-size: " + QString::number(textFontSize) + "px;");
}
