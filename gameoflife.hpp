#pragma once

#include <algorithm>
#include <iostream>
#include <omp.h>
#include <random>
#include <stdint.h>
#include <stdlib.h>
#include <vector>

class Life {
private:
  uint32_t width{32};
  uint32_t height{32};

protected:
  std::vector<std::vector<uint32_t>> board{};
  uint32_t gencount{0};
  uint32_t dupecount{0};

public:
  Life();
  Life(uint32_t width, uint32_t height);
  void updateCell(std::vector<std::vector<uint32_t>> oldboard, uint32_t posX,
                  uint32_t posY);
  void nextGeneration();
  void printBoard();
  void runGameOfLife(uint32_t maxcycles);
};