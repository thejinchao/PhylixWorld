#include "libPhylix_stdafx.h"
#include "libPhylix_ActionManager.h"

#include "libPhylix_Action_Camera.h"
#include "libPhylix_Action_Creater_Edge.h"
#include "libPhylix_Action_Creater_Box.h"
#include "libPhylix_Action_Creater_Circle.h"
#include "libPhylix_Action_Creater_Polygon.h"
#include "libPhylix_Action_Creater_FixJoint.h"
#include "libPhylix_Action_Creater_HingeJoint.h"
#include "libPhylix_Action_Creater_SpringJoint.h"
#include "libPhylix_Action_Select.h"
#include "libPhylix_Action_DragObject.h"
#include "libPhylix_Action_Creater_Chain.h"

namespace Phylix
{

//-------------------------------------------------------------------------------------------------------------------
ActionManager::ActionManager()
	: m_currentAction(0)
{
	//注册所有Action
	_registerStandardAction();
}

//-------------------------------------------------------------------------------------------------------------------
ActionManager::~ActionManager()
{
}

//-------------------------------------------------------------------------------------------------------------------
void ActionManager::registerAction(Action* action)
{
	//加入map
	m_mapAction.insert(std::make_pair(action->getType(), action));
}

//-------------------------------------------------------------------------------------------------------------------
void ActionManager::_registerStandardAction(void)
{
	registerAction(new Action_Select());

	registerAction(new Action_CameraPan());

	registerAction(new Action_Creater_Edge());
	registerAction(new Action_Creater_Box());
	registerAction(new Action_Creater_Circle());
	registerAction(new Action_Creater_Polygon());

	registerAction(new Action_Creater_FixJoint());
	registerAction(new Action_Creater_HingeJoint());
	registerAction(new Action_Creater_SpringJoint());

	registerAction(new Action_Creater_Chain());

	registerAction(new Action_DragObject());
}

//-------------------------------------------------------------------------------------------------------------------
Action* ActionManager::getAction(ACTION_TYPE type)
{
	ActionMap::iterator itFind = m_mapAction.find(type);
	if(itFind==m_mapAction.end()) return 0;

	return itFind->second;
}

//-------------------------------------------------------------------------------------------------------------------
Action* ActionManager::setActiveAction(ACTION_TYPE type)
{
	Action* currentAction = (type==AT_NULL ? 0 : getAction(type));
	if(currentAction==m_currentAction) return m_currentAction;

	if(m_currentAction)
	{
		//TODO: abort
		//m_currentAction->onAbort();
	}

	return m_currentAction=currentAction;
}

}
