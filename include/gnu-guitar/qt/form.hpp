#ifndef GNU_GUITAR_QT_FORM_HPP
#define GNU_GUITAR_QT_FORM_HPP

#include <QFrame>

class QVBoxLayout;

namespace GnuGuitar {

namespace Gui {

class StringControl;

} // namespace Gui

namespace Qt {

class Form final : public QFrame {
  Q_OBJECT
public:
  Form(QWidget *parent = nullptr);
  ~Form();
  void addControl(const Gui::StringControl &stringControl);
private:
  QVBoxLayout *layout;
};

} // namespace Qt

} // namespace GnuGuitar

#endif // GNU_GUITAR_QT_FORM_HPP

