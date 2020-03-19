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
#include <renderer_export.h>  // Generated by Cmake
#include <Renderer.h>
#include <cstdint>
#include <iostream>
#include "../Map/Map.h"
#include "../Object/Creature.h"
RENDERER_EXPORT Renderer::~Renderer() {}
RENDERER_EXPORT void Renderer::UpdateBlockTypeSize(size_t size) {
  exterior_of_block_.resize(size);
}
RENDERER_EXPORT
void Renderer::set_exterior_of_block(const char exterior,
                                     const Map::BlockType& type) {
  exterior_of_block_[type] = exterior;
}
RENDERER_EXPORT void Renderer::RenderCreaturesView(const Creature& obj) const {
  for (int32_t j = 0; j < ((obj.view_dis() << 1) | 1); ++j) {
    for (int32_t i = 0; i < ((obj.view_dis() << 1) | 1); ++i) {
      const Point tmp = {obj.now_position().x - obj.view_dis() +
                         static_cast< int32_t >(i),
                         obj.now_position().y - obj.view_dis() +
                         static_cast< int32_t >(j)};
      if (tmp == obj.now_position()) {
        std::cout << "@";
      } else if (obj.is_viewable(tmp)) {
        std::cout << exterior_of_block_[obj.now_map()->Block(tmp)];
      } else {
        std::cout << ' ';
      }
    }
    std::cout << '\n';
  }
}
RENDERER_EXPORT void Renderer::RenderGameMap(const Map& map) const {
  for (int32_t j = 0; j < map.Height(); ++j) {
    for (int32_t i = 0; i < map.Width(); ++i) {
      Map::BlockType now_block = map.Block({i, j});
      std::cout << exterior_of_block_[now_block];
    }
    std::cout << "\n";
  }
}
RENDERER_EXPORT void Renderer::RenderMemory(
                         const Space::MemoryOfMap& mem) const {
  for (int32_t j = mem.left_top.y; j <= mem.right_bottom.y; ++j) {
    for (int32_t i = mem.left_top.x; i <= mem.right_bottom.x; ++i) {
      if (mem.is_seen[i][j]) {
        Map::BlockType now_block = mem.detail->Block({i, j});
        std::cout << exterior_of_block_[now_block];
      } else {
        std::cout << ' ';
      }
    }
    std::cout << "\n";
  }
}
RENDERER_EXPORT Renderer_ref Renderer::Create() {
  return Renderer_ref(new Renderer);
}
RENDERER_NO_EXPORT Renderer::Renderer() {}
