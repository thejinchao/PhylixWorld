#include "libPhylix_stdafx.h"
#include "libPhylix_Math.h"

namespace Phylix
{

//-------------------------------------------------------------------------------------------------------------------
const fMatrix3 fMatrix3::IDENTIFY = fMatrix3(1.0, 0.0, 0.0, 0.0, 1.0, 0.0, 0.0, 0.0, 1.0);

//-------------------------------------------------------------------------------------------------------------------
int floatCompare(Real a, Real b, Real tolerance )
{
	Real c = a-b;

	if(c > tolerance) return 1;
	else if( c < -tolerance) return -1;
	return 0;
}

//-------------------------------------------------------------------------------------------------------------------
Real fastSin(const Angle& angle)
{
	return sin(angle.radian);
}

//-------------------------------------------------------------------------------------------------------------------
Real fastCos(const Angle& angle)
{
	return cos(angle.radian);
}

//-------------------------------------------------------------------------------------------------------------------
Angle fastAtan2(Real y, Real x)
{
	return Angle(atan2(y, x));
}

//-------------------------------------------------------------------------------------------------------------------
fMatrix3 mulMatrix(const fMatrix3& A, const fMatrix3& B)
{
	return fMatrix3(
		A.m[0][0]*B.m[0][0] + A.m[0][1]*B.m[1][0] + A.m[0][2]*B.m[2][0],
		A.m[0][0]*B.m[0][1] + A.m[0][1]*B.m[1][1] + A.m[0][2]*B.m[2][1],
		A.m[0][0]*B.m[0][2] + A.m[0][1]*B.m[1][2] + A.m[0][2]*B.m[2][2],

		A.m[1][0]*B.m[0][0] + A.m[1][1]*B.m[1][0] + A.m[1][2]*B.m[2][0],
		A.m[1][0]*B.m[0][1] + A.m[1][1]*B.m[1][1] + A.m[1][2]*B.m[2][1],
		A.m[1][0]*B.m[0][2] + A.m[1][1]*B.m[1][2] + A.m[1][2]*B.m[2][2],

		A.m[2][0]*B.m[0][0] + A.m[2][1]*B.m[1][0] + A.m[2][2]*B.m[2][0],
		A.m[2][0]*B.m[0][1] + A.m[2][1]*B.m[1][1] + A.m[2][2]*B.m[2][1],
		A.m[2][0]*B.m[0][2] + A.m[2][1]*B.m[1][2] + A.m[2][2]*B.m[2][2]
	);
}

//-------------------------------------------------------------------------------------------------------------------
fVector2 fMatrix3::solve(const fVector2& b) const
{
	/*
                     | 00 01 02 |
		[ax ay 1] *  | 10 11 12 | = [bx by 1]
                     | 20 21 22 |
	*/
	const Real &_00 = m[0][0], &_11 = m[1][1], &_01=m[0][1], &_10=m[1][0], &_20=m[2][0], &_21=m[2][1];
	Real det = _00*_11 - _01*_10;

	assert(floatCompare(det, 0.0)!=0);
	if(floatCompare(det, 0.0) != 0)
	{
		det = (Real)(1.0 / det);
	}

	fVector2 a;
	a.x = det * (_11 * b.x - _10 * b.y - _20*_11 + _21*_10);
	a.y = det * (_00 * b.y - _01 * b.x + _20*_01 - _21*_00);

	return a;
}

//-------------------------------------------------------------------------------------------------------------------
void fMatrix3::inverse(void)
{
	//cofactors methor

	fMatrix3 rkInverse;

	rkInverse.m[0][0] = m[1][1]*m[2][2] - m[1][2]*m[2][1];
	rkInverse.m[0][1] = m[0][2]*m[2][1] - m[0][1]*m[2][2];
	rkInverse.m[0][2] = m[0][1]*m[1][2] - m[0][2]*m[1][1];
	rkInverse.m[1][0] = m[1][2]*m[2][0] - m[1][0]*m[2][2];
	rkInverse.m[1][1] = m[0][0]*m[2][2] - m[0][2]*m[2][0];
	rkInverse.m[1][2] = m[0][2]*m[1][0] - m[0][0]*m[1][2];
	rkInverse.m[2][0] = m[1][0]*m[2][1] - m[1][1]*m[2][0];
	rkInverse.m[2][1] = m[0][1]*m[2][0] - m[0][0]*m[2][1];
	rkInverse.m[2][2] = m[0][0]*m[1][1] - m[0][1]*m[1][0];

	Real fDet = m[0][0]*rkInverse.m[0][0] + m[0][1]*rkInverse.m[1][0] + m[0][2]*rkInverse.m[2][0];

	bool invalid = floatCompare(fDet, 0)!=0;
	assert(invalid);
	if(!invalid) return; //error

	Real fInvDet = 1.0f/fDet;
	for(int i=0; i<3; i++) 
	{
		for(int j=0; j<3; j++) 
		{
			m[i][j] = rkInverse.m[i][j]*fInvDet;
		}
	}
}

//-------------------------------------------------------------------------------------------------------------------
fTransform mulTransform(const fTransform& A, const fTransform& B)
{
	fTransform ret;
	ret.position.x = A.rotation.col1.x * B.position.x + A.rotation.col2.x * B.position.y + A.position.x;
	ret.position.y = A.rotation.col1.y * B.position.x + A.rotation.col2.y * B.position.y + A.position.y;

	ret.rotation.col1.x = A.rotation.col1.x * B.rotation.col1.x + A.rotation.col2.x * B.rotation.col1.y;
	ret.rotation.col1.y = A.rotation.col1.y * B.rotation.col1.x + A.rotation.col2.y * B.rotation.col1.y;
	ret.rotation.col2.x = A.rotation.col1.x * B.rotation.col2.x + A.rotation.col2.x * B.rotation.col2.y;
	ret.rotation.col2.y = A.rotation.col1.y * B.rotation.col2.x + A.rotation.col2.y * B.rotation.col2.y;

	return ret;
}

//-------------------------------------------------------------------------------------------------------------------
INTERSECTION_ERSULT intersection(const AABB& box, const fVector2 point)
{
	int L = floatCompare(point.x, box.lowerBound.x);
	int R = floatCompare(point.x, box.upperBound.x);
	int B = floatCompare(point.y, box.lowerBound.y);
	int T = floatCompare(point.y, box.upperBound.y);

	if(L>0)
	{
		if(R<0)
		{
			if(B>0)
			{
				if(T<0)	return IE_INT;
				else if(T==0) return IE_BORDER;
				else return IE_UNINT;
			}
			else if(B==0) return IE_BORDER;
			else return IE_UNINT;
		}
		else if(R==0 && B>=0 && T<=0) return IE_BORDER;
		else return IE_UNINT;
	}
	else if(L==0)
	{
		if(B>=0 && T<=0) return IE_BORDER;
		else return IE_UNINT;
	}
	else return IE_UNINT;
}

//-------------------------------------------------------------------------------------------------------------------
INTERSECTION_ERSULT intersection(const AABB& box, const Edge& edge, fVector2* pos1, fVector2* pos2)
{

	Real dy = -edge.normal.x;
	Real dx =  edge.normal.y;

	Angle nor(edge.normal);
	nor.normalize();
	Real x0, y0, x1, y1;
	if(	(nor.radian > MATH_PI_QUARTER && nor.radian < MATH_PI*0.75f) ||
		(nor.radian > MATH_PI* 1.25f && nor.radian < MATH_PI*1.75f))
	{
		//比较平行于横线
		x0 = box.lowerBound.x;
		y0 = edge.pos.y + (box.lowerBound.x-edge.pos.x)*dy/dx;
		x1 = box.upperBound.x;
		y1 = edge.pos.y + (box.upperBound.x-edge.pos.x)*dy/dx;
	}
	else
	{
		//比较平行于竖线
		x0 = edge.pos.x + (box.lowerBound.y-edge.pos.y)*dx/dy;
		y0 = box.lowerBound.y;
		x1 = edge.pos.x + (box.upperBound.y-edge.pos.y)*dx/dy;
		y1 = box.upperBound.y;
	}

	Real t0=0.f, t1=1.f;
	Real xDelta = x1-x0;
	Real yDelta = y1-y0;

	for(int edge=0; edge<4; edge++)
	{
		Real p,q,r;
		if(edge==0)			{ p=-xDelta;	q=-(box.lowerBound.x-x0); }
		else if(edge==1)	{ p= xDelta;	q= (box.upperBound.x-x0); }
		else if(edge==2)	{ p=-yDelta;	q=-(box.lowerBound.y-y0); }
		else if(edge==3)	{ p= yDelta;	q= (box.upperBound.y-y0); }

		if(p==0 && q<0) return IE_UNINT;
		r = q/p;

		if(p<0)
		{
			if(r>t1) return IE_UNINT;
			else if(r>t0) t0=r;
		}
		else if(p>0)
		{
			if(r<t0) return IE_UNINT;
			else if(r<t1) t1=r;
		}
	}

	fVector2 p1(x0 + t0*xDelta, y0 + t0*yDelta);
	fVector2 p2(x0 + t1*xDelta, y0 + t1*yDelta);

	if(pos1) { *pos1 = p1; }
	if(pos2) { *pos2 = p2; }

	if(p1==p2) return IE_BORDER;

	return IE_INT;
}

//-------------------------------------------------------------------------------------------------------------------
INTERSECTION_ERSULT intersection(const Segment& seg1, const Segment& seg2, fVector2* pos1, fVector2* pos2)
{
	fVector2 p1 = seg1.pt1;
	fVector2 p2 = seg1.pt2;
	fVector2 p3 = seg2.pt1;
	fVector2 p4 = seg2.pt2;

	//异常，线段退化
	if(vectorSimilar(p1, p2) || vectorSimilar(p3, p4)) return IE_UNINT;

	//为方便，保证线段起点在前，终点在后
	if(p1>p2) std::swap(p1, p2);
	if(p3>p4) std::swap(p3, p4);

	//完全重合的两条线段
	if(vectorSimilar(p1, p3) && vectorSimilar(p2, p4))
	{
		if(pos1) *pos1 = p1;
		if(pos2) *pos2 = p2;
		return IE_BORDER;
	}

	//有一个端点重合
	if(	vectorSimilar(p1, p3) || vectorSimilar(p1, p4))
	{
		if(pos1) *pos1 = p1;
		return IE_INT;
	}
	if(	vectorSimilar(p2, p3) || vectorSimilar(p2, p4))
	{
		if(pos1) *pos1 = p2;
		return IE_INT;
	}

	//将线段按照起点坐标重新排列，如果线段1的起点较大，交换两条线段, 保证线段1在线段2的左下方
	if(p1>p3)
	{
		std::swap(p1, p3);
		std::swap(p2, p4);
	}

	//线段向量
	fVector2 v1 = p2-p1;
	fVector2 v2 = p4-p3;

	//两线段叉积
	Real v1_cross_v2 = crossProduct(v1, v2);

	//线段平行?
	if(floatCompare(v1_cross_v2, 0)==0)
	{
		//判断是否共线, 并且部分重合
		if(floatCompare(crossProduct(v1, p3-p1), 0)==0 && p2>p3)
		{
			if(pos1) *pos1 = p3;
			if(pos2) *pos2 = p2;
			return IE_BORDER;
		}

		//没有相交或者共线部分
		return IE_UNINT;
	}

	//进行快速排斥实验
	Real seg1_y_min=p1.y, seg1_y_max = p2.y;
	Real seg2_y_min=p3.y, seg2_y_max = p4.y;
	if(seg1_y_min > seg1_y_max) std::swap(seg1_y_min, seg1_y_max);
	if(seg2_y_min > seg2_y_max) std::swap(seg2_y_min, seg2_y_max);

	//aabb不相交
	if(p1.x > p4.x || p2.x<p3.x || seg1_y_max < seg2_y_min || seg1_y_min > seg2_y_max) //注:第一个判定条件应该不会出现
	{
		return IE_UNINT;
	}

	//跨立测试
	fVector2 p3_p1 = p1-p3, p3_p2 = p2-p3;
	fVector2 p1_p3 = p3-p1, p1_p4 = p4-p1;
	Real p3p1_cross_v2, p3p2_cross_v2, p1p3_cross_v1, p1p4_cross_v1;

	//p1在seg2上
	if(floatCompare(p3p1_cross_v2=crossProduct(p3_p1, v2), 0)==0 && p4>p1 && p1>p3)
	{
		if(pos1) *pos1 = p1;
		return IE_INT;
	}
	//p2在seg2上
	if(floatCompare(p3p2_cross_v2=crossProduct(p3_p2, v2), 0)==0 && p4>p2 && p2>p3)
	{
		if(pos1) *pos1 = p2;
		return IE_INT;
	}
	//p3在seg1上
	if(floatCompare(p1p3_cross_v1=crossProduct(p1_p3, v1), 0)==0 && p2>p3 && p3>p1)
	{
		if(pos1) *pos1 = p3;
		return IE_INT;
	}
	//p4在seg1上
	if(floatCompare(p1p4_cross_v1=crossProduct(p1_p4, v1), 0)==0 && p2>p4 && p4>p1)
	{
		if(pos1) *pos1 = p4;
		return IE_INT;
	}

	//通过跨立测试,判断是否相交
	if(p3p1_cross_v2*p3p2_cross_v2>0 || p1p3_cross_v1*p1p4_cross_v1>0)
	{
		return IE_UNINT;
	}

	//相交，计算交点
	Real conA = p1.x*v1.y - p1.y*v1.x;
	Real conB = p3.x*v2.y - p3.y*v2.x;
	
	if(pos1)
	{
		pos1->x = (conB*v1.x - conA*v2.x) / v1_cross_v2;
		pos1->y = (conB*v1.y - conA*v2.y) / v1_cross_v2;
	}
	return IE_INT;
}

//-------------------------------------------------------------------------------------------------------------------
INTERSECTION_ERSULT intersection(const Segment& seg, const fVector2& cp, Real r, fVector2* pos1, fVector2* pos2)
{
	fVector2 sp = seg.pt2-seg.pt1;

	Real A = sp.square(); //A= (x2-x1)^2+(y2-y1)^2
	Real B = 2*(sp.x*(seg.pt1.x-cp.x) + sp.y*(seg.pt1.y-cp.y)); //B = 2[(x2-x1)(x1-x3)+(y2-y1)*(y1-y3)]
	Real C = cp.square() + seg.pt1.square() - 2*(cp.x*seg.pt1.x+cp.y*seg.pt1.y)-r*r; //C = x3^2+y3^2+x1^2+y1^2-2(x3*x2+y3*y1)-r^2
	Real B2_4AC = B*B-4*A*C;

	//无解
	if(floatCompare(A, 0)==0) return IE_UNINT;
	A=0.5f/A;

	Real u1, u2;
	int flag_B2_4AC = floatCompare(B2_4AC,0);
	switch(flag_B2_4AC)
	{
	case -1: 
		return IE_UNINT; //无解;
	case  0: 
		//相切
		u1 = u2 = -B*A; 
		break; 
	case  1: 
		//相交
		B2_4AC = sqrt(B2_4AC);
		u1 = (-B + B2_4AC)*A;
		u2 = (-B - B2_4AC)*A;
		break;
	}

	bool u1_valid = (floatCompare(u1, 0)>0 && floatCompare(u1, 1)<1);
	bool u2_valid = (floatCompare(u2, 0)>0 && floatCompare(u2, 1)<1);

	if( !u1_valid && !u2_valid) return IE_UNINT; //无解
	if( u1_valid && u2_valid)
	{
		//两个交点
		*pos1 = seg.pt1 + u1*sp;
		*pos2 = seg.pt1 + u2*sp;
		return IE_INT;
	}
	
	//一个交点
	if(flag_B2_4AC==0)
	{
		*pos1 = seg.pt1 + u1*sp; //相切
		return IE_BORDER;
	}

	//相交于一个点上
	if(u1_valid) *pos1 = *pos2 = seg.pt1 + u1*sp;
	else *pos1 = *pos2 = seg.pt1 + u2*sp;
	return IE_INT;
}

//-------------------------------------------------------------------------------------------------------------------
bool boolean_intersection(const AABB& box, const Edge& edge, Polygon& polygon, fVector2& ptLeft, fVector2& ptRight)
{
	//首先计算交点
	INTERSECTION_ERSULT intRet = intersection(box, edge, &ptLeft, &ptRight);

	//没有相交或者交点只有一个
	if(intRet!=IE_INT) return false;

	//确保ptLeft在“左侧”
	if(crossProduct(edge.normal, ptLeft-edge.pos) < 0)
	{
		std::swap(ptLeft, ptRight);
	}

	//计算四个顶点中分别有那些在边界"内部"
	fVector2 corner[4] = 
	{
		fVector2(box.lowerBound.x, box.lowerBound.y), 
		fVector2(box.upperBound.x, box.lowerBound.y), 
		fVector2(box.upperBound.x, box.upperBound.y), 
		fVector2(box.lowerBound.x, box.upperBound.y), 
	};

	std::vector< int > ptInEdge;
	ptInEdge.reserve(4);

	fVector2 vCenterLeft = ptLeft-edge.pos;
	for(int i=0; i<4; i++)
	{
		if(crossProduct(vCenterLeft, corner[i]-edge.pos)>0)
		{
			ptInEdge.push_back(i);
		}
	}

	int ptCounts = (int)ptInEdge.size();
	PointVector& ver = polygon.vertices;
	ver.clear();

	//形成三角形
	if(ptCounts==1)
	{
		ver.push_back(ptLeft);
		ver.push_back(corner[ptInEdge[0]]);
		ver.push_back(ptRight);
	}
	//形成四边形
	else if(ptCounts==2)
	{
		int first = ptInEdge[0];
		int second = ptInEdge[1];

		if(first==0 && second==3)
		{
			ver.push_back(ptLeft);
			ver.push_back(corner[3]);
			ver.push_back(corner[0]);
			ver.push_back(ptRight);
		}
		else
		{
			assert(second-first==1);
			ver.push_back(ptLeft);
			ver.push_back(corner[first]);
			ver.push_back(corner[second]);
			ver.push_back(ptRight);
		}
	}
	//形成五边形
	else if(ptCounts==3)
	{
		if(	(ptInEdge[0]==0 && ptInEdge[1]==1 && ptInEdge[2]==2) ||
			(ptInEdge[0]==1 && ptInEdge[1]==2 && ptInEdge[2]==3))
		{
			ver.push_back(ptLeft);
			ver.push_back(corner[ptInEdge[0]]);
			ver.push_back(corner[ptInEdge[1]]);
			ver.push_back(corner[ptInEdge[2]]);
			ver.push_back(ptRight);
		}
		else if(ptInEdge[0]==0 && ptInEdge[1]==2 && ptInEdge[2]==3)
		{
			ver.push_back(ptLeft);
			ver.push_back(corner[2]);
			ver.push_back(corner[3]);
			ver.push_back(corner[0]);
			ver.push_back(ptRight);
		}
		else if(ptInEdge[0]==0 && ptInEdge[1]==1 && ptInEdge[2]==3)
		{
			ver.push_back(ptLeft);
			ver.push_back(corner[3]);
			ver.push_back(corner[0]);
			ver.push_back(corner[1]);
			ver.push_back(ptRight);
		}
		else
		{
			assert(false);
		}
	}
	else
	{
		//不应该出现的结果
		assert(false);
		return false;
	}

	return true;
}

//-------------------------------------------------------------------------------------------------------------------
static bool _can_optimize(const fVector2& pt1, const fVector2& pt2, const fVector2& pt3)
{
	#define THRESHOLD (0.005)

	//中间点和两端过于接近
	if(vectorSimilar(pt1, pt2) || vectorSimilar(pt2, pt3)) return true;

	Real c = crossProduct(pt2-pt1, pt3-pt2);
	return fabs(c) < THRESHOLD;
}

//-------------------------------------------------------------------------------------------------------------------
void optimize_line(PointVector& line, int counts)
{
	typedef std::list< fVector2 > PointList;

	PointList ptList;
	for(int i=0; i<(int)line.size(); i++) ptList.push_back(line[i]);


	for(int i=0; i<counts; i++)
	{
		int opt_counts = 0, org_counts = (int)ptList.size();

		PointList::iterator itLeft=ptList.begin(), itCheck, itRight;
		do
		{
			if(ptList.size() <= 2) break;

			itCheck = itLeft; itCheck++; if(itCheck==ptList.end()) break;
			itRight = itCheck; itRight++; if(itRight==ptList.end()) break;

			if(_can_optimize(*itLeft, *itCheck, *itRight))
			{
				ptList.erase(itCheck);
				opt_counts++;
			}

			itLeft = itRight;
		}while(true);

		if((float)opt_counts/(float)org_counts < 0.1f) break;
	}


	//rewrite back
	line.clear();
	for(PointList::iterator it=ptList.begin(); it!=ptList.end(); ++it)
	{
		line.push_back(*it);
	}
}

//-------------------------------------------------------------------------------------------------------------------
Real polygonArea(const Polygon& polygon)
{
	Real area = 0.0;
	int counts = (int)polygon.size();
	for(int i=0; i<counts; i++)
	{
		const fVector2& v1 = polygon.vertices[i];
		const fVector2& v2 = polygon.vertices[(i+1)%counts];

		area += crossProduct(v1, v2);
	}

	return fabs(area)/2.0f;
}

//-------------------------------------------------------------------------------------------------------------------
void polygonCentroid(const Polygon& poly, fVector2& center)
{
	int n = (int)poly.size();
	Real X=0.0, Y=0.0;
	for(int i=0; i<n; i++)
	{
		const fVector2& a = poly.vertices[i];
		const fVector2& b = poly.vertices[(i+1)%n];
		Real _cross = crossProduct(b, a);

		X += (a.x + b.x)*_cross;
		Y += (a.y + b.y)*_cross;
	}

	Real S = polygonArea(poly);
	center.x = X/(6.0f*S);
	center.y = Y/(6.0f*S);
}

//-------------------------------------------------------------------------------------------------------------------
Real convexPolygonMoment(const Polygon& polygon, const fVector2& center)
{
	//参考: http://www.physicsforums.com/showthread.php?s=e251fddad79b926d003e2d4154799c14&t=25293&page=2&pp=15
	Real num1 = 0.0;
	Real num2 = 0.0;
	int counts = (int)polygon.size();
	for(int i=0; i<counts; i++)
	{
		fVector2 v1 = polygon.vertices[i]-center;
		fVector2 v2 = polygon.vertices[(i+1)%counts]-center;

		Real a = crossProduct(v2, v1);
		Real b = v1.square() + v2.square() + dotProduct(v1, v2);

		num1 += a*b;
		num2 += a;
	}
	return num1/(6.0f*num2);
}

//-------------------------------------------------------------------------------------------------------------------
fVector2 fTransform::solve(const fVector2& b) const
{
	/*
		|r.c1.x  r.c2.x  p.x |    |x|   |bx|
		|r.c1.y  r.c2.y  p.y |  * |y| = |by|
		|   0      0      1  |    |0|   |0 |
	*/
	const Real &a11 = rotation.col1.x, &a12 = rotation.col2.x, &a21 = rotation.col1.y, &a22 = rotation.col2.y;
	Real det = a11 * a22 - a12 * a21;

	assert(floatCompare(det, 0.0)!=0);
	if(floatCompare(det, 0.0) != 0)
	{
		det = 1.0f / det;
	}
	fVector2 x;
	Real px = position.x; //a11*position.x + a12*position.y;
	Real py = position.y; //a21*position.x + a21*position.y;
	x.x = det * (a22 * b.x - a12 * b.y + a12*py - a22*px);
	x.y = det * (a11 * b.y - a21 * b.x - a11*py - a21*px);

	return x;
}

//-------------------------------------------------------------------------------------------------------------------
void fTransform::inverse(void)
{
	Real _det = rotation.col1.x*rotation.col2.y - rotation.col2.x*rotation.col1.y;
	if(floatCompare(_det, 0.0)==0) return;

	Real _invert_det = 1.0f/_det;

	Real _00 = rotation.col2.y;
	Real _10 = -rotation.col1.y;
//	Real _20 = 0.0;
	Real _01 = -rotation.col2.x;
	Real _11 = rotation.col1.x;
//	Real _32 = -0.0;
	Real _02 = rotation.col2.x * position.y - rotation.col2.y * position.x;
	Real _12 = -(rotation.col1.x * position.y - rotation.col1.y * position.x);
//	Real _22 = rotation.col1.x * rotation.col2.y - rotation.col1.y * rotation.col2.x;

	rotation.col1.x = _00 *_invert_det;
	rotation.col1.y = _10 *_invert_det;
	rotation.col2.x = _01 *_invert_det;
	rotation.col2.y = _11 *_invert_det;
	position.x = _02 * _invert_det;
	position.y = _12 * _invert_det;

	return;
}

// 相交点
struct CrossPoint
{
	int firstIndex;		//!< 主动相交线段序列号
	int secondIndex;	//!< 被动相交点序列号
	fVector2 ptCross;	//!< 相交点坐标
	fVector2 ptFristSegStart;  //!< 主动线段的起始点
};

// 相交点排序工具类
struct CrossPointSortHelper
{
	bool operator()(const struct CrossPoint& pt1, const struct CrossPoint& pt2)
	{
		if(pt1.firstIndex < pt2.firstIndex) return true;
		else if(pt1.firstIndex > pt2.firstIndex) return false;

		//相等， 判断相交点
		const fVector2& start = pt1.ptFristSegStart;  //应该等于pt2.ptFristSegStart
		return (pt1.ptCross-start).length() < (pt2.ptCross-start).length();
	}
};
    
//-------------------------------------------------------------------------------------------------------------------
int getFirstCrossPoint(const PointVector& ptArray, fVector2& ptCross, int& initiative, int& passivity)
{
	//寻找所有相交点
	std::set< CrossPoint, CrossPointSortHelper > cpSet;

	int segment_counts = (int)ptArray.size();
	for(int i=0; i<segment_counts-2; i++)
	{
		for(int j=i+2; j<segment_counts; j++)
		{
			if(i==0 && j==segment_counts-1) continue;
			//两条线段端点
			int seg1_f = i, seg1_s = i+1;
			int seg2_f = j, seg2_s = (j+1)%segment_counts;

			//可能的交点
			fVector2 pos1, pos2;

			//判断相交情况
			INTERSECTION_ERSULT itRet = intersection(
				Segment(ptArray[seg1_f], ptArray[seg1_s]), 
				Segment(ptArray[seg2_f], ptArray[seg2_s]), 
				&pos1, &pos2);

			//不相交
			if(itRet==IE_UNINT) continue;

			//相交于1点或者两点
			CrossPoint cp;
			cp.firstIndex = j;
			cp.secondIndex = i;
			cp.ptCross = pos1;
			cp.ptFristSegStart = ptArray[seg2_f];
			cpSet.insert(cp);
		}
	}

	//没有相交点
	if(cpSet.empty()) return 0;

	//第一个相交点
	const CrossPoint& pt = *(cpSet.begin());

	ptCross = pt.ptCross;
	initiative = pt.firstIndex;
	passivity = pt.secondIndex;

	return (int)cpSet.size();
}

}
