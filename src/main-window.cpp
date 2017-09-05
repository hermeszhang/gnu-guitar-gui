#include <gnu-guitar-qt/main-window.hpp>

#include <QPalette>
#include <QScrollArea>
#include <QVBoxLayout>

namespace GnuGuitar {

namespace Qt {

MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent) {

  setWindowTitle("GNU Guitar");

  scrollArea = new QScrollArea(this);
  scrollArea->setWidgetResizable(true);
  setCentralWidget(scrollArea);

  centralWidget = new QWidget(this);

  layout = new QVBoxLayout;
  centralWidget->setLayout(layout);

  menuBar = new MenuBar(this);

  audioPanel = new AudioPanel(centralWidget);
  audioPanel->resize(640, 120);
  layout->addWidget(audioPanel);

  rack = new Rack(centralWidget);
  layout->addWidget(rack);

  setMenuBar(menuBar);

  connect(rack, &Rack::effect_changed,
          this, &MainWindow::onEffectChanged);
  connect(menuBar, &MenuBar::effect_selected,
          this, &MainWindow::onEffectClicked);
  connect(menuBar, &MenuBar::preferencesClicked,
          this, &MainWindow::onPreferencesClicked);
  connect(menuBar, &MenuBar::quit_selected,
          this, &MainWindow::onQuitClicked);
  connect(audioPanel, &AudioPanel::play_triggered,
          this, &MainWindow::onPlayClicked);
  connect(audioPanel, &AudioPanel::stop_triggered,
          this, &MainWindow::onStopClicked);

  scrollArea->setWidget(centralWidget);

  setupColors();
}

MainWindow::~MainWindow() {

}

void MainWindow::addEffect(EffectView *effectView) {
  rack->add_effect(effectView);
}

void MainWindow::addEffectChoice(const QString &effectName) {
  menuBar->add_effect(effectName);
}

void MainWindow::onPlayClicked() {
  emit playClicked();
}

void MainWindow::onStopClicked() {
  emit stopClicked();
}

void MainWindow::onEffectChanged(const QString &effectName,
                                 const QString &controlName,
                                 int value) {
  emit effectChanged(effectName, controlName, value);
}

void MainWindow::onEffectClicked(const QString &effectName) {
  emit effectClicked(effectName);
}

void MainWindow::onPreferencesClicked() {
  emit preferencesClicked();
}

void MainWindow::onQuitClicked() {
  emit quitClicked();
}

void MainWindow::setupColors() {
  auto palette_ = centralWidget->palette();
  palette_.setColor(QPalette::Background, QColor(15, 15, 15));
  centralWidget->setAutoFillBackground(true);
  centralWidget->setPalette(palette_);
}

} // namespace Qt

} // namespace GnuGuitar
