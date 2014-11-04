#pragma once

#include "resource.h"

namespace Phylix
{

class CInformationDialog_Entity : public CDialogImpl<CInformationDialog_Entity>
{
public:
	/** 设置当前物体,并开始编辑 */
	void showObjectInformation(unsigned int idObject);

public:
	enum { IDD = IDD_DIALOG_INFORMATION_ENTITY };

	/******************************************************
		Message Handle
	*******************************************************/
	BEGIN_MSG_MAP(CMaterialDlg)
		MESSAGE_HANDLER(WM_INITDIALOG, OnInitDialog)
		MESSAGE_HANDLER(WM_ACTIVATE, OnDialogActive)
		COMMAND_ID_HANDLER(IDOK, OnCloseCmd)
		COMMAND_ID_HANDLER(IDCANCEL, OnCloseCmd)
	END_MSG_MAP()

	/******************************************************
		Message Handle Function
	*******************************************************/
	LRESULT OnInitDialog(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/);
	LRESULT OnCloseCmd(WORD /*wNotifyCode*/, WORD wID, HWND /*hWndCtl*/, BOOL& /*bHandled*/);
	LRESULT OnDialogActive(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/);

private:
	void _updateUI_Area(Real area);
	void _updateUI_Mass(Real mass);
	void _updateUI_Moment(Real moment);

private:
	unsigned int m_idObject;		//!< 编辑的物体

public:
	CInformationDialog_Entity();
	virtual ~CInformationDialog_Entity();
};

}
