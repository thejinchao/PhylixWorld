#include "stdafx.h"
#include "Phylix_InformationDialog_Spring.h"
#include "Phylix_Global.h"

namespace Phylix
{

//-------------------------------------------------------------------------------------------------------------------
CInformationDialog_Spring::CInformationDialog_Spring()
{
}

//-------------------------------------------------------------------------------------------------------------------
CInformationDialog_Spring::~CInformationDialog_Spring()
{
}

//-------------------------------------------------------------------------------------------------------------------
LRESULT CInformationDialog_Spring::OnInitDialog(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/)
{
	SendDlgItemMessage(IDC_SLIDER_STIFFNESS, TBM_SETRANGE, (WPARAM) FALSE, (LPARAM)MAKELONG(0, 100));
	SendDlgItemMessage(IDC_SLIDER_DAMP, TBM_SETRANGE, (WPARAM) FALSE, (LPARAM)MAKELONG(0, 100));
	SendDlgItemMessage(IDC_SLIDER_TARGET_LENGTH, TBM_SETRANGE, (WPARAM) FALSE, (LPARAM)MAKELONG(0, 100));
	return TRUE;
}

//-------------------------------------------------------------------------------------------------------------------
void CInformationDialog_Spring::showObjectInformation(unsigned int idObject)
{
	m_idObject = idObject;

	Object* obj = OBJECT_MANAGER()->findObject(m_idObject);
	assert(obj && obj->getType()==Object::OT_SPRING_JOINT);

	const ObjectSpringJoint::SpringParam param = ((ObjectSpringJoint*)obj)->getSpringParam();

	m_minStiffness = param.stiffness/10.0f;
	m_maxStiffness = param.stiffness*10.0f;
	m_minTargetLen = param.damp/10.0f;
	m_maxTargetLen = param.damp*10.0f;

	//set ui
	_updateUI_Stiffness(param.stiffness, true);
	_updateUI_TargetLength(param.targetLength, true);
	_updateUI_Damp(param.damp, true);

	//show window
	POINT pt = {400, 27};
	::ClientToScreen(GetParent(), &pt);
	SetWindowPos(HWND_TOP, pt.x, pt.y, 0, 0, SWP_NOSIZE|SWP_SHOWWINDOW);
}

//-------------------------------------------------------------------------------------------------------------------
LRESULT CInformationDialog_Spring::OnCloseCmd(WORD /*wNotifyCode*/, WORD wID, HWND /*hWndCtl*/, BOOL& /*bHandled*/)
{
	return 0;
}

//-------------------------------------------------------------------------------------------------------------------
LRESULT CInformationDialog_Spring::OnDialogActive(UINT /*uMsg*/, WPARAM wParam, LPARAM lParam, BOOL& /*bHandled*/)
{
	if(wParam==WA_INACTIVE)
	{
		ShowWindow(SW_HIDE);
	}
	return 0;
}

//-------------------------------------------------------------------------------------------------------------------
void CInformationDialog_Spring::_updateUI_TargetLength(Real target_length, bool update_slider)
{
	assert(target_length>=m_minTargetLen && target_length<=m_maxTargetLen);

	if(update_slider)
	{
		int slider_pos = (int)(((target_length-m_minTargetLen)/(m_maxTargetLen-m_minTargetLen))*100.0);
		SendDlgItemMessage(IDC_SLIDER_TARGET_LENGTH, TBM_SETPOS, (WPARAM)TRUE, slider_pos);
	}

	TCHAR temp[32]={0};
	_sntprintf(temp, 32, g_local.getString(SI_SPRING_TARGET_LENGTH), target_length);
	SetDlgItemText(IDC_STATIC_TARGET_LENGTH, temp);
}

//-------------------------------------------------------------------------------------------------------------------
void CInformationDialog_Spring::_updateUI_Damp(Real damp, bool update_slider)
{
	assert(damp>=ObjectSpringJoint::DAMP_MIN && damp<=ObjectSpringJoint::DAMP_MAX);

	if(update_slider)
	{
		int slider_pos = (int)(((damp-ObjectSpringJoint::DAMP_MIN)/(ObjectSpringJoint::DAMP_MAX-ObjectSpringJoint::DAMP_MIN))*100.0);
		SendDlgItemMessage(IDC_SLIDER_DAMP, TBM_SETPOS, (WPARAM)TRUE, slider_pos);
	}

	TCHAR temp[32]={0};
	_sntprintf(temp, 32, g_local.getString(SI_SPRING_DAMP), damp);
	SetDlgItemText(IDC_STATIC_DAMP, temp);
}

//-------------------------------------------------------------------------------------------------------------------
void CInformationDialog_Spring::_updateUI_Stiffness(Real stiffness, bool update_slider)
{
	assert(stiffness>=m_minStiffness && stiffness<=m_maxStiffness);

	if(update_slider)
	{
		int slider_pos = (int)(((stiffness-m_minStiffness)/(m_maxStiffness-m_minStiffness))*100.0);
		SendDlgItemMessage(IDC_SLIDER_STIFFNESS, TBM_SETPOS, (WPARAM)TRUE, slider_pos);
	}

	TCHAR temp[32]={0};
	_sntprintf(temp, 32, g_local.getString(SI_SPRING_STIFFNESS), stiffness);
	SetDlgItemText(IDC_STATIC_STIFFNESS, temp);
}

//-------------------------------------------------------------------------------------------------------------------
LRESULT CInformationDialog_Spring::OnHScroll(UINT /*uMsg*/, WPARAM wParam, LPARAM lParam, BOOL& /*bHandled*/)
{
	Object* obj = OBJECT_MANAGER()->findObject(m_idObject);
	assert(obj);

	int pos = (int)::SendMessage((HWND)lParam, TBM_GETPOS, 0, 0);

	switch(::GetDlgCtrlID((HWND)lParam))
	{
	case IDC_SLIDER_STIFFNESS:
		{
			Real new_value = (pos/100.0f)*(m_maxStiffness-m_minStiffness) + m_minStiffness;
			_updateUI_Stiffness(new_value, false);

			if(LOWORD(wParam) == SB_ENDSCROLL)
			{
				_updateSpring();
			}
		}
		break;

	case IDC_SLIDER_DAMP:
		{
			Real new_value = (pos/100.0f)*(ObjectSpringJoint::DAMP_MAX-ObjectSpringJoint::DAMP_MIN) + ObjectSpringJoint::DAMP_MIN;
			_updateUI_Damp(new_value, false);

			if(LOWORD(wParam) == SB_ENDSCROLL)
			{
				_updateSpring();
			}
		}
		break;

	case IDC_SLIDER_TARGET_LENGTH:
		{
			Real new_value = (pos/100.0f)*(m_maxTargetLen-m_minTargetLen) + m_minTargetLen;
			_updateUI_TargetLength(new_value, false);

			if(LOWORD(wParam) == SB_ENDSCROLL)
			{
				_updateSpring();
			}
		}
		break;
	}

	return 0;
}

//-------------------------------------------------------------------------------------------------------------------
void CInformationDialog_Spring::_updateSpring(void)
{
	ObjectSpringJoint::SpringParam springParam;

	int pos = (int)SendDlgItemMessage(IDC_SLIDER_DAMP, TBM_GETPOS, 0, 0);
	springParam.damp = (pos/100.0f)*(ObjectSpringJoint::DAMP_MAX-ObjectSpringJoint::DAMP_MIN) + ObjectSpringJoint::DAMP_MIN;

	pos = (int)SendDlgItemMessage(IDC_SLIDER_TARGET_LENGTH, TBM_GETPOS, 0, 0);
	springParam.targetLength = (pos/100.0f)*(m_maxTargetLen-m_minTargetLen) + m_minTargetLen;

	pos = (int)SendDlgItemMessage(IDC_SLIDER_STIFFNESS, TBM_GETPOS, 0, 0);
	springParam.stiffness = (pos/100.0f)*(m_maxStiffness-m_minStiffness) + m_minStiffness;

	OPERATOR_MANAGER()->applyOperator(new Operator_UpdateSpring(m_idObject, springParam));
}

}

