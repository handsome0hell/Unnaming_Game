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
#ifndef UNNAMING_GAME_SRC_COMMON_POINT_H_
#define UNNAMING_GAME_SRC_COMMON_POINT_H_
#include <cassert>
#include <type_traits>
template<typename T>
class Point {
 public:
  inline Point(const T& a, const T& b): x(a), y(b) {}
  inline Point() {}
  inline bool operator<(const Point<T>& b) const {
    return x != b.x ? x < b.x : y < b.y;
  }
  inline bool operator==(const Point<T>& b) const {
    return x == b.x && y == b.y;
  }
  inline Point<T> operator-(const T& b) const {return Point(x - b, y - b);}
  inline Point<T> operator+(const T& b) const {return Point(x + b, y + b);}
  inline bool operator<(const T& b) const {return x < b && y < b;}
  inline bool operator>(const T& b) const {return x > b && y > b;}
  inline void operator-=(const T& b) {x -= b, y -= b;}
  template<typename TT>
  inline typename std::enable_if<std::is_signed<T>::value ||
                                 std::is_unsigned<TT>::value>::type
      operator-=(const Point<TT>& b) {x -= b.x, y -= b.y;}
  template<typename TT>
  inline typename std::enable_if<std::is_signed<T>::value ||
                                 std::is_unsigned<TT>::value>::type
      operator+=(const Point<TT>& b) {x += b.x, y += b.y;}
  template<typename TT>
  inline typename std::enable_if<std::is_unsigned<T>::value &&
                                 std::is_signed<TT>::value>::type
      operator-=(const Point<TT>& b) {
    if (b.x < 0) {
      x += static_cast<T>(-b.x);
    } else {
      x -= static_cast<T>(b.x);
    }
    if (b.y < 0) {
      y += static_cast<T>(-b.y);
    } else {
      y -= static_cast<T>(b.y);
    }
  }
  template<typename TT>
  inline typename std::enable_if<std::is_unsigned<T>::value &&
                                 std::is_signed<TT>::value>::type
      operator+=(const Point<TT>& b) {
    if (b.x < 0) {
      x -= static_cast<T>(-b.x);
    } else {
      x += static_cast<T>(b.x);
    }
    if (b.y < 0) {
      y -= static_cast<T>(-b.y);
    } else {
      y += static_cast<T>(b.y);
    }
  }
  template<typename TT> Point<T> operator-(const Point<TT>& b) const {
    auto ret = *this;
    ret -= b;
    return ret;
  }
  template<typename TT> Point<T> operator+(const Point<TT>& b) const {
    auto ret = *this;
    ret += b;
    return ret;
  }
  T x, y;  // There is no need to set them as private.
};
typedef Point<int> IntPoint;
#endif  // UNNAMING_GAME_SRC_COMMON_POINT_H_
