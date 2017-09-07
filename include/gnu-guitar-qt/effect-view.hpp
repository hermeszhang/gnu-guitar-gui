#ifndef GNUITAR_QT_EFFECT_VIEW_HPP
#define GNUITAR_QT_EFFECT_VIEW_HPP

#include <QWidget>

class QLabel;
class QString;
class QGridLayout;

namespace GnuGuitar {

namespace Qt {

class Knob;

class EffectView final : public QWidget {
  Q_OBJECT
public:
  EffectView(QWidget *parent = nullptr);
  ~EffectView(void);
  void addControl(const QString &control_name);
  QString getLabel(void) const;
  void setLabel(const QString &label);
signals:
  void controlChanged(const QString &label, int value);
protected:
  void setupColors();
  int getCurrentKnobRow();
protected slots:
  void onKnobChanged(const Knob &knob);
private:
  int rowIndex;
  int columnIndex;
  QGridLayout *layout;
  QLabel *label;
}; /* class Effect */

} /* namespace Qt */

} /* namespace Gnuitar */

#endif /* GNUITAR_QT_EFFECT_VIEW_HPP */
