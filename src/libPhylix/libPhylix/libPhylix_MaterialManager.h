#ifndef __PHYSICS_WORLD_LIB_MATERIAL_MANAGER_H__
#define __PHYSICS_WORLD_LIB_MATERIAL_MANAGER_H__

#include "libPhylix_Handler.h"
#include "libPhylix_Material.h"

namespace Phylix
{

/** 材质模板定义
*/
struct MaterialTemplate
{
	int id;

	Real density;		//!< 密度，单位 kg/m^2，缺省 2
	Real fricition;		//!< 摩擦系数，无单位，缺省 0.5, 范围[0,2]
	Real restitution;	//!< 弹力系数，无单位，缺省 0.5, 范围 [0,1]
	fColor color;		//!< 渲染时使用的颜色(0表示随机颜色)

	enum { TEXTURE_NAME_LENGTH = 32 };
	char texture[TEXTURE_NAME_LENGTH];

	void* proxy;		//!< 附加数据(由上层存储运行时纹理数据）
};

/** 用于创建和管理材质
*/
class MaterialManager
{
public:
	/** 创建一个标准材质，并赋予一个颜色
	*/
	Material createStandardMaterial(const fColor& color);

	/** 获得材质模板列表 */
	MaterialTemplate* getMaterialTemplateList(void);

	/** 获得材质模板列表数量 */
	int getMaterialTemplateCount(void);

public:
	/** 工具函数，产生一个随机颜色*/
	static fColor _createRandomColor(void);

public:
	MaterialManager();
	~MaterialManager();
};

}



#endif
