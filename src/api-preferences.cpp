#include <gnu-guitar-qt/api-preferences.hpp>

#include <gnu-guitar/gui/api-settings.hpp>

namespace GnuGuitar {

namespace Qt {

ApiPreferences::ApiPreferences(QWidget *parent) : QAccordion(parent) {

}

ApiPreferences::~ApiPreferences() {

}

QString ApiPreferences::getSelectedApi() const {
  return "";
}

void ApiPreferences::addApi(const Gui::ApiSettings &apiSettings) {

  std::string apiName;
  apiSettings.getApiName(apiName);

  auto contentPane = new ContentPane(apiName.c_str(), this);

  addContentPane(contentPane);
}

void ApiPreferences::onApiClicked(const QString &apiName) {
#if 0
  for (auto api : apiList) {
    if (api->getName() == apiName)
      continue;
    else
      api->uncheck();
  }
#endif
  emit apiClicked(apiName);
}

} // namespace Qt

} // namespace GnuGuitar

