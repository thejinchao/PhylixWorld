#include "libPhylix_stdafx.h"
#include "libPhylix_Operator.h"
#include "libPhylix_ObjectManager.h"
#include "libPhylix_Object_Entity.h"
#include "libPhylix_Scene.h"
#include "libPhylix_Object.h"
#include "libPhylix_Group.h"
#include "libPhylix_Object_Joint_Fixed.h"
#include "libPhylix_Object_Joint_Hinge.h"
#include "libPhylix_Object_Joint_Spring.h"
#include "libPhylix_MassBody.h"

namespace Phylix
{
//-------------------------------------------------------------------------------------------------------------------
Operator::Operator()
{
	m_sceneSnap.makeCurrentSanp();
}

//-------------------------------------------------------------------------------------------------------------------
Operator::~Operator()
{
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
Operator_CreateObject::Operator_CreateObject(OPERATOR_TYPE type, const ObjectParams& createParam)
	: m_opType(type)
	, m_createParam(createParam)
	, m_idObject(0)
{
}

//-------------------------------------------------------------------------------------------------------------------
Operator_CreateObject::~Operator_CreateObject()
{
}

//-------------------------------------------------------------------------------------------------------------------
bool Operator_CreateObject::redo(void)
{
	m_sceneSnap.restoreToScene();

	//创建物体
	Object* obj = OBJECT_MANAGER()->createObject(m_createParam);
	if(obj==0) return false;

	//保存数据(id)
	m_idObject = obj->getID();
	return true;
}

//-------------------------------------------------------------------------------------------------------------------
void Operator_CreateObject::undo(void)
{
	assert(m_idObject);
	Object* obj = OBJECT_MANAGER()->findObject(m_idObject);
	assert(obj);
	if(obj==0) return;

	//保存objid以及z值
	m_createParam.setObjectID(m_idObject);
	m_createParam.setZValue(obj->getZOrder());

	//如果有质量体，保存质量体数据
	MassBody* massBody = obj->isEntityType(obj->getType()) ? ((ObjectEntity*)obj)->getMassBody() : 0;
	if(massBody)
	{
		MassBody::cpDataCache data;
		massBody->getCpData(data);
		m_createParam.setMassBody(data);
	}

	std::vector< ObjectParams > dummy;
	OBJECT_MANAGER()->destroyObject(obj->getID(), dummy);
	m_idObject = 0;
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
Operator_SceneSnap::Operator_SceneSnap()
{
}

//-------------------------------------------------------------------------------------------------------------------
Operator_SceneSnap::~Operator_SceneSnap()
{
}

//-------------------------------------------------------------------------------------------------------------------
bool Operator_SceneSnap::redo(void)
{
	return true;
}

//-------------------------------------------------------------------------------------------------------------------
void Operator_SceneSnap::undo(void)
{
	m_sceneSnap.restoreToScene();
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
Operator_DeleteObject::Operator_DeleteObject(unsigned int idObject)
	: m_idObject(idObject)
{
}

//-------------------------------------------------------------------------------------------------------------------
Operator_DeleteObject::~Operator_DeleteObject()
{
}

//-------------------------------------------------------------------------------------------------------------------
bool Operator_DeleteObject::redo(void)
{
	m_sceneSnap.restoreToScene();

	Object* obj = OBJECT_MANAGER()->findObject(m_idObject);
	assert(obj);
    obj=0;

	//删除物体，同时存储创建参数
	m_createParams.clear();
	OBJECT_MANAGER()->destroyObject(m_idObject, m_createParams);
	return true;
}

//-------------------------------------------------------------------------------------------------------------------
void Operator_DeleteObject::undo(void)
{
	//创建实体
	for(int i=0; i<(int)m_createParams.size(); i++)
	{
		const ObjectParams& param = m_createParams[i];
		if(Object::isEntityType(param.getObjectType()))
		{
			OBJECT_MANAGER()->createObject(param);
		}
	}

	//创建链子
	for(int i=0; i<(int)m_createParams.size(); i++)
	{
		const ObjectParams& param = m_createParams[i];
		if(param.getObjectType()==Object::OT_CHAIN)
		{
			OBJECT_MANAGER()->createObject(param);
		}
	}

	//创建连接体
	for(int i=0; i<(int)m_createParams.size(); i++)
	{
		const ObjectParams& param = m_createParams[i];
		if(Object::isJointType(param.getObjectType()))
		{
			OBJECT_MANAGER()->createObject(param);
		}
	}
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
Operator_UpdateObjectMaterial::Operator_UpdateObjectMaterial(unsigned int idObject, const Material& newMat)
	: m_idObject(idObject)
	, m_newMaterial(newMat)
{
}

//-------------------------------------------------------------------------------------------------------------------
Operator_UpdateObjectMaterial::~Operator_UpdateObjectMaterial()
{
}

//-------------------------------------------------------------------------------------------------------------------
bool Operator_UpdateObjectMaterial::redo(void)
{
	m_sceneSnap.restoreToScene();

	Object* obj = OBJECT_MANAGER()->findObject(m_idObject);
	assert(obj);

	m_oldMaterial = obj->getMaterial();
	obj->updateMaterial(m_newMaterial);

	return true;
}

//-------------------------------------------------------------------------------------------------------------------
void Operator_UpdateObjectMaterial::undo(void)
{
	Object* obj = OBJECT_MANAGER()->findObject(m_idObject);
	assert(obj);

	obj->updateMaterial(m_oldMaterial);
}


//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
Operator_UpdateHingeMoto::Operator_UpdateHingeMoto(unsigned int idObject, const ObjectHingeJoint::MotoParam& motoParam)
	: m_idObject(idObject)
	, m_motoParam(motoParam)
{
}

//-------------------------------------------------------------------------------------------------------------------
Operator_UpdateHingeMoto::~Operator_UpdateHingeMoto()
{
}

//-------------------------------------------------------------------------------------------------------------------
bool Operator_UpdateHingeMoto::redo(void)
{
	Object* obj = OBJECT_MANAGER()->findObject(m_idObject);
	assert(obj && obj->getType()==Object::OT_HINGE_JOINT);

	m_sceneSnap.restoreToScene();

	m_oldMotoParam = ((ObjectHingeJoint*)obj)->getMotoParam();
	((ObjectHingeJoint*)obj)->updateMotoParam(m_motoParam);

	return true;
}

//-------------------------------------------------------------------------------------------------------------------
void Operator_UpdateHingeMoto::undo(void)
{
	Object* obj = OBJECT_MANAGER()->findObject(m_idObject);
	assert(obj && obj->getType()==Object::OT_HINGE_JOINT);

	((ObjectHingeJoint*)obj)->updateMotoParam(m_oldMotoParam);
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
Operator_UpdateSpring::Operator_UpdateSpring(unsigned int idObject, const ObjectSpringJoint::SpringParam& springParam)
	: m_idObject(idObject)
	, m_springParam(springParam)
{
}

//-------------------------------------------------------------------------------------------------------------------
Operator_UpdateSpring::~Operator_UpdateSpring()
{
}

//-------------------------------------------------------------------------------------------------------------------
bool Operator_UpdateSpring::redo(void)
{
	Object* obj = OBJECT_MANAGER()->findObject(m_idObject);
	assert(obj && obj->getType()==Object::OT_SPRING_JOINT);

	m_sceneSnap.restoreToScene();

	m_oldSpringParam = ((ObjectSpringJoint*)obj)->getSpringParam();
	((ObjectSpringJoint*)obj)->updateSpringParam(m_springParam);

	return true;
}

//-------------------------------------------------------------------------------------------------------------------
void Operator_UpdateSpring::undo(void)
{
	Object* obj = OBJECT_MANAGER()->findObject(m_idObject);
	assert(obj && obj->getType()==Object::OT_HINGE_JOINT);

	((ObjectSpringJoint*)obj)->updateSpringParam(m_oldSpringParam);
}

}
