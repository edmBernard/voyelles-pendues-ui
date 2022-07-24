#include <memory>
#include <string>
#include <vector>

#include <QFile>
#include <QTextStream>

#include "scoreinterface.h"

ScoresInterface::ScoresInterface(const QString& saveFolder, QObject *parent)
  : QObject(parent) {

//   QFile dictFile("saveFolder/best_scores.txt");
//   if (!dictFile.open(QIODevice::ReadOnly)) {
//     throw std::runtime_error("Can't open best score file");
//   }
//   QTextStream in(&dictFile);

//   while (!in.atEnd()) {
//     QString line = in.readLine();
// //    wordList.push_back(line.toUtf8().constData());
//   }

//   dictFile.close();

  for (int i = 0; i < 10; ++i) {
    QStandardItem *it = new QStandardItem();
    it->setData(i, BestScoreModel::score);
    it->setData(i, BestScoreModel::date);
    it->setData(i % 2, BestScoreModel::gridType);
    m_bestScoreModel.insertRow(0, it);
  }
};
