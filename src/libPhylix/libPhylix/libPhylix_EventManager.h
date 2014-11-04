#ifndef __PHYSICS_WORLD_LIB_EVENT_SYSTEM_H__
#define __PHYSICS_WORLD_LIB_EVENT_SYSTEM_H__

#include "libPhylix_EventDefine.h"
#include "libPhylix_Handler.h"

namespace Phylix
{
//pre-define
struct EVENT;

//事件处理回调函数
typedef void (__stdcall* EVENT_HANDLE)(const EVENT* pEvent, unsigned int data);
//回调函数集合
typedef std::list< std::pair< EVENT_HANDLE, unsigned int > > EVENT_HANDLE_LIST;

/** 游戏事件定义
*/
struct EVENT_DEFINE
{
	EVENT_ID			idEvent;	//event 唯一id
	EVENT_HANDLE_LIST	listFuncNotify;
};

/** 事件参数
*/
union EVENT_PARAM
{
	int			nParam;
	float		fParam;
	const char*	szParam;
};

/** 游戏事件
*/
struct EVENT
{
	EVENT_DEFINE* eventDefine;	//事件定义
	std::vector< EVENT_PARAM >	args;	//参数
};

/** 游戏中event分发管理器
*/
class EventManager
{
public:
	/** 注册一个事件处理接口 */
	bool registerEventHandle(EVENT_ID id, EVENT_HANDLE handle, unsigned int data);
	/** 产生事件 */
	void fireEvent(EVENT_ID id);

private:
	/** 注册事件定义 */
	void _registerEventDefine(void);

private:
	typedef std::map< EVENT_ID, EVENT_DEFINE > EVENT_MAP;
	EVENT_MAP m_mapEventDefine;	//!< 事件定义
		
public:
	EventManager();
	~EventManager();
};

}

#endif

