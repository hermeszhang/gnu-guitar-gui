#ifndef GNU_GUITAR_QT_API_OPTIONS_HPP
#define GNU_GUITAR_QT_API_OPTIONS_HPP

#include <QWidget>

class QHBoxLayout;
class QLabel;
class QRadioButton;

namespace GnuGuitar {

namespace Qt {

class ApiOptions final : public QWidget {
  Q_OBJECT
public:
  ApiOptions(QWidget *parent = nullptr);
  ~ApiOptions();
  QString getName() const;
  void setName(const QString &name_);
signals:
  void clicked(const QString &name);
protected slots:
  void onClicked();
private:
  QRadioButton *radioButton;
  QLabel *label;
  QHBoxLayout *layout;
};

} // namespace Qt

} // namespace GnuGuitar

#endif // GNU_GUITAR_QT_API_OPTIONS_HPP
