#include "doc-browser.h"

#include <QApplication>

#include <cstdlib>

int
main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    Gnuitar::Qt::DocBrowser docs_browser;
    docs_browser.resize(640, 480);
    docs_browser.show();

    return a.exec();
}

