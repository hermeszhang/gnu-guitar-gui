#include <gnu-guitar/qt/labelled-switch.hpp>

#include <QHBoxLayout>
#include <QLabel>
#include <QRadioButton>

namespace GnuGuitar {

namespace Qt {

LabelledSwitch::LabelledSwitch(QWidget *parent) : QWidget(parent) {

  label = new QLabel(this);

  radioButton = new QRadioButton(this);

  layout = new QHBoxLayout(this);
  layout->addWidget(label);
  layout->addWidget(radioButton);
}

LabelledSwitch::~LabelledSwitch() {

}

void LabelledSwitch::setLabel(const QString &labelText) {
  label->setText(labelText);
}

} // namespace Qt

} // namespace GnuGuitar
