#include "NiceMaze.h"

// Функции для использования в питон-коде

MazeGen *MazeGenNew() { return new MazeGen(); }

void MazeGenDel(MazeGen *mz) { delete mz; }

void PGen(MazeGen *mz, short length, short height) {
  mz->Generation(length, height);
}

short *PVert(MazeGen *mz) {
  MazeGen::Matrix<short> vert = mz->GetVert();
  mz->SetVertArr(new short[vert.size() * vert[0].size()]);
  short *flat_array = mz->GetVertArr();

  size_t index = 0;
  for (const auto &row : vert) {
    for (const short value : row) {
      flat_array[index] = value;
      ++index;
    }
  }
  return flat_array;
}

short *PHoriz(MazeGen *mz) {
  MazeGen::Matrix<short> horiz = mz->GetHoriz();
  mz->SetHorArr(new short[horiz.size() * horiz[0].size()]);
  short *flat_array = mz->GetHorArr();

  size_t index = 0;
  for (const auto &row : horiz) {
    for (const short value : row) {
      flat_array[index] = value;
      ++index;
    }
  }
  return flat_array;
}

// Основные функции класса генерации лабиринта

void MazeGen::Generation(short length, short height) {
  length_ = length;
  height_ = height;
  vert_.clear();
  horiz_.clear();
  line_.clear();
  line_vert_.clear();
  line_horiz_.clear();
  stat_set_ = 0;

  line_.resize(length_);
  line_vert_.resize(length_);
  line_horiz_.resize(length_);

  std::iota(line_.begin(), line_.end(), 1);
  stat_set_ = length_ + 1;

  for (size_t k = 0; k < height_; ++k) {
    FormVert();
    FormHoriz();

    vert_.push_back(line_vert_);
    horiz_.push_back(line_horiz_);

    if (k != height_ - 1) {
      FormNewLine();
    } else {
      FormLastLine();
    }
  }
}

short MazeGen::CounterEl(short &now_set, short it) {
  short count = 0;
  for (size_t i = it;; ++i) {
    if (line_[i] == now_set) {
      ++count;
    } else {
      break;
    }
  }
  return count;
}

void MazeGen::FormVert() {
  std::random_device rd;
  std::mt19937 gen(rd());

  short min = 0;
  short max = 1;
  std::uniform_int_distribution<> dis(min, max);

  for (size_t i = 0; i < line_.size() - 1; ++i) {
    short flag_gen = dis(gen);
    if (flag_gen) {
      line_vert_[i] = 1;
    } else if (line_[i] == line_[i + 1]) {
      line_vert_[i] = 1;
    } else {
      short check = line_[i + 1];
      for (size_t j = 0; j < line_.size(); ++j) {
        if (line_[j] == check) {
          line_[j] = line_[i];
        }
      }
    }
  }
}

void MazeGen::FormHoriz() {
  std::random_device rd;
  std::mt19937 gen(rd());

  short min = 0;
  short max = 1;
  std::uniform_int_distribution<> dis(min, max);

  for (size_t i = 0; i < line_.size(); ++i) {
    short now_set = line_[i];
    short count_set = CounterEl(now_set, i);
    short count_busy = 0;
    if (count_set > 1) {
      short stop_gen = i + count_set;
      for (size_t j = i; j < stop_gen; ++j, ++i) {
        short flag_gen = dis(gen);
        if (flag_gen && count_busy + 1 < count_set) {
          line_horiz_[i] = 1;
          ++count_busy;
        }
      }
    }
  }
}

void MazeGen::FormNewLine() {
  for (size_t i = 0; i < line_.size(); ++i) {
    line_vert_[i] = 0;
    if (line_horiz_[i] == 1) {
      line_horiz_[i] = 0;
      line_[i] = stat_set_++;
    }
  }
}

void MazeGen::FormLastLine() {
  for (size_t i = 0; i < line_.size() - 1; ++i) {
    if (line_[i] != line_[i + 1]) {
      vert_[vert_.size() - 1][i] = 0;
      short check = line_[i + 1];
      for (size_t j = 0; j < line_.size(); ++j) {
        if (line_[j] == check) {
          line_[j] = line_[i];
        }
      }
    }
    horiz_[horiz_.size() - 1][i] = 1;
  }
  horiz_[horiz_.size() - 1].back() = 1;
}