#include <gnu-guitar/qt/error-dialog.hpp>

#ifdef GNU_GUITAR_QT_WITH_CORE
#include <gnu-guitar-core/error.hpp>
#endif

namespace GnuGuitar {

namespace Qt {

ErrorDialog::ErrorDialog(const Core::Error &error) : QMessageBox() {
#ifdef GNU_GUITAR_QT_WITH_CORE
  setIcon(QMessageBox::Critical);
  addButton(QMessageBox::Ok);
  setInformativeText(error.what());
#else // GNU_GUITAR_QT_WITH_CORE
  (void) error;
#endif // GNU_GUITAR_QT_WITH_CORE
}

ErrorDialog::~ErrorDialog() {

}

} // namespace Qt

} // namespace GnuGuitar

