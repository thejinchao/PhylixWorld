#ifndef __PHYSICS_WORLD_LIB_MATH_H__
#define __PHYSICS_WORLD_LIB_MATH_H__

#include <limits>
#include "libPhylix_Vector.h"

namespace Phylix
{
//pre-define
struct Angle;

//pi!
#define MATH_PI				(Real)(3.14159265)
#define MATH_PI_DOUBLE		(Real)(3.14159265*2.0)
#define MATH_PI_HALF		(Real)(3.14159265*0.5)
#define MATH_PI_QUARTER		(Real)(3.14159265*0.25)

/** 浮点数比较 
@return 
	-1 : a<b
	 0 : a==b
	 1 : a>b
*/
int floatCompare(Real a, Real b, Real tolerance = std::numeric_limits<Real>::epsilon());

//近似相等
inline bool vectorSimilar(const fVector2& a, const fVector2& b)
{
	return floatCompare(a.x, b.x)==0 && floatCompare(a.y, b.y)==0;
}

//四舍五入
inline int real2int(Real a)
{
	return (int)(a + (Real)0.5);
}

/** 正弦计算 */
Real fastSin(const Angle& angle);
/** 余弦计算 */
Real fastCos(const Angle& angle);
/** 反余切*/
Angle fastAtan2(Real y, Real x);

/** 角度
*/
struct Angle
{
	Angle() : radian(0.f) {}
	Angle(const Angle& other) : radian(other.radian) {}
	Angle(Real _radian) : radian(_radian) {}
	Angle(const fVector2& dir) : radian(fastAtan2(dir.y, dir.x).radian) {}

	/** 将角度规整到0~2*pi之间 */
	void normalize(void)
	{
		if(radian>=MATH_PI_DOUBLE)
		{
			radian -= ((int)(radian/MATH_PI_DOUBLE))*MATH_PI_DOUBLE;
		}
		else if(radian<0)
		{
			radian += (1-(int)(radian/MATH_PI_DOUBLE))*MATH_PI_DOUBLE;
		}
	}
	Real radian;	//弧度值 [0~2*pi)
};

/** 边界
*/
struct Edge
{
	Edge() : pos(), normal() {}
	Edge(const fVector2& _pos, const fVector2& _nor) : pos(_pos), normal(_nor) {}
	Edge(const fVector2& _pos, const Angle& _ang) : pos(_pos), normal(fastCos(_ang.radian), fastSin(_ang.radian)) {}

	fVector2 pos;		//经过的点
	fVector2 normal;	//向量方向
};

/** 多边形，按照顺时针顺序排列
*/
struct Polygon
{
	bool empty(void) const { return vertices.empty(); }
	size_t size(void) const { return vertices.size(); }

	PointVector vertices;
};

/** 碰撞盒
*/
struct AABB
{
	AABB(const fVector2& l, const fVector2& u) : lowerBound(l), upperBound(u) {}
	
	fVector2 lowerBound, upperBound;
};

/** 线段
*/
struct Segment
{
	Segment() : pt1(), pt2() {}
	Segment(const fVector2& _pt1, const fVector2& _pt2) : pt1(_pt1), pt2(_pt2) {}

	fVector2 pt1, pt2;  //!< 线段的两个端点
};

/** 相交计算的结果*/
enum INTERSECTION_ERSULT
{
	IE_INT,		//!< 结果相交
	IE_UNINT,	//!< 不相交
	IE_BORDER,	//!< 处于边界状态
};

/** 点是否在碰撞盒内*/
INTERSECTION_ERSULT intersection(const AABB& box, const fVector2 point);

/** 边界和碰撞盒的相交计算(通过Liang-Barsky算法进行二维裁减)
*/
INTERSECTION_ERSULT intersection(const AABB& box, const Edge& edge, fVector2* pos1, fVector2* pos2);

/** 两条线段的相交情况
@remark
	ref: http://www.cnblogs.com/devymex/archive/2010/08/19/1803885.html
	#异常情况, seg1==0 或者 seg2==0, 返回IE_UNINT
	#有重合情况
		*完全重合(seg1==seg2), pos1,pos2等于两个端点, 返回(IE_BORDER)
		*一端重合, pos1返回重合端点, 返回IE_INT；
		*部分重合,pos1,pos2返回重合端点，返回IE_BORDER
	#无重合情况
		*有交点,返回IE_INT
		*无交点,返回IE_UNINT
*/
INTERSECTION_ERSULT intersection(const Segment& seg1, const Segment& seg2, fVector2* pos1, fVector2* pos2);

/** 计算线段和圆的相交情况(线段不包括端点)
@remark
	ref: http://paulbourke.net/geometry/sphereline/
	#没有交点，返回IE_UNINT
	#有一个交点或者两个交点，返回IE_INT存入pos1和pos2中
	#相切，返回IE_BORDER，交点存入pos1中
*/
INTERSECTION_ERSULT intersection(const Segment& seg, const fVector2& cp, Real r, fVector2* pos1, fVector2* pos2);

/** 边界(harf-space)和碰撞盒(aabb)的相交布尔计算，生成一个多边形,如果没有相交或者只有一个交点,返回false
*/
bool boolean_intersection(const AABB& box, const Edge& edge, Polygon& polygon, fVector2& ptLeft, fVector2& ptRight);

/** 曲线优化
*/
void optimize_line(PointVector& line, int counts);

/** 计算多边形面积
*/
Real polygonArea(const Polygon& polygon);

/** 计算多边形的质心(顶点顺序必须是顺时针)
*/
void polygonCentroid(const Polygon& polygon, fVector2& center);

/** 工具函数，计算凸多边形相对于质心的转动惯量
@param polygon 多边形
@param center 质心坐标，和顶点坐标必须属于同一个坐标系
*/
Real convexPolygonMoment(const Polygon& polygon, const fVector2& center);

/** 2x2矩阵,一般用于存储旋转矩阵,以列数据存储
*/
struct fMatrix2
{
	fMatrix2(): col1(1.0, -0.0), col2(0.0, 1.0) {}
	fMatrix2(const fVector2& _col1, const fVector2& _col2): col1(_col1), col2(_col2) {}
	fMatrix2(const Angle& ang) : col1(), col2() 
	{
		setAngle(ang);
	}

	void setAngle(const Angle& ang)
	{
		// TODO_ERIN compute sin+cos together.
		Real c = fastCos(ang), s = fastSin(ang);

		col1.x = c; col2.x = -s;
		col1.y = s; col2.y = c;
	}

	Angle getAngle(void) const
	{
		return fastAtan2(col1.y, col1.x);
	}

	fVector2 col1;
	fVector2 col2;
};
/** 3x3矩阵，存储完整的二维变换数据 
              |00 01 02|
    [x,y,1] * |10 11 12|
              |20 21 22|
*/
struct fMatrix3
{
	fMatrix3()
	{
		m[0][0]=1.0; m[0][1]=0.0; m[0][2]=0.0;
		m[1][0]=0.0; m[1][1]=1.0; m[1][2]=0.0;
		m[2][0]=0.0; m[2][1]=0.0; m[2][2]=1.0;
	}

	fMatrix3 (	Real fEntry00, Real fEntry01, Real fEntry02,
				Real fEntry10, Real fEntry11, Real fEntry12,
				Real fEntry20, Real fEntry21, Real fEntry22)
	{
		m[0][0] = fEntry00;
		m[0][1] = fEntry01;
		m[0][2] = fEntry02;
		m[1][0] = fEntry10;
		m[1][1] = fEntry11;
		m[1][2] = fEntry12;
		m[2][0] = fEntry20;
		m[2][1] = fEntry21;
		m[2][2] = fEntry22;
	}
	/// 旋转矩阵
	fMatrix3(const Angle& ang)
	{
		// TODO_ERIN compute sin+cos together.
		Real c = fastCos(ang), s = fastSin(ang);

		m[0][0]= c;  m[0][1]= s;  m[0][2]=0.0;
		m[1][0]=-s;  m[1][1]= c;  m[1][2]=0.0;
		m[2][0]=0.0; m[2][1]=0.0; m[2][2]=1.0;
	}
	/// 平移矩阵
	fMatrix3(const fVector2& offset)
	{
		m[0][0]=1.0;		m[0][1]=0.0;		m[0][2]=0.0;
		m[1][0]=0.0;		m[1][1]=1.0;		m[1][2]=0.0;
		m[2][0]=offset.x;	m[2][1]=offset.y; m[2][2]=1.0;
	}
	///绕远点旋转一定角度后，然后平移
	fMatrix3(const Angle& ang, const fVector2& offset)
	{
		Real c = fastCos(ang), s = fastSin(ang);

		m[0][0]= c;			m[0][1]= s;			m[0][2]=0.0;
		m[1][0]=-s;			m[1][1]= c;			m[1][2]=0.0;
		m[2][0]=offset.x;	m[2][1]=offset.y;	m[2][2]=1.0;
	}

	fVector2 transform(const fVector2& v)
	{
		Real x = v.x*m[0][0] + v.y*m[1][0] + m[2][0];
		Real y = v.x*m[0][1] + v.y*m[1][1] + m[2][1];

		return fVector2(x, y);
	}

	/** 解方程 a*M=b, b是一个目标向量, 返回a */
	fVector2 solve(const fVector2& b) const;
	/** 求逆矩阵*/
	void inverse(void);
	
	/** 获得其中的位移部分 */
	fVector2 getOffsetPart(void) const { return fVector2(m[2][0], m[2][1]); }

	Real m[3][3];

	//常用静态向量
	static const fMatrix3 IDENTIFY;
};

/** 叠加两个变换矩阵 AxB
*/
fMatrix3 mulMatrix(const fMatrix3& A, const fMatrix3& B);

/** 保存有旋转和移动数据的变换矩阵
*/
struct fTransform
{
	fTransform() : position(), rotation() {}
	fTransform(const fVector2& p, const fMatrix2& r) : position(p), rotation(r) {}
	fTransform(const fVector2& p, const Angle& ang) : position(p), rotation(ang) {}

	inline fVector2 transform(const fVector2& v) const
	{
		Real x = position.x + rotation.col1.x * v.x + rotation.col2.x * v.y;
		Real y = position.y + rotation.col1.y * v.x + rotation.col2.y * v.y;

		return fVector2(x, y);
	}

	/** 解方程 A*x=B, b是一个目标向量, 返回x */
	fVector2 solve(const fVector2& b) const;
	/** 求逆矩阵*/
	void inverse(void);

	fVector2 position;
	fMatrix2 rotation;
};

/** 叠加两个变换矩阵 AxB
*/
fTransform mulTransform(const fTransform& A, const fTransform& B);

/** 计算折线段序列中第一个交点
@return 返回交点数
@remark 定义交点中，主动去相交别的线段的一方为主动线段，被动被相交的为被动线段
那么，所谓第一个交点，是指所有相交点中，主动线段序列最小的点
*/
int getFirstCrossPoint(const PointVector& ptArray, fVector2& ptCross, int& initiative, int& passivity);

}

#endif
