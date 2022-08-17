#include "chalkboardtext.h"
#include "notegrid.h"
#include "chalkboardwidget.h"

ChalkboardTextWidget::ChalkboardTextWidget(NoteGrid *notegrid_pass, QWidget *parent) : TextWidget(notegrid_pass) {
    notegrid = notegrid_pass;
    chalkboard = reinterpret_cast<ChalkboardWidget*>(parent);
}

void ChalkboardTextWidget::keyPressEvent(QKeyEvent* event) {
    if (event->key() == Qt::Key_Return && ((event->modifiers() & Qt::ControlModifier) == Qt::ControlModifier))
        chalkboard->addChalkboardToList();
    else
        QTextEdit::keyPressEvent(event);
}
