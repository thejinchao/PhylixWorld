#pragma once

#include "libPhylix_Action.h"
#include "libPhylix_Color.h"

namespace Phylix
{
//pre-define
class ObjectEntity;

/** 拖动物体
*/
class Action_DragObject : public Action
{
public:
	/** 获得类型 */
	virtual ACTION_TYPE getType(void) { return AT_DRAG_OBJECT; }
	virtual void draw(IRenderInterface* render);

protected:
	virtual void _onBegin(const fVector2& pt);
	virtual void _onDrag(const fVector2& delta);
	virtual void _onEnd(const fVector2& pt, bool canceled);
	virtual bool _needCameraGrid(void) const { return false; }

private:
	ObjectEntity*	m_dragObject;
	cpBody*			m_mouseBody;
	cpConstraint*	m_springConstraint;
	fVector2		m_dragPoint;

protected:
	Action_DragObject() : m_dragObject(0), m_mouseBody(0), m_springConstraint(0) {}
	virtual ~Action_DragObject() {}
	friend class ActionManager;
};

}
