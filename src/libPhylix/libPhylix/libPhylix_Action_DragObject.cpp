#include "libPhylix_stdafx.h"
#include "libPhylix_Action_DragObject.h"
#include "libPhylix_ActionManager.h"
#include "libPhylix_Object_Entity.h"
#include "libPhylix_Scene.h"
#include "libPhylix_RenderInterface.h"
#include "libPhylix_MassBody.h"

namespace Phylix
{

//-------------------------------------------------------------------------------------------------------------------
void Action_DragObject::_onBegin(const fVector2& pt)
{
	//清空选择
	m_dragObject = 0;
	SCENE()->selectObject(0);

	//是否选择上一个活动物体
	Object* obj = SCENE()->queryFirstObjectFromPoint(pt);
	if(!obj) return;
	if(!(obj->isEntityType(obj->getType()))) return;
	
	//是否拥有质量体
	MassBody* massBody = ((ObjectEntity*)obj)->getMassBody();
	if(massBody==0) return;

	//ok
	
	//选择物体
	SCENE()->selectObject(obj->getID());
	m_dragObject = (ObjectEntity*)obj;

	//创建鼠标质量体
	m_mouseBody = cpBodyNew(INFINITY, INFINITY);
	m_mouseBody->p = cpv(pt.x, pt.y);

	m_dragPoint = m_dragObject->getTrnasFormMatrix().solve(pt);

	//创建弹簧
	cpBody *body = massBody->getImpl();
	m_springConstraint = cpPivotJointNew2(m_mouseBody, body, cpvzero, cpv(m_dragPoint.x, m_dragPoint.y));
	m_springConstraint->maxForce = 50000.0f;
	m_springConstraint->errorBias = cpfpow(1.0f - 0.15f, 60.0f);
	cpSpaceAddConstraint(SCENE()->getImpl(), m_springConstraint);
}

//-------------------------------------------------------------------------------------------------------------------
void Action_DragObject::_onDrag(const fVector2& delta)
{
	if(m_dragObject==0) return;

	m_mouseBody->p = cpv(m_dragCurrent.x, m_dragCurrent.y);
	m_mouseBody->v = cpvmult(cpv(delta.x, delta.y), 60.0f);
}

//-------------------------------------------------------------------------------------------------------------------
void Action_DragObject::_onEnd(const fVector2& pt, bool canceled)
{
	if(m_dragObject==0) return;

	cpSpaceRemoveConstraint(SCENE()->getImpl(), m_springConstraint);
	cpConstraintFree(m_springConstraint);
	m_springConstraint = 0;

	cpBodyDestroy(m_mouseBody);
	m_mouseBody = 0;

	m_dragObject = 0;
}

//-------------------------------------------------------------------------------------------------------------------
void Action_DragObject::draw(IRenderInterface* render)
{
	if(!m_dragStarted || render==0 || m_dragObject==0) return;

	render->drawSegment(m_dragObject->getTrnasFormMatrix().transform(m_dragPoint), m_dragCurrent, 1.0, fColor(1.0, 1.0, 1.0, 1.0));
}

}
