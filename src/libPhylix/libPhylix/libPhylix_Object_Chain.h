#ifndef __PHYSICS_WORLD_LIB_OBJECT_CHAIN_H__
#define __PHYSICS_WORLD_LIB_OBJECT_CHAIN_H__

#include "libPhylix_Object.h"

namespace Phylix
{
//pre-define
class ObjectEntity;
class ObjectHingeJoint;

/** 链子
*/
class ObjectChain : public Object
{
public:
	/** 获得类型 */
	virtual Type getType(void) const { return OT_CHAIN; }
	/** 渲染 */
	virtual void draw(IRenderInterface* render);
	/** 重新构造创建参数 */
	virtual void buildCreateParam(ObjectParams& param);
	/** 第一个hinge id*/
	unsigned int getHeadHinge(void) const;
	/** 最后一个hinge*/
	unsigned int getTailHinge(void) const;
	/** 通知第一个hinge会被删除 */
	void deleteHead(void);
	/** 通知第一个hinge会被恢复 */
	void resumeHead(ObjectHingeJoint* hinge);
	/** 通知最后一个hinge会被删除 */
	void deleteTail(void);
	/** 通知最后一个hinge会被恢复 */
	void resumeTail(ObjectHingeJoint* hinge);

protected:
	Real m_width;		//!< 链子宽度

	bool m_lostFirst;	//!< 第一个hinge被删除
	fVector2 m_firstHingePos;

	bool m_lostTail;			//!< 最后一个hinge被伸出

	std::vector< ObjectEntity* > m_circleList;
	std::vector< ObjectHingeJoint* > m_hingeList;

protected:
	ObjectChain(unsigned int id);
	virtual ~ObjectChain();
	friend class ObjectManager;
};

}

#endif
