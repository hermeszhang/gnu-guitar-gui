#ifndef GNU_GUITAR_QT_POWER_BUTTON_HPP
#define GNU_GUITAR_QT_POWER_BUTTON_HPP

#include <QLabel>
#include <QPixmap>

namespace GnuGuitar {

namespace Qt {

class PowerButton final : public QLabel {
  Q_OBJECT
public:
  PowerButton(QWidget *parent = nullptr);
  ~PowerButton();
signals:
  void activated();
  void deactivated();
protected:
  void mousePressEvent(QMouseEvent *) override;
  void setOnImage(const QString &path);
  void setOffImage(const QString &path);
  void toggleState();
  void setOffState();
  void setOnState();
private:
  QPixmap onPixmap;
  QPixmap offPixmap;
  bool on;
};

} // namespace Qt

} // namespace GnuGuitar

#endif // GNU_GUITAR_QT_POWER_BUTTON
