#include "rack.h"

#include "knob.h"

#include <QDial>
#include <QLabel>
#include <QVBoxLayout>

namespace Gnuitar
{

namespace Qt
{

EffectView::EffectView (const Effect *effect, QWidget *parent) : QWidget(parent)
{
  std::string effect_name;
  effect->get_name(effect_name);
  effect_name = "<i><b>" + effect_name + "</i></b>";

  auto effect_label = new QLabel;
  effect_label->setTextFormat (::Qt::RichText);
  effect_label->setText (effect_name.c_str ());
  effect_label->setAlignment (::Qt::AlignLeft);

  layout = new QHBoxLayout;
  layout->addWidget (effect_label);
  setLayout(layout);

  show_controls (effect);
}

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
EffectView::show_controls (const Effect *effect)
{
  if (effect == nullptr)
    throw std::invalid_argument ("effect is null");

  auto control_names = effect->get_control_names ();
  for (const auto& control_name : control_names)
    {
      auto control_knob = new Knob (control_name.c_str (), this);
      layout->addWidget (control_knob);
    }
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

