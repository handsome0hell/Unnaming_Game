// This program is free software: you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation, either version 3 of the License, or
// (at your option) any later version.
//
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.
//
// You should have received a copy of the GNU General Public License
// along with this program.  If not, see <https://www.gnu.org/licenses/>.
#include "shadowcasting.h"
#include <cstring>
#include <cassert>
#include <cmath>
#include "./fov_cmake.h"
#include "../Common/Point.h"
static const int kMult[8][4] = {
    {-1, 0, 0, -1}, {1, 0, 0, -1}, {-1, 0, 0, 1}, {1, 0, 0, 1},
    {0, -1, -1, 0}, {0, -1, 1, 0}, {0, 1, 1, 0}, {0, 1, -1, 0}
};
FOV_EXPORT void FunctorShadowCasting::operator()(const MapPoint& pos,
                                                 const size_t& radius) {
  if (IsValid(pos)) {
    for (int i = 0; i < 8; ++i) {
      CastLight(pos, radius, 1, 0, 1, kMult[i]);
    }
  }
}
FOV_EXPORT void FunctorShadowCasting::CastLight(
    const MapPoint& pos, const size_t& radius, const int& column,
    IntFraction start, IntFraction end, const int mult[4]) {
  if (start < 0) start = 0;
  if (end > 1) end = 1;
  if (start >= end) return;
  // Hold 0 < start < end < 1
  // Too far to see
  intmax_t radius_squared = static_cast<intmax_t>(radius)*
                            static_cast<intmax_t>(radius);
  intmax_t dx_squared = static_cast<intmax_t>(column)*column;
  bool costly = false;
  IntFraction new_start = start;
  for (int dy = 0; dy <= column; ++dy) {
    // Translate the dx, dy coordinates into map coordinates.
    // * dx equals to column
    IntPoint delta(column*mult[0] + dy*mult[1], column*mult[2] + dy*mult[3]);
    if ((delta.x < 0 && pos.x < static_cast<size_t>(-delta.x)) ||
        (delta.y < 0 && pos.y < static_cast<size_t>(-delta.y))) break;
    MapPoint now_pos = pos + delta;
    IntFraction lower_slope(2*dy - 1, 2*column + 1),
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
    // TODO(handsome0hell): Check overflow here.
    if (static_cast<intmax_t>(dy)*dy + dx_squared > radius_squared) continue;
    // Our light beam is touching this block; light it.
    int now_cost = GetCost(now_pos);
    SetViewable(now_pos);
    if (costly) {
      // we're scanning a column of costly blocks.
      if (now_cost != 0) {
        if (now_cost < 0 || radius > static_cast<unsigned int>(now_cost)) {
          CastLight(pos, radius - static_cast<unsigned int>(now_cost),
                    column + 1, IntFraction(2*dy - 1, 2*column - 1),
                    upper_slope, mult);
        }
        new_start = upper_slope;
        continue;
      } else {
        costly = false;
        start = new_start;
      }
    } else if (now_cost != 0) {
      // This is a costly block, start a child scan.
      costly = true;
      CastLight(pos, radius, column + 1, start, lower_slope, mult);
      if (now_cost < 0 || radius > static_cast<unsigned int>(now_cost)) {
        CastLight(pos, radius - static_cast<unsigned int>(now_cost), column + 1,
                  lower_slope, upper_slope, mult);
      }
      new_start = upper_slope;
    }
  }
  if (!costly && static_cast<unsigned int>(column) + 1 <= radius) {
    // Scan next column in the same area
    CastLight(pos, radius, column + 1, start, end, mult);
  }
}
