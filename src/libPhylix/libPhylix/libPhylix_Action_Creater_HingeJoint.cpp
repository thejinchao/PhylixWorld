#include "libPhylix_stdafx.h"
#include "libPhylix_Action_Creater_HingeJoint.h"
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
void Action_Creater_HingeJoint::_onBegin(const fVector2& pt)
{
	m_randomColor = MaterialManager::_createRandomColor();
}

//-------------------------------------------------------------------------------------------------------------------
void Action_Creater_HingeJoint::_onEnd(const fVector2& pt, bool canceled)
{
	//根据摄像机参数决定半径
	Real viewZoom = CAMERA()->getViewZoom();

	//构造创建参数
	ObjectParams param(Object::OT_HINGE_JOINT);
	param.setPosition1(m_dragOrigin);
	param.setMaterial(MATERIAL_MANAGER()->createStandardMaterial(m_randomColor));
	param.setRadius((Real)(1.0/(viewZoom*16.0)));
	param.setBoolean1(false);
	param.setBoolean2(false);

	OPERATOR_MANAGER()->applyOperator(new Operator_CreateObject(OT_CREATE_HINGE_JOINT, param));
}

//-------------------------------------------------------------------------------------------------------------------
void Action_Creater_HingeJoint::_onDrag(const fVector2& delta)
{
}

//-------------------------------------------------------------------------------------------------------------------
void Action_Creater_HingeJoint::draw(IRenderInterface* render)
{
}

}
