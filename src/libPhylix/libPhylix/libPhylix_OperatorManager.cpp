#include "libPhylix_stdafx.h"
#include "libPhylix_OperatorManager.h"
#include "libPhylix_Operator.h"

namespace Phylix
{

static void _getOpName(OPERATOR_TYPE type, char* name, int size)
{
	switch(type)
	{
	case OT_SCENE_SNAP:	strncpy(name, "--time--", size); break;
	case OT_CREATE_EDGE:	strncpy(name, "Edge", size); break;
	case OT_CREATE_BOX:	strncpy(name, "Box", size); break;
	case OT_CREATE_CIRCLE:	strncpy(name, "Circle", size); break;
	case OT_CREATE_POLYGON:	strncpy(name, "Ploygon", size); break;
	case OT_CREATE_FIXED_JOINT:	strncpy(name, "-Fixed-", size); break;
	case OT_CREATE_HINGE_JOINT:	strncpy(name, "-Hinge-", size); break;
	case OT_CREATE_SPRING_JOINT:	strncpy(name, "-Spring-", size); break;
	case OT_DELETE_OBJ:	strncpy(name, "DELETE", size); break;
	case OT_UPDATE_OBJECT_MATERIAL:	strncpy(name, "*Mat*", size); break;
	default: strncpy(name, "<ERROR>", size); break;
	}
}

//-------------------------------------------------------------------------------------------------------------------
OperatorManager::OperatorManager()
{
}

//-------------------------------------------------------------------------------------------------------------------
OperatorManager::~OperatorManager()
{
}

//-------------------------------------------------------------------------------------------------------------------
void OperatorManager::_debug(void)
{
#if 0
	std::string message;

	OperatorList::iterator it, end=m_undoList.end();
	int i=(int)m_undoList.size()-1;
	for(it=m_undoList.begin(); it!=end; ++it)
	{
		char value[256]={0};
		_getOpName((*it)->getType(), value, 32);

		if(message.empty()) message+=value;
		else { message += "|"; message+=value;}
	}

	//AxVar(0, 0, AX_STRING, "UNDO", (void*)message.c_str());

	message.clear();
	i=0;
	end=m_redoList.end();
	for(it=m_redoList.begin(); it!=end; ++it)
	{
		char name[32]={0};
		_snprintf(name, 32, "REDO%d", i++);

		char value[256]={0};
		_getOpName((*it)->getType(), value, 32);

		if(message.empty()) message+=value;
		else { message += "|"; message+=value;}
	}
	AxVar(0, 1, AX_STRING, "REDO", (void*)message.c_str());
#endif
}

//-------------------------------------------------------------------------------------------------------------------
void OperatorManager::_clearOperatorList(OperatorList& opList)
{
	OperatorList::const_iterator it;
	for (it = opList.begin(); it != opList.end(); ++it)
	{
		delete *it;
	}
	opList.clear();
}

//-------------------------------------------------------------------------------------------------------------------
void OperatorManager::applyOperator(Operator* op)
{
	assert(op);

	//执行操作
	if(!(op->redo())) return;

	//清空redo列表
	_clearOperatorList(m_redoList);
	//添加到undo列表中
	m_undoList.push_back(op);
}

//-------------------------------------------------------------------------------------------------------------------
void OperatorManager::undo(void)
{
	if(m_undoList.empty()) return;

	Operator* op = m_undoList.back();

	op->undo();

	m_redoList.push_front(op);
	m_undoList.pop_back();
}

//-------------------------------------------------------------------------------------------------------------------
void OperatorManager::redo(void)
{
	if(m_redoList.empty()) return;

	while(!m_redoList.empty())
	{
		Operator* op = m_redoList.front();
		bool snap = (op->getType()==OT_SCENE_SNAP);

		if(!snap) op->redo();

		m_undoList.push_back(op);
		m_redoList.pop_front();

		if(!snap) break;
	}
}

//-------------------------------------------------------------------------------------------------------------------
bool OperatorManager::canUndo(void) const
{
	return !m_undoList.empty();
}

//-------------------------------------------------------------------------------------------------------------------
bool OperatorManager::canRedo(void) const
{
	if(m_redoList.empty()) return false;
	//只剩下一个scenesnap
	if(m_redoList.size()==1 && (*(m_redoList.begin()))->getType()==OT_SCENE_SNAP) return false;

	return true;
}

//-------------------------------------------------------------------------------------------------------------------
void OperatorManager::cleanRedoList(void)
{
	_clearOperatorList(m_redoList);
}

//-------------------------------------------------------------------------------------------------------------------
Operator* OperatorManager::getLastOperator(void)
{
	if(m_undoList.empty()) return 0;
	return m_undoList.back();
}

}
