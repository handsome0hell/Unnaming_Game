// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.
//
// You should have received a copy of the GNU General Public License
// along with this program.  If not, see <https://www.gnu.org/licenses/>.
#ifndef UNNAMING_GAME_SRC_MAP_BASEBUILDING_H_
#define UNNAMING_GAME_SRC_MAP_BASEBUILDING_H_
#include <memory>
#include "./BuildingPtr.h"
class BaseBuilding final {
 public:
  static size_t BaseBuildingSize();
  operator BuildingPtr();
  BaseBuilding();
  BaseBuilding(const BuildingPtr&);

 private:
  class Implementation;
  std::shared_ptr<Implementation> impl_;
};
#endif  // UNNAMING_GAME_SRC_MAP_BASEBUILDING_H_