// main.C

#include <QApplication>
#include <QFile>
#include <QTextStream>
#include <QDebug>
#include <QtGlobal>

#include "MainWindow.H"
#include "Universe.H"

void addchars(Universe &uverse) {
  QFile chars(":/tables/chars.txt");
  chars.open(QFile::ReadOnly);
  QTextStream ts(&chars);
  while (!ts.atEnd()) {
    QString line = ts.readLine();
    if (line.isNull())
      break;
    int c = line.mid(0, 6).toInt(0, 16);
    uverse.add(c, line.mid(7));
  }
}

void addalias(Universe &uverse) {
  QFile alias(":/tables/alias.txt");
  alias.open(QFile::ReadOnly);
  QTextStream ts(&alias);
  while (!ts.atEnd()) {
    QString line = ts.readLine();
    if (line.isNull())
      break;
    int c = line.mid(0, 6).toInt(0, 16);
    uverse.add(c, line.mid(7));
  }
}

void addblocks(Universe &uverse) {
  QFile blocks(":/tables/blocks.txt");
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
      uverse.addgroup(i, desc);
    }
  }
}
 
void addgroups(Universe &uverse) {
  QFile blocks(":/tables/groups.txt");
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
      uverse.addgroup(i, desc);
    }
  }
}
 
int main(int argc, char **argv) {
  QApplication app(argc, argv);

  QByteArray fontname = qgetenv("UNIWORDFONT");

  Universe uverse;
  addchars(uverse);
  addalias(uverse);
  addblocks(uverse);
  addgroups(uverse);

  MainWindow mw(uverse, fontname);
  mw.show();
  return app.exec();
}

  
