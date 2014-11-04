#ifndef __PHYSICS_WORLD_LIB_SHAPE_H__
#define __PHYSICS_WORLD_LIB_SHAPE_H__

struct cpShape;
struct cpPolyShape;

namespace Phylix
{
//pre-define
class IRenderInterface;
class Material;
class ShapeManager;
class ObjectEntity;
class MassBody;

/** 具有特定形状的物体, 对于某些形状，比如凹多边形，可能由chipmunk中多个cpShape来组成
*/
class Shape
{
public:
	enum Type
	{
		ST_UNKNOWN = 0,

		ST_CIRCLE = 1,			//!< 圆形
		ST_EDGE = 2,			//!< 边界
		ST_COMPLEX_POLY = 3,	//!< 复杂形状的多边形，例如凹多边形
	};

public:
	/** 获得类型*/
	virtual Type getType(void) const = 0;
	/** 获得chipmunk数据*/
	cpShape* getImpl(int index) const { return m_cmShape[index]; }
	/** 获得chipmunk指针数量*/
	int getImplCounts(void) const { return m_cmShapeCounts; }
	/** 获得所属的obj指针 */
	ObjectEntity* getOwner(void) { return m_objectOwner; }
	/** 重新设置所属的质量体 */
	virtual void resetMassOwner(MassBody* body) = 0;
	/** 获取该Shape得变换矩阵*/
	const fMatrix3& getTrnasFormMatrix(void) const { return m_transForm; }
	/** 该Shape的质心的相对坐标(相对于所属质量体的质心)*/
	fVector2 getPosition(void) const { return getTrnasFormMatrix().getOffsetPart(); }
	/** 获得面积 */
	Real getArea(void) const;
	/** 获得转动惯量(相对于所属质量体质心)(缺少质量部分,即I/m)*/
	Real getMoment(void) const;
	/** 渲染 */
	virtual void draw(IRenderInterface* render, const Material& material, bool selected) const = 0;

protected:
	virtual void _updateAreaImpl(void) const = 0;
	virtual void _updateMomentImpl(void) const = 0;
	
protected:
	ShapeManager* m_shapManager;	//!< 形状管理器
	fMatrix3 m_transForm;			//!< 该shape的变换矩阵(相对于所属的质量体)
	cpShape** m_cmShape;			//!< chipmunk的形状指针
	int m_cmShapeCounts;			//!< chipmunk形状数量
	ObjectEntity* m_objectOwner;	//!< 所属的obj

	//内部变量，自行计算所得

	mutable Real m_area;			//!< 面积
	mutable bool m_areaDirt;		//!< 需要重新计算面积

	mutable Real m_moment;			//!< 转动惯量
	mutable bool m_momentDirt;		//!< 需要重新计算转动惯量

protected:
	Shape(ShapeManager* shapManager, ObjectEntity* ownerObj);
	virtual ~Shape();
	friend class ShapeManager;
};

/** 圆形
*/
class Shape_Circle : public Shape
{
public:
	/** 获得类型*/
	virtual Type getType(void) const { return ST_CIRCLE; }
	/** 渲染 */
	virtual void draw(IRenderInterface* render, const Material& material, bool selected) const;
	/** 半径 */
	Real getRadius(void) const { return m_radius; }
	/** 获得指示方向 */
	const fVector2& getAxis(void) const { return m_axis; }
	/** 重新设置所属的质量体 */
	virtual void resetMassOwner(MassBody* body);

protected:
	virtual void _updateAreaImpl(void) const;
	virtual void _updateMomentImpl(void) const;

private:
	Real m_radius;		//!< 半径
	fVector2 m_axis;	//!< 指示方向
	bool m_axisValid;	//!< 指示方向是否有效

protected:
	Shape_Circle(ShapeManager* shapManager, ObjectEntity* ownerObj) : Shape(shapManager, ownerObj), m_axisValid(true) {}
	virtual ~Shape_Circle(){}
	friend class ShapeManager;
};

/** 边界
*/
class Shape_Edge : public Shape
{
public:
	/** 获得类型*/
	virtual Type getType(void) const { return ST_EDGE; }
	/** 渲染 */
	virtual void draw(IRenderInterface* render, const Material& material, bool selected) const;
	/** 设置基准位置 */
	virtual void setBasePosition(const fVector2& pos) { m_basePoint=pos; m_borderDirt=true; }
	/** 获取基点位置*/
	const fVector2& getBasePoint(void) const { return m_basePoint; }
	/** 设置法线方向 */
	void setNormalAngle(const Angle& ang) { m_normalAngle=ang; m_borderDirt=true; }
	/** 获得法线方向 */
	const Angle& getNormalAngle(void) const { return m_normalAngle; }
	/** 重新设置所属的质量体 */
	virtual void resetMassOwner(MassBody* body);

protected:
	virtual void _updateAreaImpl(void) const;
	virtual void _updateMomentImpl(void) const;

	//更新边界和场景固定边界的交点数据
	void _updateBorder(void) const;

private:
	fVector2 m_basePoint;	//!< 基点
	Angle m_normalAngle;	//!< 法线方向

	mutable Polygon m_border;
	mutable bool m_borderDirt;

protected:
	Shape_Edge(ShapeManager* shapManager, ObjectEntity* ownerObj) : Shape(shapManager, ownerObj), m_basePoint(fVector2::ZERO), m_normalAngle(MATH_PI_HALF) {}
	virtual ~Shape_Edge(){}
	friend class ShapeManager;
};


/** 多边形(可以是凸多边形也可以是凹多边形)
*/
class Shape_Poly : public Shape
{
public:
	/** 获得类型*/
	virtual Type getType(void) const { return ST_COMPLEX_POLY; }
	/** 渲染 */
	virtual void draw(IRenderInterface* render, const Material& material, bool selected) const;
	/** 重新设置所属的质量体 */
	virtual void resetMassOwner(MassBody* body);
	/** 获得顶点数据 */
	const Polygon& getPolygonData(void) const { return m_poly; }

protected:
	virtual void _updateAreaImpl(void) const;
	virtual void _updateMomentImpl(void) const;

private:
	Polygon m_poly;	 //!< 凹多边形(相对坐标，基于凹多边形的质心)

	struct SubConvex //!< 分割后的凸多边形
	{
		Polygon convex;			//!< 凸多边形顶点数据(相对坐标，基于凹多边形的质心)
		fVector2 center;		//!< 凸多边形质心位置(相对坐标，基于凹多边形的质心)
		Real area;				//!< 面积
		cpPolyShape* cmShape;	//!< 对应的chipmunk数据
	};
	std::list< SubConvex > m_convexList;	//!<凸多边形列表

protected:
	Shape_Poly(ShapeManager* shapManager, ObjectEntity* ownerObj) : Shape(shapManager, ownerObj) {}
	virtual ~Shape_Poly(){}
	friend class ShapeManager;
};

}

#endif
