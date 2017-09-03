#ifndef GNU_GUITAR_PREFERENCES_HPP
#define GNU_GUITAR_PREFERENCES_HPP

#include <QDialog>

namespace GnuGuitar {

class Preferences final : public QDialog {
public:
  Preferences(QWidget *parent = nullptr);
  ~Preferences();
};

} // namespace GnuGuitar

#endif // GNU_GUITAR_PREFERENCES_HPP
