#ifndef GNUITAR_QT_EFFECT_VIEW_H
#define GNUITAR_QT_EFFECT_VIEW_H

#include <QWidget>
#include <QHBoxLayout>
#include <QLabel>

#include <gnu-guitar-qt/knob.h>

namespace Gnuitar
{

namespace Qt
{

class EffectView final : public QWidget
{
  Q_OBJECT
public:
  EffectView (const QString& label_text, QWidget *parent = nullptr);
  ~EffectView (void);
  void add_control (const QString& control_name);
  const QString& get_label (void) const;
  void set_label (const QString& label);
signals:
  void control_changed (const QString& label, int value);
protected slots:
  void on_knob_changed (const Knob *knob);
private:
  QString label_text;
  QHBoxLayout *layout;
  QLabel *label;
}; /* class Effect */

} /* namespace Qt */

} /* namespace Gnuitar */

#endif /* GNUITAR_QT_EFFECT_VIEW_H */

