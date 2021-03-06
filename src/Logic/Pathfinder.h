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
#ifndef UNNAMING_GAME_SRC_LOGIC_PATHFINDER_H_
#define UNNAMING_GAME_SRC_LOGIC_PATHFINDER_H_
#include <cstdint>
#include <list>
#include <map>
#include <vector>
#include "../Map/Building/BaseBuilding.h"
#include "../Map/Map.h"
class PathFinder {
 public:
  std::list<MapPoint> FindShortestPath(const MapPoint&, const MapPoint&);
  inline void set_value(const Building& type, int value);
  void set_target_map(const Map& target);

 private:
  struct AstarStat {
    MapPoint self;
    PathFinder* original_finder;
  };
  void UpdateNearby(const MapPoint& now);
  bool TryAPoint(const Building& type, uint64_t walked_dis,
                 const MapPoint& now);
  void PushPointToAstarList(const MapPoint&);
  std::vector<int> value_;
  // A star data
  std::vector< std::vector<bool> > walked_;
  std::vector< std::vector<uint64_t> > walked_dis_;
  std::vector< std::vector<bool> > is_first_check_;
  std::vector< std::vector<MapPoint> > father_;
  std::list<MapPoint> searching_list;
  const Map* target_map_;
};
inline void PathFinder::set_value(const Building& type, int value) {
  const auto block_size = type.Size();
  if (value_.size() < block_size) value_.resize(block_size);
  value_[type.Index()] = value;
}
#endif  // UNNAMING_GAME_SRC_LOGIC_PATHFINDER_H_
