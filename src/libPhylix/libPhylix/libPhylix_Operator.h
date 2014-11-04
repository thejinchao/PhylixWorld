#ifndef __PHYSICS_WORLD_LIB_OPERATOR_H__
#define __PHYSICS_WORLD_LIB_OPERATOR_H__

#include "libPhylix_Color.h"
#include "libPhylix_SceneSnap.h"
#include "libPhylix_ObjectParams.h"
#include "libPhylix_Object_Joint_Hinge.h"
#include "libPhylix_Object_Joint_Spring.h"

namespace Phylix
{
//pre-define
class Object;
class ObjectFixedJoint;
class ObjectHingeJoint;
class ObjectSpringJoint;

/** 操作类型
*/
enum OPERATOR_TYPE
{
	OT_NULL = 0,

	OT_SCENE_SNAP,			//!< 时间流逝形成的场景快照

	OT_CREATE_EDGE,			//!< 创建一个边界
	OT_CREATE_BOX,			//!< 创建一个盒子
	OT_CREATE_CIRCLE,		//!< 创建一个圆形
	OT_CREATE_POLYGON,		//!< 创建一个多边形
	OT_CREATE_FIXED_JOINT,	//!< 创建一个固定焊接点
	OT_CREATE_HINGE_JOINT,	//!< 创建一个旋转铰链
	OT_CREATE_SPRING_JOINT,	//!< 创建一个弹簧连接
	OT_CREATE_CHAIN,		//!< 创建一个链子

	OT_DELETE_OBJ,		//!< 删除一个物体

	OT_UPDATE_OBJECT_MATERIAL,	//!< 更新材质
	OT_UPDATE_HINGE_MOTO,		//!< 编辑旋转铰链的引擎功能
	OP_UPDATE_SPRING,			//!< 编辑弹簧参数
};

/** Operator表示一次可逆的动作
*/
class Operator
{
public:
	/** 返回类型 */
	virtual OPERATOR_TYPE getType(void) const = 0;
	/** 执行操作 */
	virtual bool redo(void) = 0;
	/** 执行逆操作*/
	virtual void undo(void) = 0;

protected:
	SceneSnap m_sceneSnap;	//!< 场景快照

public:
	Operator();
	virtual ~Operator();
};

/** 创建一个物体
*/
class Operator_CreateObject : public Operator
{
public:
	/** 返回类型 */
	virtual OPERATOR_TYPE getType(void) const { return m_opType; }
	/** 执行操作 */
	virtual bool redo(void);
	/** 执行逆操作*/
	virtual void undo(void);

private:
	OPERATOR_TYPE m_opType;			//!< 操作类型
	ObjectParams m_createParam;		//!< 创建参数
	unsigned int m_idObject;		//!< 创建的物体

public:
	Operator_CreateObject(OPERATOR_TYPE type, const ObjectParams& createParam);
	virtual ~Operator_CreateObject();
};

/** 场景快照
*/
class Operator_SceneSnap : public Operator
{
public:
	/** 返回类型 */
	virtual OPERATOR_TYPE getType(void) const { return OT_SCENE_SNAP; }

	/** 执行操作 */
	virtual bool redo(void);
	/** 执行逆操作*/
	virtual void undo(void);

private:

public:
	Operator_SceneSnap();
	virtual ~Operator_SceneSnap();
};

/** 删除物体
*/
class Operator_DeleteObject : public Operator
{
public:
	/** 返回类型 */
	virtual OPERATOR_TYPE getType(void) const { return OT_DELETE_OBJ; }

	/** 执行操作 */
	virtual bool redo(void);
	/** 执行逆操作*/
	virtual void undo(void);

private:
	unsigned int m_idObject;
	std::vector< ObjectParams > m_createParams;	//!< 创建参数

public:
	Operator_DeleteObject(unsigned int idObject);
	virtual ~Operator_DeleteObject();
};

/** 更新物体材质
*/
class Operator_UpdateObjectMaterial : public Operator
{
public:
	/** 返回类型 */
	virtual OPERATOR_TYPE getType(void) const { return OT_UPDATE_OBJECT_MATERIAL; }

	/** 执行操作 */
	virtual bool redo(void);
	/** 执行逆操作*/
	virtual void undo(void);

private:
	unsigned int m_idObject;
	Material m_oldMaterial;	//!< 更新前材质
	Material m_newMaterial;	//!< 更新后材质

public:
	Operator_UpdateObjectMaterial(unsigned int idObject, const Material& newMat);
	virtual ~Operator_UpdateObjectMaterial();
};

/**  编辑旋转铰链的引擎功能
*/
class Operator_UpdateHingeMoto : public Operator
{
public:
	/** 返回类型 */
	virtual OPERATOR_TYPE getType(void) const { return OT_UPDATE_HINGE_MOTO; }

	/** 执行操作 */
	virtual bool redo(void);
	/** 执行逆操作*/
	virtual void undo(void);

private:
	unsigned int m_idObject;
	
	ObjectHingeJoint::MotoParam m_motoParam;
	ObjectHingeJoint::MotoParam m_oldMotoParam;

public:
	Operator_UpdateHingeMoto(unsigned int idObject, const ObjectHingeJoint::MotoParam& motoParam);
	virtual ~Operator_UpdateHingeMoto();
};

/**  编辑弹簧参数
*/
class Operator_UpdateSpring : public Operator
{
public:
	/** 返回类型 */
	virtual OPERATOR_TYPE getType(void) const { return OP_UPDATE_SPRING; }

	/** 执行操作 */
	virtual bool redo(void);
	/** 执行逆操作*/
	virtual void undo(void);

private:
	unsigned int m_idObject;
	
	ObjectSpringJoint::SpringParam m_springParam;
	ObjectSpringJoint::SpringParam m_oldSpringParam;

public:
	Operator_UpdateSpring(unsigned int idObject, const ObjectSpringJoint::SpringParam& springParam);
	virtual ~Operator_UpdateSpring();
};

}

#endif
