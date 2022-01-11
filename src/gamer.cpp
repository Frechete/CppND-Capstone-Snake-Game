#include "gamer.h"

#include <algorithm>
#include <fstream>
#include <iomanip>
#include <memory>
#include <sstream>

Gamer::Gamer(const std::string &Name, uint32_t Score)
    : _name(Name), _score(Score) {}

void Gamer::setName(const std::string Name) { _name = Name; }

std::string Gamer::getName() const { return _name; }

void Gamer::setScore(const uint32_t Score) { _score = Score; }

uint32_t Gamer::getScore() { return _score; }

void Gamer::gamerAdd(std::string Name, uint32_t score) {
  // std::unique_ptr<Gamer> gamer;
  // gamer = new Gamer(Name, score);
  _list.emplace_back(Name, score);
}

void Gamer::printList() {
  std::sort(_list.begin(), _list.end(),
            [](auto const &left, auto const &right) {
              return left.second > right.second;
            });
  for (auto const &gamer : _list) {
    std::cout << gamer.second << "\t\t" << gamer.first << "\n";
  }
}

void Gamer::SaveList(std::string File) {
  std::ofstream myFile(File);
  if (myFile.is_open()) {
    for (auto const &gamer : _list) {
      myFile << gamer.first << "\t" << gamer.second << "\n";
    }
    myFile.close();
  }
}

void Gamer::LoadList(std::string File) {
  std::ifstream myFile(File);
  std::string line;

  if (myFile.is_open()) {
    while (getline(myFile, line)) {
      std::istringstream stream(line);
      std::string name;
      uint32_t score;
      std::istringstream linestream(line);
      linestream >> name >> score;
      gamerAdd(name, score);
    }
    myFile.close();
  }
}
