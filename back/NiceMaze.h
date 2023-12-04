#ifndef NICE_MAZE_H_
#define NICE_MAZE_H_

#include <algorithm>
#include <iostream>
#include <numeric>
#include <random>
#include <set>
#include <vector>

class MazeGen {
 public:
  template <typename T>
  using Matrix = std::vector<std::vector<T>>;

  void Generation(short, short);

  Matrix<short> GetVert() const { return vert_; }
  Matrix<short> GetHoriz() const { return horiz_; }

  void SetVertArr(short *array) {
    DeleteVertFlat();
    vert_array_ = array;
  }
  void SetHorArr(short *array) {
    DeleteHorFlat();
    hor_array_ = array;
  }

  short *GetVertArr() const { return vert_array_; }
  short *GetHorArr() const { return hor_array_; }

  ~MazeGen() {
    DeleteVertFlat();
    DeleteHorFlat();
  }

 private:
  void FormVert();
  void FormHoriz();
  void FormNewLine();
  void FormLastLine();
  short CounterEl(short &, short);

  void DeleteVertFlat() { delete[] vert_array_; }
  void DeleteHorFlat() { delete[] hor_array_; }

  short length_ = 0;
  short height_ = 0;
  short stat_set_ = 0;
  short *vert_array_ = nullptr;
  short *hor_array_ = nullptr;
  Matrix<short> vert_;
  Matrix<short> horiz_;
  std::vector<short> line_;
  std::vector<short> line_vert_;
  std::vector<short> line_horiz_;
};

#ifdef __cplusplus
extern "C" {
#endif

MazeGen *MazeGenNew();
void PGen(MazeGen *mz, short length, short height);
void MazeGenDel(MazeGen *mz);
short *PVert(MazeGen *mz);
short *PHoriz(MazeGen *mz);

#ifdef __cplusplus
}
#endif

#endif  // NICE_MAZE_H_
