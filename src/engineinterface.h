#ifndef ENGINEINTERFACE_H
#define ENGINEINTERFACE_H

#pragma once

#include <QObject>
#include <QStandardItemModel>

#include <memory>
#include <iostream>

#include "engine.h"

#include <QStandardItemModel>

constexpr int kScoreOnValid = 10;
constexpr int kScoreOnInvalid = 0;
constexpr int kScoreOnReset = -50;

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

  Q_INVOKABLE int getGridSize() {
    return m_gridSize;
  }

  Q_INVOKABLE uint64_t getIndex() {
    return m_currentWordIndex;
  }
  Q_INVOKABLE uint64_t getTotalRemaining() {
    return m_numberWords;
  }
  Q_INVOKABLE void previousWord() {
    incrIndex(-1);
    resetWildcardModel();
  }
  Q_INVOKABLE void nextWord() {
    incrIndex(1);
    resetWildcardModel();
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
    search();
    resetWildcardModel();
  }

  Q_INVOKABLE void generateNewPuzzle() {
    if (m_numberWords != 0) {
      incrScore(kScoreOnReset); // when the player manually reset
    }
    m_engine->generateNewPuzzle();
    resetGridModel();
    resetWildcardModel();
    m_numberWords = m_engine->getWordsToFindLength();
    emit updateMeta();
  }

  void search() {
    // Search was trigger when we release touch screen
    // create word from wildcard model
    QString word;
    for (int i = 0; i < m_wildcardModel.rowCount(); ++i) {
      auto standardItem = m_wildcardModel.item(i);
      word.append(standardItem->data(WildcardModel::role1).toString());
    }

    // search word in engine list
    auto result = m_engine->search(word.toUtf8().constData());
    switch (result) {
    case vowels::SearchReturnCode::kWordInList:
      incrScore(kScoreOnValid);
      --m_numberWords;
      if (m_currentWordIndex > 0) {
        incrIndex(-1);
      }
      if (m_numberWords == 0) {
        generateNewPuzzle();
        return;
      }
      resetGridModel(); // to update bloom filter
      break;
    case vowels::SearchReturnCode::kWordExist:
      incrScore(kScoreOnValid);
      emit notify("Ce mot exist mais ce n'est pas celui qu'on cherche");
      break;
    default:
      incrScore(kScoreOnInvalid);
      emit notify("Ce mot n'exist pas");
    }
  }

  Q_INVOKABLE void resetGridModel() {
    const auto grid = m_engine->getGrid();
    const auto bloom = m_engine->getBloom();
    m_gridModel.clear();
    for (uint64_t i = 0; i < grid.size(); ++i) {
      QStandardItem* it = new QStandardItem();
      it->setData(QString("%0").arg(grid[i]), GridModel::role1);
      it->setData(QString("%0").arg(bloom[i]), GridModel::role2);
      it->setData(QString("%0").arg(0), GridModel::role3);
      m_gridModel.appendRow(it);
    }
  }

  Q_INVOKABLE void resetWildcardModel() {
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

  void updateScore(uint64_t score);

  void notify(QString message);

public slots:

private:
  void incrScore(int value) {
    m_playerScore += value;
    if (m_playerScore < 0) {
      m_playerScore = 0;
    }
    emit updateScore(m_playerScore);
  }

  void incrIndex(int value) {
    m_currentWordIndex += value;
    if (m_currentWordIndex < 0) {
        m_currentWordIndex = m_numberWords - 1;
        return;
    }
    if (m_currentWordIndex >= m_numberWords) {
        m_currentWordIndex = 0;
        return;
    }
  }
  int64_t m_currentWordIndex = 0;
  uint64_t m_numberWords = 0;
  std::vector<uint64_t> m_pressedIndex;

  std::unique_ptr<vowels::Engine> m_engine;
  uint64_t m_gridSize = 3;
  int64_t m_playerScore = 0;
  GridModel m_gridModel;
  WildcardModel m_wildcardModel;
};

#endif // ENGINEINTERFACE_H
