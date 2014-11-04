#ifndef __PHYSICS_WORLD_LIB_OBJECT_JOINT_H__
#define __PHYSICS_WORLD_LIB_OBJECT_JOINT_H__

#include "libPhylix_Object.h"

namespace Phylix
{
//pre-define
class ObjectEntity;
class ObjectManager;

/** 连接物体的关节基类
*/
class ObjectJoint : public Object
{
public:
	/** 第一个连接物体 */
	ObjectEntity* getFirstObject(void) const { return m_firstObj; }
	/** 第二个连接物体 */
	ObjectEntity* getSecondObject(void) const { return m_secondObj; }
	/** 某点是否在该点示意图范围内，用于计算鼠标选择*/
	virtual bool isInLegendArea(const fVector2& pt) const { return false; }
	/** 重新检查所连接的质量体是否有变化 */
	virtual void reCheckMassBody(void) { }

protected:
	ObjectEntity* m_firstObj;	//!< 所连接的第一个object
	ObjectEntity* m_secondObj;	//!< 所连接的第二个object

	fVector2 m_posFirst;		//!< 位置(相对于第一个obj坐标)
	fVector2 m_posSecond;		//!< 位置(相对于第二个obj坐标)

protected:
	ObjectJoint(unsigned int id);
	virtual ~ObjectJoint();
	friend class ObjectManager;
};

}

#endif
