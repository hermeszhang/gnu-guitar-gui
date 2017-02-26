#include "doc-browser.h"

#include <QApplication>

#include <cstdlib>

int
main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    QString docs_path;
    const char * docs_path_env = std::getenv("GNUITAR_DOCS_PATH");
    if (docs_path_env != nullptr) {
        docs_path = docs_path_env;
    } else {
        docs_path = ".";
    }

    QStringList docs_search_paths;
    docs_search_paths << docs_path;

    Gnuitar::Qt::DocBrowser docs_browser;
    docs_browser.setSearchPaths(docs_search_paths);
    docs_browser.setSource(QUrl("index.html"));
    docs_browser.resize(640, 480);
    docs_browser.show();

    return a.exec();
}

