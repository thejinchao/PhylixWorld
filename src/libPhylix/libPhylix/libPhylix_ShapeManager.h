#ifndef __PHYSICS_WORLD_LIB_SHAPE_MANAGER_H__
#define __PHYSICS_WORLD_LIB_SHAPE_MANAGER_H__

#include "libPhylix_Handler.h"

namespace Phylix
{
//pre-define
class Shape;
class Material;
class ObjectEntity;

/** 用于生成、管理、销毁形状
*/
class ShapeManager
{
public:
	enum { COLLISION_DEFAULT_LAYER = 1, COLLISION_ALL_LAYER = CP_ALL_LAYERS };

public:
	/** 创建一个标准的矩形形状 */
	Shape* createBoxShape(ObjectEntity* owner, const fVector2& size, const Material* material);
	/** 创建一个标准的圆形形状 */
	Shape* createCircleShape(ObjectEntity* owner, Real radius, bool axisValid, const Angle& ang, const Material* material);
	/** 创建一个标准的边界形状 */
	Shape* createEdgeShape(ObjectEntity* owner, const fVector2& ptCenter, const Angle& normal, const Material* material);
	/** 创建一个多边形形状 */
	Shape* createPolygon(ObjectEntity* owner, const PointVector& ptArray, const Material* material, fVector2& ptCenter);
	/** 删除一个形状 */
	void destroyShape(Shape* shape);

public:
	ShapeManager();
	~ShapeManager();
};

}

#endif
