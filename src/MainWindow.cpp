// MainWindow.C

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

#include "MainWindow.h"

#include <QSettings>
#include <QFontDialog>
#include <QApplication>
#include <QMessageBox>
#include <QMenuBar>
#include <QMenu>
#include <QAction>
#include <QFile>
#include <QTextStream>

#include "CenterWindow.h"

#include <QDebug>

MainWindow::MainWindow(Universe &uverse) {
  cw = new CenterWindow(uverse, this);
  setCentralWidget(cw);

  QMenu *mFile = new QMenu("File", this);
  QMenu *mView = new QMenu("View", this);
  QMenu *mHelp = new QMenu("Help", this);

  connect(mFile->addAction("Quit"), SIGNAL(triggered(bool)), SLOT(sQuit()));
  connect(mView->addAction("Font..."), SIGNAL(triggered(bool)), SLOT(sFont()));
  connect(mHelp->addAction("About"), SIGNAL(triggered(bool)), SLOT(sAbout()));
  connect(mHelp->addAction("Contents"), SIGNAL(triggered(bool)), SLOT(sHelp()));

  menuBar()->addMenu(mFile);
  menuBar()->addMenu(mView);
  menuBar()->addMenu(mHelp);

  QSettings s;
  if (s.contains("font"))
    cw->setDisplayFont(s.value("font").value<QFont>());
  else
    cw->setDisplayFont(QFont("FreeSerif", 24));
}

MainWindow::~MainWindow() {
}

void MainWindow::sAbout() {
  if (aboutText.isEmpty()) {
    QFile f(":/about.html");
    f.open(QFile::ReadOnly);
    QTextStream ts(&f);
    aboutText = ts.readAll();
  }
  QMessageBox::about(this, "About Uniword", aboutText);
}

void MainWindow::sHelp() {
  if (helpText.isEmpty()) {
    QFile f(":/help.html");
    f.open(QFile::ReadOnly);
    QTextStream ts(&f);
    helpText = ts.readAll();
  }
  
  QMessageBox::about(this, "Help for Uniword", helpText);
}

void MainWindow::sFont() {
  bool ok;
  QFont f = QFontDialog::getFont(&ok, cw->displayFont(), this, "Select font for character display");
  if (ok) {
    cw->setDisplayFont(f);
    QSettings s;
    s.setValue("font", QVariant(f));
  }
}

void MainWindow::sQuit() {
  QApplication::quit();
}
