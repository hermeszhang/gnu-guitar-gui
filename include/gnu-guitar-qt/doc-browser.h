#ifndef GNUITAR_QT_DOC_BROWSER_H
#define GNUITAR_QT_DOC_BROWSER_H

#include <QTextBrowser>

namespace GnuGuitar {

namespace Qt {

class DocBrowser : public QTextBrowser {
  Q_OBJECT
public:
  DocBrowser(QWidget *parent = 0);
  ~DocBrowser(void);
}; /* class DocBrowser */

} /* namespace Qt */

} /* namespace Gnuitar */

#endif /* GNUITAR_qT_DOC_BROWSER_H */
