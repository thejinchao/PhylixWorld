#include "libPhylix_stdafx.h"
#include "libPhylix_Object_Joint_Hinge.h"
#include "libPhylix_Object_Entity.h"
#include "libPhylix_ObjectManager.h"
#include "libPhylix_MassBody.h"
#include "libPhylix_RenderInterface.h"
#include "libPhylix_Material.h"
#include "libPhylix_Scene.h"
#include "libPhylix_Object_Chain.h"

namespace Phylix
{

//-------------------------------------------------------------------------------------------------------------------
const Real ObjectHingeJoint::MOTO_MIN_RATE = 0.0;	//引擎最小速率 0
const Real ObjectHingeJoint::MOTO_MAX_RATE = 10.0;	//引擎最大速率 10.0

//-------------------------------------------------------------------------------------------------------------------
ObjectHingeJoint::ObjectHingeJoint(unsigned int id)
	: ObjectJoint(id)
	, m_radius(1.0)
	, m_nFirstMassBody(0)
	, m_nSecondMassBody(0)
	, m_cpMoto(0)
	, m_chainID(0)
	, m_headHinge(false)
	, m_tailHinge(false)
{
	m_motoParam.enableMoto = false;
	m_motoParam.is_cw = false;
	m_motoParam.rate = 2.0;
}

//-------------------------------------------------------------------------------------------------------------------
ObjectHingeJoint::~ObjectHingeJoint()
{
}

//-------------------------------------------------------------------------------------------------------------------
void ObjectHingeJoint::draw(IRenderInterface* render)
{
	assert(m_firstObj);
	if(!m_firstObj) return;

	unsigned int selectID = SCENE()->getSelectedObject();
	bool selected = (selectID!=0 && (selectID==getID() || selectID==m_chainID));

	fVector2 posWorld = m_secondObj ? m_secondObj->getTrnasFormMatrix().transform(m_posSecond) : m_posSecond;

	render->drawSolidCircle(posWorld, m_radius, false, posWorld, m_material, selected);
}

//-------------------------------------------------------------------------------------------------------------------
bool ObjectHingeJoint::isInLegendArea(const fVector2& pt) const
{
	assert(m_firstObj);
	if(!m_firstObj) return false;

	fVector2 dir = m_firstObj->getTrnasFormMatrix().transform(m_posFirst) - pt;
	if(dir.square() > m_radius*m_radius) return false;

	return true;
}

//-------------------------------------------------------------------------------------------------------------------
void ObjectHingeJoint::reCheckMassBody(void)
{
	assert(m_firstObj);

	MassBody* body1 = m_firstObj->getMassBody();
	MassBody* body2 = m_secondObj ? m_secondObj->getMassBody() : 0;

	int newid_first  = body1 ? body1->getID() : 0;
	int newid_second = body2 ? body2->getID() : 0;

	//没有变化
	if(newid_first==m_nFirstMassBody && newid_second==m_nSecondMassBody) return;

	//有变化，需要重新设置
	m_nFirstMassBody = newid_first;
	m_nSecondMassBody = newid_second;

	cpSpace* space = SCENE()->getImpl();

	//删除旧数据
	if(m_cpConstraint)
	{
		cpSpaceRemoveConstraint(space, m_cpConstraint);
	}
	m_cpConstraint = 0;

	//两端都是静态物体
	if(body1==0 && body2==0) return;

	//世界坐标
	fVector2 wordPos = m_firstObj->getTrnasFormMatrix().transform(m_posFirst);

	//添加新的数据
	fVector2 p1 = body1 ? body1->getTrnasFormMatrix().solve(wordPos) : wordPos;
	fVector2 p2 = body2 ? body2->getTrnasFormMatrix().solve(wordPos) : wordPos;

	m_cpConstraint = cpPivotJointNew2(
		body1 ? body1->getImpl() : space->staticBody, 
		body2 ? body2->getImpl() : space->staticBody, 
		cpv(p1.x, p1.y), cpv(p2.x, p2.y)); 

	cpSpaceAddConstraint(space, m_cpConstraint);
}

//-------------------------------------------------------------------------------------------------------------------
fVector2 ObjectHingeJoint::getWorldPos(void) const
{
	fVector2 posWorld = m_firstObj->getTrnasFormMatrix().transform(m_posFirst);
	return posWorld;
}

//-------------------------------------------------------------------------------------------------------------------
void ObjectHingeJoint::buildCreateParam(ObjectParams& param)
{
	param.reset(OT_HINGE_JOINT);

	param.setPosition1(getWorldPos());
	param.setTargetID1(m_firstObj ? m_firstObj->getID() : 0);
	param.setTargetID2(m_secondObj ? m_secondObj->getID() : 0);
	param.setBoolean1(m_headHinge);
	param.setBoolean2(m_tailHinge);

	param.setMaterial(getMaterial());
	param.setRadius(m_radius);
	param.setZValue(getZOrder());
	param.setObjectID(getID());
}

//-------------------------------------------------------------------------------------------------------------------
void ObjectHingeJoint::updateMotoParam(const MotoParam& param)
{
	if(param==m_motoParam) return;

	if(param.enableMoto != m_motoParam.enableMoto) _enableMoto(param);
	m_motoParam = param;
	if(!param.enableMoto) return;

	((cpSimpleMotor*)m_cpMoto)->rate = m_motoParam.is_cw ? -m_motoParam.rate : m_motoParam.rate;
}

//-------------------------------------------------------------------------------------------------------------------
void ObjectHingeJoint::_enableMoto(const MotoParam& param)
{
	cpSpace* space = SCENE()->getImpl();

	if(param.enableMoto)
	{
		//打开引擎功能
		MassBody* body1 = m_firstObj ? m_firstObj->getMassBody() : 0;
		MassBody* body2 = m_secondObj ? m_secondObj->getMassBody() : 0;

		m_cpMoto = cpSimpleMotorNew(
			body1 ? body1->getImpl() : space->staticBody, 
			body2 ? body2->getImpl() : space->staticBody, 
			param.is_cw ? -param.rate : param.rate);
		cpSpaceAddConstraint(space, m_cpMoto);
	}
	else
	{
		cpSpaceRemoveConstraint(space, m_cpMoto);
		cpConstraintDestroy(m_cpMoto);
		m_cpMoto = 0;
	}
}

//-------------------------------------------------------------------------------------------------------------------
void ObjectHingeJoint::notifySelected(void)
{
	if(m_chainID!=0)
	{
		ObjectChain* parentChain = (ObjectChain*)OBJECT_MANAGER()->findObject(m_chainID);
		if(parentChain && parentChain->getHeadHinge()!=getID() && parentChain->getTailHinge()!=getID())
		{
			SCENE()->selectObject(m_chainID);
		}
	}
}

//-------------------------------------------------------------------------------------------------------------------
void ObjectHingeJoint::notifyDelete(void)
{
	if(m_chainID!=0)
	{
		ObjectChain* parentChain = (ObjectChain*)OBJECT_MANAGER()->findObject(m_chainID);
		if(parentChain && parentChain->getHeadHinge()==getID())
		{
			parentChain->deleteHead();
		}
		else if(parentChain && parentChain->getTailHinge()==getID())
		{
			parentChain->deleteTail();
		}
	}
}

}
