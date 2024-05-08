#include "textrec.h"

#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    textrec w;
    w.show();
    return a.exec();
}
