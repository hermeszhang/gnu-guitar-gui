#include "audio-panel.h"

#include <QApplication>

#include <cstdlib>

int main(int argc, char *argv[]) {
  QApplication a(argc, argv);
  Gnuitar::Qt::AudioPanel audio_panel;
  audio_panel.add_control("asdf");
  audio_panel.resize(320, 240);
  audio_panel.show();

  return a.exec();
}
