#include "libPhylix_stdafx.h"
#include "libPhylix_Object_Joint_Spring.h"
#include "libPhylix_Object_Entity.h"
#include "libPhylix_ObjectManager.h"
#include "libPhylix_MassBody.h"
#include "libPhylix_RenderInterface.h"
#include "libPhylix_Material.h"
#include "libPhylix_Scene.h"

namespace Phylix
{

//-------------------------------------------------------------------------------------------------------------------
const Real ObjectSpringJoint::DAMP_DEFAULT=0.5;		//弹簧缺省的阻尼系数(0.5)
const Real ObjectSpringJoint::DAMP_MIN=0.0;			//最小阻尼系数 0.0
const Real ObjectSpringJoint::DAMP_MAX=2.0;			//最大阻尼系数 2.0

//-------------------------------------------------------------------------------------------------------------------
ObjectSpringJoint::ObjectSpringJoint(unsigned int id)
	: ObjectJoint(id)
	, m_width(1.0)
	, m_nFirstMassBody(0)
	, m_nSecondMassBody(0)
{
}

//-------------------------------------------------------------------------------------------------------------------
ObjectSpringJoint::~ObjectSpringJoint()
{
}

//-------------------------------------------------------------------------------------------------------------------
void ObjectSpringJoint::draw(IRenderInterface* render)
{
	bool selected = (SCENE()->getSelectedObject()==getID());

	fVector2 pos1 = m_firstObj ? m_firstObj->getTrnasFormMatrix().transform(m_posFirst) : m_posFirst;
	fVector2 pos2 = m_secondObj ? m_secondObj->getTrnasFormMatrix().transform(m_posSecond) : m_posSecond;

	render->drawSpring(pos1, pos2, m_material.color, m_width, m_springParam.targetLength, selected);
}

//-------------------------------------------------------------------------------------------------------------------
bool ObjectSpringJoint::isInLegendArea(const fVector2& pt) const
{
	fVector2 pos1 = m_firstObj ? m_firstObj->getTrnasFormMatrix().transform(m_posFirst) : m_posFirst;
	fVector2 pos2 = m_secondObj ? m_secondObj->getTrnasFormMatrix().transform(m_posSecond) : m_posSecond;

	//生成变换矩阵
	fMatrix3 trans = fMatrix3(Angle(pos2-pos1), pos1);
	fVector2 p = trans.solve(pt);
	return (p.x>=0 && p.x<=(pos2-pos1).length() && p.y>=-m_width/2 && p.y<=m_width/2);
}

//-------------------------------------------------------------------------------------------------------------------
void ObjectSpringJoint::reCheckMassBody(void)
{
	MassBody* body1 = m_firstObj ? m_firstObj->getMassBody() : 0;
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
	fVector2 pos1 = m_firstObj ? m_firstObj->getTrnasFormMatrix().transform(m_posFirst) : m_posFirst;
	fVector2 pos2 = m_secondObj ? m_secondObj->getTrnasFormMatrix().transform(m_posSecond) : m_posSecond;

	//添加新的数据
	fVector2 p1 = body1 ? body1->getTrnasFormMatrix().solve(pos1) : pos1;
	fVector2 p2 = body2 ? body2->getTrnasFormMatrix().solve(pos2) : pos2;

	m_cpConstraint = cpDampedSpringNew(
		body1 ? body1->getImpl() : SCENE()->getImpl()->staticBody, 
		body2 ? body2->getImpl() : SCENE()->getImpl()->staticBody, 
		cpv(p1.x, p1.y), 
		cpv(p2.x, p2.y), 
		m_springParam.targetLength, 
		m_springParam.stiffness, 
		m_springParam.damp);
	cpSpaceAddConstraint(SCENE()->getImpl(), m_cpConstraint);

}

//-------------------------------------------------------------------------------------------------------------------
void ObjectSpringJoint::buildCreateParam(ObjectParams& param)
{
	param.reset(OT_SPRING_JOINT);

	fVector2 pos1 = m_firstObj ? m_firstObj->getTrnasFormMatrix().transform(m_posFirst) : m_posFirst;
	fVector2 pos2 = m_secondObj ? m_secondObj->getTrnasFormMatrix().transform(m_posSecond) : m_posSecond;

	param.setPosition1(pos1);
	param.setPosition2(pos2);

	param.setMaterial(getMaterial());
	param.setWidth(m_width);
	param.setZValue(getZOrder());
	param.setObjectID(getID());
}

//-------------------------------------------------------------------------------------------------------------------
void ObjectSpringJoint::updateSpringParam(const SpringParam& param)
{
	if(param == m_springParam) return;

	m_springParam = param;

	((cpDampedSpring*)m_cpConstraint)->restLength = m_springParam.targetLength;
	((cpDampedSpring*)m_cpConstraint)->damping = m_springParam.damp;
	((cpDampedSpring*)m_cpConstraint)->stiffness = m_springParam.stiffness;
}

}
