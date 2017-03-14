#include "rack.h"

#include "knob.h"

#include <QDial>
#include <QLabel>
#include <QVBoxLayout>

namespace Gnuitar
{

namespace Qt
{

Rack::Rack (QWidget *parent) : QWidget (parent)
{
  layout = new QVBoxLayout;
  setLayout (layout);
}

Rack::~Rack (void)
{

}

void
Rack::add_effect (EffectView *effect_view)
{
  layout->addWidget (effect_view);
}

} /* namespace Qt */

} /* namespace Gnuitar */

