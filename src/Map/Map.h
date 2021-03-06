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
#ifndef UNNAMING_GAME_SRC_MAP_MAP_H_
#define UNNAMING_GAME_SRC_MAP_MAP_H_
#include <cassert>
#include <cstdint>
#include <functional>
#include <list>
#include <memory>
#include <vector>
#include "./Building/Building.h"
#include "../Common/Point.h"
typedef Point<size_t> MapPoint;
struct Rect {
  size_t w, h;
};
class Space;
class Map;
typedef std::shared_ptr< Map > Map_ref;
class Map final {
 public:
  struct RectWithPos {
    MapPoint left_top;
    Rect size;
  };
  static Map_ref Create(const Rect&);
  static Map_ref Create(Space*, const Rect&);
  // Basic attributes
  size_t Id();
  const Rect& Size() const;
  size_t Width() const;
  size_t Height() const;
  Space* space() const;
  void SetDestroy(const std::function<void()>&);
  // Layer
  const Building& GroundIn(const MapPoint&) const;
  const Building& BuildingIn(const MapPoint&) const;
  void SetGroundIn(const MapPoint&, const Building&);
  void SetBuildingIn(const MapPoint&, const Building&);
  void DestroyGroundIn(const MapPoint&);
  ~Map();
  void ForEachGround(const std::function< void(Building*) >& applier);
  void ForEachGroundIn(const RectWithPos& region,
                      const std::function< void(Building*) >& applier);
  void ForEachBuilding(const std::function< void(Building*) >& applier);
  void Link();
  void Unlink();
  MapPoint PickRandomPointIn(const std::list<Building>& valid_list) const;
  void CopyFromIn(const Map&, const MapPoint&);
  inline bool has(const IntPoint& pos) const {
    if (pos < 0) return false;
    return static_cast<size_t>(pos.x) < Width() &&
           static_cast<size_t>(pos.y) < Height();
  }
  inline bool has(const MapPoint& pos) const {
    return pos.x < Width() && pos.y < Height();
  }

 private:
  explicit Map(Space*, const Rect&);
  void Init();
  void get_id();
  size_t GetIndex(const MapPoint& pos) const {
    assert(has(pos));
    return pos.y*Width() + pos.x;
  }
  Building* GroundPtrIn(const MapPoint& pos);
  Building* BuildingPtrIn(const MapPoint& pos);
  static size_t kMapSize;
  const Rect size_;
  Space* space_;
  bool is_got_id_;
  size_t id_;
  int links_num_;
  std::function<void()> destroy_;
  std::vector<Building> blocks_;
  std::vector<Building> buildings_;
};
#endif  // UNNAMING_GAME_SRC_MAP_MAP_H_
