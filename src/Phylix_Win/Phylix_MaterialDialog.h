#pragma once

#include "resource.h"

namespace Phylix
{

class CMaterialDlg : public CDialogImpl<CMaterialDlg>
{
public:
	/** 设置当前材质,并开始编辑 */
	void editObjectMaterial(unsigned int idObject);

public:
	enum { IDD = IDD_DIALOG_MATERIAL };

	/******************************************************
		Message Handle
	*******************************************************/
	BEGIN_MSG_MAP(CMaterialDlg)
		MESSAGE_HANDLER(WM_INITDIALOG, OnInitDialog)
		MESSAGE_HANDLER(WM_ACTIVATE, OnDialogActive)
		MESSAGE_HANDLER(WM_HSCROLL, OnHScroll)
		COMMAND_ID_HANDLER(IDOK, OnCloseCmd)
		COMMAND_ID_HANDLER(IDCANCEL, OnCloseCmd)
		NOTIFY_HANDLER(IDC_LIST_MATERIAL, LVN_ITEMCHANGED, OnMaterialListItemChanged)
	END_MSG_MAP()

	/******************************************************
		Message Handle Function
	*******************************************************/
	LRESULT OnInitDialog(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/);
	LRESULT OnCloseCmd(WORD /*wNotifyCode*/, WORD wID, HWND /*hWndCtl*/, BOOL& /*bHandled*/);
	LRESULT OnHScroll(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/);
	LRESULT OnDialogActive(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/);
	LRESULT OnMaterialListItemChanged(INT /*id*/, LPNMHDR /*pNMHDR*/, BOOL& /*bHandled*/);

private:
	void _updateUI_Density(Real density, bool updateSlider);
	void _updateUI_Friction(Real friction, bool updateSlider);
	void _updateUI_Restitution(Real restitution, bool updateSlider);
	void _updateUI_Color(const fColor& color);
	void _updateUI_MaterialTemplate(const MaterialTemplate* matTemplate);

	void _updateMaterial(void);
	void _onMaterialTemplateChanged(int index);

private:
	unsigned int m_idObject;		//!< 编辑的物体
	int m_currentSelected;			//!< 当前选中的index

private:
	static LRESULT CALLBACK __colorPickerProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
	LRESULT _colorPickerProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam);

	WNDPROC m_oldColorPickProc;
	fColor m_curMaterialColor;

public:
	CMaterialDlg();
	virtual ~CMaterialDlg();
};

}
