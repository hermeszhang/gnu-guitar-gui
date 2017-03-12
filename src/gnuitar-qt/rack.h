#ifndef GNUITAR_QT_RACK_H
#define GNUITAR_QT_RACK_H

#include <QWidget>
#include <QHBoxLayout>

#include <gnuitar-qt/knob.h>

#include <libgnuitar/effect.h>

namespace Gnuitar
{

namespace Qt
{

class EffectView final : public QWidget
{
  Q_OBJECT
public:
  EffectView (const Effect *effect, QWidget *parent = nullptr);
  EffectView (const QString& name, QWidget *parent = nullptr);
  ~EffectView (void);
protected:
  void show_controls (const Effect *effect);
private:
  QHBoxLayout *layout;
}; /* class Effect */

class Rack final : public QWidget
{
  Q_OBJECT
public:
  Rack (QWidget *parent = nullptr);
  ~Rack (void);
  void add_effect (EffectView *effect_view);
private:
  QVBoxLayout *layout;
}; /* class Rack */

} /* namespace Qt */

} /* namespace Gnuitar */

#endif /* GNUITAR_QT_RACK_H */

