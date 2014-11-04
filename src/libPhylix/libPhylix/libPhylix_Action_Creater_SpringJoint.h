#pragma once

#include "libPhylix_Action.h"
#include "libPhylix_Color.h"

namespace Phylix
{
//pre-define
class ObjectEntity;

/** 创建弹簧
*/
class Action_Creater_SpringJoint : public Action
{
public:
	/** 获得类型 */
	virtual ACTION_TYPE getType(void) { return AT_CREATE_SPRING_JOINT; }
	virtual void draw(IRenderInterface* render);

protected:
	virtual void _onBegin(const fVector2& pt);
	virtual void _onDrag(const fVector2& delta);
	virtual void _onEnd(const fVector2& pt, bool canceled);

private:
	fColor m_randomColor;	//!< 临时材质颜色

public:
	Action_Creater_SpringJoint() {}
	virtual ~Action_Creater_SpringJoint() {}
};


}
