#include "libPhylix_stdafx.h"
#include "libPhylix_Action_Creater_Circle.h"
#include "libPhylix_ActionManager.h"
#include "libPhylix_ObjectManager.h"
#include "libPhylix_RenderInterface.h"
#include "libPhylix_MaterialManager.h"
#include "libPhylix_Object_Entity.h"
#include "libPhylix_OperatorManager.h"
#include "libPhylix_operator.h"

namespace Phylix
{
//-------------------------------------------------------------------------------------------------------------------
void Action_Creater_Circle::_onBegin(const fVector2& pt)
{
	m_randomColor = MaterialManager::_createRandomColor();
}

//-------------------------------------------------------------------------------------------------------------------
void Action_Creater_Circle::_onEnd(const fVector2& pt, bool canceled)
{
	//create box
	fVector2 axis = pt-m_dragOrigin;
	Real radius = axis.length();

	//如果太小，忽略
	if(floatCompare(radius, 0.0)<=0) return;
	axis.normalize();

	//构造创建参数
	ObjectParams param(Object::OT_ENTITY_CIRCLE);
	param.setPosition1(m_dragOrigin);
	param.setRadius(radius);
	param.setAngle(fastAtan2(axis.y, axis.x));
	param.setMaterial(MATERIAL_MANAGER()->createStandardMaterial(m_randomColor));

	OPERATOR_MANAGER()->applyOperator(new Operator_CreateObject(OT_CREATE_CIRCLE, param));
}

//-------------------------------------------------------------------------------------------------------------------
void Action_Creater_Circle::_onDrag(const fVector2& delta)
{
}

//-------------------------------------------------------------------------------------------------------------------
void Action_Creater_Circle::draw(IRenderInterface* render)
{
	if(!m_dragStarted || render==0) return;

	fVector2 axis = m_dragCurrent-m_dragOrigin;
	axis.normalize();

	render->drawSolidCircle(m_dragOrigin, (m_dragCurrent-m_dragOrigin).length(), true, m_dragCurrent, Material(m_randomColor), false);
}

}
