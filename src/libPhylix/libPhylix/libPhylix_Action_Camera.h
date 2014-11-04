#pragma once

#include "libPhylix_Action.h"

namespace Phylix
{

/** 镜头平移
*/
class Action_CameraPan : public Action
{
public:
	/** 获得类型 */
	virtual ACTION_TYPE getType(void) { return AT_CAMERA_PAN; }

protected:
	virtual void _onBegin(const fVector2& pt);
	virtual void _onDrag(const fVector2& delta);
	virtual bool _needCameraGrid(void) const { return false; }

private:
	fVector2 m_anchorPos;

protected:
	Action_CameraPan() {}
	virtual ~Action_CameraPan() {}
	friend class ActionManager;
};

}
