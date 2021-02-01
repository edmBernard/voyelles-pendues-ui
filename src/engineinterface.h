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

class EngineInterface : public QObject
{
  Q_OBJECT
public:
  explicit EngineInterface(QObject *parent = nullptr);

  Q_INVOKABLE GridModel *getGrid() {
    return &m_model;
  }

  Q_INVOKABLE QString getWord() {
    return m_engine->getWord(m_currentWordIndex).wildCard.data();
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
    emit updateWord();
  }
  Q_INVOKABLE void nextWord() {
    ++m_currentWordIndex;
    if (m_currentWordIndex >= m_numberWords) {
      m_currentWordIndex = 0;
    }
    emit updateWord();
  }
  Q_INVOKABLE void addLetter(uint64_t index) {
    std::cout << index << std::endl;
  }
  Q_INVOKABLE void cleanLetter() {
    std::cout << "clean Letter" << std::endl;
  }
signals:
  void updateWord();

  void updateGrid();

  void updateScore(uint64_t score);

  void notify(QString message);

public slots:
  void generateNewPuzzle() {
    m_engine->generateNewPuzzle();
    emit updateWord();
    emit updateGrid();
  }

  void search(QString word) {
    auto result = m_engine->search(word.toUtf8().constData());
    switch (result) {
    case vowels::SearchReturnCode::kWordInList:
      ++m_playerScore;
      --m_numberWords;
      emit updateWord();
      emit updateScore(m_playerScore);
      break;
    case vowels::SearchReturnCode::kWordExist:
      emit notify("Ce mot exist mais ce n'est pas celui qu'on cherche");
      break;
    default:
      emit notify("Ce mot n'exist pas");
    }
    emit updateWord();
  }


private:
  uint64_t m_currentWordIndex = 0;
  uint64_t m_numberWords;
  std::unique_ptr<vowels::Engine> m_engine;
  uint64_t m_playerScore = 0;
  GridModel m_model;
};

#endif // ENGINEINTERFACE_H
