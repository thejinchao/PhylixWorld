#include "StdAfx.h"
#include "Phylix_Localization.h"

namespace Phylix
{

//-------------------------------------------------------------------------------------------------------------------
const static TCHAR* g_szStringTable[] = 
{
	_T(""),
	//1: SI_DENSITY_VALUE
	_T("�ܶ�: %.3f kg/m\xB2\x00"),	
	//2: SI_FRICITION_VALUE
	_T("Ħ��ϵ��: %.3f"),
	//3: SI_RESITUTION_VALUE
	_T("����ϵ��: %.3f"),
	//4: SI_ENGINE_ENABLE
	_T("�����湦��"),
	//5: SI_ENGINE_DIRECTION
	_T("����˳ʱ�뷽��"),
	//6: SI_ENGINE_RATE
	_T("�����ٶ�: %.3f"),
	//7: SI_AREA_VALUE
	_T("���: %.3f m\xB2\x00"),
	//8: SI_MASS_VALUE
	_T("����: %.3f kg"),
	//9: SI_MOMENT_VALUE
	_T("ת������: %.3f"),
	//10: SI_SPRING_TARGET_LENGTH
	_T("Ŀ�곤��: %.3f m"),
	//11: SI_SPRING_STIFFNESS
	_T("����ϵ��: %.3f N/m"),
	//12: SI_SPRING_DAMP
	_T("����ϵ��: %.3f"),
	//13: SI_COLOR_VALUE
	_T("��ɫ: %d,%d,%d"),
	//14: SI_MATERIAL_TEMPLATE_NAME_0
	_T("ȱʡ"),
	//15: SI_MATERIAL_TEMPLATE_NAME_1
	_T("����"),
	//16: SI_MATERIAL_TEMPLATE_NAME_2
	_T("ʯͷ"),
	//17: SI_MATERIAL_TEMPLATE_NAME_3
	_T("����"),
	//18: SI_MATERIAL_TEMPLATE_NAME_4
	_T("��"),
	//19: SI_MATERIAL_TEMPLATE_NAME_5
	_T("��"),
	//20: SI_MATERIAL_TEMPLATE_NAME_6
	_T("����"),
	//21: SI_MATERIAL_TEMPLATE_NAME_7
	_T("�ƽ�"),
	//22: SI_MATERIAL_TEMPLATE_NAME_8
	_T("ľ��"),
};

//-------------------------------------------------------------------------------------------------------------------
Localization::Localization()
{
}

//-------------------------------------------------------------------------------------------------------------------
Localization::~Localization()
{
}

//-------------------------------------------------------------------------------------------------------------------
void Localization::init(void)
{
}

//-------------------------------------------------------------------------------------------------------------------
const TCHAR* Localization::getString(STRING_ID id)
{
	int counts = sizeof(g_szStringTable)/sizeof(g_szStringTable[0]);
	assert(id>0 && id<counts);

	return g_szStringTable[id];
}

}
