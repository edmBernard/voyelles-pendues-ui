#ifndef ENGINEINTERFACE_H
#define ENGINEINTERFACE_H

#pragma once

#include <QObject>
#include <QStandardItemModel>

#include <memory>
#include <iostream>

#include "engine.h"

#include <QStandardItemModel>

class GridModel : public QStandardItemModel
{

public:

  enum Role {
    role1=Qt::UserRole,
    role2,
    role3
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
    roles[role1] = "letter";
    roles[role2] = "bloom";
    roles[role3] = "selected";
    return roles;
  }
};

class WildcardModel : public QStandardItemModel
{

public:

  enum Role {
    role1=Qt::UserRole,
    role2,
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
    roles[role1] = "letter";
    roles[role2] = "empty";
    return roles;
  }
};

class EngineInterface : public QObject
{
  Q_OBJECT
public:
  explicit EngineInterface(QObject *parent = nullptr);

  Q_INVOKABLE GridModel *getGrid() {
    return &m_gridModel;
  }

  Q_INVOKABLE WildcardModel *getWord() {
    return &m_wildcardModel;
  }
  Q_INVOKABLE uint64_t getIndex() {
    return m_currentWordIndex;
  }
  Q_INVOKABLE uint64_t getTotalRemaining() {
    return m_numberWords;
  }
  Q_INVOKABLE void previousWord() {
    if (m_currentWordIndex == 0) {
      m_currentWordIndex = m_numberWords - 1;
    } else {
      --m_currentWordIndex;
    }
    updateWildcardModel();
  }
  Q_INVOKABLE void nextWord() {
    ++m_currentWordIndex;
    if (m_currentWordIndex >= m_numberWords) {
      m_currentWordIndex = 0;
    }
    updateWildcardModel();
  }
  Q_INVOKABLE void addLetter(uint64_t index) {
    if (index >= 0 && index < m_engine->getGridSize() * m_engine->getGridSize()) {
      if (fillWildcard(index)) {
        auto item = m_gridModel.item(index);
        item->setData(QString("%0").arg(1), GridModel::role3);
      }
    }
  }
  Q_INVOKABLE void cleanLetter() {
    for (uint64_t i = 0; i < m_engine->getGridSize() * m_engine->getGridSize(); ++i) {
      auto item = m_gridModel.item(i);
      item->setData(QString("%0").arg(0), GridModel::role3);
    }
    updateWildcardModel();
  }

  Q_INVOKABLE void generateNewPuzzle() {
    m_engine->generateNewPuzzle();
    updateWildcardModel();
    emit updateGrid();
  }

  void search() {
    // Search was trigger when we release touch screen
    // create word from wildcard model
    for (auto it = word.begin(); it != word.end(); ++it) {
      QStandardItem* standardItem = new QStandardItem();
      standardItem->setData(QString("%0").arg(*it), WildcardModel::role1);
      standardItem->setData(QString("%0").arg(*it == '*'), WildcardModel::role2);
      m_wildcardModel.appendRow(standardItem);
    }

    // search word in engine list
    auto result = m_engine->search(word.toUtf8().constData());
    switch (result) {
    case vowels::SearchReturnCode::kWordInList:
      ++m_playerScore;
      --m_numberWords;
      updateWildcardModel();
      emit updateScore(m_playerScore);
      break;
    case vowels::SearchReturnCode::kWordExist:
      emit notify("Ce mot exist mais ce n'est pas celui qu'on cherche");
      break;
    default:
      emit notify("Ce mot n'exist pas");
    }
    updateWildcardModel();
  }

  Q_INVOKABLE void updateWildcardModel() {
    const std::string wildcard = m_engine->getWord(m_currentWordIndex).wildCard;
    const QString word(wildcard.data());
    m_wildcardModel.clear();
    m_pressedIndex.clear();
    for (auto it = word.begin(); it != word.end(); ++it) {
      QStandardItem* standardItem = new QStandardItem();
      standardItem->setData(QString("%0").arg(*it), WildcardModel::role1);
      standardItem->setData(QString("%0").arg(*it == '*'), WildcardModel::role2);
      m_wildcardModel.appendRow(standardItem);
    }

    emit updateMeta();
  }

  Q_INVOKABLE bool fillWildcard(uint64_t index) {
    if (index >= m_engine->getGrid().size()) {
      return false;
    }
    if (m_pressedIndex.size() != 0) {
      // Check if index already pressed
      auto found = std::find(m_pressedIndex.begin(), m_pressedIndex.end(), index);
      if (found != m_pressedIndex.end()) {
        return false;
      }
    }
    m_pressedIndex.push_back(index);
    for (int i = 0; i < m_wildcardModel.rowCount(); ++i) {
      auto standardItem = m_wildcardModel.item(i);
      if (standardItem->data(WildcardModel::role2) == "1") {
        standardItem->setData("0", WildcardModel::role2);
        standardItem->setData(QString("%0").arg(m_engine->getGrid()[index]), WildcardModel::role1);
        return true;
      }
    }
    return false;
  }


signals:
  void updateMeta();

  void updateGrid();

  void updateScore(uint64_t score);

  void notify(QString message);

public slots:

private:
  uint64_t m_currentWordIndex = 0;
  uint64_t m_numberWords = 0;
  std::vector<uint64_t> m_pressedIndex;

  std::unique_ptr<vowels::Engine> m_engine;
  uint64_t m_playerScore = 0;
  GridModel m_gridModel;
  WildcardModel m_wildcardModel;
};

#endif // ENGINEINTERFACE_H
