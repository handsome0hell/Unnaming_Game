//    Copyright (C) 2018  Handsome0hell
//
//    This program is free software: you can redistribute it and/or modify
//    it under the terms of the GNU General Public License as published by
//    the Free Software Foundation, either version 3 of the License, or
//    (at your option) any later version.
//
//    This program is distributed in the hope that it will be useful,
//    but WITHOUT ANY WARRANTY; without even the implied warranty of
//    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
//    GNU General Public License for more details.
//
//    You should have received a copy of the GNU General Public License
//    along with this program.  If not, see <https://www.gnu.org/licenses/>.
//
//    Email: handsome0hell@gmail.com
#ifndef UNNAMING_GAME_SRC_INTERFACE_OBJECT_H_
#define UNNAMING_GAME_SRC_INTERFACE_OBJECT_H_
#include "../Map/Map.h"
#include <cstdint>
#include <vector>
#include <map>
class Object {
 public:
  struct CostOfBlock {
    int32_t destory;
    int32_t move;
    int32_t see_through;
  };
  inline Object() {
    have_id_ = false;
  }
  virtual inline ~Object() = default;
  virtual inline void set_view_dis(const int32_t& d) {
    view_dis_ = d;
  };
  virtual inline const int32_t view_dis() const {
    return view_dis_;
  } 
  virtual inline void set_now_map(Map* const map) {
    now_map_ = map;
  }
  virtual inline Map& now_map() const {
    return *now_map_;
  }
  virtual inline void set_now_pos(const Point& pos) {
    now_pos_ = pos;
  }
  virtual inline const Point now_pos() const {
    return now_pos_;
  }
  virtual inline void set_cost(const Map::BlockType& t, const CostOfBlock& d) {
    cost_[t] = d;
  }
  inline const int32_t id();
  virtual bool is_viewable(const Point& pos) const = 0;
  virtual World::MemoryOfMap& GetMemory() = 0;
  virtual void GoTo(const Point& des) = 0;
  virtual void Transfer(const Point& des, const Map::BlockType& to) = 0;

 protected:
  virtual void get_id() = 0;
  virtual inline const CostOfBlock& cost(const Map::BlockType& t) {
    return cost_[t];
  }
  inline void set_id(int32_t id);

 private:
  int32_t id_;
  bool have_id_;
  Map* now_map_;
  Point now_pos_;
  int32_t view_dis_;
  CostOfBlock cost_[Map::kBlockMax];
};
inline const int32_t Object::id() {
  if (!have_id_) get_id();
  return id_;
}
inline void Object::set_id(int32_t id) {
  have_id_ = true;
  id_ = id;
}
#endif  // UNNAMING_GAME_SRC_INTERFACE_OBJECT_H_
