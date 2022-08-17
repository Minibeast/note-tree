#include "notetree.h"

int main(int argc, char *argv[]) {
    QApplication app(argc, argv);

    NoteTree window;

    window.show();

    return app.exec();
}
