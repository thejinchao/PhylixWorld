#ifndef __PHYSICS_WORLD_LIB_OBJECT_JOINT_HINGE_H__
#define __PHYSICS_WORLD_LIB_OBJECT_JOINT_HINGE_H__

#include "libPhylix_Object_Joint.h"

struct cpConstraint;
namespace Phylix
{
/** 旋转铰链
*/
class ObjectHingeJoint : public ObjectJoint
{
public:
	static const Real MOTO_MIN_RATE;		//引擎最小速率 0
	static const Real MOTO_MAX_RATE;		//引擎最大速率 10.0

	/** 动力引擎相关参数 */
	struct MotoParam
	{
		bool enableMoto;		//是否打开引擎
		bool is_cw;				//旋转方向是否是顺时针，缺省时逆时针
		Real rate;				//旋转速度(相对角速度,单位???)  [0~10]之间

		bool operator ==(const MotoParam& other) const
		{
			return (enableMoto==other.enableMoto && is_cw==other.is_cw && rate==other.rate);
		}
	};

public:
	/** 获得类型 */
	virtual Type getType(void) const { return OT_HINGE_JOINT; }
	/** 渲染 */
	virtual void draw(IRenderInterface* render);
	/** 某点是否在该点示意图范围内，用于计算鼠标选择*/
	virtual bool isInLegendArea(const fVector2& pt) const;
	/** 重新检查所连接的质量体是否有变化 */
	virtual void reCheckMassBody(void);
	/** 重新构造创建参数 */
	virtual void buildCreateParam(ObjectParams& param);
	/** 通知被选中 */
	virtual void notifySelected(void);
	/** 更新引擎参数 */
	void updateMotoParam(const MotoParam& param);
	/** 获得引擎参数 */
	const MotoParam& getMotoParam(void) const { return m_motoParam; }
	/** 获得绝对坐标位置 */
	fVector2 getWorldPos(void) const;
	/** 通知即将被删除 */
	virtual void notifyDelete(void);

private:
	void _enableMoto(const MotoParam& param);

protected:
	Real  m_radius;
	cpConstraint* m_cpConstraint;	//!< chipmunk 数据
	int m_nFirstMassBody;			//!< 第一个massbody的id
	int m_nSecondMassBody;			//!< 第二个massbody的id

	MotoParam m_motoParam;			//!< 引擎参数
	cpConstraint* m_cpMoto;			//!< 引擎连接

	unsigned int m_chainID;			//!< 如果属于某Chain，记录其id
	bool m_headHinge;				//!< 属于某Chain，并且属于第一个hinge
	bool m_tailHinge;				//!< 属于某Chain，并且属于最后一个hinge

protected:
	ObjectHingeJoint(unsigned int id);
	virtual ~ObjectHingeJoint();
	friend class ObjectManager;
};

}

#endif
