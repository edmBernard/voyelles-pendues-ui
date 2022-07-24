#pragma once

#include <QObject>
#include <QStandardItemModel>
#include <QStandardPaths>

#include <iostream>
#include <memory>

#include "models.h"

class ScoresInterface : public QObject {
  Q_OBJECT
public:
  explicit ScoresInterface(const QString& saveFolder, QObject *parent = nullptr);

  Q_INVOKABLE BestScoreModel *getBestScores() { return &m_bestScoreModel; }

private:
  std::vector<int> bestScores;
  BestScoreModel m_bestScoreModel;
};
