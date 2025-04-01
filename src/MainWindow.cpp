// MainWindow.cpp

/*
    Uniword: keyword-based unicode character selector
    Copyright (C) 2012-2025  Daniel A. Wagenaar <daw@caltech.edu>

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

  connect(mFile->addAction("Quit"), &QAction::triggered, this, &MainWindow::sQuit);
  connect(mView->addAction("Font..."), &QAction::triggered, this, &MainWindow::sFont);
  QAction *a;
  a = mView->addAction("Show glyphs imported from substitute fonts");
  a->setCheckable(true);
  a->setChecked(false);
  connect(a, &QAction::triggered, this, &MainWindow::sMerging);
  cw->setMerging(false);
  a = mView->addAction("Show codepoints without any available glyph");
  a->setCheckable(true);
  a->setChecked(false);
  connect(a, &QAction::triggered, this, &MainWindow::sMissing);
  cw->setExclude(true);
   
  connect(mHelp->addAction("About"), &QAction::triggered, this, &MainWindow::sAbout);
  connect(mHelp->addAction("Contents"), &QAction::triggered, this, &MainWindow::sHelp);

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

void MainWindow::sMissing(bool x) {
  cw->setExclude(!x);
}


void MainWindow::sMerging(bool x) {
  cw->setMerging(x);
}
