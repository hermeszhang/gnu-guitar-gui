#include <gnu-guitar-qt/api-preferences.hpp>

#include <gnu-guitar-qt/api-options.hpp>

#include <QVBoxLayout>

namespace GnuGuitar {

namespace Qt {

ApiPreferences::ApiPreferences(QWidget *parent) : QWidget(parent) {
  layout = new QVBoxLayout(this);
  setLayout(layout);
}

ApiPreferences::~ApiPreferences() {
  if (layout != nullptr) {
    delete layout;
    layout = nullptr;
  }
}

QString ApiPreferences::getSelectedApi() const {
  return "";
}

void ApiPreferences::addApi(const QString &apiName) {

  auto apiOptions = new ApiOptions(this);
  apiOptions->setName(apiName);

  connect(apiOptions, &ApiOptions::clicked, this, &ApiPreferences::onApiClicked);

  layout->addWidget(apiOptions);
}

void ApiPreferences::onApiClicked(const QString &apiName) {
  emit apiClicked(apiName);
}

} // namespace Qt

} // namespace GnuGuitar

