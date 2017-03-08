#ifndef GNUITAR_QT_RACK_H
#define GNUITAR_QT_RACK_H

#include <QWidget>
#include <QHBoxLayout>

namespace Gnuitar
{

namespace Qt
{

class EffectView final : public QWidget
{
  Q_OBJECT
public:
  EffectView (const QString& name, QWidget *parent = nullptr);
  ~EffectView (void);
  void add_parameter (const QString& name, double value = 0.0);
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

