#include "libPhylix_stdafx.h"
#include "libPhylix_EventManager.h"

namespace Phylix
{

//-------------------------------------------------------------------------------------------------------------------
EventManager::EventManager()
{
	_registerEventDefine();
}

//-------------------------------------------------------------------------------------------------------------------
EventManager::~EventManager()
{
}

//-------------------------------------------------------------------------------------------------------------------
void EventManager::_registerEventDefine(void)
{
	for(unsigned int i=0; i<EVENT_COUNTS; i++)
	{
		EVENT_DEFINE def;
		def.idEvent = (EVENT_ID)i;

		m_mapEventDefine.insert(std::make_pair(def.idEvent, def));
	}
}

//-------------------------------------------------------------------------------------------------------------------
bool EventManager::registerEventHandle(EVENT_ID id, EVENT_HANDLE handle, unsigned int data)
{
	EVENT_MAP::iterator it = m_mapEventDefine.find(id);
	if(it==m_mapEventDefine.end()) return false;

	EVENT_DEFINE& eventDefine = it->second;
	eventDefine.listFuncNotify.push_back(std::make_pair(handle, data));
	return true;
}

//-------------------------------------------------------------------------------------------------------------------
void EventManager::fireEvent(EVENT_ID id)
{
	EVENT_MAP::iterator it = m_mapEventDefine.find(id);
	if(it==m_mapEventDefine.end()) return;

	EVENT _event;
	_event.eventDefine = &(it->second);
	//_event.args; //TODO:...

	//FIRE!
	EVENT_HANDLE_LIST::iterator fun_it, fun_end=_event.eventDefine->listFuncNotify.end();
	for(fun_it=_event.eventDefine->listFuncNotify.end(); fun_it!=fun_end; ++fun_it)
	{
		(*(fun_it->first))(0, 0);
	}

}


}
