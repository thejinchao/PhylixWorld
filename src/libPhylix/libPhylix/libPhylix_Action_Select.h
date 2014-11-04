#pragma once

#include "libPhylix_Action.h"

namespace Phylix
{

class Action_Select : public Action
{
public:
	/** 获得类型 */
	virtual ACTION_TYPE getType(void) { return AT_SELECT; }

protected:
	virtual void _onBegin(const fVector2& pt);
	virtual void _onDrag(const fVector2& delta);
	virtual void _onEnd(const fVector2& pt, bool canceled);

public:
	Action_Select();
	virtual ~Action_Select();

};

}

