#include "knob.h"

namespace Gnuitar {

namespace Qt {

Knob::Knob(const QString& name, QWidget *parent) : QWidget(parent)
{
    dial = new QDial;

    label = new QLabel;
    label->setText(name);
    label->setAlignment(::Qt::AlignHCenter);

    layout = new QVBoxLayout;
    layout->addWidget(dial);
    layout->addWidget(label);

    setLayout(layout);
}

Knob::~Knob(void)
{

}

} /* namespace Qt */

} /* namespace Gnuitar */

