#include "libPhylix_stdafx.h"
#include "libPhylix_Action.h"
#include "libPhylix_ActionManager.h"
#include "libPhylix_Camera.h"

namespace Phylix
{
//-------------------------------------------------------------------------------------------------------------------
Action::Action()
	: m_dragStarted(false)
{
}

//-------------------------------------------------------------------------------------------------------------------
Action::~Action()
{
}

//-------------------------------------------------------------------------------------------------------------------
void Action::onBegin(int x, int y)
{
    m_dragStarted = true;
	m_dragOrigin = m_dragCurrent = CAMERA()->convertScreenToWorld(x, y, _needCameraGrid());
    m_dragDelta = fVector2(0, 0);
    _onBegin(m_dragCurrent);
}

//-------------------------------------------------------------------------------------------------------------------
void Action::onMotion(int x, int y)
{
	Camera* camera = CAMERA();
    if (!m_dragStarted) return;

	bool closeGridEnough;
	fVector2 pos_now = camera->convertScreenToWorld(x, y, _needCameraGrid(), &closeGridEnough);
	if(!closeGridEnough) return;

    //NSLog(@"motion:pt1=<%g,%g>, pt2=<%g,%g>", m_dragOrigin.x, m_dragOrigin.y, pos_now.x, pos_now.y);

	fVector2 dragDelta = pos_now - m_dragCurrent;

	if(floatCompare(dragDelta.x, 0)==0 && floatCompare(dragDelta.y, 0)==0) return;
	m_dragCurrent = pos_now;

	_onDrag(m_dragDelta=dragDelta);
}

//-------------------------------------------------------------------------------------------------------------------
void Action::onEnd(int x, int y, bool canceled)
{
	if(!m_dragStarted) return;

    _onEnd(CAMERA()->convertScreenToWorld(x, y, _needCameraGrid()), canceled);
    m_dragStarted = false;
}

//-------------------------------------------------------------------------------------------------------------------
void Action::onUpdate(int x, int y, float seconds)
{
    _onUpdate(CAMERA()->convertScreenToWorld(x, y, _needCameraGrid()), seconds);
}

}
