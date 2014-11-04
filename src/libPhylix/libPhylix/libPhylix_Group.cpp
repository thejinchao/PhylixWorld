#include "libPhylix_stdafx.h"
#include "libPhylix_Group.h"
#include "libPhylix_Object_Joint.h"
#include "libPhylix_Object_Entity.h"

namespace Phylix
{
//-------------------------------------------------------------------------------------------------------------------
Group::Group(ObjectEntity* obj1, ObjectEntity* obj2, bool fixedJointOnly)
{
	_insertObject(obj1, fixedJointOnly);
	_insertObject(obj2, fixedJointOnly);
}

//-------------------------------------------------------------------------------------------------------------------
Group::~Group()
{
}

//-------------------------------------------------------------------------------------------------------------------
void Group::_insertObject(ObjectEntity* obj, bool fixedJointOnly)
{
	if(obj==0 || m_objects.find(obj)!=m_objects.end()) return;
	
	//加入物体
	m_objects.insert(obj);
	
	//关节
	ObjectEntity::JointSet joints = obj->getJointSet();
	ObjectEntity::JointSet::iterator it, end=joints.end();
	for(it=joints.begin(); it!=end; ++it)
	{
		ObjectJoint* joint = *it;
		if(fixedJointOnly && joint->getType()!=Object::OT_FIXED_JOINT) continue;

		//加入铰链
		m_joints.insert(joint);

		_insertObject(joint->getFirstObject(), fixedJointOnly);
		_insertObject(joint->getSecondObject(), fixedJointOnly);
	}
}

}
