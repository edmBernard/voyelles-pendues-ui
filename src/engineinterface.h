#ifndef ENGINEINTERFACE_H
#define ENGINEINTERFACE_H

#pragma once

#include <QObject>
#include <QStandardItemModel>

#include <iostream>
#include <memory>

#include "engine.h"
#include "models.h"

constexpr int kScoreOnInvalid = 0;
constexpr int kScoreOnReset = -50;

class EngineInterface : public QObject {
  Q_OBJECT
public:
  explicit EngineInterface(int gridSize, int wordsPerPuzzle, QObject *parent = nullptr);

  Q_INVOKABLE GridModel *getGrid() { return &m_gridModel; }
  Q_INVOKABLE WildcardModel *getWord() { return &m_wildcardModel; }
  Q_INVOKABLE FoundWordModel *getFoundWords() { return &m_foundwordModel; }
  Q_INVOKABLE int getGridSize() { return m_gridSize; }
  Q_INVOKABLE uint64_t getFound() { return m_foundWords.size(); }
  Q_INVOKABLE int64_t getTotal() { return m_numberWords; }

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
        item->setData(QString("%0").arg(1), GridModel::selected);
      }
    }
  }
  Q_INVOKABLE void cleanLetter() {
    for (uint64_t i = 0; i < m_engine->getGridSize() * m_engine->getGridSize(); ++i) {
      auto item = m_gridModel.item(i);
      item->setData(QString("%0").arg(0), GridModel::selected);
    }
    search();
    resetWildcardModel();
  }

  Q_INVOKABLE void generateNewPuzzle() {
    if (m_numberWords != m_foundWords.size()) {
      incrScore(kScoreOnReset); // when the player manually reset
    }
    m_foundWords.clear();
    m_foundwordModel.clear();
    m_currentWordIndex = 0;
    m_engine->generateNewPuzzle();
    m_numberWords = m_engine->getWordsToFindLength();

    resetGridModel();
    resetWildcardModel();
    emit updateMeta();
  }

  void search() {
    // Search was trigger when we release touch screen
    // create word from wildcard model
    QString word;
    for (int i = 0; i < m_wildcardModel.rowCount(); ++i) {
      auto standardItem = m_wildcardModel.item(i);
      word.append(standardItem->data(WildcardModel::letter).toString());
    }

    // search word in engine list
    auto result = m_engine->search(word.toUtf8().constData());

    switch (result) {

    case vowels::SearchReturnCode::kWordInList:
      addFoundWord(word);
      incrScore(m_scoreReserve);
      resetScoreReserve();
      if (m_currentWordIndex >= m_numberWords - m_foundWords.size()) {
        incrIndex(-1);
      }
      if (m_numberWords - m_foundWords.size() <= 0) {
        generateNewPuzzle();
        return;
      }
      resetGridModel(); // to update bloom filter
      break;

    case vowels::SearchReturnCode::kWordExist:
      if (addFoundWord(word)) {
        incrScore(m_gridSize * m_gridSize);
      }
      break;

    default:
      decrScoreReserve();
      incrScore(kScoreOnInvalid);
    }
  }

  Q_INVOKABLE void resetGridModel() {
    const auto grid = m_engine->getGrid();
    const auto bloom = m_engine->getBloom();
    m_gridModel.clear();
    for (uint64_t i = 0; i < grid.size(); ++i) {
      QStandardItem *it = new QStandardItem();
      it->setData(QString("%0").arg(grid[i]), GridModel::letter);
      it->setData(QString("%0").arg(bloom[i]), GridModel::bloom);
      it->setData(QString("%0").arg(0), GridModel::selected);
      m_gridModel.appendRow(it);
    }
  }

  Q_INVOKABLE void resetWildcardModel() {
    const std::string wildcard = m_engine->getWord(m_currentWordIndex).wildCard;
    const QString word(wildcard.data());
    m_wildcardModel.clear();
    m_pressedIndex.clear();
    for (auto it = word.begin(); it != word.end(); ++it) {
      QStandardItem *standardItem = new QStandardItem();
      standardItem->setData(QString("%0").arg(*it), WildcardModel::letter);
      standardItem->setData(QString("%0").arg(*it == '*'), WildcardModel::empty);
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
      if (standardItem->data(WildcardModel::empty) == "1") {
        standardItem->setData("0", WildcardModel::empty);
        standardItem->setData(QString("%0").arg(m_engine->getGrid()[index]), WildcardModel::letter);
        return true;
      }
    }
    return false;
  }

signals:
  void updateMeta();

  void updateScore(uint64_t score);

  void notify(QString message);

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
      m_currentWordIndex = m_numberWords - m_foundWords.size() - 1;
      return;
    }
    if (m_currentWordIndex >= m_numberWords - m_foundWords.size()) {
      m_currentWordIndex = 0;
      return;
    }
  }
  void decrScoreReserve() {
    if (m_scoreReserve > 0) {
      --m_scoreReserve;
    }
  }
  void resetScoreReserve() {
    m_scoreReserve = m_gridSize * m_gridSize;
  }

  bool addFoundWord(const QString &word) {
    const std::string wordStr = word.toUtf8().constData();
    auto result = std::find(m_foundWords.begin(), m_foundWords.end(), wordStr);
    if (result != m_foundWords.end()) {
      return false;
    }
    m_foundWords.push_back(wordStr);
    QStandardItem *it = new QStandardItem();
    it->setData(word, FoundWordModel::word);
    m_foundwordModel.insertRow(0, it);
    return true;
  }

  int m_gridSize;
  int m_wordsPerPuzzle;

  int64_t m_currentWordIndex = 0;
  std::vector<std::string> m_foundWords;
  int64_t m_numberWords = 0;
  std::vector<uint64_t> m_pressedIndex;

  std::unique_ptr<vowels::Engine> m_engine;
  int64_t m_playerScore = 0;
  uint8_t m_scoreReserve;
  GridModel m_gridModel;
  WildcardModel m_wildcardModel;
  FoundWordModel m_foundwordModel;
};

#endif // ENGINEINTERFACE_H
