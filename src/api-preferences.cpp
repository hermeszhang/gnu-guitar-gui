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

  addApi(apiOptions);
}

void ApiPreferences::addApi(ApiOptions *apiOptions) {

  connect(apiOptions, &ApiOptions::clicked, this, &ApiPreferences::onApiClicked);

  layout->addWidget(apiOptions);

  apiList.push_back(apiOptions);
}

void ApiPreferences::onApiClicked(const QString &apiName) {

  for (auto api : apiList) {
    if (api->getName() == apiName)
      continue;
    else
      api->uncheck();
  }

  emit apiClicked(apiName);
}

} // namespace Qt

} // namespace GnuGuitar

