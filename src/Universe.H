// Universe.H

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
  bool isCombiner(int) const;
  bool isModifier(int) const;
private:
  void addDescription(int, QString);
  void addGroup(int, QString);
private:
  QMap<int, QSet<QString> > words;
  QMap<int, QString> descs;
  QMap<int, QStringList> groups;
  QSet<int> combiners;
  QSet<int> modifiers;
};

#endif
