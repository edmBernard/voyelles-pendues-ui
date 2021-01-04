#include "engine.h"
#include <iostream>

Engine::Engine(std::vector<std::string> &&wordList) : m_wordList(wordList) {
    newPuzzle();
    std::cout << m_wordList.size() << std::endl;

};

void Engine::newPuzzle() {
}
