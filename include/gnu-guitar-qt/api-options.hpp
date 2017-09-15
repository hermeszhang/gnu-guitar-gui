#ifndef GNU_GUITAR_QT_API_OPTIONS_HPP
#define GNU_GUITAR_QT_API_OPTIONS_HPP

#include <qaccordion/contentpane.hpp>

namespace GnuGuitar {

namespace Qt {

class ApiOptions final : public ContentPane {
  Q_OBJECT
public:
  ApiOptions(QWidget *parent = nullptr);
  ~ApiOptions();
  QString getName();
  void setName(const QString &name_);
  void uncheck();
signals:
  void clicked(const QString &name);
};

} // namespace Qt

} // namespace GnuGuitar

#endif // GNU_GUITAR_QT_API_OPTIONS_HPP
