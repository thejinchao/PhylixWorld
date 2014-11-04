#ifndef __PHYSICS_WORLD_LIB_OBJECT_MANAGER_H__
#define __PHYSICS_WORLD_LIB_OBJECT_MANAGER_H__

#include "libPhylix_Handler.h"
#include "libPhylix_IteratorWrappers.h"
#include "libPhylix_Color.h"
#include "libPhylix_ObjectParams.h"

namespace Phylix
{

//pre-define
class Material;
class Object;
class ObjectEntity;
class CircleObject;
class ObjectJoint;
class ObjectFixedJoint;
class ObjectHingeJoint;
class ObjectSpringJoint;
class ObjectChain;
class Group;

/** 物体管理器，用于生成、销毁、管理物体
*/
class ObjectManager
{
public:
	typedef std::map< unsigned int, Object* > ObjectMap; 

public:
	/** 根据id查找物体*/
	Object* findObject(unsigned int id);
	/** 现有物体数量 */
	int getObjectCounts(void) const { return (int)m_allObject.size(); }
	/** 创建一个物体 */
	Object* createObject(const ObjectParams& createDefine);
	/** 删除一个物体*/
	void destroyObject(unsigned int id, std::vector< ObjectParams >& params);

	void debug(const char* fileName);

private:
	/** 创建一个标准的矩形物体 */
	ObjectEntity* _createStandardBox(const ObjectParams& params);
	/** 创建一个标准的圆形物体 */
	CircleObject* _createStandardCircle(const ObjectParams& param);
	/** 创建一个标准的边界物体	*/
	ObjectEntity* _createStandardEdge(const ObjectParams& param);
	/** 创建一个多边形，可能是凹多边形 */
	ObjectEntity* _createPolygon(const ObjectParams& param);
	/** 创建一个固定链接 */
	ObjectFixedJoint* _createFixedJoint(const ObjectParams& param);
	/** 创建一个旋转链接*/
	ObjectHingeJoint* _createHingeJoint(const ObjectParams& param);
	/** 创建一个弹簧链接*/
	ObjectSpringJoint* _createSpringJoint(const ObjectParams& param);
	/** 创建一个链子*/
	ObjectChain* _createChain(const ObjectParams& param);

private:
	ObjectMap m_allObject;		//!< 物体检索表

public:
	typedef MapIterator<ObjectMap> ObjectMapIterator;
	/** 返回object浏览器*/
	ObjectMapIterator getObjectIterator(void) 
	{
		return ObjectMapIterator(m_allObject.begin(), m_allObject.end());
	}

private:
	void _destroyEntityObject(ObjectEntity* entityObj, std::vector< ObjectParams >& params);
	void _destroyJointObject(ObjectJoint* jointObj, std::vector< ObjectParams >& params);
	void _destroyFixedJointObject(ObjectFixedJoint* jointObj, std::vector< ObjectParams >& params);
	void _destroyChainObject(ObjectChain* jointObj, std::vector< ObjectParams >& params);
	void _rebuildMassBody(Group* group, const MassBody::cpDataCache* massData);

private:
	unsigned int _generateID(int counts=1);
	Real _generateZValue(int counts=1);

	unsigned int m_nextID;
	Real m_nextZValue;
	const Real Z_VALUE_STEP;

public:
	ObjectManager();
	~ObjectManager();
};

}

#endif
