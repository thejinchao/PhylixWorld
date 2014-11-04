#ifndef __PHYSICS_WORLD_LIB_OBJECT_ENTITY_H__
#define __PHYSICS_WORLD_LIB_OBJECT_ENTITY_H__

#include "libPhylix_Object.h"

namespace Phylix
{
//pre-define
class Shape;
class MassBody;
class ObjectJoint;

/** 放置在场景中，具有均匀材质和一定形状的单独物体
*/
class ObjectEntity : public Object
{
public:
	typedef std::set< ObjectJoint* > JointSet;

public:
	/** 获得类型 */
	virtual Type getType(void) const { return m_type; }
	/** 渲染 */
	virtual void draw(IRenderInterface* render);
	/** 获取质量体 */
	virtual MassBody* getMassBody(void) { return m_massBody; }
	/** 获得形状 */
	Shape* getShape(void) { return m_shape; }
	/** 获得变换矩阵 */
	fMatrix3 getTrnasFormMatrix(void) const;
	/** 获得所连接的关节 */
	JointSet& getJointSet(void) { return m_joints; }
	/** 重新构造创建参数 */
	virtual void buildCreateParam(ObjectParams& param);
	/** 更新材质 */
	virtual void updateMaterial(const Material& material);

protected:
	Type		m_type;			//!< 类型
	Shape*		m_shape;		//!< 形状
	MassBody*	m_massBody;		//!< 质量体,如果是0表示静态物体
	JointSet	m_joints;		//!< 所连接的关节

protected:
	ObjectEntity(unsigned int id, Type type);
	virtual ~ObjectEntity();
	friend class ObjectManager;
};


/** 圆形物体
*/
class CircleObject : public ObjectEntity
{
public:
	/** 渲染 */
	virtual void draw(IRenderInterface* render);
	/** 通知被选中 */
	virtual void notifySelected(void);

protected:
	unsigned int m_chainID;		//!< 如果属于某Chain，记录其id
	
private:
	CircleObject(unsigned int id, Type type);
	virtual ~CircleObject();
	friend class ObjectManager;
};
}

#endif
