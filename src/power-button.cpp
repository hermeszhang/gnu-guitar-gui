#include <gnu-guitar-qt/power-button.hpp>

#include <iostream>

namespace GnuGuitar::Qt {

PowerButton::PowerButton(QWidget *parent) : QLabel(parent) {
  setOnImage(":/icons/power-button-on.png");
  setOffImage(":/icons/power-button-off.png");
  // setOnImage(":/power-button-on.png");
  // setOffImage(":/power-button-off.png");
  setOffState();
}

PowerButton::~PowerButton() {

}

void PowerButton::setOnImage(const QString &path) {
  onPixmap.load(path);
}

void PowerButton::setOffImage(const QString &path) {
  offPixmap.load(path);
}

void PowerButton::toggleState() {
  if (on) {
    setOffState();
    emit deactivated();
  } else {
    setOnState();
    emit activated();
  }
}

void PowerButton::setOnState() {
  on = true;
  setPixmap(onPixmap);
}

void PowerButton::setOffState() {
  on = false;
  setPixmap(offPixmap);
}

void PowerButton::mousePressEvent(QMouseEvent *) {
  toggleState();
}

} // namespace GnuGuitar::Qt

