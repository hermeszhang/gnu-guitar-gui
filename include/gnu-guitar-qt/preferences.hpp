#ifndef GNU_GUITAR_PREFERENCES_HPP
#define GNU_GUITAR_PREFERENCES_HPP

#include <QDialog>

class QVBoxLayout;
class QTabWidget;

namespace GnuGuitar {

namespace Gui {

class ApiSettings;

} // namepace Gui

namespace Qt {

class ApiPreferences;
class DriverPreferences;

class Preferences final : public QDialog {
  Q_OBJECT
public:
  Preferences(QWidget *parent = nullptr);
  ~Preferences();
  void addApi(const Gui::ApiSettings &apiSettings);
  void getSelectedApi(Gui::ApiSettings &apiSettings) const;
private:
  QVBoxLayout *layout;
  QTabWidget *tabWidget;
  ApiPreferences *apiPreferences;
  DriverPreferences *driverPreferences;
};

} // namespace Qt

} // namespace GnuGuitar

#endif // GNU_GUITAR_PREFERENCES_HPP
