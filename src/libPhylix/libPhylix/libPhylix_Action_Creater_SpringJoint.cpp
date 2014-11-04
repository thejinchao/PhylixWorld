#include "libPhylix_stdafx.h"
#include "libPhylix_Action_Creater_SpringJoint.h"
#include "libPhylix_ObjectManager.h"
#include "libPhylix_RenderInterface.h"
#include "libPhylix_ActionManager.h"
#include "libPhylix_Scene.h"
#include "libPhylix_Object.h"
#include "libPhylix_MaterialManager.h"
#include "libPhylix_Camera.h"
#include "libPhylix_OperatorManager.h"
#include "libPhylix_operator.h"

namespace Phylix
{
//-------------------------------------------------------------------------------------------------------------------
void Action_Creater_SpringJoint::_onBegin(const fVector2& pt)
{
	m_randomColor = MaterialManager::_createRandomColor();
}

//-------------------------------------------------------------------------------------------------------------------
void Action_Creater_SpringJoint::_onEnd(const fVector2& pt, bool canceled)
{
	//根据摄像机参数决定半径
	Real viewZoom = CAMERA()->getViewZoom();
	
	//构造创建参数
	ObjectParams param(Object::OT_SPRING_JOINT);
	param.setPosition1(m_dragOrigin);
	param.setPosition2(m_dragCurrent);
	param.setMaterial(MATERIAL_MANAGER()->createStandardMaterial(m_randomColor));
	param.setWidth((Real)(2.0/(viewZoom*16.0)));

	OPERATOR_MANAGER()->applyOperator(new Operator_CreateObject(OT_CREATE_SPRING_JOINT, param));
}

//-------------------------------------------------------------------------------------------------------------------
void Action_Creater_SpringJoint::_onDrag(const fVector2& delta)
{
}

//-------------------------------------------------------------------------------------------------------------------
void Action_Creater_SpringJoint::draw(IRenderInterface* render)
{
	if(!m_dragStarted || render==0) return;

	//根据摄像机参数弹簧宽度
	Real viewZoom = (Real)(2.0/(CAMERA()->getViewZoom()*16.0));

	render->drawSpring(m_dragOrigin, m_dragCurrent, m_randomColor, viewZoom, (m_dragCurrent-m_dragOrigin).length(), false);
}

}
