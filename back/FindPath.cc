#include "FindPath.h"

// Функции для использования в питон-коде

WaveAlg *FindNew() { return new WaveAlg(); }

void FindDel(WaveAlg *wa) { delete wa; }

short *PFind(WaveAlg *wa, MazeGen *mg, short *WhatFind) {
  MazeGen::Matrix<short> VM = mg->GetVert();
  MazeGen::Matrix<short> HM = mg->GetHoriz();
  short size = VM.size();

  WaveAlg::Pairss start(WhatFind[0], WhatFind[1]);
  WaveAlg::Pairss end(WhatFind[2], WhatFind[3]);
  std::vector<WaveAlg::Pairss> answer = wa->Find(VM, HM, start, end);
  // short *back = new short[answer.size() * 2 + 1];

  wa->SetBack(new short[answer.size() * 2 + 1]);
  short *back = wa->GetBack();

  back[0] = answer.size() * 2 + 1;
  for (int i = 0, k = 1; i < answer.size(); ++i, k += 2) {
    back[k] = answer[i].first;
    back[k + 1] = answer[i].second;
  }
  return back;
}

// Основные функции класса поиска пути

std::vector<WaveAlg::Pairss> WaveAlg::Find(MazeGen::Matrix<short> vert,
                                           MazeGen::Matrix<short> horiz,
                                           Pairss start, Pairss end) {
  answer_.clear();
  MazeGen::Matrix<short> wave(vert.size(), std::vector<short>(vert[0].size()));
  vert_ = std::move(vert);
  horiz_ = std::move(horiz);
  wave_ = std::move(wave);
  start_ = std::move(start);
  end_ = std::move(end);
  wave_[start.first][start.second] = 1;
  FindPath();
  RecordPath();
  return answer_;
}

bool WaveAlg::LeftCheck() {
  if (buffer_move_.second >= 0 &&
      vert_[buffer_move_.first][buffer_move_.second] == 0) {
    return true;
  }
  return false;
}

bool WaveAlg::RightCheck() {
  if (buffer_move_.second < wave_[0].size() &&
      vert_[now_.first][now_.second] == 0) {
    return true;
  }
  return false;
}

bool WaveAlg::UpCheck() {
  if (buffer_move_.first >= 0 &&
      horiz_[buffer_move_.first][buffer_move_.second] == 0) {
    return true;
  }
  return false;
}

bool WaveAlg::DownCheck() {
  if (buffer_move_.first < wave_.size() &&
      horiz_[now_.first][now_.second] == 0) {
    return true;
  }
  return false;
}

void WaveAlg::FindCheck(std::queue<Pairss> &step_wave) {
  if (Value(wave_, buffer_move_) == 0) {
    Value(wave_, buffer_move_) = Value(wave_, now_) + 1;
    step_wave.push({buffer_move_.first, buffer_move_.second});
  }
}

void WaveAlg::RecordCheck() {
  if (Value(wave_, buffer_move_) == Value(wave_, now_) - 1) {
    answer_.push_back({buffer_move_.first, buffer_move_.second});
  }
}

short &WaveAlg::Value(MazeGen::Matrix<short> &mas, Pairss &coord) {
  return mas[coord.first][coord.second];
}

void WaveAlg::FindPath() {
  bool running = true;
  now_ = start_;
  std::queue<Pairss> step_wave;

  while (running) {
    buffer_move_ = {now_.first, now_.second - 1};
    if (LeftCheck()) {
      FindCheck(step_wave);
    }

    buffer_move_ = {now_.first, now_.second + 1};
    if (RightCheck()) {
      FindCheck(step_wave);
    }

    buffer_move_ = {now_.first - 1, now_.second};
    if (UpCheck()) {
      FindCheck(step_wave);
    }

    buffer_move_ = {now_.first + 1, now_.second};
    if (DownCheck()) {
      FindCheck(step_wave);
    }
    now_ = step_wave.front();
    step_wave.pop();
    if (Value(wave_, end_) != 0) {
      running = false;
    }
  }
}

void WaveAlg::RecordPath() {
  bool running = true;
  now_ = end_;
  answer_.push_back(end_);
  while (running) {
    buffer_move_ = {now_.first, now_.second - 1};
    if (LeftCheck()) {
      RecordCheck();
    }

    buffer_move_ = {now_.first, now_.second + 1};
    if (RightCheck()) {
      RecordCheck();
    }

    buffer_move_ = {now_.first - 1, now_.second};
    if (UpCheck()) {
      RecordCheck();
    }

    buffer_move_ = {now_.first + 1, now_.second};
    if (DownCheck()) {
      RecordCheck();
    }
    now_ = answer_.back();
    if (now_ == start_) {
      running = false;
    }
  }
}
