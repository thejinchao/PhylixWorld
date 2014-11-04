#include "libPhylix_stdafx.h"
#include "libPhylix_Material.h"
#include "libPhylix_MaterialManager.h"

namespace Phylix
{

//-------------------------------------------------------------------------------------------------------------------
const Real Material::MIN_DENSITY = 0.001f;		//最小密度
const Real Material::MAX_DENSITY = 100.0f;		//最大密度
const Real Material::MAX_FRICITION = 2.0f;		//最大摩擦系数
const Real Material::MAX_RESTITUTION = 1.0f;	//最大弹力系数

//-------------------------------------------------------------------------------------------------------------------
Material::Material()
	: color(0.0, 0.0, 0.0, 1.0)
	, pTemplate(0)
{
	MaterialTemplate* defaultTemplate = MATERIAL_MANAGER()->getMaterialTemplateList();

	density = defaultTemplate->density;
	fricition = defaultTemplate->fricition;
	restitution = defaultTemplate->restitution;
}

//-------------------------------------------------------------------------------------------------------------------
Material::Material(const fColor& color)
	: color(color)
	, pTemplate(0)
{
	MaterialTemplate* defaultTemplate = MATERIAL_MANAGER()->getMaterialTemplateList();

	density = defaultTemplate->density;
	fricition = defaultTemplate->fricition;
	restitution = defaultTemplate->restitution;
}

//-------------------------------------------------------------------------------------------------------------------
Material::~Material()
{
}


}
