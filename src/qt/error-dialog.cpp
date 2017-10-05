#include <gnu-guitar/qt/error-dialog.hpp>

#ifdef GNU_GUITAR_QT_WITH_CORE
#include <gnu-guitar-core/error.hpp>
#include <gnu-guitar-core/missing-device.hpp>
#include <sstream>
#endif

namespace GnuGuitar {

namespace Qt {

ErrorDialog::ErrorDialog(const Core::Error &error) : QMessageBox() {
#ifdef GNU_GUITAR_QT_WITH_CORE
  setIcon(QMessageBox::Critical);
  addButton(QMessageBox::Ok);
  setText(error.what());
#else // GNU_GUITAR_QT_WITH_CORE
  (void) error;
#endif // GNU_GUITAR_QT_WITH_CORE
}

ErrorDialog::ErrorDialog(const Core::MissingDevice &missingDevice) : QMessageBox() {
#ifdef GNU_GUITAR_QT_WITH_CORE

  setIcon(QMessageBox::Critical);
  addButton(QMessageBox::Ok);

  std::stringstream message;
  message << missingDevice;

  setText(message.str().c_str());
#else // GNU_GUITAR_QT_WITH_CORE
  (void) missingDevice;
#endif // GNU_GUITAR_QT_WITH_CORE
}

ErrorDialog::~ErrorDialog() {

}

} // namespace Qt

} // namespace GnuGuitar

