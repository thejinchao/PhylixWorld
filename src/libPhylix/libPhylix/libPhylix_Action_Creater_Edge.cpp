#include "libPhylix_stdafx.h"
#include "libPhylix_Action_Creater_Edge.h"
#include "libPhylix_ActionManager.h"
#include "libPhylix_ObjectManager.h"
#include "libPhylix_RenderInterface.h"
#include "libPhylix_MaterialManager.h"
#include "libPhylix_Scene.h"
#include "libPhylix_OperatorManager.h"
#include "libPhylix_operator.h"
#include "libPhylix_ObjectParams.h"
#include "libPhylix_Camera.h"

namespace Phylix
{

//-------------------------------------------------------------------------------------------------------------------
Action_Creater_Edge::Action_Creater_Edge() 
	: m_normal(0, 1)
{
}

//-------------------------------------------------------------------------------------------------------------------
Action_Creater_Edge::~Action_Creater_Edge() 
{
}

//-------------------------------------------------------------------------------------------------------------------
void Action_Creater_Edge::_onBegin(const fVector2& pt)
{
	m_randomColor = MaterialManager::_createRandomColor();
}

//-------------------------------------------------------------------------------------------------------------------
void Action_Creater_Edge::_onEnd(const fVector2& pt, bool canceled)
{
	fVector2 pos = m_dragOrigin;

	//构造创建参数
	ObjectParams param(Object::OT_ENTITY_EDGE);
	param.setPosition1(m_dragOrigin);
	param.setMaterial(MATERIAL_MANAGER()->createStandardMaterial(m_randomColor));

	fVector2 _n = m_normal;
	_n.normalize();
	param.setAngle(fastAtan2(_n.y, _n.x));

	OPERATOR_MANAGER()->applyOperator(new Operator_CreateObject(OT_CREATE_EDGE, param));

	m_polygon.vertices.clear();
}

//-------------------------------------------------------------------------------------------------------------------
Real _getAngleFromX(const fVector2& v)
{
	fVector2 P1 = fVector2(1.f, 0);	//X轴正向
	fVector2 P2 = v; P2.normalize();

	//计算点积
	Real dot = dotProduct(P1, P2);
	//是否是特殊值(0, pi/2)
	if(floatCompare(dot,  1.f)==0) return 0;
	if(floatCompare(dot, -1.f)==0) return MATH_PI;

	Real angle = acos(dot);
	//计算叉积
	Real cross = crossProduct(P1, P2);
	if(cross<0) angle = MATH_PI_DOUBLE-angle;

	return angle;
}

//-------------------------------------------------------------------------------------------------------------------
void Action_Creater_Edge::_onDrag(const fVector2& delta)
{
	fVector2 ptDir = m_dragCurrent-m_dragOrigin;
	if(floatCompare(ptDir.x, 0)!=0 || floatCompare(ptDir.y, 0)!=0) 
	{
		m_normal = ptDir;
	}

	//是否启动snap功能
	int SNAP_STEP_AT_HALF_CIRCLE = 12;

	Real asCircleRadius = CAMERA()->convertScreenSizeToWorld(ASSIST_CIRCLE_RADIUS);
	Real length = m_normal.length();
	if(length<asCircleRadius)
	{
		Real angle = _getAngleFromX(m_normal);

		Real _angle = (int)(angle/(MATH_PI/SNAP_STEP_AT_HALF_CIRCLE) + 0.5f) * (MATH_PI/SNAP_STEP_AT_HALF_CIRCLE);
		m_normal = fVector2(length*cos(_angle), length*sin(_angle));
	}

	//计算交界点
	fVector2 ptLeft, ptRight;
	boolean_intersection(SCENE()->getWorldBounderBox(), Edge(m_dragOrigin, m_normal), m_polygon, ptLeft, ptRight);
}

//-------------------------------------------------------------------------------------------------------------------
void Action_Creater_Edge::draw(IRenderInterface* render)
{
	if(!m_dragStarted || render==0 || m_polygon.vertices.empty()) return;

	render->drawPolygon(&(m_polygon.vertices[0]), (unsigned int)m_polygon.vertices.size(), Material(m_randomColor), true, true, false);

	//draw assis circle
	render->drawCircle(m_dragOrigin, CAMERA()->convertScreenSizeToWorld(ASSIST_CIRCLE_RADIUS), fColor(1.f, 1.f, 1.f, 1.f));

	fVector2 normal = m_normal; normal.normalize();

	fVector2 arrowStart = m_dragOrigin + CAMERA()->convertScreenSizeToWorld(ASSIST_CIRCLE_RADIUS*0.1f) * normal;
	fVector2 arrowEnd = m_dragOrigin + CAMERA()->convertScreenSizeToWorld(ASSIST_CIRCLE_RADIUS*0.9f) * normal;

	render->drawArrow(arrowStart, arrowEnd, fColor(1.f, 1.f, 1.f, 1.f));
}


}
