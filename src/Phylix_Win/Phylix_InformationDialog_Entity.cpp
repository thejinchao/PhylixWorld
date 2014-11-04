#include "stdafx.h"
#include "Phylix_InformationDialog_Entity.h"
#include "Phylix_Global.h"

namespace Phylix
{

//-------------------------------------------------------------------------------------------------------------------
CInformationDialog_Entity::CInformationDialog_Entity()
{
}

//-------------------------------------------------------------------------------------------------------------------
CInformationDialog_Entity::~CInformationDialog_Entity()
{
}

//-------------------------------------------------------------------------------------------------------------------
LRESULT CInformationDialog_Entity::OnInitDialog(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/)
{
	return TRUE;
}

//-------------------------------------------------------------------------------------------------------------------
LRESULT CInformationDialog_Entity::OnCloseCmd(WORD /*wNotifyCode*/, WORD wID, HWND /*hWndCtl*/, BOOL& /*bHandled*/)
{
	return 0;
}

//-------------------------------------------------------------------------------------------------------------------
LRESULT CInformationDialog_Entity::OnDialogActive(UINT /*uMsg*/, WPARAM wParam, LPARAM lParam, BOOL& /*bHandled*/)
{
	if(wParam==WA_INACTIVE)
	{
		ShowWindow(SW_HIDE);
	}
	return 0;
}

//-------------------------------------------------------------------------------------------------------------------
void CInformationDialog_Entity::_updateUI_Area(Real area)
{
	TCHAR temp[32]={0};
	_sntprintf(temp, 32, g_local.getString(SI_AREA_VALUE), area);
	SetDlgItemText(IDC_STATIC_AREA, temp);
}

//-------------------------------------------------------------------------------------------------------------------
void CInformationDialog_Entity::_updateUI_Mass(Real mass)
{
	TCHAR temp[32]={0};
	_sntprintf(temp, 32, g_local.getString(SI_MASS_VALUE), mass);
	SetDlgItemText(IDC_STATIC_MASS, temp);
}

//-------------------------------------------------------------------------------------------------------------------
void CInformationDialog_Entity::_updateUI_Moment(Real moment)
{
	TCHAR temp[32]={0};
	_sntprintf(temp, 32, g_local.getString(SI_MOMENT_VALUE), moment);
	SetDlgItemText(IDC_STATIC_MOMENT, temp);
}

//-------------------------------------------------------------------------------------------------------------------
void CInformationDialog_Entity::showObjectInformation(unsigned int idObject)
{
	m_idObject = idObject;

	Object* obj = OBJECT_MANAGER()->findObject(m_idObject);
	assert(obj && Object::isEntityType(obj->getType()));

	ObjectEntity* entity = (ObjectEntity*)obj;
	
	//set ui
	_updateUI_Area(entity->getShape()->getArea());
	_updateUI_Mass(entity->getShape()->getArea()*entity->getMaterial().density);
	_updateUI_Moment(0.0);

	//show window
	POINT pt = {400, 27};
	::ClientToScreen(GetParent(), &pt);
	SetWindowPos(HWND_TOP, pt.x, pt.y, 0, 0, SWP_NOSIZE|SWP_SHOWWINDOW);
}

}
