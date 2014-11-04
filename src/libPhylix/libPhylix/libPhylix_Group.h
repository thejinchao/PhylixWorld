#ifndef __PHYSICS_WORLD_LIB_GROUP_H__
#define __PHYSICS_WORLD_LIB_GROUP_H__


namespace Phylix
{
//pre-define
class ObjectEntity;
class ObjectJoint;

/** group，在场景中，通过关节连接起来的物体组合
*/
class Group
{
public:
	typedef std::set< ObjectEntity* > EntityObjectSet;
	typedef std::set< ObjectJoint* > JointObjectSet;
	
	/** 获得所包含的物体 */
	EntityObjectSet& getObjects(void) { return m_objects; }
	/** 获得所包含的铰链 */
	JointObjectSet& getJoints(void) { return m_joints; }

private:
	/** 递归加入物体 */
	void _insertObject(ObjectEntity* obj, bool fixedJointOnly);

private:
	EntityObjectSet m_objects;		//!< 包含的物体
	JointObjectSet m_joints;		//!< 包含的铰链

public:
	Group(ObjectEntity* obj1, ObjectEntity* obj2, bool fixedJointOnly);
	virtual ~Group();
};

}

#endif


