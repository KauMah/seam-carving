#include "seam_carver.hpp"

// implement the rest of SeamCarver's functions here

using namespace std;

const ImagePPM& SeamCarver::GetImage() const {
  return image_;
}

int SeamCarver::GetHeight() const {
  return height_;
}

int SeamCarver::GetWidth() const {
  return width_;
}

int SeamCarver::GetEnergy(int row, int col) const {
  Pixel top = image_.GetPixel(row - 1, col);
  Pixel bot = image_.GetPixel(row + 1, col);
  Pixel lef = image_.GetPixel(row, col - 1);
  Pixel rig = image_.GetPixel(row, col + 1);

  return (top.GetRed() - bot.GetRed())*(top.GetRed() - bot.GetRed()) +
         (top.GetGreen() - bot.GetGreen())*(top.GetGreen() - bot.GetGreen()) +
         (top.GetBlue() - bot.GetBlue())*(top.GetBlue() - bot.GetBlue()) +
         (rig.GetRed() - lef.GetRed())*(rig.GetRed() - lef.GetRed()) +
         (rig.GetGreen() - lef.GetGreen())*(rig.GetGreen() - lef.GetGreen()) +
         (rig.GetBlue() - lef.GetBlue())*(rig.GetBlue() - lef.GetBlue());
}

int* SeamCarver::GetHorizontalSeam() const {
  int* out = new int[width_];
  int** e = new int*[height_];
  for (int row = 0; row < height_; row++) e[row] = new int[width_];
  for (int row = 0; row < height_; row++) for (int col = 0; col < width_; col++) e[row][col] = GetEnergy(row, col);
  // // -------
  // for (int i=0;i<height_;i++) {
  //   for(int j=0;j<width_;j++) cout << e[i][j] << '\t';
  //   cout << endl;
  // }
  // cout << endl;
  // // --------
  for (int col = width_ - 2; col >= 0; col--) {
    for (int row = 0; row < height_; ++row) {
      if (row + 1 >= height_ && !(row - 1 < 0)) e[row][col] += e[row - 1][col + 1] > e[row][col + 1] ? e[row][col + 1] : e[row - 1][col + 1];
      else if (row - 1 < 0) { e[row][col] += e[row + 1][col + 1] > e[row][col + 1] ? e[row][col + 1] : e[row + 1][col + 1];}
      else {
        int low = e[row + 1][col + 1] < e[row][col + 1] ? e[row + 1][col + 1] : e[row][col + 1];
        e[row][col] += low < e[row - 1][col + 1] ? low : e[row - 1][col + 1];
      }
    }
  }
  // // -------
  // for (int i=0;i<height_;i++) {
  //   for(int j=0;j<width_;j++) cout << e[i][j] << '\t';
  //   cout << endl;
  // }
  // cout << endl;
  // // --------
  for (int col = 0; col < width_; col++) {
    int min = 2147483646, minr = 0;
    if (col == 0) {
      for (int row = height_ - 1; row >= 0; row--) if (e[row][col] <= min) {
        out[0] = row;
        min = e[row][col];
      }
    } else {
      int row = out[col - 1];
      for (int i = 1; i >= -1; i -= 2) {
        if (row + i >= height_ || row + i < 0) continue;
        else if (e[row + i][col] <= min) {
          min = e[row + i][col];
          minr = row + i;
        }
      }
      if (e[row][col] <= min) out[col] = row;
      else out[col] = minr;
    }
  }
  for (int row = 0; row < height_; row++) delete[] e[row];
  delete[] e;
  // // -----
  // cout << endl;
  // for (int i=0;i<width_;i++) cout << out[i] << ' ';
  // // -----
  return out;
}

void SeamCarver::RemoveHorizontalSeam() {
  int* seam = GetHorizontalSeam();
  image_.unshiftRow(seam);
  height_--;
  delete[] seam;
}

int* SeamCarver::GetVerticalSeam() const {
  int* out = new int[height_];
  int** e = new int*[height_];
  for (int row = 0; row < height_; row++) e[row] = new int[width_];
  for (int row = 0; row < height_; row++) for (int col = 0; col < width_; col++) e[row][col] = GetEnergy(row, col);
  // // -------
  // for (int i=0;i<height_;i++) {
  //   for(int j=0;j<width_;j++) cout << e[i][j] << '\t';
  //   cout << endl;
  // }
  // cout << endl;
  // // --------
  for (int row = height_ - 2; row >= 0; --row) {
    for (int col = 0; col < width_; ++col) {
      if (col + 1 >= width_ && !(col - 1 < 0)) e[row][col] += e[row + 1][col - 1] > e[row + 1][col] ? e[row + 1][col] :e[row + 1][col - 1];
      else if (col - 1 < 0) e[row][col] += e[row + 1][col + 1] > e[row + 1][col] ? e[row + 1][col] : e[row + 1][col + 1];
      else {
        int low = e[row + 1][col - 1] < e[row + 1][col] ? e[row + 1][col - 1] : e[row + 1][col];
        e[row][col] += low < e[row + 1][col + 1] ? low : e[row + 1][col + 1];
      }
    }
  }
  // // ---------
  // for (int i=0;i<height_;i++) {
  //   for(int j=0;j<width_;j++) cout << e[i][j] << '\t';
  //   cout << endl;
  // }
  // // ----------
  for (int row = 0; row < height_; row++) {
    int min = 2147483647, minc = width_ - 1;
    if (row == 0) {
      for (int col = width_ - 1; col >= 0; col--) if (e[row][col] <= min) {
        out[0] = col;
        min = e[row][col];
      }
    } else {
      int col = out[row - 1];
      for (int i = 1; i >= -1; i -= 2) {
        if (col + i > width_ || col + i < 0) continue;
        else if (e[row][col + i] <= min) {
          min = e[row][col + i];
          minc = col + i;
        }
      }
      if (e[row][col] <= min) out[row] = col;
      else out[row] = minc;
    }
  }
  for (int row = 0; row < height_; row++) delete[] e[row];
  delete[] e;
  // // -----
  // cout << endl;
  // for (int i=0;i<height_;i++) cout << out[i] << ' ';
  // // -----
  return out;
}

void SeamCarver::RemoveVerticalSeam() {
  int* seam = GetVerticalSeam();
  image_.unshiftCol(seam);
  width_--;
  delete[] seam;
}
// given functions below, DO NOT MODIFY

SeamCarver::SeamCarver(const ImagePPM& image): image_(image) {
  height_ = image.GetHeight();
  width_ = image.GetWidth();
}

void SeamCarver::SetImage(const ImagePPM& image) {
  image_ = image;
  width_ = image.GetWidth();
  height_ = image.GetHeight();
}
