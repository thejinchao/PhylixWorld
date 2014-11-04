#include "libPhylix_stdafx.h"
#include "libPhylix_Action_Creater_Box.h"
#include "libPhylix_ActionManager.h"
#include "libPhylix_ObjectManager.h"
#include "libPhylix_RenderInterface.h"
#include "libPhylix_MaterialManager.h"
#include "libPhylix_OperatorManager.h"
#include "libPhylix_Operator.h"

namespace Phylix
{

//-------------------------------------------------------------------------------------------------------------------
Action_Creater_Box::Action_Creater_Box() 
{
}

//-------------------------------------------------------------------------------------------------------------------
Action_Creater_Box::~Action_Creater_Box() 
{
}

//-------------------------------------------------------------------------------------------------------------------
void Action_Creater_Box::_onBegin(const fVector2& pt)
{
	m_randomColor = MaterialManager::_createRandomColor();
}

//-------------------------------------------------------------------------------------------------------------------
void Action_Creater_Box::_onEnd(const fVector2& pt, bool canceled)
{
    //NSLog(@"end:pt1=<%g,%g>, pt2=<%g,%g>", m_dragOrigin.x, m_dragOrigin.y, pt.x, pt.y);

	//create box
	const fVector2& pt1 = m_dragOrigin;
	const fVector2& pt2 = pt;
	Real width = fabs(pt1.x-pt2.x);
	Real height = fabs(pt1.y-pt2.y);

	//如果太小，忽略
	if(floatCompare(width, 0.0)<=0 || floatCompare(height, 0.0)<=0 ) return;

	//构造创建参数
	ObjectParams param(Object::OT_ENTITY_BOX);
	param.setPosition1(0.5f*(pt1+pt2));
	param.setSize(fVector2(width, height));
	param.setMaterial(MATERIAL_MANAGER()->createStandardMaterial(m_randomColor));

	OPERATOR_MANAGER()->applyOperator(new Operator_CreateObject(OT_CREATE_BOX, param));
}

//-------------------------------------------------------------------------------------------------------------------
void Action_Creater_Box::_onDrag(const fVector2& delta)
{
}

//-------------------------------------------------------------------------------------------------------------------
void Action_Creater_Box::draw(IRenderInterface* render)
{
	if(!m_dragStarted || render==0) return;

	fVector2 pt1 = m_dragOrigin;
	fVector2 pt2 = m_dragCurrent;

	Real min_x = MIN(pt1.x, pt2.x);
	Real max_x = MAX(pt1.x, pt2.x);
	Real min_y = MIN(pt1.y, pt2.y);
	Real max_y = MAX(pt1.y, pt2.y);

	//draw box
	fVector2 vertices[4];
	vertices[0] = fVector2(min_x, max_y);
	vertices[1] = fVector2(min_x, min_y);
	vertices[2] = fVector2(max_x, min_y);
	vertices[3] = fVector2(max_x, max_y);

	render->drawPolygon(vertices, 4, Material(m_randomColor), true, true, false);
}

}
