#ifndef __PHYSICS_WORLD_LIB_OBJECT_JOINT_SPRING_H__
#define __PHYSICS_WORLD_LIB_OBJECT_JOINT_SPRING_H__

#include "libPhylix_Object_Joint.h"

struct cpConstraint;
namespace Phylix
{
/** 弹簧
*/
class ObjectSpringJoint : public ObjectJoint
{
public:
	static const Real DAMP_DEFAULT;  //弹簧缺省的阻尼系数 0.5
	static const Real DAMP_MIN;		//最小阻尼系数 0.0
	static const Real DAMP_MAX;		//最大阻尼系数 2.0

	/** 弹簧参数 */
	struct SpringParam
	{
		Real targetLength;		//!< 目标长度
		Real stiffness;			//!< 刚性系数 N/m
		Real damp;				//!< 阻尼系数 [0~2.0]

		bool operator ==(const SpringParam& other) const
		{
			return (targetLength==other.targetLength && stiffness==other.stiffness && damp==other.damp);
		}
	};

public:
	/** 获得类型 */
	virtual Type getType(void) const { return OT_SPRING_JOINT; }
	/** 渲染 */
	virtual void draw(IRenderInterface* render);
	/** 某点是否在该点示意图范围内，用于计算鼠标选择*/
	virtual bool isInLegendArea(const fVector2& pt) const;
	/** 重新检查所连接的质量体是否有变化 */
	virtual void reCheckMassBody(void);
	/** 重新构造创建参数 */
	virtual void buildCreateParam(ObjectParams& param);

	/** 更新弹簧参数 */
	void updateSpringParam(const SpringParam& param);
	/** 获得弹簧参数 */
	const SpringParam& getSpringParam(void) const { return m_springParam; }

protected:
	Real m_width;					//!< 宽度，用于显示
	cpConstraint* m_cpConstraint;	//!< chipmunk 数据
	int m_nFirstMassBody;			//!< 第一个massbody的id
	int m_nSecondMassBody;			//!< 第二个massbody的id
	SpringParam m_springParam;		//!< 弹簧系数

protected:
	ObjectSpringJoint(unsigned int id);
	virtual ~ObjectSpringJoint();
	friend class ObjectManager;
};

}

#endif
