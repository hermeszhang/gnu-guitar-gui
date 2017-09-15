#ifndef GNU_GUITAR_DRIVER_PREFERENCES_HPP
#define GNU_GUITAR_DRIVER_PREFERENCES_HPP

#include <QWidget>

namespace GnuGuitar {

namespace Qt {

class DriverPreferences final : public QWidget {
  Q_OBJECT
public:
  DriverPreferences(QWidget *parent = nullptr);
  ~DriverPreferences();
};

} // namespace Qt

} // namespace GnuGuitar

#endif // GNU_GUITAR_DRIVER_PREFERENCES_HPP
