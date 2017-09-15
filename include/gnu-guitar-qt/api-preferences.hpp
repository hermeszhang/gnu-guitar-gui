#ifndef GNU_GUITAR_QT_API_PREFERENCES_HPP
#define GNU_GUITAR_QT_API_PREFERENCES_HPP

#include <QList>
#include <QWidget>

class QString;
class QVBoxLayout;

namespace GnuGuitar {

namespace Qt {

class ApiOptions;

class ApiPreferences final : public QWidget {
  Q_OBJECT
public:
  ApiPreferences(QWidget *parent = nullptr);
  ~ApiPreferences();
  void addApi(const QString &name);
  void addApi(ApiOptions *apiOptions);
  QString getSelectedApi() const;
signals:
  void apiClicked(const QString &apiName);
protected slots:
  void onApiClicked(const QString &apiName);
private:
  QList<ApiOptions *> apiList;
  QVBoxLayout *layout;
};

} // namespace Qt

} // namespace GnuGuitar

#endif // GNU_GUITAR_QT_API_PREFERENCES_HPP
