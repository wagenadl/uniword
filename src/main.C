// main.C

/*
    Uniword: keyword-based unicode character selector
    Copyright (C) 2012  Daniel A. Wagenaar <daw@caltech.edu>

    This program is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/

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
    uverse.add(c, line.mid(7));
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
    uverse.add(c, line.mid(7));
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
      uverse.addgroup(i, desc);
    }
  }
}
 
void addgroups(Universe &uverse) {
  QFile blocks(":/groups.txt");
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
  app.setOrganizationName("www.danielwagenaar.net");
  app.setOrganizationDomain("www.danielwagenaar.net");
  app.setApplicationName("Uniword");
  
  Universe uverse;
  addchars(uverse);
  addalias(uverse);
  addblocks(uverse);
  addgroups(uverse);

  MainWindow mw(uverse);
  mw.show();

  return app.exec();
}

  
