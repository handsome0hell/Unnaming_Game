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
#include <map_export.h>  // Generated by Cmake
#include <functional>
#include "Map.h"
constexpr Map::Target Map::kNullTarget;
int32_t Map::kMapSize = 0;
MAP_EXPORT Point Map::PickARandomPointInGroundOrPath(
    const std::function< int32_t(int32_t, int32_t) >& ran,
    const std::list< Map::BlockType >& valid_list) const {
  int32_t total_valid = 0;
  for (int32_t y = 0; y < height_; ++y) {
    for (int32_t x = 0; x < width_; ++x) {
      for (auto i : valid_list) {
        if (i == block_[x][y]) ++total_valid;
      }
    }
  }
  total_valid = ran(0, total_valid - 1);
  for (int32_t y = 0; y < height_; ++y) {
    for (int32_t x = 0; x < width_; ++x) {
      for (auto i : valid_list) {
        if (i == block_[x][y]) {
          if (total_valid-- == 0) return Point({x, y});
        }
      }
    }
  }
  return Point({0, 0});
}
MAP_EXPORT Map_ref Map::Create(int32_t w, int32_t h) {
  return Map_ref(new Map(w, h));
}
MAP_NO_EXPORT Map::Map(int32_t w, int32_t h) : width_(w), height_(h) {
  block_.resize(w, std::vector< BlockType >(h));
  is_got_id_ = false;
}
MAP_EXPORT int32_t Map::Id() {
  if (!is_got_id_) get_id();
  return id_;
}
MAP_EXPORT int32_t Map::Width() const {return width_;}
MAP_EXPORT int32_t Map::Height() const {return height_;}
MAP_EXPORT const Map::BlockType& Map::Block(const Point& pos) const {
  return block_[pos.x][pos.y];
}
MAP_EXPORT void Map::SetBlock(const Point& pos, const BlockType& block) {
  block_[pos.x][pos.y] = block;
}
MAP_EXPORT const Map::BuildingType& Map::Building(const Point& pos) const {
  return building_[pos.x][pos.y];
}
MAP_EXPORT void Map::SetBuilding(const Point& pos,
                         const BuildingType& building) {
  if (building_[pos.x][pos.y] == building) return;
  if (building_[pos.x][pos.y] == kBuildingPortal) {
    SetPortalTarget(pos, {nullptr, {0, 0}});
  }
  building_[pos.x][pos.y] = building;
}
MAP_EXPORT const Map::Target& Map::PortalTarget(const Point& pos) const {
  auto ret = portal_target_.find(pos);
  if (ret == portal_target_.end()) {
    return kNullTarget;
  } else {
    return ret->second;
  }
}
MAP_EXPORT void Map::SetPortalTarget(const Point& pos, const Target& target) {
  auto finder = portal_target_.find(pos);
  if (target.map == nullptr) {
    if (finder != portal_target_.end()) {
      portal_target_.erase(finder);
    }
  } else {
    portal_target_[pos] = target;
  }
}
MAP_EXPORT Map::~Map() {}
MAP_NO_EXPORT void Map::get_id() {
  if (!is_got_id_) {
    id_ = kMapSize++;
    is_got_id_ = true;
  }
}
MAP_EXPORT void Map::ForEachBuilding(
    const std::function< void(BuildingType*) >& applier) {
  for (int32_t y = 0; y < height_; ++y) {
    for (int32_t x = 0; x < width_; ++x) {
      applier(&(building_[x][y]));
    }
  }
}
MAP_EXPORT void Map::ForEachBlock(
    const std::function< void(BlockType*) >& applier) {
  for (int32_t y = 0; y < height_; ++y) {
    for (int32_t x = 0; x < width_; ++x) {
      applier(&(block_[x][y]));
    }
  }
}
MAP_EXPORT
void Map::ForEachBlockIn(const RectWithPos& region,
                         const std::function< void(BlockType*) >& applier) {
  const int32_t end_y = region.left_top.y + region.size.h;
  const int32_t end_x = region.left_top.x + region.size.w;
  for (int32_t y = region.left_top.y; y < end_y; ++y) {
    for (int32_t x = region.left_top.x; x < end_x; ++x) {
      applier(&(block_[x][y]));
    }
  }
}
