#pragma once

#include "resource.h"

namespace Phylix
{

class CInformationDialog_Hinge : public CDialogImpl<CInformationDialog_Hinge>
{
public:
	/** 设置当前物体,并开始编辑 */
	void showObjectInformation(unsigned int idObject);

public:
	enum { IDD = IDD_DIALOG_INFORMATION_HINGE };

	/******************************************************
		Message Handle
	*******************************************************/
	BEGIN_MSG_MAP(CMaterialDlg)
		MESSAGE_HANDLER(WM_INITDIALOG, OnInitDialog)
		MESSAGE_HANDLER(WM_ACTIVATE, OnDialogActive)
		MESSAGE_HANDLER(WM_HSCROLL, OnHScroll)
		COMMAND_ID_HANDLER(IDOK, OnCloseCmd)
		COMMAND_ID_HANDLER(IDCANCEL, OnCloseCmd)
		COMMAND_ID_HANDLER(IDC_CHECK_MOTO_ENABLE, OnMotoCheck)
		COMMAND_ID_HANDLER(IDC_CHECK_MOTO_DIRECTION, OnMotoCheck)
	END_MSG_MAP()

	/******************************************************
		Message Handle Function
	*******************************************************/
	LRESULT OnInitDialog(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/);
	LRESULT OnCloseCmd(WORD /*wNotifyCode*/, WORD wID, HWND /*hWndCtl*/, BOOL& /*bHandled*/);
	LRESULT OnDialogActive(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/);
	LRESULT OnMotoCheck(WORD /*wNotifyCode*/, WORD wID, HWND /*hWndCtl*/, BOOL& /*bHandled*/);
	LRESULT OnHScroll(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/);

private:
	void _updateUI_Rate(Real new_value, bool updateSlider);
	void _updateMoto(void);

private:
	unsigned int m_idObject;		//!< 编辑的物体

public:
	CInformationDialog_Hinge();
	virtual ~CInformationDialog_Hinge();
};

}
