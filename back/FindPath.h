#include <cmath>
#include <cstdlib>
#include <iostream>
#include <queue>
#include <vector>

#include "NiceMaze.h"

class WaveAlg {
 public:
  using Pairss = std::pair<short, short>;

  std::vector<Pairss> Find(MazeGen::Matrix<short>, MazeGen::Matrix<short>,
                           Pairss, Pairss);

  void SetBack(short *answer) {
    DelBack();
    back_to_python_ = answer;
  }
  short *GetBack() const { return back_to_python_; }
  void DelBack() { delete[] back_to_python_; }

  ~WaveAlg() { DelBack(); }

 private:
  bool LeftCheck();
  bool RightCheck();
  bool UpCheck();
  bool DownCheck();
  void FindCheck(std::queue<Pairss> &);
  void RecordCheck();
  short &Value(MazeGen::Matrix<short> &, Pairss &);
  void FindPath();
  void RecordPath();

  std::vector<Pairss> answer_;
  MazeGen::Matrix<short> wave_;
  MazeGen::Matrix<short> vert_;
  MazeGen::Matrix<short> horiz_;
  Pairss start_;
  Pairss end_;
  Pairss now_;
  Pairss buffer_move_;
  short *back_to_python_ = nullptr;
};

#ifdef __cplusplus
extern "C" {
#endif

WaveAlg *FindNew();
void FindDel(WaveAlg *wa);
short *PFind(WaveAlg *wa, MazeGen *mg, short *WhatFind);

#ifdef __cplusplus
}
#endif
