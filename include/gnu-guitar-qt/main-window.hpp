#ifndef GNUITAR_QT_MAIN_WINDOW_HPP
#define GNUITAR_QT_MAIN_WINDOW_HPP

#include <QMainWindow>

#include <gnu-guitar-qt/audio-panel.h>
#include <gnu-guitar-qt/menu-bar.h>
#include <gnu-guitar-qt/rack.h>

class QScrollArea;
class QVBoxLayout;

// TODO : remove old names
using Gnuitar::Qt::AudioPanel;
using Gnuitar::Qt::EffectView;
using Gnuitar::Qt::MenuBar;
using Gnuitar::Qt::Rack;

namespace GnuGuitar {

namespace Qt {

class MainWindow : public QMainWindow {
  Q_OBJECT
public:
  MainWindow(QWidget *parent = 0);
  ~MainWindow();
  void addEffect(EffectView *effectView);
  void addEffectChoice(const QString &effectName);
signals:
  void effectChanged(const QString &effectName,
                     const QString &controlName,
                     int controlValue);
  void effectClicked(const QString &effectName);
  void playClicked();
  void stopClicked();
  void quitClicked();
private slots:
  /// @brief User clicked the play button.
  void onPlayClicked();
  /// @brief User clicked the stop button.
  void onStopClicked();
  /// @brief User changed the value of an effect
  ///  control.
  void onEffectChanged(const QString &effectName,
                       const QString &controlName,
                       int value);
  /// @brief User clicked an effect from the
  ///  menu bar.
  void onEffectClicked(const QString &name);
  /// @brief User clicked the 'quit' button.
  void onQuitClicked();

private:
  /// @brief The area that will be encapsulated
  ///  by scroll bars for navigation.
  QScrollArea *scrollArea;
  /// @brief The widget that will contain all of
  ///  the content (except for the menu bar).
  QWidget *centralWidget;
  /// @brief Aligns the audio panel and rack
  ///  vertically.
  QVBoxLayout *layout;
  /// @brief The top menu bar, used for traditional
  ///  user interface items.
  MenuBar *menuBar;
  /// @brief Contains the buttons that will control
  ///  the audio library.
  AudioPanel *audioPanel;
  /// @brief Contains all the effects and their
  ///  controls.
  Rack *rack;
};

} // namespace Qt

} // namespace GnuGuitar

#endif // GNUITAR_QT_MAIN_WINDOW_HPP
