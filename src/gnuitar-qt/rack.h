#ifndef GNUITAR_QT_RACK_H
#define GNUITAR_QT_RACK_H

#include <QWidget>
#include <QVBoxLayout>

#include <gnuitar-qt/effect-view.h>

namespace Gnuitar
{

namespace Qt
{

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

