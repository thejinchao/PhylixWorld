#ifndef __PHYSICS_WORLD_LIBEX_CONVEX_PARTITION_H__
#define __PHYSICS_WORLD_LIBEX_CONVEX_PARTITION_H__

namespace Phylix
{

/** 将凹多边形分割成数个凸多边形, 并调整顶点顺序为指定的顺序
@param polygon 需要分割的多边形，可以使凸多边形也可以使凹多边形，但不能自相交
@param convexList 分割后的凸多边形列表
@param cw 指定一个方向，true表示顺时针false表示逆时针，分割后的凸多边形数据以及传入的凹多边形数据会根据这个参数作调整
*/
void convexPartition(Polygon& polygon, std::list< Polygon >& convexList, bool cw);

}

#endif
