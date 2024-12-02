// Malte Hermann
// Game of Life

// TODO extend bounds by 1 each so no bounds checking :)
#include "gameoflife.hpp"

std::random_device randDevice;
std::mt19937 mersenneEngine{randDevice()};
std::uniform_int_distribution<uint32_t> dist{0, 1};

Life::Life() {
  this->board = std::vector<std::vector<uint32_t>>(height);
  auto gen = [&]() { return dist(mersenneEngine); };

  for (uint32_t i = 0; i < this->height; i++) {
    this->board[i] = std::vector<uint32_t>(this->width);
    std::generate(this->board[i].begin(), this->board[i].end(), gen);
  }
}

Life::Life(uint32_t width, uint32_t height) {
  this->width = width;
  this->height = height;
  this->board = std::vector<std::vector<uint32_t>>(height);
  auto gen = [&]() { return dist(mersenneEngine); };

#pragma omp parallel for
  for (uint32_t i = 0; i < this->height; i++) {
    this->board[i] = std::vector<uint32_t>(this->width);
    std::generate(this->board[i].begin(), this->board[i].end(), gen);
  }
}

void Life::updateCell(std::vector<std::vector<uint32_t>> oldboard,
                      uint32_t posX, uint32_t posY) {
  // this->board[Y][X]

  uint32_t aliveNeighbors = 0;
  uint32_t currentStatus = oldboard[posY][posX];

  if (posX != 0) {
    aliveNeighbors += oldboard[posY][posX - 1];
    if (posY != 0) {
      aliveNeighbors += oldboard[posY - 1][posX - 1];
      aliveNeighbors += oldboard[posY - 1][posX];
    }
    if (posY != this->height - 1) {
      aliveNeighbors += oldboard[posY + 1][posX - 1];
    }
  }

  if (posX != this->width - 1) {
    aliveNeighbors += oldboard[posY][posX + 1];
    if (posY != 0) {
      aliveNeighbors += oldboard[posY - 1][posX + 1];
    }
    if (posY != this->height - 1) {
      aliveNeighbors += oldboard[posY + 1][posX + 1];
      aliveNeighbors += oldboard[posY + 1][posX];
    }
  }

  if (aliveNeighbors < 2 && currentStatus)
    board[posY][posX] = 0;
  else if (aliveNeighbors > 3 && currentStatus)
    board[posY][posX] = 0;
  else if (aliveNeighbors == 3 && currentStatus == 0)
    board[posY][posX] = 1;
}

void Life::nextGeneration() {

  std::vector<std::vector<uint32_t>> oldboard = this->board;

#pragma omp parallel for
  for (uint32_t i = 0; i < this->height; i++) {
#pragma omp parallel for
    for (uint32_t j = 0; j < this->width; j++) {
      updateCell(oldboard, i, j);
    }
  }
  this->gencount += 1;

  if (!std::ranges::equal(this->board, oldboard)) {
    this->dupecount = 0;
  } else {
    this->dupecount += 1;
  }
}

void Life::printBoard() {
  uint32_t val = 0;
  std::cout << std::endl;
  for (uint32_t i = 0; i < this->height; i++) {
    for (uint32_t j = 0; j < this->height; j++) {
      val = this->board[i][j];
      if (val == 0)
        std::cout << " ";
      else if (val == 1)
        std::cout << "o";
    }
    std::cout << std::endl;
  }
}

void Life::runGameOfLife(uint32_t maxcycles) {
  while (this->gencount < maxcycles) {
    this->nextGeneration();
    if (this->gencount % 10 == 0 || this->gencount == 1) {
      std::cout << "Generation: " << this->gencount
                << "   Dupecount:" << this->dupecount << std::endl;
      this->printBoard();
    }
  }
}

int main(int argc, char **argv) {

  if (argc > 1) {
    if ("-h" == (std::string)argv[1]) {
      std::cout << "gameoflife -> defaults\ngameoflife boardwidth maxcycles"
                << std::endl;
      return 0;
    }
    uint32_t argwidth = atoi(argv[1]);
    uint32_t maxcycles = atoi(argv[2]);
    Life conway = Life(argwidth, argwidth);
    conway.runGameOfLife(maxcycles);
  } else {
    Life conway = Life();
    conway.runGameOfLife(1000);
  }

  return 0;
}
