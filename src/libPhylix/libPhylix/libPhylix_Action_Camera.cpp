#include "libPhylix_stdafx.h"
#include "libPhylix_Action_Camera.h"
#include "libPhylix_Camera.h"
#include "libPhylix_ActionManager.h"

namespace Phylix
{

//-------------------------------------------------------------------------------------------------------------------
void Action_CameraPan::_onBegin(const fVector2& pt)
{
	m_anchorPos = pt;
}

//-------------------------------------------------------------------------------------------------------------------
void Action_CameraPan::_onDrag(const fVector2& /*delta*/)
{
	fVector2 delta = m_dragCurrent-m_anchorPos;

	CAMERA()->pan(delta);
}


}
