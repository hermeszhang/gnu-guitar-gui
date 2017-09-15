#ifndef GNU_GUITAR_CONTROL_LIST_HPP
#define GNU_GUITAR_CONTROL_LIST_HPP

#include <QWidget>

namespace GnuGuitar {

namespace Qt {

class ControlList final : public QWidget {
public:
  ControlList(QWidget *parent = nullptr);
  ~ControlList();
  void add(const QString &controlName);
};

} // namespace Qt

} // namespace GnuGuitar

#endif // GNU_GUITAR_CONTROL_LIST_HPP
