#include "libPhylix_stdafx.h"
#include "libPhylix_SceneSnap.h"
#include "libPhylix_Scene.h"
#include "libPhylix_MassBody.h"
#include "libPhylix_MassBodyManager.h"
#include "libPhylix_Object_Entity.h"
#include "libPhylix_Shape.h"

namespace Phylix
{

//-------------------------------------------------------------------------------------------------------------------
SceneSnap::SceneSnap()
	: m_timeStamp(-1.0)
{
}

//-------------------------------------------------------------------------------------------------------------------
SceneSnap::~SceneSnap()
{
}

//-------------------------------------------------------------------------------------------------------------------
void SceneSnap::clean(void)
{
	m_massBodyData.clear();
	m_timeStamp = -1.0;
}

//-------------------------------------------------------------------------------------------------------------------
void SceneSnap::makeCurrentSanp(void)
{
	//清空旧数据
	clean();

	//查询新数据
	//cpSpace* space = SCENE()->getImpl();
	m_timeStamp = SCENE()->getTimeStamp();

	m_massBodyData.reserve(MASS_BODY_MANAGER()->getMassBodyCounts());
	MassBodyManager::MassBodyIterator it = MASS_BODY_MANAGER()->getMassBodyIterator();
	while(it.hasMoreElements())
	{
		MassBody::cpDataCache data;
		it.getNext()->getCpData(data);
		m_massBodyData.push_back(data);
	}
}

//-------------------------------------------------------------------------------------------------------------------
void SceneSnap::restoreToScene(void)
{
	//同一个时间
	if(floatCompare(m_timeStamp, SCENE()->getTimeStamp())==0) return;

	cpSpace* space = SCENE()->getImpl();

	//恢复质量体数据
	for(int i=0; i<(int)m_massBodyData.size(); i++)
	{
		const MassBody::cpDataCache& cache = m_massBodyData[i];
		MassBody* body = (MassBody*)(MASS_BODY_MANAGER()->findMassBody(cache.massBodyID));
		assert(body);

		cpBody* b = body->getImpl();

		b->p = cache.p;
		b->v = cache.v;
		b->f = cache.f;
		b->a = cache.a;
		b->w = cache.w;
		b->t = cache.t;
		b->rot = cache.rot;

		//刷新shape
		const MassBody::ObjectSet& objSet = body->getAttachedObject();
		for(MassBody::ObjectSet::const_iterator it=objSet.begin(); it!=objSet.end(); ++it)
		{
			ObjectEntity* obj = *it;
			for(int i=0; i<obj->getShape()->getImplCounts(); i++)
			{
				cpSpaceReindexShape(space, obj->getShape()->getImpl(i));
			}
		}

	}

	//刷新场景
	SCENE()->restoreSceneSanp(m_timeStamp);
}

}
