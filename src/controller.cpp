#include <gnu-guitar/qt/controller.hpp>

#include <gnu-guitar/gui/driver.hpp>

#include <gnu-guitar-qt/effect-view.hpp>
#include <gnu-guitar-qt/main-window.hpp>
#include <gnu-guitar-qt/preferences.hpp>

#ifdef GNU_GUITAR_QT_WITH_CORE
#include <gnu-guitar/gui/core-driver.hpp>
#else
#include <gnu-guitar/gui/fake-driver.hpp>
#endif // GNU_GUITAR_QT_WITH_CORE

#include <gnu-guitar/gui/api-settings.hpp>
#include <gnu-guitar/gui/string-control.hpp>

namespace GnuGuitar {

namespace Qt {

Controller::Controller() {

  mainWindow = new MainWindow();

  mainWindow->resize(640, 480);
  mainWindow->show();

  mainWindow->connect(mainWindow, &MainWindow::effectChanged,
                      this, &Controller::onEffectChanged);
  mainWindow->connect(mainWindow, &MainWindow::effectClicked,
                      this, &Controller::addEffect);
  mainWindow->connect(mainWindow, &MainWindow::playClicked,
                      this, &Controller::onPlayClicked);
  mainWindow->connect(mainWindow, &MainWindow::preferencesClicked,
                      this, &Controller::onPreferencesClicked);
  mainWindow->connect(mainWindow, &MainWindow::stopClicked,
                      this, &Controller::onStopClicked);
  mainWindow->connect(mainWindow, &MainWindow::quitClicked,
                      this, &Controller::onQuitClicked);
#ifdef GNU_GUITAR_QT_WITH_CORE
  driver = new Gui::CoreDriver();
#else
  driver = new Gui::FakeDriver();
#endif

  updateEffectList();
}

Controller::~Controller() {
  if (driver != nullptr) {
    delete driver;
    driver = nullptr;
  }
  if (mainWindow != nullptr) {
    delete mainWindow;
    mainWindow = nullptr;
  }
}

void Controller::addEffect(const QString &effectName) {

  driver->addEffect(effectName.toStdString());

  std::vector<std::string> controlList;
  driver->listEffectControls(effectName.toStdString(), controlList);

  auto effectView = new EffectView(mainWindow);
  effectView->setLabel(effectName);
  for (const auto& control : controlList)
    effectView->addControl(control.c_str());

  mainWindow->addEffect(effectView);
}

void Controller::onEffectChanged(const QString &effectName,
                                 const QString &controlName,
                                 int value) {
  float valueRatio = 0.0f;
  valueRatio = (float) value;
  valueRatio /= 100.0f;
  driver->setEffectControlValue(effectName.toStdString(),
                                controlName.toStdString(),
                                valueRatio);
}

void Controller::onPlayClicked() {
  driver->start();
}

void Controller::onPreferencesClicked() {

  Preferences preferences;
  preferences.resize(320, 240);

  std::vector<Gui::ApiSettings> apiList;
  driver->listApis(apiList);

  for (const auto &apiSettings : apiList)
    preferences.addApi(apiSettings);

  preferences.exec();

  driver->setApi(preferences.getSelectedApi().toStdString());
}

void Controller::onStopClicked() {
  driver->stop();
}

void Controller::onQuitClicked() {
  mainWindow->close();
}

void Controller::updateEffectList() {
  std::vector<std::string> effectList;
  driver->listEffects(effectList);
  for (const auto &effectName : effectList)
    mainWindow->addEffectChoice(effectName.c_str());
}

} /* namespace Qt */

} /* namespace GnuGuitar */
