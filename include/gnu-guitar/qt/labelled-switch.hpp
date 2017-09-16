#ifndef GNU_GUITAR_QT_LABELLED_SWITCH_HPP
#define GNU_GUITAR_QT_LABELLED_SWITCH_HPP

#include <QWidget>

class QHBoxLayout;
class QLabel;
class QRadioButton;

namespace GnuGuitar {

namespace Qt {

class LabelledSwitch final : public QWidget {
  Q_OBJECT
public:
  LabelledSwitch(QWidget *parent = nullptr);
  ~LabelledSwitch();
  void setLabel(const QString &labelText);
private:
  QLabel *label;
  QRadioButton *radioButton;
  QHBoxLayout *layout;
};

} // namespace Qt

} // namespace GnuGuitar

#endif // GNU_GUITAR_QT_LABELLED_SWITCH_HPP
