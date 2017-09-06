#include <gnu-guitar-qt/controller.hpp>

#include <gnu-guitar-qt/driver.hpp>
#include <gnu-guitar-qt/driver.hpp>
#include <gnu-guitar-qt/main-window.hpp>

#ifdef GNU_GUITAR_QT_WITH_CORE
#include <gnu-guitar-qt/core-driver.hpp>
#else
#include <gnu-guitar-qt/fake-driver.hpp>
#endif // GNU_GUITAR_QT_WITH_CORE

using namespace Gnuitar::Qt;

#if 0

namespace {

class ControlUpdater final : public RtAudio::ProcessorVisitor {
  /// @brief The name of the control
  ///  to update.
  std::string controlName;
  /// @brief The name of the effect
  ///  to update.
  std::string effectName;
  /// @brief The new value to set the
  ///  control to. This value should be
  ///  between 0.0 and 1.0 because it is
  ///  used as a ratio of the min and max
  ///  values of the control.
  float controlValue;
public:
  void setControlName(const std::string &name) {
    controlName = name;
  }
  void setControlValue(float value) {
    if (value > 1.0f)
      controlValue = 1.0f;
    else if (value < 0.0f)
      controlValue = 0.0f;
    else
      controlValue = value;
  }
  void setEffectName(const std::string &name) {
    effectName = name;
  }
  void visit(RtAudio::CompositeProcessor &compositeProcessor) {
    for (auto processor : compositeProcessor)
      processor->accept(*this);
  }
  void visit(RtAudio::LadspaProcessor &processor) {

    // TODO : throw exception if a control is not found

    if (processor.getName() != effectName)
      return;

    auto functor = [&](RtAudio::LadspaControl &control) {
      control.value = controlValue * (control.value_max - control.value_min);
    };

    RtAudio::LadspaControlFinder<decltype(functor)> controlFinder(controlName, functor);

    processor.acceptPortVisitor(controlFinder);
  }
};

} // namespace

#endif

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
  driver = new CoreDriver();
#else
  driver = new FakeDriver();
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

  auto effectView = new EffectView(effectName, mainWindow);
  for (const auto& control : controlList)
    effectView->add_control(control.c_str());

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
