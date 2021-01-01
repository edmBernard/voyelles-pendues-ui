#ifndef ENGINE_H
#define ENGINE_H

#pragma once

#include <vector>
#include <string>

class Engine
{
public:
    Engine(std::vector<std::string> &&wordList);

    void newPuzzle();
    void createGrid();
    const std::vector<std::string>& getGrid() const;

    uint64_t getWordListLength() const { return m_wordsInternalIndex.size(); };

    const std::string& getWordWildcard(uint64_t index) const { return m_wordListWithWildcard[index]; }
    const std::string& getWordComplete(uint64_t index) const { return m_wordList[index]; }

private:
    std::vector<uint64_t> m_wordsInternalIndex;
    std::vector<std::string> m_grid;
    std::vector<std::string> m_wordList;
    std::vector<std::string> m_wordListWithoutVowels;
    std::vector<std::string> m_wordListWithWildcard;
};

#endif // ENGINE_H
