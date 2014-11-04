#ifndef __PHYSICS_WORLD_LIB_OBJECT_JOINT_FIXED_H__
#define __PHYSICS_WORLD_LIB_OBJECT_JOINT_FIXED_H__

#include "libPhylix_Object_Joint.h"
#include "libPhylix_MassBody.h"

namespace Phylix
{
/** 焊接点，用于固定连接物体
*/
class ObjectFixedJoint : public ObjectJoint
{
public:
	/** 获得类型 */
	virtual Type getType(void) const { return OT_FIXED_JOINT; }
	/** 渲染 */
	virtual void draw(IRenderInterface* render);
	/** 某点是否在该点示意图范围内，用于计算鼠标选择*/
	virtual bool isInLegendArea(const fVector2& pt) const;
	/** 重新构造创建参数 */
	virtual void buildCreateParam(ObjectParams& param);
	/** 获得所删除掉的first质量体数据*/
	const MassBody::cpDataCache& getFirstDeleteMassBody(void) const { return m_firstDeleteMassBody; }
	/** 获得所删除掉的second质量体数据*/
	const MassBody::cpDataCache& getSecondDeleteMassBody(void) const { return m_secondDeleteMassBody; }

protected:
	Real  m_radius;
	//因为创建该焊接点，删除掉的质量体数据
	MassBody::cpDataCache m_firstDeleteMassBody;
	MassBody::cpDataCache m_secondDeleteMassBody;

protected:
	ObjectFixedJoint(unsigned int id);
	virtual ~ObjectFixedJoint();
	friend class ObjectManager;
};

}

#endif
