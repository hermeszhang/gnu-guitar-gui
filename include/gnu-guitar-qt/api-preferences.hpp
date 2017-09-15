#ifndef GNU_GUITAR_QT_API_PREFERENCES_HPP
#define GNU_GUITAR_QT_API_PREFERENCES_HPP

#include <qaccordion/qaccordion.hpp>

class QString;
class QVBoxLayout;

namespace GnuGuitar {

namespace Gui {

class ApiSettings;

} // namespace Gui

namespace Qt {

class ApiPreferences final : public QAccordion {
  Q_OBJECT
public:
  ApiPreferences(QWidget *parent = nullptr);
  ~ApiPreferences();
  void addApi(const Gui::ApiSettings &apiSettings);
  QString getSelectedApi() const;
signals:
  void apiClicked(const QString &apiName);
protected slots:
  void onApiClicked(const QString &apiName);
};

} // namespace Qt

} // namespace GnuGuitar

#endif // GNU_GUITAR_QT_API_PREFERENCES_HPP
