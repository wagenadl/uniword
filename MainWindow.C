// MainWindow.C

#include <QLabel>
#include <QLineEdit>
#include <QStringList>
#include <QRegExp>
#include <QDebug>
#include <QApplication>
#include <QClipboard>

#include "MainWindow.H"
#include "Universe.H"

#define FONT "Linux Libertine"
#define fg QColor("#000000")
#define bg QColor("#eeeeee")

MainWindow::MainWindow(Universe const &uverse, QWidget *parent):
  QWidget(parent),
  uverse(uverse) {
  resize(300,200);
  input = new QLineEdit(this);
  input->setGeometry(10,10,280,50);
  output = new QLabel(this);
  output->setWordWrap(true);
  output->setTextInteractionFlags(Qt::TextSelectableByMouse);
  output->setGeometry(10,80,280,110);
    output->setFont(QFont(FONT, 15));
    output->setText("(type some words)");
  QPalette p = palette();
  p.setColor(QPalette::Button,bg); // this is what our children use
  p.setColor(QPalette::Window,bg); // this is what our children use
  p.setColor(QPalette::ButtonText,fg);
  p.setColor(QPalette::WindowText,fg);
  setPalette(p);

  connect(input, SIGNAL(textChanged(QString const &)),
	  SLOT(edited(QString const &)));
}

MainWindow::~MainWindow() {
}

void MainWindow::edited(QString const &s) {
  if (QRegExp("[\\s.]*").exactMatch(s)) {
    output->setFont(QFont(FONT, 15));
    output->setText("(type some words)");
    return;
  }

  QStringList lst = s.split(QRegExp("\\s+"));

  QSet<int> cc = uverse.find(lst);
  if (cc.isEmpty()) {
    output->setFont(QFont(FONT, 15));
    output->setText("(none)");
  } else {
    QString s = "";
    QList<int> ordered;
    ordered = cc.toList();
    qSort(ordered);
    //    if (ordered.size()>150)
    //      ordered.resize(150);
    foreach (int c, ordered) {
      s += " ";
      if (c>=65536) {
	s += QChar(QChar::highSurrogate(c));
	s += QChar(QChar::lowSurrogate(c));
      } else {
	s += QChar(c);
      }
    }
    output->setText(s.mid(1));
    if (cc.size()>80)
      output->setFont(QFont(FONT, 8));
    else if (cc.size()>40)
      output->setFont(QFont(FONT, 12));
    else if (cc.size()>30)
      output->setFont(QFont(FONT, 14));
    else if (cc.size()>20)
      output->setFont(QFont(FONT, 18));
    else if (cc.size()>10)
      output->setFont(QFont(FONT, 24));
    else
      output->setFont(QFont(FONT, 32));
    if (cc.size()==1) 
      QApplication::clipboard()->setText(QString(*cc.begin()));
  }
}
