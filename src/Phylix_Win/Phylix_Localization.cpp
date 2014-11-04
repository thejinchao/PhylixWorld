#include "StdAfx.h"
#include "Phylix_Localization.h"

namespace Phylix
{

//-------------------------------------------------------------------------------------------------------------------
const static TCHAR* g_szStringTable[] = 
{
	_T(""),
	//1: SI_DENSITY_VALUE
	_T("密度: %.3f kg/m\xB2\x00"),	
	//2: SI_FRICITION_VALUE
	_T("摩擦系数: %.3f"),
	//3: SI_RESITUTION_VALUE
	_T("弹力系数: %.3f"),
	//4: SI_ENGINE_ENABLE
	_T("打开引擎功能"),
	//5: SI_ENGINE_DIRECTION
	_T("引擎顺时针方向"),
	//6: SI_ENGINE_RATE
	_T("引擎速度: %.3f"),
	//7: SI_AREA_VALUE
	_T("面积: %.3f m\xB2\x00"),
	//8: SI_MASS_VALUE
	_T("质量: %.3f kg"),
	//9: SI_MOMENT_VALUE
	_T("转动惯量: %.3f"),
	//10: SI_SPRING_TARGET_LENGTH
	_T("目标长度: %.3f m"),
	//11: SI_SPRING_STIFFNESS
	_T("刚性系数: %.3f N/m"),
	//12: SI_SPRING_DAMP
	_T("阻尼系数: %.3f"),
	//13: SI_COLOR_VALUE
	_T("颜色: %d,%d,%d"),
	//14: SI_MATERIAL_TEMPLATE_NAME_0
	_T("缺省"),
	//15: SI_MATERIAL_TEMPLATE_NAME_1
	_T("玻璃"),
	//16: SI_MATERIAL_TEMPLATE_NAME_2
	_T("石头"),
	//17: SI_MATERIAL_TEMPLATE_NAME_3
	_T("钢铁"),
	//18: SI_MATERIAL_TEMPLATE_NAME_4
	_T("橡胶"),
	//19: SI_MATERIAL_TEMPLATE_NAME_5
	_T("冰"),
	//20: SI_MATERIAL_TEMPLATE_NAME_6
	_T("空气"),
	//21: SI_MATERIAL_TEMPLATE_NAME_7
	_T("黄金"),
	//22: SI_MATERIAL_TEMPLATE_NAME_8
	_T("木材"),
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
