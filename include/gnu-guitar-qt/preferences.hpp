#ifndef GNU_GUITAR_PREFERENCES_HPP
#define GNU_GUITAR_PREFERENCES_HPP

#include <QDialog>

class QScrollArea;

namespace GnuGuitar {

namespace Qt {

class ApiPreferencesList;

class Preferences final : public QDialog {
  Q_OBJECT
public:
  Preferences(QWidget *parent = nullptr);
  ~Preferences();
private:
  ApiPreferencesList *apiPreferencesList;
  QScrollArea *scrollArea;
};

} // namespace Qt

} // namespace GnuGuitar

#endif // GNU_GUITAR_PREFERENCES_HPP
