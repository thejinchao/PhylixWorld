#include "libPhylix_stdafx.h"
#include "libPhylix_Object_Joint_Fixed.h"
#include "libPhylix_Object_Entity.h"
#include "libPhylix_ObjectManager.h"
#include "libPhylix_MassBody.h"
#include "libPhylix_RenderInterface.h"
#include "libPhylix_Material.h"
#include "libPhylix_Scene.h"

namespace Phylix
{

//-------------------------------------------------------------------------------------------------------------------
ObjectFixedJoint::ObjectFixedJoint(unsigned int id)
	: ObjectJoint(id)
	, m_radius(1.0)
{
}

//-------------------------------------------------------------------------------------------------------------------
ObjectFixedJoint::~ObjectFixedJoint()
{
}

//-------------------------------------------------------------------------------------------------------------------
void ObjectFixedJoint::draw(IRenderInterface* render)
{
	assert(m_firstObj);
	if(!m_firstObj) return;

	bool selected = (SCENE()->getSelectedObject()==getID());

	fVector2 posWorld = m_firstObj->getTrnasFormMatrix().transform(m_posFirst);

	render->drawSolidCircle(posWorld, m_radius, false, posWorld, m_material, selected);
}

//-------------------------------------------------------------------------------------------------------------------
bool ObjectFixedJoint::isInLegendArea(const fVector2& pt) const
{
	assert(m_firstObj);
	if(!m_firstObj) return false;

	fVector2 dir = m_firstObj->getTrnasFormMatrix().transform(m_posFirst) - pt;
	if(dir.square() > m_radius*m_radius) return false;

	return true;
}

//-------------------------------------------------------------------------------------------------------------------
void ObjectFixedJoint::buildCreateParam(ObjectParams& param)
{
	param.reset(OT_FIXED_JOINT);

	fVector2 posWorld = m_firstObj->getTrnasFormMatrix().transform(m_posFirst);
	param.setPosition1(posWorld);

	param.setMaterial(getMaterial());
	param.setRadius(m_radius);
	param.setZValue(getZOrder());
	param.setObjectID(getID());
}

}
