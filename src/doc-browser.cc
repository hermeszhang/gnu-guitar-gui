#include <gnu-guitar-qt/doc-browser.h>

#include <QFile>

namespace GnuGuitar {

namespace Qt {

DocBrowser::DocBrowser(QWidget *parent) : QTextBrowser(parent) {
  resize(640, 480);
  setOpenLinks(true);
  setOpenExternalLinks(true);
  setSearchPaths(QStringList() << ":/docs");
  setSource(QString("index.html"));
#if 0
  QFile index_file("index.html");
  if (index_file.open(QIODevice::ReadOnly))
    setHtml(index_file.readAll());
#endif
}

DocBrowser::~DocBrowser(void) {

}

} /* namespace Qt */

} /* namespace Gnuitar */
