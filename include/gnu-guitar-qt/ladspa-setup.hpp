#ifndef GNU_GUITAR_QT_LADSPA_SETUP_HPP
#define GNU_GUITAR_QT_LADSPA_SETUP_HPP

#include <QDialog>

class QComboBox;
class QGridLayout;
class QPushButton;

namespace GnuGuitar {

/// @brief Determines what input and output
///  port will be used for the LADSPA effect.
class LadspaSetup : public QDialog {
  Q_OBJECT
public:
  LadspaSetup(QWidget *parent = nullptr);
  ~LadspaSetup();
  void addInput(const std::string &name);
  void addOutput(const std::string &name);
  bool cancelled(void) const noexcept;
  std::string getInputName();
  std::string getOutputName();
  void setInputName(const std::string &inputName);
  void setOutputName(const std::string &outputName);
private:
  bool cancelPushed;
  QGridLayout *layout;
  QComboBox *inputBox;
  QComboBox *outputBox;
  QPushButton *okayButton;
  QPushButton *cancelButton;
};

} // GnuGuitar

#endif // GNU_GUITAR_QT_LADSPA_SETUP_HPP
