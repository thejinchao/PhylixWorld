#pragma once

#include "resource.h"
#include "Phylix_MainView.h"
#include "Phylix_MaterialDialog.h"
#include "Phylix_InformationDialog_Hinge.h"
#include "Phylix_InformationDialog_Spring.h"
#include "Phylix_InformationDialog_Entity.h"

namespace Phylix
{

class CMainFrame : public CFrameWindowImpl<CMainFrame>, public CUpdateUI<CMainFrame>, public CMessageFilter, public CIdleHandler
{
public:
	DECLARE_FRAME_WND_CLASS(NULL, IDR_MAINFRAME)
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	virtual BOOL OnIdle(void);

	/******************************************************
		Message Handle
	*******************************************************/
	BEGIN_MSG_MAP_EX(CMainFrame)
		MSG_WM_CREATE(OnCreate)

		COMMAND_ID_HANDLER(ID_SYSTEM_RUN, OnSystemRun)
		COMMAND_ID_HANDLER(ID_EDIT_EDGE, OnDrawEdge)
		COMMAND_ID_HANDLER(ID_EDIT_BOX, OnDrawBox)
		COMMAND_ID_HANDLER(ID_EDIT_CIRCLE, OnDrawCircle)
		COMMAND_ID_HANDLER(ID_EDIT_POLYGON, OnDrawPolygon)
		COMMAND_ID_HANDLER(ID_EDIT_FIXED_JOINT, OnDrawFixedJoint)
		COMMAND_ID_HANDLER(ID_HINGE_JOINT, OnDrawHingeJoint)
		COMMAND_ID_HANDLER(ID_SPRING_JOINT, OnDrawSpringJoint)
		COMMAND_ID_HANDLER(ID_EDIT_CHAIN, OnDrawChain)
		COMMAND_ID_HANDLER(ID_MODE_SELECT, OnModeSelect)
		COMMAND_ID_HANDLER(ID_EDIT_DELETE, OnEditDelete)
		COMMAND_ID_HANDLER(ID_EDIT_UNDO, OnEditUndo)
		COMMAND_ID_HANDLER(ID_EDIT_REDO, OnEditRedo)
		COMMAND_ID_HANDLER(ID_EDIT_DRAG, OnEditDragObject)
		COMMAND_ID_HANDLER(ID_EDIT_MATERIAL, OnEditMaterial)
		COMMAND_ID_HANDLER(ID_SYSTEM_GRID, OnSystemGrid)
		COMMAND_ID_HANDLER(ID_EDIT_INFORMATION, OnEditInformation)
		COMMAND_ID_HANDLER(ID_FILE_OPEN, OnFileOpen)
		COMMAND_ID_HANDLER(ID_FILE_SAVE, OnFileSave)

		CHAIN_MSG_MAP(CUpdateUI<CMainFrame>)
		CHAIN_MSG_MAP(CFrameWindowImpl<CMainFrame>)
	END_MSG_MAP()

	/******************************************************
		Update UI
	*******************************************************/
	BEGIN_UPDATE_UI_MAP(CMainFrame)
		UPDATE_ELEMENT(ID_SYSTEM_RUN, UPDUI_TOOLBAR)
		UPDATE_ELEMENT(ID_SYSTEM_GRID, UPDUI_TOOLBAR)
		UPDATE_ELEMENT(ID_EDIT_EDGE, UPDUI_TOOLBAR)
		UPDATE_ELEMENT(ID_EDIT_BOX, UPDUI_TOOLBAR)
		UPDATE_ELEMENT(ID_EDIT_CIRCLE, UPDUI_TOOLBAR)
		UPDATE_ELEMENT(ID_EDIT_POLYGON, UPDUI_TOOLBAR)
		UPDATE_ELEMENT(ID_EDIT_FIXED_JOINT, UPDUI_TOOLBAR)
		UPDATE_ELEMENT(ID_HINGE_JOINT, UPDUI_TOOLBAR)
		UPDATE_ELEMENT(ID_SPRING_JOINT, UPDUI_TOOLBAR)
		UPDATE_ELEMENT(ID_MODE_SELECT, UPDUI_TOOLBAR)
		UPDATE_ELEMENT(ID_EDIT_CHAIN, UPDUI_TOOLBAR)
		UPDATE_ELEMENT(ID_EDIT_DELETE, UPDUI_TOOLBAR)
		UPDATE_ELEMENT(ID_EDIT_UNDO, UPDUI_TOOLBAR)
		UPDATE_ELEMENT(ID_EDIT_REDO, UPDUI_TOOLBAR)
		UPDATE_ELEMENT(ID_EDIT_MOVE, UPDUI_TOOLBAR)
		UPDATE_ELEMENT(ID_EDIT_DRAG, UPDUI_TOOLBAR)
		UPDATE_ELEMENT(ID_EDIT_MATERIAL, UPDUI_TOOLBAR)
		UPDATE_ELEMENT(ID_EDIT_INFORMATION, UPDUI_TOOLBAR)
	END_UPDATE_UI_MAP()

	/******************************************************
		Message Handle Function
	*******************************************************/
	int OnCreate(LPCREATESTRUCT /*lpCreateStruct*/);
	LRESULT OnSystemRun(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/);

	LRESULT OnDrawEdge(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/);
	LRESULT OnDrawBox(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/);
	LRESULT OnDrawCircle(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/);
	LRESULT OnDrawPolygon(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/);
	LRESULT OnDrawFixedJoint(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/);
	LRESULT OnDrawHingeJoint(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/);
	LRESULT OnDrawSpringJoint(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/);
	LRESULT OnModeSelect(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/);
	LRESULT OnEditDelete(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/);
	LRESULT OnEditUndo(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/);
	LRESULT OnEditRedo(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/);
	LRESULT OnEditDragObject(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/);
	LRESULT OnEditMaterial(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/);
	LRESULT OnSystemGrid(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/);
	LRESULT OnEditInformation(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/);
	LRESULT OnFileOpen(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/);
	LRESULT OnFileSave(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/);
	LRESULT OnDrawChain(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/);
	
	
private:
	CMainView m_view;		//Canvas View
	CMaterialDlg m_materialDialg; //材质窗口
	CInformationDialog_Hinge m_infoDialog_Hinage;	//详细信息对话框(旋转铰链)
	CInformationDialog_Spring m_infoDialog_Spring;	//详细信息对话框(弹簧)
	CInformationDialog_Entity m_infoDialog_Entity;	//详细信息对话框(实体)

public:
	CMainFrame();
	virtual ~CMainFrame();
};

}

