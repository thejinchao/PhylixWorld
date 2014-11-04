#include "libPhylix_stdafx.h"
#include "libPhylix_Action_Select.h"
#include "libPhylix_ActionManager.h"
#include "libPhylix_Scene.h"

namespace Phylix
{

//-------------------------------------------------------------------------------------------------------------------
Action_Select::Action_Select() 
{
}

//-------------------------------------------------------------------------------------------------------------------
Action_Select::~Action_Select() 
{
}

//-------------------------------------------------------------------------------------------------------------------
void Action_Select::_onBegin(const fVector2& pt)
{
}

//-------------------------------------------------------------------------------------------------------------------
void Action_Select::_onEnd(const fVector2& pt, bool canceled)
{
	Object* obj = SCENE()->queryFirstObjectFromPoint(pt);
	if(obj)
	{
		SCENE()->selectObject(obj->getID());
	}
	else
	{
		SCENE()->selectObject(0);
	}
}

//-------------------------------------------------------------------------------------------------------------------
void Action_Select::_onDrag(const fVector2& delta)
{
}

}
