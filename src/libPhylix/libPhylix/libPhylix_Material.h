#ifndef __PHYSICS_WORLD_LIB_MATERIAL_H__
#define __PHYSICS_WORLD_LIB_MATERIAL_H__

#include "libPhylix_Color.h"

namespace Phylix
{

//pre-define
struct MaterialTemplate;

/** 材质，定义物体的密度、摩擦、阻尼等参数
*/
class Material
{
public:
	static const Real MIN_DENSITY;		//最小密度	0.1
	static const Real MAX_DENSITY;		//最大密度	100.0
	static const Real MAX_FRICITION;	//最大摩擦系数	2.0
	static const Real MAX_RESTITUTION;	//最大弹力系数	1.0

public:
	Real density;		//!< 密度，单位 kg/m^2，缺省 2
	Real fricition;		//!< 摩擦系数，无单位，缺省 0.5, 范围[0,2]
	Real restitution;	//!< 弹力系数，无单位，缺省 0.5, 范围 [0,1]
	fColor color;		//!< 渲染时使用的颜色

	MaterialTemplate * pTemplate;	//!< 材质模板
public:
	Material();
	Material(const fColor& color);
	~Material();
};


}

#endif
