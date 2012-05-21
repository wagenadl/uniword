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
  void addgroup(int c, QString desc);
  QSet<int> find(QStringList const &selectors) const;
  QSet<int> find(QString const &selector) const;
  QSet<int> all() const;
  QSet<int> refine(QSet<int> const &uverse, QString selector) const;
  QString describe(int) const;
  QString getgroups(int) const;
private:
  void addDescription(int, QString);
  void addGroup(int, QString);
private:
  QMap<int, QSet<QString> > words;
  QMap<int, QString> descs;
  QMap<int, QStringList> groups;
};

#endif
