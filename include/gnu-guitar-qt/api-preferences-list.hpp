#ifndef GNU_GUITAR_QT_API_PREFERENCES_LIST_HPP
#define GNU_GUITAR_QT_API_PREFERENCES_LIST_HPP

#include <QWidget>

class QString;
class QVBoxLayout;

namespace GnuGuitar {

namespace Qt {

class ApiPreferencesList final : public QWidget {
  Q_OBJECT
public:
  ApiPreferencesList(QWidget *parent = nullptr);
  ~ApiPreferencesList();
  QString getSelectedApi() const;
protected:
  void addApi(const QString &name);
  void createApiList();
private:
    QVBoxLayout *layout;
};

} // namespace Qt

} // namespace GnuGuitar

#endif // GNU_GUITAR_QT_API_PREFERENCES_LIST_HPP
