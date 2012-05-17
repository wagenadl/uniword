// main.C

#include <QApplication>
#include <QFile>
#include <QTextStream>
#include <QDebug>

#include "MainWindow.H"
#include "Universe.H"

void addchars(Universe &uverse) {
  QFile chars(":/chars.txt");
  chars.open(QFile::ReadOnly);
  QTextStream ts(&chars);
  while (!ts.atEnd()) {
    QString line = ts.readLine();
    if (line.isNull())
      break;
    int c = line.mid(0, 6).toInt(0, 16);
    uverse.add(c, line.mid(9));
  }
}

void addmorechars(Universe &uverse) {
  QFile morechars(":/morechars.txt");
  morechars.open(QFile::ReadOnly);
  QTextStream ts(&morechars);
  while (!ts.atEnd()) {
    QString line = ts.readLine();
    if (line.isNull())
      break;
    int c = line.mid(0, 6).toInt(0, 16);
    uverse.add(c, line.mid(9));
  }
}

void addalias(Universe &uverse) {
  QFile alias(":/alias.txt");
  alias.open(QFile::ReadOnly);
  QTextStream ts(&alias);
  while (!ts.atEnd()) {
    QString line = ts.readLine();
    if (line.isNull())
      break;
    int c = line.mid(0, 6).toInt(0, 16);
    uverse.add(c, line.mid(9));
  }
}

void addblocks(Universe &uverse) {
  QFile blocks(":/blocks.txt");
  blocks.open(QFile::ReadOnly);
  QTextStream ts(&blocks);
  while (!ts.atEnd()) {
    QString line = ts.readLine();
    if (line.isNull())
      break;
    int start = line.mid(0, 6).toInt(0, 16);
    int end = line.mid(7, 6).toInt(0, 16);
    QString desc = line.mid(14);
    //qDebug() << "block: " << start << " -- " << end << " : " << desc;
    for (int i=start; i<=end; i++) {
      if (i<32)
	continue;
      if (i>=127 && i<160)
	continue;
      uverse.addperhaps(i, desc);
    }
  }
}
  

int main(int argc, char **argv) {
  QApplication app(argc, argv);
  Universe uverse;
  addmorechars(uverse);
  addchars(uverse);
  addalias(uverse);
  addblocks(uverse);
  MainWindow mw(uverse, 0);
  mw.show();
  return app.exec();
}

  
