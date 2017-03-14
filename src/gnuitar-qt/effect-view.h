#ifndef GNUITAR_QT_EFFECT_VIEW_H
#define GNUITAR_QT_EFFECT_VIEW_H

#include <QWidget>
#include <QHBoxLayout>

#include <gnuitar-qt/knob.h>

namespace Gnuitar
{

namespace Qt
{

class EffectView final : public QWidget
{
  Q_OBJECT
public:
  EffectView (const QString& effect_name, QWidget *parent = nullptr);
  ~EffectView (void);
  void add_control (const QString& control_name);
signals:
  void control_changed (const QString& label, int value);
protected slots:
  void on_knob_changed (const Knob *knob);
private:
  QHBoxLayout *layout;
}; /* class Effect */

} /* namespace Qt */

} /* namespace Gnuitar */

#endif /* GNUITAR_QT_EFFECT_VIEW_H */

