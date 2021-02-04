#include <QStandardItemModel>

#pragma once

class GridModel : public QStandardItemModel
{

public:

  enum Role {
    letter=Qt::UserRole,
    bloom,
    selected
  };


  explicit GridModel(QObject * parent = 0): QStandardItemModel(parent)
  {
  }
  explicit GridModel( int rows, int columns, QObject * parent = 0 ): QStandardItemModel(rows, columns, parent)
  {
  }

  QHash<int, QByteArray> roleNames() const
  {
    QHash<int, QByteArray> roles;
    roles[letter] = "letter";
    roles[bloom] = "bloom";
    roles[selected] = "selected";
    return roles;
  }
};

class WildcardModel : public QStandardItemModel
{

public:

  enum Role {
    letter=Qt::UserRole,
    empty,
  };


  explicit WildcardModel(QObject * parent = 0): QStandardItemModel(parent)
  {
  }
  explicit WildcardModel( int rows, int columns, QObject * parent = 0 ): QStandardItemModel(rows, columns, parent)
  {
  }

  QHash<int, QByteArray> roleNames() const
  {
    QHash<int, QByteArray> roles;
    roles[letter] = "letter";
    roles[empty] = "empty";
    return roles;
  }
};

class FoundWordModel : public QStandardItemModel
{

public:

  enum Role {
    word=Qt::UserRole,
    wasInList
  };


  explicit FoundWordModel(QObject * parent = 0): QStandardItemModel(parent)
  {
  }
  explicit FoundWordModel(int rows, int columns, QObject * parent = 0): QStandardItemModel(rows, columns, parent)
  {
  }

  QHash<int, QByteArray> roleNames() const
  {
    QHash<int, QByteArray> roles;
    roles[word] = "word";
    roles[wasInList] = "wasInList";
    return roles;
  }
};
