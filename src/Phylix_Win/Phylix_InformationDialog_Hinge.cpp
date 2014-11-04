#include "stdafx.h"
#include "Phylix_InformationDialog_Hinge.h"
#include "Phylix_Global.h"

namespace Phylix
{

//-------------------------------------------------------------------------------------------------------------------
CInformationDialog_Hinge::CInformationDialog_Hinge()
{
}

//-------------------------------------------------------------------------------------------------------------------
CInformationDialog_Hinge::~CInformationDialog_Hinge()
{
}

//-------------------------------------------------------------------------------------------------------------------
LRESULT CInformationDialog_Hinge::OnInitDialog(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/)
{
	TCHAR temp[32]={0};

	_sntprintf(temp, 32, g_local.getString(SI_ENGINE_ENABLE));
	SetDlgItemText(IDC_CHECK_MOTO_ENABLE, temp);

	_sntprintf(temp, 32, g_local.getString(SI_ENGINE_DIRECTION));
	SetDlgItemText(IDC_CHECK_MOTO_DIRECTION, temp);

	_sntprintf(temp, 32, g_local.getString(SI_ENGINE_RATE));
	SetDlgItemText(IDC_STATIC_MOTO_RATE, temp);

	//×ªËÙ
	SendDlgItemMessage(IDC_SLIDER_MOTO_RATE, TBM_SETRANGE, (WPARAM) FALSE, (LPARAM)MAKELONG(0, 100));

	return TRUE;
}

//-------------------------------------------------------------------------------------------------------------------
LRESULT CInformationDialog_Hinge::OnCloseCmd(WORD /*wNotifyCode*/, WORD wID, HWND /*hWndCtl*/, BOOL& /*bHandled*/)
{
	return 0;
}

//-------------------------------------------------------------------------------------------------------------------
LRESULT CInformationDialog_Hinge::OnDialogActive(UINT /*uMsg*/, WPARAM wParam, LPARAM lParam, BOOL& /*bHandled*/)
{
	if(wParam==WA_INACTIVE)
	{
		ShowWindow(SW_HIDE);
	}
	return 0;
}

//-------------------------------------------------------------------------------------------------------------------
void CInformationDialog_Hinge::showObjectInformation(unsigned int idObject)
{
	m_idObject = idObject;

	Object* obj = OBJECT_MANAGER()->findObject(m_idObject);
	assert(obj && obj->getType()==Object::OT_HINGE_JOINT);

	const ObjectHingeJoint::MotoParam param = ((ObjectHingeJoint*)obj)->getMotoParam();

	//set ui
	CheckDlgButton(IDC_CHECK_MOTO_ENABLE, param.enableMoto?BST_CHECKED:BST_UNCHECKED);
	CheckDlgButton(IDC_CHECK_MOTO_DIRECTION, param.is_cw?BST_CHECKED:BST_UNCHECKED);
	_updateUI_Rate(param.rate, true);

	//show window
	POINT pt = {400, 27};
	::ClientToScreen(GetParent(), &pt);
	SetWindowPos(HWND_TOP, pt.x, pt.y, 0, 0, SWP_NOSIZE|SWP_SHOWWINDOW);
}

//-------------------------------------------------------------------------------------------------------------------
LRESULT CInformationDialog_Hinge::OnMotoCheck(WORD /*wNotifyCode*/, WORD wID, HWND /*hWndCtl*/, BOOL& /*bHandled*/)
{
	Object* obj = OBJECT_MANAGER()->findObject(m_idObject);
	assert(obj);

	if(obj->getType()==Object::OT_HINGE_JOINT)
	{
		_updateMoto();
	}

	return 0;
}

//-------------------------------------------------------------------------------------------------------------------
LRESULT CInformationDialog_Hinge::OnHScroll(UINT /*uMsg*/, WPARAM wParam, LPARAM lParam, BOOL& /*bHandled*/)
{
	Object* obj = OBJECT_MANAGER()->findObject(m_idObject);
	assert(obj);

	int pos = (int)::SendMessage((HWND)lParam, TBM_GETPOS, 0, 0);

	switch(::GetDlgCtrlID((HWND)lParam))
	{
	case IDC_SLIDER_MOTO_RATE:
		{
			Real new_value = (pos/100.0f)*(ObjectHingeJoint::MOTO_MAX_RATE-ObjectHingeJoint::MOTO_MIN_RATE) + ObjectHingeJoint::MOTO_MIN_RATE;
			_updateUI_Rate(new_value, false);

			if(LOWORD(wParam) == SB_ENDSCROLL)
			{
				_updateMoto();
			}
		}
	}
	return 0;
}

//-------------------------------------------------------------------------------------------------------------------
void CInformationDialog_Hinge::_updateUI_Rate(Real rate, bool updateSlider)
{
	assert(rate>=ObjectHingeJoint::MOTO_MIN_RATE && rate<=ObjectHingeJoint::MOTO_MAX_RATE);

	if(updateSlider)
		SendDlgItemMessage(IDC_SLIDER_MOTO_RATE, TBM_SETPOS, (WPARAM)TRUE, 
			(LPARAM)(((rate-ObjectHingeJoint::MOTO_MIN_RATE)/(ObjectHingeJoint::MOTO_MAX_RATE-ObjectHingeJoint::MOTO_MIN_RATE))*100.0));

	TCHAR temp[32]={0};
	_sntprintf(temp, 32, g_local.getString(SI_ENGINE_RATE), rate);
	SetDlgItemText(IDC_STATIC_MOTO_RATE, temp);
}

//-------------------------------------------------------------------------------------------------------------------
void CInformationDialog_Hinge::_updateMoto(void)
{
	ObjectHingeJoint::MotoParam motoParam;
	motoParam.enableMoto = (IsDlgButtonChecked(IDC_CHECK_MOTO_ENABLE)==BST_CHECKED);
	motoParam.is_cw = (IsDlgButtonChecked(IDC_CHECK_MOTO_DIRECTION)==BST_CHECKED);

	int rate_pos = (int)SendDlgItemMessage(IDC_SLIDER_MOTO_RATE, TBM_GETPOS, 0, 0);
	motoParam.rate = (rate_pos/100.0f)*(ObjectHingeJoint::MOTO_MAX_RATE-ObjectHingeJoint::MOTO_MIN_RATE) + ObjectHingeJoint::MOTO_MIN_RATE;

	OPERATOR_MANAGER()->applyOperator(new Operator_UpdateHingeMoto(m_idObject, motoParam));
}

}

