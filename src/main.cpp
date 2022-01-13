#include <iostream>

#include "controller.h"
#include "game.h"
#include "gamer.h"
#include "renderer.h"

int main() {
  constexpr std::size_t kFramesPerSecond{60};
  constexpr std::size_t kMsPerFrame{1000 / kFramesPerSecond};
  constexpr std::size_t kScreenWidth{640};
  constexpr std::size_t kScreenHeight{640};
  constexpr std::size_t kGridWidth{32};
  constexpr std::size_t kGridHeight{32};
  std::string kFile{"../scores.txt"};

  std::string Nickname;
  std::cout << "Enter Nickname: ";
  getline(std::cin, Nickname);

  Renderer renderer(kScreenWidth, kScreenHeight, kGridWidth, kGridHeight);
  Controller controller;
  Game game(kGridWidth, kGridHeight);
  game.Run(controller, renderer, kMsPerFrame);
  std::cout << "Game has terminated successfully!\n";
  std::cout << "User: " << Nickname << "\n";
  std::cout << "Score: " << game.GetScore() << "\n";
  std::cout << "Size: " << game.GetSize() << "\n";
  std::cout << "---------------------------------\n";
  Gamer gamers(kFile);
  // gamers.LoadList(kFile);
  gamers.gamerAdd(Nickname, game.GetScore());
  gamers.printList();
  gamers.SaveList(kFile);
  return 0;
}
