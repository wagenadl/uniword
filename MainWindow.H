// MainWindow.H

#ifndef MAINWINDOW_H

#define MAINWINDOW_H

#include <QString>
#include <QWidget>

class MainWindow: public QWidget {
  Q_OBJECT;
public:
  MainWindow(class Universe const &uverse, QString fontname,
	     QWidget *parent=0);
  virtual ~MainWindow();
private slots:
  void edited(QString const &s);
  void copyAvailable(bool s);
  void selected();
  void clipinfo();
private:
  void setComment(int c);
  void setEmptyComment();
  void setMultiComment();
private:
  class QTextEdit *output;
  class QLineEdit *input;
  class QTextEdit *comment;
  Universe const &uverse;
  bool copyAv;
  QString fontname;
  QString itsmine;
};

#endif
