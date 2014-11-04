#include "libPhylix_stdafx.h"
#include "libPhylix_MaterialManager.h"

namespace Phylix
{

//-------------------------------------------------------------------------------------------------------------------
MaterialTemplate _MaterialTemplateList[] = 
{
	//缺省材质列表
	// id		density		fricition,	restitution,	texture		
	{ 0,	(Real)2.0,		(Real)0.5,	(Real)0.5,	fColor(0.f, 0.f, 0.f, 0.f), "", 0},

	{ 1,	(Real)2.5,		(Real)0.2,	(Real)0.5,	fColor(0.75f,0.9f,1.f,0.375f), "Material/glass.png",		0 },
	{ 2,	(Real)2.4,		(Real)0.9,	(Real)0.2,	fColor(0.9f, 0.8f, 0.8f,1.f), "Material/stone.png",		0 },
	{ 3,	(Real)7.8,		(Real)0.74, (Real)0.75, fColor(0.7f, 0.75f,0.8f,1.f), "Material/steel.png",		0 },
	{ 4,	(Real)1.5,		(Real)1.5,	(Real)0.85, fColor(0.325f,0.325f,0.325f,1.f), "Material/rubber.png",	0 },
	{ 5,	(Real)0.9,		(Real)0.05,	(Real)0.05, fColor(0.75f,0.95f,1.f, 0.875f), "Material/ice.png",		0 },
	{ 6,	(Real)0.005,	(Real)1.0,	(Real)0.25, fColor(1.f, 0.65f, 0.75f, 0.65f), "",	0 },
	{ 7,	(Real)19.3,		(Real)0.49,	(Real)0.1,	fColor(1.f, 0.83f,0.05f,1.f), "Material/gold.png",		0 },
	{ 8,	(Real)0.4,		(Real)0.4,	(Real)0.6,	fColor(1.f, 0.65f, 0.25f,1.f), "Material/wood.png",	0	 },
};

//-------------------------------------------------------------------------------------------------------------------
MaterialManager::MaterialManager()
{
}

//-------------------------------------------------------------------------------------------------------------------
MaterialManager::~MaterialManager()
{
}


//-------------------------------------------------------------------------------------------------------------------
Material MaterialManager::createStandardMaterial(const fColor& color)
{
	Material material;

	//产生一个随机颜色
	material.color = color;

	return material;
}

//-------------------------------------------------------------------------------------------------------------------
fColor MaterialManager::_createRandomColor(void)
{
	unsigned int val = (((unsigned int)rand())<<16) |(unsigned int)rand();
	val = (val+0x7ed55d16) + (val<<12);
	val = (val^0xc761c23c) ^ (val>>19);
	val = (val+0x165667b1) + (val<<5);
	val = (val+0xd3a2646c) ^ (val<<9);
	val = (val+0xfd7046c5) + (val<<3);
	val = (val^0xb55a4f09) ^ (val>>16);

	unsigned char r = (val>>0) & 0xFF;
	unsigned char g = (val>>8) & 0xFF;
	unsigned char b = (val>>16) & 0xFF;
	
	unsigned char max = r>g ? (r>b ? r : b) : (g>b ? g : b);
	const int mult = 127;
	const int add = 63;
	r = (r*mult)/max + add;
	g = (g*mult)/max + add;
	b = (b*mult)/max + add;

	return fColor(r/255.f, g/255.f, b/255.f, 1.f);
}

//-------------------------------------------------------------------------------------------------------------------
MaterialTemplate* MaterialManager::getMaterialTemplateList(void)
{
	return _MaterialTemplateList;
}

//-------------------------------------------------------------------------------------------------------------------
int MaterialManager::getMaterialTemplateCount(void)
{
	return sizeof(_MaterialTemplateList)/sizeof(MaterialTemplate);
}

}
