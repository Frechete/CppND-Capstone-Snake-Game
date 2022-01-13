#ifndef SRC_GAMER_H_
#define SRC_GAMER_H_

#include <iostream>
#include <memory>
#include <string>
#include <utility>
#include <vector>

class Gamer {
 public:
  explicit Gamer(const std::string &File);
  void setName(std::string Name);
  std::string getName() const;
  void setScore(uint32_t Score);
  uint32_t getScore() const;

  void gamerAdd(std::string Name, uint32_t score);
  void printList();
  void SaveList(std::string File);
  void LoadList(std::string File);

 private:
  std::string _name;
  uint32_t _score{0};
  std::vector<std::pair<std::string, uint32_t>> _list;
};

#endif  // SRC_GAMER_H_
