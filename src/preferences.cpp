#include <gnu-guitar-qt/preferences.hpp>

#include <gnu-guitar-qt/api-preferences.hpp>
#include <gnu-guitar-qt/driver-preferences.hpp>

#include <QTabWidget>
#include <QVBoxLayout>

namespace GnuGuitar {

namespace Qt {

Preferences::Preferences(QWidget *parent) : QDialog(parent) {

  setWindowTitle("Preferences");

  driverPreferences = new DriverPreferences(this);

  apiPreferences = new ApiPreferences(this);

  tabWidget = new QTabWidget(this);
  tabWidget->addTab(apiPreferences, "API");
  tabWidget->addTab(driverPreferences, "Driver");

  layout = new QVBoxLayout(this);
  layout->addWidget(tabWidget);

  setLayout(layout);
}

Preferences::~Preferences() {
  if (apiPreferences != nullptr) {
    delete apiPreferences;
    apiPreferences = nullptr;
  }
  if (driverPreferences != nullptr) {
    delete driverPreferences;
    driverPreferences = nullptr;
  }
  if (tabWidget != nullptr) {
    delete tabWidget;
    tabWidget = nullptr;
  }
  if (layout != nullptr) {
    delete layout;
    layout = nullptr;
  }
}

void Preferences::addApi(const QString &apiName) {
  apiPreferences->addApi(apiName);
}

} // namespace Qt

} // namespace GnuGuitar
