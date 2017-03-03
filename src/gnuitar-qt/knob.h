#ifndef GNUITAR_QT_KNOB_H
#define GNUITAR_QT_KNOB_H

#include <QDial>
#include <QLabel>
#include <QVBoxLayout>
#include <QWidget>

namespace Gnuitar {

namespace Qt {

class Knob final : public QWidget {
    Q_OBJECT
public:
    Knob(const QString& name, QWidget *parent = nullptr);
    ~Knob(void);
private:
    QDial *dial;
    QLabel *label;
    QVBoxLayout *layout;
}; /* class Knob */

} /* namespace Qt */

} /* namespace Gnuitar */

#endif /* GNUITAR_QT_KNOB_H */
