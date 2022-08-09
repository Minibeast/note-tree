#include "textwidget.h"
#include "notegrid.h"
#include "notetree.h"

TextWidget::TextWidget(QWidget *parent) : QTextEdit(parent) {
    style = "font-size: %1px;";
    notegrid = reinterpret_cast<NoteGrid*>(parent);
    this->setAcceptRichText(false);
    textFontSize = notegrid->notetree->settings->value("view/textFont", 12).toInt();
    this->setStyleSheet(style.arg(QString::number(textFontSize)));
}

void TextWidget::focusInEvent(QFocusEvent* event) {
    notegrid->unselectList();
    QTextEdit::focusInEvent(event);
}

void TextWidget::keyPressEvent(QKeyEvent* event) {
    if (event->key() == Qt::Key_Return && !((event->modifiers() & Qt::ShiftModifier) == Qt::ShiftModifier))
        notegrid->addTextToList();
    else if (getEditStyle() && event->key() == Qt::Key_Escape)
        notegrid->exitEdit();
    else
        QTextEdit::keyPressEvent(event);
}

void TextWidget::increaseFontSize() {
    textFontSize += 2;
    if (textFontSize >= 100) { textFontSize -= 2; return; }
    notegrid->notetree->settings->setValue("view/textFont", textFontSize);
    this->setStyleSheet(style.arg(QString::number(textFontSize)));
}

void TextWidget::decreaseFontSize() {
    textFontSize -= 2;
    if (textFontSize <= 0) { textFontSize += 2; return; }
    notegrid->notetree->settings->setValue("view/textFont", textFontSize);
    this->setStyleSheet(style.arg(QString::number(textFontSize)));
}

void TextWidget::resetZoom() {
    textFontSize = 12;
    this->setStyleSheet(style.arg(QString::number(textFontSize)));
}

void TextWidget::setEditStyle(bool setColor) {
    if (setColor) {
        style = "font-size: %1px; background-color: %2";
        this->setStyleSheet(style.arg(QString::number(textFontSize)).arg(notegrid->notetree->settings->value("edit/editcolor", "#696db8").toString()));
    } else {
        style = "font-size: %1px;";
        this->setStyleSheet(style.arg(QString::number(textFontSize)));
    }
}

bool TextWidget::getEditStyle() {
    return style.contains("background-color");
}
