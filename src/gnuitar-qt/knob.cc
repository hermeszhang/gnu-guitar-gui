#include <gnuitar-qt/knob.h>

namespace Gnuitar
{

namespace Qt
{

Knob::Knob (const QString& name, QWidget *parent) : QWidget (parent)
{
  dial = new QDial;

  connect (dial, &QDial::sliderMoved, this, &Knob::on_adjustment);

  label = new QLabel;
  label->setText (name);
  label->setAlignment (::Qt::AlignHCenter);

  layout = new QVBoxLayout;
  layout->addWidget (dial);
  layout->addWidget (label);

  setLayout (layout);
}

Knob::~Knob (void)
{

}

void
Knob::on_adjustment (int value)
{
  emit adjusted (static_cast<float> (value));
}

} /* namespace Qt */

} /* namespace Gnuitar */

