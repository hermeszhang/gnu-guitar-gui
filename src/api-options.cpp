#include <gnu-guitar-qt/api-options.hpp>

#include <QHBoxLayout>
#include <QLabel>
#include <QRadioButton>

namespace GnuGuitar {

namespace Qt {

ApiOptions::ApiOptions(QWidget *parent) : QWidget(parent) {

  radioButton = new QRadioButton(this);

  label = new QLabel(this);

  layout = new QHBoxLayout(this);
  layout->addWidget(radioButton);
  layout->addWidget(label);

  setLayout(layout);

  connect(radioButton, &QRadioButton::clicked, this, &ApiOptions::onClicked);
}

ApiOptions::~ApiOptions() {

}

QString ApiOptions::getName() const {
  return label->text();
}

void ApiOptions::setName(const QString &name_) {
  label->setText(name_);
}

void ApiOptions::onClicked() {
  emit clicked(getName());
}

} // namespace Qt

} // namespace GnuGuitar

