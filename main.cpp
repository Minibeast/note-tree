#include "notetree.h"

int main(int argc, char *argv[]) {
    QApplication app(argc, argv);

    NoteTree window;

    window.resize(600, 900);
    window.show();

    return app.exec();
}
