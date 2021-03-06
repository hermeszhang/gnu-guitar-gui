#ifndef GNU_GUITAR_QT_ERROR_DIALOG_HPP
#define GNU_GUITAR_QT_ERROR_DIALOG_HPP

#include <QMessageBox>

namespace GnuGuitar {

namespace Core {

class Error;
class MissingDevice;

} // namespace Core

namespace Qt {

class ErrorDialog final : public QMessageBox {
  Q_OBJECT
public:
  ErrorDialog(const Core::Error &error);
  ErrorDialog(const Core::MissingDevice &missingDevice);
  ~ErrorDialog();
};

} // namespace Qt

} // namespace GnuGuitar

#endif // GNU_GUITAR_QT_ERROR_DIALOG_HPP
