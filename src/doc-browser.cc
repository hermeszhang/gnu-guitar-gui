#include <gnu-guitar-qt/doc-browser.h>

#include <QFile>

namespace Gnuitar {

namespace Qt {

DocBrowser::DocBrowser(QWidget *parent) : QTextBrowser(parent) {
  QFile index_file(":/docs/index.html");
  if (index_file.open(QIODevice::ReadOnly))
    setHtml(index_file.readAll());
}

DocBrowser::~DocBrowser(void) {}

} /* namespace Qt */

} /* namespace Gnuitar */
