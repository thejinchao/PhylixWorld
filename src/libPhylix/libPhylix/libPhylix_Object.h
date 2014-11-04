#ifndef __PHYSICS_WORLD_LIB_OBJECT_H__
#define __PHYSICS_WORLD_LIB_OBJECT_H__

#include "libPhylix_Material.h"

namespace Phylix
{
//pre-define
class IRenderInterface;
class ObjectManager;
class ObjectParams;
class MassBody;

/** 放置在场景中所有物体基类
*/
class Object
{
public:
	enum Type
	{
		OT_NULL = 0,

		OT_ENTITY_EDGE,		//!< 边界
		OT_ENTITY_BOX,		//!< 盒子
		OT_ENTITY_CIRCLE,	//!< 圆形
		OT_ENTITY_POLYGON,	//!< 多边形
		OT_FIXED_JOINT,	//!< 焊接点
		OT_HINGE_JOINT,	//!< 旋转铰链
		OT_SPRING_JOINT,//!< 弹簧
		OT_CHAIN,		//!< 链子
	};
public:
	/** 获得类型 */
	virtual Type getType(void) const = 0;
	/** 获得id */
	unsigned int getID(void) const { return m_id; }
	/** 获得z值*/
	Real getZOrder(void) const { return m_zOrder; }
	/** 渲染 */
	virtual void draw(IRenderInterface* render) = 0;
	/** 获得材质 */
	const Material& getMaterial(void) const { return m_material; }
	/** 更新材质 */
	virtual void updateMaterial(const Material& material) { }
	/** 重新构造创建参数 */
	virtual void buildCreateParam(ObjectParams& param)=0;
	/** 通知被选中 */
	virtual void notifySelected(void) {}
	/** 通知即将被删除 */
	virtual void notifyDelete(void) {}

public:
	/** 工具函数，判断某种类型是否是实体 */
	static bool isEntityType(Type type) { return type==OT_ENTITY_EDGE || type==OT_ENTITY_BOX || type==OT_ENTITY_CIRCLE || type==OT_ENTITY_POLYGON; }
	/** 工具函数，判断某种类型是否是铰链 */
	static bool isJointType(Type type) { return type==OT_FIXED_JOINT || type==OT_HINGE_JOINT || type==OT_SPRING_JOINT; }

protected:
	unsigned int m_id;		//!< 唯一id
	Real m_zOrder;			//!< z方向值
	Material m_material;	//!< 材质

protected:
	Object(unsigned int id);
	virtual ~Object();
	friend class ObjectManager;
};

//按照z轴排序所有Obj
struct ZOrderSortHelper
{
	bool operator()(const Object* p1, const Object* p2)
	{
		return p1->getZOrder() < p2->getZOrder();
	}
};
typedef std::multiset< Object*, ZOrderSortHelper > ZOrderSet;

}

#endif
