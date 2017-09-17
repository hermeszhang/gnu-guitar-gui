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

class ApiForm;

class ApiPreferences final : public QAccordion {
  Q_OBJECT
public:
  ApiPreferences(QWidget *parent = nullptr);
  ~ApiPreferences();
  void addApi(const Gui::ApiSettings &apiSettings);
  void addApi(const QString &apiName, ApiForm *apiForm);
  void getSelectedApi(Gui::ApiSettings &apiSettings) const;
protected slots:
  void onApiSelected(ApiForm *apiForm);
private:
  std::vector<ApiForm *> forms;
};

} // namespace Qt

} // namespace GnuGuitar

#endif // GNU_GUITAR_QT_API_PREFERENCES_HPP
