#include "stdafx.h"
#include "Phylix_MaterialDialog.h"
#include "Phylix_Global.h"

namespace Phylix
{

//-------------------------------------------------------------------------------------------------------------------
CMaterialDlg::CMaterialDlg()
	: m_currentSelected(0)
	, m_oldColorPickProc(0)
{
}

//-------------------------------------------------------------------------------------------------------------------
CMaterialDlg::~CMaterialDlg()
{
}

//-------------------------------------------------------------------------------------------------------------------
LRESULT CMaterialDlg::OnInitDialog(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/)
{
	//密度
	SendDlgItemMessage(IDC_SLIDER_DENSITY, TBM_SETRANGE, (WPARAM) FALSE, (LPARAM)MAKELONG(0, 100));
	//摩擦系数
	SendDlgItemMessage(IDC_SLIDER_FRICTION, TBM_SETRANGE, (WPARAM) FALSE, (LPARAM)MAKELONG(0, 100));
	//弹力系数
	SendDlgItemMessage(IDC_SLIDER_RESTITUTION, TBM_SETRANGE, (WPARAM) FALSE, (LPARAM)MAKELONG(0, 100));

	//材质列表
    // Create the full-sized icon image lists. 
    HIMAGELIST hLarge = ImageList_Create(GetSystemMetrics(SM_CXICON), 
                              GetSystemMetrics(SM_CYICON), 
                              ILC_MASK|ILC_COLOR24, 1, 1); 

    HIMAGELIST hSmall = ImageList_Create(GetSystemMetrics(SM_CXSMICON), 
                              GetSystemMetrics(SM_CYSMICON), 
                              ILC_MASK|ILC_COLOR24, 1, 1); 
    
    // Add an icon to each image list.  
	HBITMAP hIconBmp = LoadBitmap(::GetModuleHandle(0), MAKEINTRESOURCE(IDB_BITMAP_MATERIAL));

	ImageList_AddMasked(hLarge, hIconBmp, RGB(255,0, 255));
	ImageList_AddMasked(hSmall, hIconBmp, RGB(255,0, 255));

	::DeleteObject(hIconBmp);

	// Attach them
	SendDlgItemMessage(IDC_LIST_MATERIAL, LVM_SETIMAGELIST, LVSIL_NORMAL, (LPARAM)hLarge);
	SendDlgItemMessage(IDC_LIST_MATERIAL, LVM_SETIMAGELIST, LVSIL_SMALL, (LPARAM)hLarge);

	// add material icon
	LVITEM lvi;
	lvi.mask =  LVIF_IMAGE | LVIF_TEXT;

	for(int i=0; i<MATERIAL_MANAGER()->getMaterialTemplateCount(); i++)
	{
		const MaterialTemplate* material = MATERIAL_MANAGER()->getMaterialTemplateList()+i;

		lvi.iItem = i;
		lvi.iSubItem = 0;
		lvi.pszText = (LPTSTR)g_local.getString((STRING_ID)(SI_MATERIAL_TEMPLATE_NAME_0+i));
		lvi.iImage = i;
		SendDlgItemMessage(IDC_LIST_MATERIAL, LVM_INSERTITEM, 0, (LPARAM)&lvi);
	}

	//color picker process
	HWND colorPicker = GetDlgItem(IDC_COLOR_PICKER);
	::SetWindowLong(colorPicker, GWL_USERDATA, (LONG)(LONG_PTR)this);
	m_oldColorPickProc = (WNDPROC)(LONG_PTR)::SetWindowLong(colorPicker, GWL_WNDPROC, (LONG)(LONG_PTR)__colorPickerProc);

	return TRUE;
}

//-------------------------------------------------------------------------------------------------------------------
LRESULT CMaterialDlg::OnCloseCmd(WORD /*wNotifyCode*/, WORD wID, HWND /*hWndCtl*/, BOOL& /*bHandled*/)
{
	return 0;
}

//-------------------------------------------------------------------------------------------------------------------
LRESULT CMaterialDlg::OnDialogActive(UINT /*uMsg*/, WPARAM wParam, LPARAM lParam, BOOL& /*bHandled*/)
{
	if(wParam==WA_INACTIVE)
	{
		if(::GetParent((HWND)lParam) != m_hWnd)
		{
			ShowWindow(SW_HIDE);
		}
	}
	return 0;
}

//-------------------------------------------------------------------------------------------------------------------
void CMaterialDlg::_updateUI_Density(Real density, bool updateSlider)
{
	assert(density>=Material::MIN_DENSITY && density<=Material::MAX_DENSITY);

	if(updateSlider)
		SendDlgItemMessage(IDC_SLIDER_DENSITY, TBM_SETPOS, (WPARAM)TRUE, 
		(LPARAM)(((density-Material::MIN_DENSITY)/Material::MAX_DENSITY)*100.0));

	TCHAR temp[32]={0};
	_sntprintf(temp, 32, g_local.getString(SI_DENSITY_VALUE), density);
	SetDlgItemText(IDC_STATIC_DENSITY, temp);
}

//-------------------------------------------------------------------------------------------------------------------
void CMaterialDlg::_updateUI_Friction(Real friction, bool updateSlider)
{
	assert(friction>=0 && friction<=Material::MAX_FRICITION);

	if(updateSlider)
		SendDlgItemMessage(IDC_SLIDER_FRICTION, TBM_SETPOS, (WPARAM)TRUE, (LPARAM)((friction/Material::MAX_FRICITION)*100.0));

	TCHAR temp[32]={0};
	_sntprintf(temp, 32, g_local.getString(SI_FRICTION_VALUE), friction);
	SetDlgItemText(IDC_STATIC_FRICTION, temp);
}

//-------------------------------------------------------------------------------------------------------------------
void CMaterialDlg::_updateUI_Restitution(Real restitution, bool updateSlider)
{
	assert(restitution>=0 && restitution<=Material::MAX_RESTITUTION);

	if(updateSlider)
		SendDlgItemMessage(IDC_SLIDER_RESTITUTION, TBM_SETPOS, (WPARAM)TRUE, (LPARAM)((restitution/Material::MAX_RESTITUTION)*100.0));

	TCHAR temp[32]={0};
	_sntprintf(temp, 32, g_local.getString(SI_RESTITUTION_VALUE), restitution);
	SetDlgItemText(IDC_STATIC_RESTITUTION, temp);
}

//-------------------------------------------------------------------------------------------------------------------
void CMaterialDlg::_updateUI_MaterialTemplate(const MaterialTemplate* matTemplate)
{
	int selIndex = matTemplate==0 ? 0 : matTemplate->id;
	HWND hlist = GetDlgItem(IDC_LIST_MATERIAL);

	for(int i=0; i<=MATERIAL_MANAGER()->getMaterialTemplateCount(); i++)
	{
		if(i==selIndex)
		{
			ListView_SetItemState(hlist, i, LVIS_SELECTED, LVIS_SELECTED | LVIS_FOCUSED);

			m_currentSelected = i;
		}
		else
		{
			ListView_SetItemState(hlist, i, 0, LVIS_SELECTED | LVIS_FOCUSED);
		}
	}
}

//-------------------------------------------------------------------------------------------------------------------
void CMaterialDlg::editObjectMaterial(unsigned int idObject)
{
	m_idObject = idObject;

	Object* obj = OBJECT_MANAGER()->findObject(m_idObject);
	assert(obj);

	const Material& material = obj->getMaterial();

	//set ui
	_updateUI_Density(material.density, true);
	_updateUI_Friction(material.fricition, true);
	_updateUI_Restitution(material.restitution, true);
	_updateUI_Color(material.color);
	_updateUI_MaterialTemplate(material.pTemplate);

	//show window
	POINT pt = {365, 27};
	::ClientToScreen(GetParent(), &pt);
	SetWindowPos(HWND_TOP, pt.x, pt.y, 0, 0, SWP_NOSIZE|SWP_SHOWWINDOW);
}

//-------------------------------------------------------------------------------------------------------------------
void CMaterialDlg::_updateMaterial(void)
{
	Object* obj = OBJECT_MANAGER()->findObject(m_idObject);
	assert(obj);

	Material newMat(obj->getMaterial());

	int pos = (int)SendDlgItemMessage(IDC_SLIDER_DENSITY, TBM_GETPOS, 0, 0);
	newMat.density = (pos/100.0f)*(Material::MAX_DENSITY-Material::MIN_DENSITY) + Material::MIN_DENSITY;

	pos = (int)SendDlgItemMessage(IDC_SLIDER_FRICTION, TBM_GETPOS, 0, 0);
	newMat.fricition = (pos/100.0f)*Material::MAX_FRICITION;

	pos = (int)SendDlgItemMessage(IDC_SLIDER_RESTITUTION, TBM_GETPOS, 0, 0);
	newMat.restitution = (pos/100.0f)*Material::MAX_RESTITUTION;

	newMat.color = m_curMaterialColor;

	OPERATOR_MANAGER()->applyOperator(new Operator_UpdateObjectMaterial(m_idObject, newMat));
}

//-------------------------------------------------------------------------------------------------------------------
LRESULT CMaterialDlg::OnHScroll(UINT /*uMsg*/, WPARAM wParam, LPARAM lParam, BOOL& /*bHandled*/)
{
	Object* obj = OBJECT_MANAGER()->findObject(m_idObject);
	assert(obj);
	const Material& material = obj->getMaterial();

	//new pos
	int pos = (int)::SendMessage((HWND)lParam, TBM_GETPOS, 0, 0);

	switch(::GetDlgCtrlID((HWND)lParam))
	{
	case IDC_SLIDER_DENSITY:
		{
			Real new_value = (pos/100.0f)*(Material::MAX_DENSITY-Material::MIN_DENSITY) + Material::MIN_DENSITY;
			_updateUI_Density(new_value, false);

			if(LOWORD(wParam) == SB_ENDSCROLL) _updateMaterial();
		}
		break;
	case IDC_SLIDER_FRICTION:
		{
			Real new_value = (pos/100.0f)*Material::MAX_FRICITION;
			_updateUI_Friction(new_value, false);

			if(LOWORD(wParam) == SB_ENDSCROLL) _updateMaterial();
		}
		break;
	case IDC_SLIDER_RESTITUTION:
		{
			Real new_value = (pos/100.0f)*Material::MAX_RESTITUTION;
			_updateUI_Restitution(new_value, false);

			if(LOWORD(wParam) == SB_ENDSCROLL) _updateMaterial();
		}
		break;
	}
	return 0;
}

//-------------------------------------------------------------------------------------------------------------------
LRESULT CMaterialDlg::OnMaterialListItemChanged(INT id, LPNMHDR pNMHDR, BOOL& bHandled)
{
	NM_LISTVIEW* pNMListView = (NM_LISTVIEW*)pNMHDR;

	int index = pNMListView->iItem;
	UINT changedMask = pNMListView->uChanged;

	if(pNMListView->uChanged == LVIF_STATE && 
		((pNMListView->uOldState & LVIS_SELECTED)==0) &&
		((pNMListView->uNewState & LVIS_SELECTED)!=0))
	{
		if( index == m_currentSelected) return 0;

		//changed!
		_onMaterialTemplateChanged(index);
	}

	return 0;
}

//-------------------------------------------------------------------------------------------------------------------
void CMaterialDlg::_updateUI_Color(const fColor& color)
{
	m_curMaterialColor = color;
	::InvalidateRect(GetDlgItem(IDC_COLOR_PICKER), 0, FALSE);

	TCHAR temp[32]={0};
	_sntprintf(temp, 32, g_local.getString(SI_COLOR_VALUE), 
		real2int(color.r*255), real2int(color.g*255), real2int(color.b*255));
	SetDlgItemText(IDC_STATIC_COLOR, temp);
}

//-------------------------------------------------------------------------------------------------------------------
void CMaterialDlg::_onMaterialTemplateChanged(int index)
{
	Object* obj = OBJECT_MANAGER()->findObject(m_idObject);
	assert(obj);

	Material newMat(obj->getMaterial());

	MaterialTemplate* newTemplate =MATERIAL_MANAGER()->getMaterialTemplateList()+index;

	newMat.density = newTemplate->density;
	_updateUI_Density(newMat.density, true);

	newMat.fricition = newTemplate->fricition;
	_updateUI_Friction(newMat.fricition, true);

	newMat.restitution = newTemplate->restitution;
	_updateUI_Restitution(newMat.restitution, true);

	newMat.color = newTemplate->color;
	_updateUI_Color(newMat.color);

	newMat.pTemplate = newTemplate;

	OPERATOR_MANAGER()->applyOperator(new Operator_UpdateObjectMaterial(m_idObject, newMat));
}

//-------------------------------------------------------------------------------------------------------------------
LRESULT CALLBACK CMaterialDlg::__colorPickerProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	CMaterialDlg* pThis = (CMaterialDlg*)(LONG_PTR)::GetWindowLong(hwnd, GWL_USERDATA);
	
	if(pThis)
	{
		return pThis->_colorPickerProc(hwnd, uMsg, wParam, lParam);
	}

	return ::DefWindowProc(hwnd, uMsg, wParam, lParam);
}

//-------------------------------------------------------------------------------------------------------------------
LRESULT CMaterialDlg::_colorPickerProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	switch(uMsg)
	{
	case WM_ERASEBKGND:
		{
			return TRUE;
		}
		break;

	case WM_PAINT:
		{
			PAINTSTRUCT ps;
			HDC hdc = ::BeginPaint(hwnd, &ps);

			RECT rect;
			::GetClientRect(hwnd, &rect);

			HBRUSH hbr = ::CreateSolidBrush(RGB(
				real2int(m_curMaterialColor.r*255), 
				real2int(m_curMaterialColor.g*255), 
				real2int(m_curMaterialColor.b*255)));

			::FillRect(hdc, &rect, hbr);
			::DeleteObject(hbr);
			
			::EndPaint(hwnd, &ps);
			return TRUE;
		}
		break;

	case WM_LBUTTONUP:
		{
			Object* obj = OBJECT_MANAGER()->findObject(m_idObject);
			assert(obj);

			fColor col = obj->getMaterial().color;

			CColorDialog dlg(
				RGB(real2int(col.r*255), real2int(col.g*255), real2int(col.b*255)), 
				CC_FULLOPEN, m_hWnd);
			if(IDOK != dlg.DoModal()) break;

			COLORREF newColor = dlg.GetColor();
			fColor fNewColor(GetRValue(newColor)/255.f, GetGValue(newColor)/255.f, GetBValue(newColor)/255.f, 1.f);

			_updateUI_Color(fNewColor);

			_updateMaterial();
		}
		break;
	}

	if(m_oldColorPickProc)
	{
		return m_oldColorPickProc(hwnd, uMsg, wParam, lParam);
	}
	return TRUE;
}

}
