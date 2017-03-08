#include "rack.h"

#include "knob.h"

#include <QDial>
#include <QLabel>
#include <QVBoxLayout>

namespace Gnuitar
{

namespace Qt
{

EffectView::EffectView (const QString& name, QWidget *parent) : QWidget(parent)
{
  auto effect_label = new QLabel;
  effect_label->setTextFormat (::Qt::RichText);
  effect_label->setText ("<i><b>" + name + "</i></b>");
  effect_label->setAlignment (::Qt::AlignCenter);
  layout = new QHBoxLayout;
  layout->addWidget (effect_label);
  setLayout (layout);
}

EffectView::~EffectView (void)
{

}

void
EffectView::add_parameter (const QString& name, double value)
{
  auto parameter = new Knob (name, this);
  layout->addWidget (parameter);
  (void) value;
}

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

