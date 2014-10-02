#include "csv.h"
#include <QApplication>

int main(int argc, char *argv[])
{
  QApplication app(argc, argv);

  CSV window;
  window.show();

  return app.exec();
}
