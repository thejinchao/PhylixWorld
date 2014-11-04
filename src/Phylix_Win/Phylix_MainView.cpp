#include "stdafx.h"
#include "Phylix_MainView.h"
#include "Phylix_Global.h"
#include "resource.h"

namespace Phylix
{
//-------------------------------------------------------------------------------------------------------------------
CMainView::CMainView()
	: m_actionBeforePan(AT_NULL)
{
}

//-------------------------------------------------------------------------------------------------------------------
CMainView::~CMainView()
{
}

//-------------------------------------------------------------------------------------------------------------------
void CMainView::OnGLInit(void)
{
	//init system
	RECT rect;
	GetClientRect(&rect);

	System::getSingleton().initSystem(rect.right-rect.left, rect.bottom-rect.top);
}

//-------------------------------------------------------------------------------------------------------------------
void CMainView::OnGLRender(void)
{
	//render system
	System::getSingleton().render();
}

//-------------------------------------------------------------------------------------------------------------------
void CMainView::OnGLResize(int width, int height)
{
	if(CAMERA())
	{
		CAMERA()->setWindowSize(width, height);
	}
}

//-------------------------------------------------------------------------------------------------------------------
BOOL CMainView::OnIdle(void)
{
	//Âß¼­Ö¡
	System::getSingleton().tick();

	//äÖÈ¾´°¿Ú
	RedrawWindow();
	return FALSE;
}

//-------------------------------------------------------------------------------------------------------------------
LRESULT CMainView::OnMouseMove(UINT key, const CPoint& point)
{
	Phylix::Action* action = ACTION_MANAGER()->getActiveAction();
	if(action)
	{
		action->onMotion(point.x, point.y);
	}

	return 0;
}

//-------------------------------------------------------------------------------------------------------------------
LRESULT CMainView::OnMouseLButtonDown(UINT key, const CPoint& point)
{
	Phylix::Action* action = ACTION_MANAGER()->getActiveAction();
	if(action)
	{
		SetCapture();
		action->onBegin(point.x, point.y);
	}

	return 0;
}

//-------------------------------------------------------------------------------------------------------------------
LRESULT CMainView::OnMouseLButtonUp(UINT key, const CPoint& point)
{
	Phylix::Action* action = ACTION_MANAGER()->getActiveAction();
	if(action)
	{
		ReleaseCapture();
		action->onEnd(point.x, point.y);
	}

	return 0;
}

//-------------------------------------------------------------------------------------------------------------------
LRESULT CMainView::OnMouseMButtonDown(UINT key, const CPoint& point)
{
	m_actionBeforePan = ACTION_MANAGER()->getActiveAction() ? ACTION_MANAGER()->getActiveAction()->getType() : Phylix::AT_NULL;

	Phylix::Action* panAction = ACTION_MANAGER()->setActiveAction(Phylix::AT_CAMERA_PAN);
	panAction->onBegin(point.x, point.y);
	SetCapture();
	return 0;
}

//-------------------------------------------------------------------------------------------------------------------
LRESULT CMainView::OnMouseMButtonUp(UINT key, const CPoint& point)
{
	Phylix::Action* panAction = ACTION_MANAGER()->setActiveAction(Phylix::AT_CAMERA_PAN);
	panAction->onEnd(point.x, point.y);
	ReleaseCapture();

	ACTION_MANAGER()->setActiveAction(m_actionBeforePan);
	return 0;
}

//-------------------------------------------------------------------------------------------------------------------
LRESULT CMainView::OnMouseWheel(UINT key, short zDelta, const CPoint& point)
{
	Phylix::Camera* camera = CAMERA();
	if(!camera) return 0;

	if (zDelta > 0)
	{
		camera->setViewZoom(camera->getViewZoom()*1.1f);
	}
	else
	{
		camera->setViewZoom(camera->getViewZoom()/1.1f);
	}
	return 0;
}

//-------------------------------------------------------------------------------------------------------------------
void CMainView::OnCharDown(TCHAR tchar, UINT repeatCount, UINT flags)
{
	switch(tchar)
	{
	case ' ':			SendMessage(GetParent(), WM_COMMAND, ID_SYSTEM_RUN, 0); break;
	case 'E': case 'e':	SendMessage(GetParent(), WM_COMMAND, ID_EDIT_EDGE, 0); break;
	case 'B': case 'b': SendMessage(GetParent(), WM_COMMAND, ID_EDIT_BOX, 0); break;
	case 'C': case 'c': SendMessage(GetParent(), WM_COMMAND, ID_EDIT_CIRCLE, 0); break;
	case 'P': case 'p': SendMessage(GetParent(), WM_COMMAND, ID_EDIT_POLYGON, 0); break;

	case 'F': case 'f': SendMessage(GetParent(), WM_COMMAND, ID_EDIT_FIXED_JOINT, 0); break;
	case 'H': case 'h': SendMessage(GetParent(), WM_COMMAND, ID_HINGE_JOINT, 0); break;
	case 'S': case 's': SendMessage(GetParent(), WM_COMMAND, ID_SPRING_JOINT, 0); break;

	case 0x1A:			SendMessage(GetParent(), WM_COMMAND, ID_EDIT_UNDO, 0); break;
	case 0x19:			SendMessage(GetParent(), WM_COMMAND, ID_EDIT_REDO, 0); break;

	case 'D': System::getSingleton().loadDemo(); break;
	case 'd': OBJECT_MANAGER()->debug("D:\\out.txt"); break;
	}
}

//-------------------------------------------------------------------------------------------------------------------
void CMainView::OnKeyDown(TCHAR tchar, UINT repeatCount, UINT flags)
{
	switch(tchar)
	{
	case VK_DELETE:		SendMessage(GetParent(), WM_COMMAND, ID_EDIT_DELETE, 0); break;

	}
}

}

