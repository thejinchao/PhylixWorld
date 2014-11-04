#include "libPhylix_stdafx.h"
#include "libPhylix_MassBodyManager.h"
#include "libPhylix_MassBody.h"
#include "libPhylix_Shape.h"
#include "libPhylix_Material.h"
#include "libPhylix_Scene.h"

namespace Phylix
{

//-------------------------------------------------------------------------------------------------------------------
MassBodyManager::MassBodyManager()
	: m_nextID(0)
{
}

//-------------------------------------------------------------------------------------------------------------------
MassBodyManager::~MassBodyManager()
{
}

//-------------------------------------------------------------------------------------------------------------------
unsigned int MassBodyManager::_generateID(void)
{
	return ++m_nextID;
}

//-------------------------------------------------------------------------------------------------------------------
MassBody* MassBodyManager::createMassBody(const fVector2& pos, Real mass, Real moment, const MassBody::cpDataCache* cacheData)
{
	bool useCacheData = (cacheData!=0 && cacheData->massBodyID!=0);

	//生成质量体
	MassBody* massBody = new MassBody(useCacheData ? cacheData->massBodyID : _generateID(), pos);

	//计算质量
	massBody->m_mass = mass;

	//创建chipmunk质量体
	cpBody* b = massBody->m_cmBody = cpBodyNew(mass, moment);
	b->data = massBody;
	b->p = cpv(pos.x, pos.y);

	//恢复数据（用于deleteobject操作的undo)
	if(cacheData)
	{
		//b->p = cacheData->p;
		b->v = cacheData->v;
		b->f = cacheData->f;
		b->a = cacheData->a;
		b->w = cacheData->w;
		b->t = cacheData->t;
		b->rot = cacheData->rot;

		massBody->updateFromImpl();
	}	

	//加入map
	assert(m_massBodyMap.find(massBody->getID())==m_massBodyMap.end());
	m_massBodyMap.insert(std::make_pair(massBody->getID(), massBody));

	return massBody;
}

//-------------------------------------------------------------------------------------------------------------------
void MassBodyManager::destroyMassBody(MassBody* massBody)
{
	if(massBody==0) return;

	//从map中删除
	m_massBodyMap.erase(massBody->getID());

	//销毁chipmunk质量体
	cpBody* cmBody = massBody->m_cmBody;
	cpSpaceRemoveBody(SCENE()->getImpl(), cmBody);
	cpBodyFree(cmBody);

	//销毁质量体
	delete massBody;
}

//-------------------------------------------------------------------------------------------------------------------
MassBody* MassBodyManager::findMassBody(unsigned int id)
{
	MassBodyMap::iterator it = m_massBodyMap.find(id);
	if(it==m_massBodyMap.end()) return 0;

	return it->second;
}

}
