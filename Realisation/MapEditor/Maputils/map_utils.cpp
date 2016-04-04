#include "map_utils.hpp"
#include "stubs.hpp"
#include <vector>

std::vector< std::vector<RectInfo> > RectInfo_from_map_using_tiles(Map map, int amountx, int amounty) {
    std::vector< std::vector<RectInfo> > result;

    for (int y = 0; y < amounty; y++) {
        result.push_back(std::vector<RectInfo>());
    }
    Box aabb = map.get_bounding_box(); //Axis Aligned Bounding Box
    int deltax = aabb.getTopRight().get_x() - aabb.getBottomLeft().get_x();
    int deltay = aabb.getBottomLeft().get_y() - aabb.getTopRight().get_y();

    int sizex = deltax / amountx;
    int sizey = deltay / amounty;

    for (int y = 0; y < amounty; y++) {
        for (int x = 0; x < amountx; x++) {
            Box current_box = Box(Coordinate(sizex * x, sizey * y, 0), Coordinate(sizex * (x + 1), sizey * (y + 1), 1));
            BoxInfo info = map.get_box_info(current_box);
            TileType new_type = TileType::UNKNOWN;
            if (info.has_navigatable)   new_type = TileType::EMPTY;
            if (info.has_obstacles)     new_type = TileType::BLOCKED;
            result[y].push_back(RectInfo(current_box, new_type));
        }
    }
}
//
// std::vector< std::vector<RectInfo> > RectInfo_from_map_using_quadtree(Map map, Box size) {
//     std::vector<RectInfo> boxes;
//     BoxInfo info = map.get_box_info(current_box);
//     RectInfo rectinfo = RectInfo(aabb, TileType::UNKNOWN);
//
//     return get_rectinfo_recursive(map, aabb, size, rectinfo);
// }
//
// //NOT FINISHED
// RectInfo get_rectinfo_recursive(Map map, Box total, RectInfo& parent) {
//
//     sizex = total.getTopRight().get_x() - total.getBottomLeft().get_x();
//     sizey = total.getTopRight().get_y() - total.getBottomLeft().get_y();
//
//     // Box origin_box(Coordinate(0,0,0), Coordinate(sizex, sizey, 1))
//
//     int i = 0;
//
//     TileType new_type = TileType::UNKNOWN;
//     if (info.has_navigatable)   new_type = TileType::EMPTY;
//     if (info.has_unknown)       new_type = TileType::UNKNOWN;
//     if (info.has_obstacles)     new_type = TileType::BLOCKED;
//
//     if (info.has_navigatable)   i++;
//     if (info.has_unknown)       i++;
//     if (info.has_obstacles)     i++;
//
//     if (i > 1) {
//         new_type = TileType::MIXED;
//     }
//
//     RectInfo current_info = RectInfo(new_box, new_type);
//
//     if (i > 1) {
//         sizex /= 2;
//         sizey /= 2;
//         for (int y = 0; y < 2; y++) {
//             for (int x = 0; x < 2; x++) {
//                 //Omzetten naar de goede hoeken :/
//                 current_box = Box(Coordinate(total.getBottomLeft().get_x() + , total.getTopRight().get_y(), 0),
//                                   Coordinate(total.getTopRight().get_x(), total.getBottomLeft().get_y(), 1))
//             }
//         }
//         current_box = Box(Coordinate(x_l, y_l, 0),
//                         Coordinate(x_r, y_r, 1))
//         RectInfo new_info = get_rectinfo_recursive(map, current_box, info);
//         boxes.push_back(info);
//     }
//     else {
//         parent.add_rekt(current_info);
//     }
// }