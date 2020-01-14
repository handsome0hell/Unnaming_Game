#include <fov_export.h>  // Generated by Cmake
#include <cstring>
#include <cassert>
#include <cmath>
#include "shadowcasting.h"
static const int8_t kMult[8][4] = {
    {-1, 0, 0, -1}, {1, 0, 0, -1}, {-1, 0, 0, 1}, {1, 0, 0, 1},
    {0, -1, -1, 0}, {0, -1, 1, 0}, {0, 1, 1, 0}, {0, 1, -1, 0}
};
FOV_EXPORT void FunctorShadowCasting::operator()(const Point& pos,
                                                 const int32_t& radius) {
  if (IsValid(pos)) {
    for (int i = 0; i < 8; ++i) {
      CastLight(pos, radius, 1, 0, 1, kMult[i]);
    }
  }
}
FOV_EXPORT void FunctorShadowCasting::CastLight(
    const Point& pos, const int32_t& radius, const int32_t& column,
    FractionType start, FractionType end, const int8_t mult[4]) {
  if (start < 0) start = 0;
  if (end > 1) end = 1;
  if (start >= end || radius <= 0) return;
  // Hold 0 < start < end < 1
  // Too far to see
  uint64_t radius_squared = static_cast<uint64_t>(radius)*radius;
  uint64_t dx_squared = static_cast<uint64_t>(column)*column;
  bool costly = false;
  FractionType new_start = start;
  for (int32_t dy = 0; dy <= column; ++dy) {
    // Translate the dx, dy coordinates into map coordinates.
    // *dx equals to column
    Point now_pos = {pos.x + column*mult[0] + dy*mult[1],
                     pos.y + column*mult[2] + dy*mult[3]};
    FractionType lower_slope(2*dy - 1, 2*column + 1),
                 upper_slope(2*dy + 1, 2*column - 1);
    // Store the slopes of the lower and upper
    // extremities of the block we're considering.
    //     / 1 +------+
    //    /    |      |
    //   /   0 |      |
    //  /      |      |
    // +---- -1+------+
    //         -1  1  0
    if (start >= upper_slope) continue;
    else if (end <= lower_slope) break;
    if (static_cast<uint64_t>(dy)*dy + dx_squared > radius_squared) continue;
    // Our light beam is touching this block; light it.
    int32_t now_cost = GetCost(now_pos);
    SetViewable(now_pos);
    if (costly) {
      // we're scanning a column of costly blocks.
      if (now_cost != 0) {
        CastLight(pos, radius - now_cost, column + 1,
                  FractionType(2*dy - 1, 2*column - 1), upper_slope, mult);
        new_start = upper_slope;
        continue;
      }else {
        costly = false;
        start = new_start;
      }
    }else if (now_cost != 0) {
      // This is a costly block, start a child scan.
      costly = true;
      CastLight(pos, radius, column + 1, start, lower_slope, mult);
      CastLight(pos, radius - now_cost, column + 1, lower_slope, upper_slope,
                mult);
      new_start = upper_slope;
    }
  }
  if (!costly && column + 1 <= radius) {
    // Scan next column in the same area
    CastLight(pos, radius, column + 1, start, end, mult);
  }
}
