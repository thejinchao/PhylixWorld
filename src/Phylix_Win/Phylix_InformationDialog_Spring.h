#pragma once

#include "resource.h"

namespace Phylix
{

class CInformationDialog_Spring : public CDialogImpl<CInformationDialog_Spring>
{
public:
	/** 设置当前物体,并开始编辑 */
	void showObjectInformation(unsigned int idObject);

public:
	enum { IDD = IDD_DIALOG_INFORMATION_SPRING };

	/******************************************************
		Message Handle
	*******************************************************/
	BEGIN_MSG_MAP(CMaterialDlg)
		MESSAGE_HANDLER(WM_INITDIALOG, OnInitDialog)
		MESSAGE_HANDLER(WM_ACTIVATE, OnDialogActive)
		MESSAGE_HANDLER(WM_HSCROLL, OnHScroll)
		COMMAND_ID_HANDLER(IDOK, OnCloseCmd)
		COMMAND_ID_HANDLER(IDCANCEL, OnCloseCmd)
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
	void _updateUI_TargetLength(Real target_length, bool update_slider);
	void _updateUI_Damp(Real damp, bool update_slider);
	void _updateUI_Stiffness(Real stiffness, bool update_slider);
	void _updateSpring(void);

private:
	unsigned int m_idObject;		//!< 编辑的物体

	Real m_minStiffness, m_maxStiffness;
	Real m_minTargetLen, m_maxTargetLen;

public:
	CInformationDialog_Spring();
	virtual ~CInformationDialog_Spring();
};

}
