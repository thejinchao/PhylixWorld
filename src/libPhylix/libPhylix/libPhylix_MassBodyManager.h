#ifndef __PHYSICS_WORLD_LIB_MASS_BODY_MANAGER_H__
#define __PHYSICS_WORLD_LIB_MASS_BODY_MANAGER_H__

#include "libPhylix_Handler.h"
#include "libPhylix_IteratorWrappers.h"
#include "libPhylix_MassBody.h"

namespace Phylix
{

//pre-define
class Shape;
class Material;

/** 质量体管理器，用于生成、合并、分割质量体
*/
class MassBodyManager
{
public:
	/** 根据给定的质量和转动惯量，生成一个新的质量体 
	*/
	MassBody* createMassBody(const fVector2& pos, Real mass, Real moment, const MassBody::cpDataCache* cacheData);
	/** 删除一个质量体
	*/
	void destroyMassBody(MassBody* massBody);
	/** 根据id获得MassBody指针
	*/
	MassBody* findMassBody(unsigned int id);
	/** 获得质量体的数量
	*/
	unsigned int getMassBodyCounts(void) const { return (unsigned int)m_massBodyMap.size(); }

private:
	unsigned int _generateID(void);
	unsigned int m_nextID;

	typedef std::map< unsigned int, MassBody* > MassBodyMap;
	MassBodyMap m_massBodyMap;

public:
	typedef MapIterator<MassBodyMap> MassBodyIterator;
	/** 返回object浏览器*/
	MassBodyIterator getMassBodyIterator(void) 
	{
		return MassBodyIterator(m_massBodyMap.begin(), m_massBodyMap.end());
	}


public:
	MassBodyManager();
	~MassBodyManager();
};

}

#endif
