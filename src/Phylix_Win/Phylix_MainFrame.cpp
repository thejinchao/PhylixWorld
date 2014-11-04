#include "stdafx.h"
#include "Phylix_MainFrame.h"
#include "Phylix_Global.h"
#include "Phylix_FileStream.h"

namespace Phylix
{
//-------------------------------------------------------------------------------------------------------------------
CMainFrame::CMainFrame()
{
}

//-------------------------------------------------------------------------------------------------------------------
CMainFrame::~CMainFrame()
{
}

//-------------------------------------------------------------------------------------------------------------------
BOOL CMainFrame::PreTranslateMessage(MSG* pMsg)
{
	return CFrameWindowImpl<CMainFrame>::PreTranslateMessage(pMsg);
}

//-------------------------------------------------------------------------------------------------------------------
BOOL CMainFrame::OnIdle(void)
{
	UISetCheck(ID_SYSTEM_RUN, !System::getSingleton().isSystemPause());
	UISetCheck(ID_SYSTEM_GRID, System::getSingleton().isGridMode());

	Phylix::Action* activeAction = ACTION_MANAGER()->getActiveAction();
	
	ACTION_TYPE activeActionType = activeAction ? activeAction->getType() : AT_NULL;
	UISetCheck(ID_EDIT_EDGE, activeActionType==AT_CREATER_EDGE);
	UISetCheck(ID_EDIT_BOX, activeActionType==AT_CREATER_BOX);
	UISetCheck(ID_EDIT_CIRCLE, activeActionType==AT_CREATER_CIRCLE);
	UISetCheck(ID_EDIT_POLYGON, activeActionType==AT_CREATER_POLYGON);
	UISetCheck(ID_EDIT_FIXED_JOINT, activeActionType==AT_CREATE_FIX_JOINT);
	UISetCheck(ID_HINGE_JOINT, activeActionType==AT_CREATE_HINGE_JOINT);
	UISetCheck(ID_SPRING_JOINT, activeActionType==AT_CREATE_SPRING_JOINT);
	UISetCheck(ID_EDIT_CHAIN, activeActionType==AT_CREATE_CHAIN);

	UISetCheck(ID_EDIT_DRAG, activeActionType==AT_DRAG_OBJECT);

	UISetCheck(ID_MODE_SELECT, activeActionType==AT_SELECT);
	UIEnable(ID_EDIT_DELETE, SCENE()->getSelectedObject()!=0);
	UIEnable(ID_EDIT_MATERIAL, SCENE()->getSelectedObject()!=0);
	UIEnable(ID_EDIT_INFORMATION, SCENE()->getSelectedObject()!=0);

	UIEnable(ID_EDIT_UNDO, OPERATOR_MANAGER()->canUndo());
	UIEnable(ID_EDIT_REDO, OPERATOR_MANAGER()->canRedo());

	UIEnable(ID_EDIT_MOVE, FALSE);
	UIEnable(ID_EDIT_DRAG, !System::getSingleton().isSystemPause());

	UIUpdateToolBar();

	OPERATOR_MANAGER()->_debug();

	return FALSE;
}

//-------------------------------------------------------------------------------------------------------------------
int CMainFrame::OnCreate(LPCREATESTRUCT /*lpCreateStruct*/)
{
	//remove old menu
	SetMenu(NULL);	

	// create toolbar and rebar
	HWND hWndToolBar = CreateSimpleToolBarCtrl(m_hWnd, IDR_MAINFRAME, FALSE, ATL_SIMPLE_TOOLBAR_PANE_STYLE);
	CreateSimpleReBar(ATL_SIMPLE_REBAR_NOBORDER_STYLE);
	AddSimpleReBarBand(hWndToolBar, NULL, TRUE);

	UIAddToolBar(hWndToolBar);

	// create view window
	m_hWndClient = m_view.Create(m_hWnd, rcDefault, NULL, WS_CHILD | WS_VISIBLE | WS_CLIPSIBLINGS | WS_CLIPCHILDREN, WS_EX_CLIENTEDGE);

	//create material dialog
	m_materialDialg.Create(m_hWnd);
	m_infoDialog_Hinage.Create(m_hWnd);
	m_infoDialog_Spring.Create(m_hWnd);
	m_infoDialog_Entity.Create(m_hWnd);

	//add idle filter
	CMessageLoop* pLoop = _Module.GetMessageLoop();
	ATLASSERT(pLoop != NULL);
	pLoop->AddMessageFilter(this);
	pLoop->AddIdleHandler(this);
	pLoop->AddIdleHandler(&m_view);

	return 0;
}

//-------------------------------------------------------------------------------------------------------------------
LRESULT CMainFrame::OnSystemRun(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/)
{
	bool is_pause = System::getSingleton().isSystemPause();
	System::getSingleton().pauseSystem(!is_pause);

	//重新运行系统
	if(is_pause)
	{
		//清空redo列表
		OPERATOR_MANAGER()->cleanRedoList();
	}
	return 0;
}

//-------------------------------------------------------------------------------------------------------------------
LRESULT CMainFrame::OnDrawEdge(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/)
{
	ACTION_MANAGER()->setActiveAction(Phylix::AT_CREATER_EDGE);
	return 0;
}

//-------------------------------------------------------------------------------------------------------------------
LRESULT CMainFrame::OnDrawBox(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/)
{
	ACTION_MANAGER()->setActiveAction(Phylix::AT_CREATER_BOX);
	return 0;
}

//-------------------------------------------------------------------------------------------------------------------
LRESULT CMainFrame::OnDrawCircle(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/)
{
	ACTION_MANAGER()->setActiveAction(Phylix::AT_CREATER_CIRCLE);
	return 0;
}

//-------------------------------------------------------------------------------------------------------------------
LRESULT CMainFrame::OnDrawPolygon(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/)
{
	ACTION_MANAGER()->setActiveAction(Phylix::AT_CREATER_POLYGON);
	return 0;
}

//-------------------------------------------------------------------------------------------------------------------
LRESULT CMainFrame::OnDrawFixedJoint(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/)
{
	ACTION_MANAGER()->setActiveAction(Phylix::AT_CREATE_FIX_JOINT);
	return 0;
}

//-------------------------------------------------------------------------------------------------------------------
LRESULT CMainFrame::OnDrawHingeJoint(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/)
{
	ACTION_MANAGER()->setActiveAction(Phylix::AT_CREATE_HINGE_JOINT);
	return 0;
}

//-------------------------------------------------------------------------------------------------------------------
LRESULT CMainFrame::OnDrawSpringJoint(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/)
{
	ACTION_MANAGER()->setActiveAction(Phylix::AT_CREATE_SPRING_JOINT);
	return 0;
}

//-------------------------------------------------------------------------------------------------------------------
LRESULT CMainFrame::OnDrawChain(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/)
{
	ACTION_MANAGER()->setActiveAction(Phylix::AT_CREATE_CHAIN);
	return 0;
}

//-------------------------------------------------------------------------------------------------------------------
LRESULT CMainFrame::OnModeSelect(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/)
{
	ACTION_MANAGER()->setActiveAction(Phylix::AT_SELECT);
	return 0;
}

//-------------------------------------------------------------------------------------------------------------------
LRESULT CMainFrame::OnEditDelete(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/)
{
	unsigned int selObj = SCENE()->getSelectedObject();
	if(selObj==0) return 0;

	OPERATOR_MANAGER()->applyOperator(new Operator_DeleteObject(selObj));
	return 0;
}

//-------------------------------------------------------------------------------------------------------------------
LRESULT CMainFrame::OnEditUndo(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/)
{
	System::getSingleton().pauseSystem(true);
	OPERATOR_MANAGER()->undo();
	return 0;
}

//-------------------------------------------------------------------------------------------------------------------
LRESULT CMainFrame::OnEditRedo(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/)
{
	OPERATOR_MANAGER()->redo();
	return 0;
}

//-------------------------------------------------------------------------------------------------------------------
LRESULT CMainFrame::OnEditDragObject(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/)
{
	if(System::getSingleton().isSystemPause()) return 0;

	ACTION_MANAGER()->setActiveAction(Phylix::AT_DRAG_OBJECT);
	return 0;
}

//-------------------------------------------------------------------------------------------------------------------
LRESULT CMainFrame::OnEditMaterial(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/)
{
	unsigned int selObj = SCENE()->getSelectedObject();
	if(selObj==0) return 0;

	m_materialDialg.editObjectMaterial(selObj);
	return 0;
}

//-------------------------------------------------------------------------------------------------------------------
LRESULT CMainFrame::OnSystemGrid(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/)
{
	bool grid_mode = System::getSingleton().isGridMode();
	System::getSingleton().setGridMode(!grid_mode);

	return 0;
}


//-------------------------------------------------------------------------------------------------------------------
LRESULT CMainFrame::OnEditInformation(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/)
{
	unsigned int selObj = SCENE()->getSelectedObject();
	if(selObj==0) return 0;
	Object* obj = OBJECT_MANAGER()->findObject(selObj);
	assert(obj);

	if(Object::isEntityType(obj->getType()))
	{
		m_infoDialog_Entity.showObjectInformation(selObj);
	}
	else if(obj->getType()==Object::OT_HINGE_JOINT)
	{
		m_infoDialog_Hinage.showObjectInformation(selObj);
	}
	else if(obj->getType()==Object::OT_SPRING_JOINT)
	{
		m_infoDialog_Spring.showObjectInformation(selObj);
	}
	return 0;
}

//-------------------------------------------------------------------------------------------------------------------
LRESULT CMainFrame::OnFileOpen(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/)
{
//	FileStream fileStream("d:\\test.xml", "rb");
//	SCENE()->loadScene(&fileStream);
	return 0;
}

//-------------------------------------------------------------------------------------------------------------------
LRESULT CMainFrame::OnFileSave(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/)
{
//	FileStream fileStream("d:\\test.xml", "wb");
//	SCENE()->saveScene(&fileStream);
	return 0;
}

}
