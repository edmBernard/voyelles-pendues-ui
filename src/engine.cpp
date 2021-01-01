#include "engine.h"
#include <iostream>

Engine::Engine(std::vector<std::string> &&wordList) : m_wordList(wordList) {
    newPuzzle();
    std::cout << m_wordList.size() << std::endl;
    for (auto word : m_wordList) {
        std::cout << word << std::endl;
    }
};

void Engine::newPuzzle() {
}
