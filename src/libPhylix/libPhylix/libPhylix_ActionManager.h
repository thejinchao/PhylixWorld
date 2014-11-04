#ifndef __PHYSICS_WORLD_LIB_ACTION_MANAGER_H__
#define __PHYSICS_WORLD_LIB_ACTION_MANAGER_H__

#include "libPhylix_Action.h"
#include "libPhylix_Handler.h"

namespace Phylix
{

/** 用于注册管理操作类
*/
class ActionManager
{
public:
	/** 注册Action */
	void registerAction(Action* action);
	/** 获得Action */
	Action* getAction(ACTION_TYPE type);
	/** 设置当前Action */
	Action* setActiveAction(ACTION_TYPE type);
	/** 获得当前激活的Action*/
	Action* getActiveAction(void) { return m_currentAction; }

protected:
	/** 注册标准Action */
	void _registerStandardAction(void);

protected:
	typedef std::map< ACTION_TYPE, Action* > ActionMap;
	ActionMap m_mapAction;		//!< 所有Action实例
	Action* m_currentAction;	//!< 当前激活的Action

public:
	ActionManager();
	~ActionManager();
};


}

#endif

