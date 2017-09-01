#include <gnu-guitar-qt/knob.h>

namespace Gnuitar {

namespace Qt {

Knob::Knob(const QString &name, QWidget *parent) : QWidget(parent) {
  dial = new QDial;

  connect(dial, &QDial::sliderReleased, this, &Knob::on_released);

  label = new QLabel;
  label->setText(name);
  label->setAlignment(::Qt::AlignHCenter);

  layout = new QVBoxLayout;
  layout->addWidget(dial);
  layout->addWidget(label);

  setLayout(layout);
}

Knob::~Knob(void) {}

QString Knob::get_label(void) const { return label->text(); }

int Knob::get_value(void) const { return dial->value(); }

void Knob::set_value(int value) { dial->setValue(value); }

void Knob::on_released(void) { emit released(); }

} /* namespace Qt */

} /* namespace Gnuitar */
