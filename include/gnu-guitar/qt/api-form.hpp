#ifndef GNU_GUITAR_QT_API_FORM_HPP
#define GNU_GUITAR_QT_API_FORM_HPP

#include <gnu-guitar/qt/form.hpp>

namespace GnuGuitar {

namespace Qt {

class LabelledSwitch;

class ApiForm final : public Form {
  Q_OBJECT
public:
  ApiForm(QWidget *parent = nullptr);
  ~ApiForm();
  void getName(QString &name) const;
  void setName(const QString &name);
  bool selected() const;
  void select();
  void unselect();
signals:
  void apiSelected();
private slots:
  void onApiSelected();
private:
  LabelledSwitch *useSwitch;
  bool isSelected;
  QString name;
};

} // namespace Qt

} // namespace GnuGuitar

#endif // GNU_GUITAR_QT_API_FORM_HPP
