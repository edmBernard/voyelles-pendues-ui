#include <vector>
#include <string>
#include <memory>

#include <QFile>
#include <QTextStream>

#include "engineinterface.h"

EngineInterface::EngineInterface(QObject *parent) : QObject(parent)
{

    QFile dictFile(":/datas/valid_word.txt");
    if(!dictFile.open(QIODevice::ReadOnly)) {
        throw std::runtime_error("Can't open dictionnary file");
    }
    QTextStream in(&dictFile);

    std::vector<std::string> wordList;

    while(!in.atEnd()) {
        QString line = in.readLine();
        wordList.push_back(line.toUtf8().constData());
    }

    dictFile.close();

    m_engine = std::make_unique<vowels::Engine>(5, wordList);
    m_numberWords = m_engine->getWordsToFindLength();


    const auto grid = m_engine->getGrid();
    const auto bloom = m_engine->getBloom();
    for (uint64_t i = 0; i < grid.size(); ++i) {
      QStandardItem* it = new QStandardItem();
      it->setData(QString("%0").arg(grid[i]), GridModel::role1);
      it->setData(QString("%0").arg(bloom[i]), GridModel::role2);
      it->setData(QString("%0").arg(0), GridModel::role3);
      m_gridModel.appendRow(it);
    }

    updateWildcardModel();
};
