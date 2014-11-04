#ifndef __PHYSICS_WORLD_LIB_OBJECT_CREATE_DEFINE_H__
#define __PHYSICS_WORLD_LIB_OBJECT_CREATE_DEFINE_H__

#include "libPhylix_Color.h"
#include "libPhylix_Object.h"
#include "libPhylix_Material.h"

#include "libPhylix_MassBody.h"

namespace Phylix
{

/** 用于存储object创建信息
*/
class ObjectParams
{
public:
	/** 参数类型 */
	enum PARAM_TYPE
	{
		PT_POSITION_1=0,	//!< 位置1 fVector2
		PT_POSITION_2,		//!< 位置2 fVector2
		PT_ANGLE,			//!< 角度 Angle
		PT_SIZE,			//!< 大小 fVector2
		PT_RADIUS,			//!< 半径 Real
		PT_WIDTH,			//!< 宽度 Real
		PT_POINTS,			//!< 顶点数据 PointVector
		PT_MATERIAL,		//!< 材质
		PT_OBJECT_ID,		//!< 物体ID
		PT_TARGET_ID1,		//!< 目标物体1
		PT_TARGET_ID2,		//!< 目标物体2
		PT_MASSBODY,		//!< 质量体数据(id, 速度，角速度...)
		PT_CHILD_MASS,		//!< 子物体质量体数据
		PT_Z_VALUE,			//!< Z值
		PT_BOOLEAN_1,		//!< 布尔参数1
		PT_BOOLEAN_2,		//!< 布尔参数2

		PT_COUNTS,			//!< 总类型参数
	};

	typedef MassBody::cpDataCache MassCache;
	typedef std::vector< MassCache > ChildMassCache;

private:
	Object::Type m_objectType;
	unsigned int m_flags;
	/** 设置参数 */
	void _setFlag(PARAM_TYPE _type) { unsigned int mask = 1<<((unsigned int)_type); m_flags = (m_flags&(~mask)) | mask; }

public:
	/** 获得所创建的物体类型*/
	Object::Type getObjectType(void) const { return m_objectType; }
	/** 重置参数*/
	void reset(Object::Type type) { m_objectType=type; m_flags=0; }
	/** 某参数是否存在 */
	bool getFlag(PARAM_TYPE _type) const { unsigned int mask = 1<<((unsigned int)_type); return (m_flags&mask)!=0; }

	/// 参数定义宏
#ifdef _MSC_VER 
	#define DECLARE_PARAM(_type, _data_type, _value, _getFunc, _setFunc) \
	public: \
		const _data_type& _getFunc(void) const { assert(getFlag(_type)); return _value; } \
		void _setFunc(const _data_type& _d) { _value=_d; _setFlag(_type); } \
	private: \
			 _data_type _value; 
#else
    #define DECLARE_PARAM(_type, _data_type, _value, _getFunc, _setFunc) \
    public: \
        const _data_type& _getFunc(void) const { return _value; } \
        void _setFunc(const _data_type& _d) { _value=_d; _setFlag(_type); } \
        private: \
        _data_type _value; 
#endif
    
	DECLARE_PARAM(PT_POSITION_1,	fVector2,		m_position1,	getPosition1,	setPosition1);
	DECLARE_PARAM(PT_POSITION_2,	fVector2,		m_position2,	getPosition2,	setPosition2);
	DECLARE_PARAM(PT_ANGLE,			Angle,			m_angle,		getAngle,		setAngle);
	DECLARE_PARAM(PT_SIZE,			fVector2,		m_size,			getSize,		setSize);
	DECLARE_PARAM(PT_RADIUS,		Real,			m_radius,		getRadius,		setRadius);
	DECLARE_PARAM(PT_WIDTH,			Real,			m_width,		getWidth,		setWidth);
	DECLARE_PARAM(PT_POINTS,		PointVector,	m_points,		getPoints,		setPoints);
	DECLARE_PARAM(PT_MATERIAL,		Material,		m_material,		getMaterial,	setMaterial);
	DECLARE_PARAM(PT_OBJECT_ID,		unsigned int,	m_objectID,		getObjectID,	setObjectID);
	DECLARE_PARAM(PT_TARGET_ID1,	unsigned int,	m_targetID1,	getTargetID1,	setTargetID1);
	DECLARE_PARAM(PT_TARGET_ID2,	unsigned int,	m_targetID2,	getTargetID2,	setTargetID2);
	DECLARE_PARAM(PT_MASSBODY,		MassCache,		m_massBody,		getMassBody,	setMassBody);
	DECLARE_PARAM(PT_CHILD_MASS,	ChildMassCache,	m_childMass,	getChildMass,	setChildMass);
	DECLARE_PARAM(PT_Z_VALUE,		Real,			m_zValue,		getZValue,		setZValue);
	DECLARE_PARAM(PT_BOOLEAN_1,		bool,			m_boolean1,		getBoolean1,	setBoolean1);
	DECLARE_PARAM(PT_BOOLEAN_2,		bool,			m_boolean2,		getBoolean2,	setBoolean2);


public:
	ObjectParams(Object::Type type=Object::OT_NULL) : m_objectType(type), m_flags(0) { }
	~ObjectParams() { }
};

}


#endif
