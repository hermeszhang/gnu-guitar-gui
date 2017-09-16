#include <gnu-guitar/qt/combo-box.hpp>

#include <QComboBox>
#include <QHBoxLayout>
#include <QLabel>

namespace GnuGuitar {

namespace Qt {

ComboBox::ComboBox(QWidget *parent) : QWidget(parent) {

  comboBox = new QComboBox(this);

  label = new QLabel(this);

  layout = new QHBoxLayout(this);
  layout->addWidget(label);
  layout->addWidget(comboBox);

  setLayout(layout);
}

ComboBox::~ComboBox() {

}

void ComboBox::addOption(const QString &option) {
  comboBox->addItem(option);
}

void ComboBox::setLabel(const QString &labelText) {
  label->setText(labelText);
}

} // namespace Qt

} // namespace GnuGuitar

