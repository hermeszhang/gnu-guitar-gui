#include <gnuitar-qt/effect-view.h>
#include <gnuitar-qt/knob.h>

#include <QLabel>

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
EffectView::add_control (const QString& control_name)
{
  auto control_knob = new Knob (control_name, this);
  auto control_callback = [this, control_knob]{
    EffectView::on_knob_changed (control_knob);
  };
  connect (control_knob, &Knob::released, this, control_callback);
  layout->addWidget (control_knob);
}

void
EffectView::on_knob_changed (const Knob *knob)
{
  auto text = knob->get_label ();
  auto value = knob->get_value ();
  emit control_changed (text, value);
}

} /* namespace Qt */

} /* namespace Gnuitar */

