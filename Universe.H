// Universe.H

#ifndef UNIVERSE_H

#define UNIVERSE_H

#include <QSet>
#include <QChar>
#include <QString>
#include <QStringList>
#include <QMap>

class Universe {
public:
  Universe();
  virtual ~Universe();
  void add(int c, QString desc);
  void addperhaps(int c, QString desc);
  QSet<int> find(QStringList const &selectors) const;
  QSet<int> find(QString const &selector) const;
  QSet<int> all() const;
  QSet<int> refine(QSet<int> const &uverse, QString selector) const;
private:
  QMap<int, QSet<QString> > words;
};

#endif
