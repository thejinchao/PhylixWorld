#include "libPhylix_stdafx.h"
#include "libPhylix_ShapeManager.h"
#include "libPhylix_Shape.h"
#include "libPhylixEx_ConvexPartition.h"
#include "libPhylix_Material.h"
#include "libPhylix_Scene.h"

namespace Phylix
{

//-------------------------------------------------------------------------------------------------------------------
ShapeManager::ShapeManager()
{
}

//-------------------------------------------------------------------------------------------------------------------
ShapeManager::~ShapeManager()
{
}

//-------------------------------------------------------------------------------------------------------------------
Shape* ShapeManager::createBoxShape(ObjectEntity* owner, const fVector2& size, const Material* material)
{
	Shape_Poly* shape = new Shape_Poly(this, owner);

	//初始化顶点数据
	/*
         (0)    (1)
          *-----*
          |  +  |
		  *-----*
         (3)    (2)
	*/
	PointVector& vertices = shape->m_poly.vertices;
	vertices.resize(4);
	vertices[0] = fVector2(-size.x/2.0f,  size.y/2.0f);
	vertices[1] = fVector2( size.x/2.0f,  size.y/2.0f);
	vertices[2] = fVector2( size.x/2.0f, -size.y/2.0f);
	vertices[3] = fVector2(-size.x/2.0f, -size.y/2.0f);

	//面积
	shape->m_area = size.x*size.y;
	shape->m_areaDirt = false;

	//转动惯量
	shape->m_moment = (size.x*size.x + size.y*size.y)/12.0f;
	shape->m_momentDirt = false; 

	//chipmunk指针
	shape->m_cmShape = new cpShape*[1];
	shape->m_cmShapeCounts = 1;
	shape->m_cmShape[0] = cpBoxShapeNew(0, size.x, size.y);
	shape->m_cmShape[0]->data = shape;
	shape->m_cmShape[0]->e = material->restitution;
	shape->m_cmShape[0]->u = material->fricition;
	shape->m_cmShape[0]->layers = COLLISION_DEFAULT_LAYER;

	//加入一个子凸多边形
	Shape_Poly::SubConvex subConvex;
	subConvex.convex = shape->m_poly;
	subConvex.area = shape->m_area;
	subConvex.center = fVector2::ZERO;
	subConvex.cmShape = (cpPolyShape*)(shape->m_cmShape[0]);

	shape->m_convexList.push_back(subConvex);

	return shape;
}

//-------------------------------------------------------------------------------------------------------------------
Shape* ShapeManager::createCircleShape(ObjectEntity* owner, Real radius, bool axisValid, const Angle& ang, const Material* material)
{
	Shape_Circle* shape = new Shape_Circle(this, owner);

	//半径
	shape->m_radius = radius;
	//方向(用于指示)
	shape->m_axisValid = axisValid;
	shape->m_axis = fVector2(fastCos(ang), fastSin(ang));
	//面积
	shape->_updateAreaImpl();
	//转动惯量
	shape->_updateMomentImpl();
	
	//chipmunk指针
	shape->m_cmShape = new cpShape*;
	shape->m_cmShapeCounts = 1;
	shape->m_cmShape[0] = cpCircleShapeNew(0, radius, cpv(0, 0));
	shape->m_cmShape[0]->data = shape;
	shape->m_cmShape[0]->e = material->restitution;
	shape->m_cmShape[0]->u = material->fricition;
	shape->m_cmShape[0]->layers = COLLISION_DEFAULT_LAYER;

	return shape;
}

//-------------------------------------------------------------------------------------------------------------------
Shape* ShapeManager::createEdgeShape(ObjectEntity* owner, const fVector2& ptCenter, const Angle& normal, const Material* material)
{
	Shape_Edge* shape = new Shape_Edge(this, owner);

	//设置法线
	shape->setBasePosition(ptCenter);
	shape->setNormalAngle(normal);

	//chipmunk指针

	/*
		用多边形

		(0)
         * 
         | `
         |   `*(1)
         |    |
         *----*
        (3)   (2)
	*/

	//计算交界点
	Polygon poly;
	fVector2 ptLeft, ptRight;
	boolean_intersection(SCENE()->getWorldBounderBox(), Edge(ptCenter, shape->getNormalAngle()), poly, ptLeft, ptRight);
	assert(poly.size()==4);

	cpVect temp[4];
	for(int i=0; i<4; i++)temp[i] = cpv(poly.vertices[3-i].x, poly.vertices[3-i].y);

	shape->m_cmShape = new cpShape*;
	shape->m_cmShapeCounts = 1;
	shape->m_cmShape[0] = (cpShape*)cpPolyShapeNew(0, 4, temp, cpv(0,0));
	shape->m_cmShape[0]->data = shape;
	shape->m_cmShape[0]->e = material->restitution;
	shape->m_cmShape[0]->u = material->fricition;
	shape->m_cmShape[0]->layers = COLLISION_ALL_LAYER;

	return shape;
}

//-------------------------------------------------------------------------------------------------------------------
Shape* ShapeManager::createPolygon(ObjectEntity* owner, const PointVector& ptArray, const Material* material, fVector2& ptCenter)
{
	Polygon poly;  //凹多边形
	poly.vertices = ptArray;

	std::list< Polygon > convexPolyList;  //凸多边形列表

	//分割成数个凸多边形,并且调成顺时针
	convexPartition(poly, convexPolyList, true);

	//计算凹多边形的质心
	fVector2 center;
	polygonCentroid(poly, center);

	//调整成相对凹多边形质心的坐标
	for(int i=0; i<(int)poly.size(); i++)
	{
		poly.vertices[i] -= center;
	}

	Shape* shape = new Shape_Poly(this, owner);
	((Shape_Poly* )shape)->m_poly = poly; //凹多边形

	//chipmunk内部指针
	int polyCounts = (int)convexPolyList.size();
	shape->m_cmShape = new cpShape*[polyCounts];
	shape->m_cmShapeCounts = polyCounts;

	//加入子凸多边形
	int index=0;
	for(std::list< Polygon >::iterator it=convexPolyList.begin(); it!=convexPolyList.end(); ++it, index++)
	{
		Polygon& _poly = *it;
		for(int i=0; i<(int)_poly.size(); i++)
		{
			//调整成相对凹多边形质心的坐标
			_poly.vertices[i] -= center;
		}

		//加入凸多边形列表
		{
			Shape_Poly::SubConvex newSubConvex;
			((Shape_Poly* )shape)->m_convexList.push_back(newSubConvex);
		}
		Shape_Poly::SubConvex& newSubConvex = *(((Shape_Poly* )shape)->m_convexList.rbegin());
		newSubConvex.convex = _poly; //顶点
		polygonCentroid(_poly, newSubConvex.center); //质心
		newSubConvex.area = polygonArea(_poly); //面积

		//chipmunk指针
		int counts = (int)_poly.size();
		cpVect* v = new cpVect[counts];
		for(int i=0; i<counts; i++)v[i] = cpv(_poly.vertices[i].x, _poly.vertices[i].y);

		newSubConvex.cmShape = (cpPolyShape*)cpPolyShapeNew(0, counts, v, cpv(0,0));
		shape->m_cmShape[index] = (cpShape*)newSubConvex.cmShape;
		shape->m_cmShape[index]->data = shape;
		shape->m_cmShape[index]->e = material->restitution;
		shape->m_cmShape[index]->u = material->fricition;
		shape->m_cmShape[index]->layers = COLLISION_DEFAULT_LAYER;

		delete[] v; v=0;
	}

	ptCenter = center;
	return shape;
}

//-------------------------------------------------------------------------------------------------------------------
void ShapeManager::destroyShape(Shape* shape)
{
	if(shape==0) return;

	cpSpace* space = SCENE()->getImpl();
	int cpShapeCounts = shape->getImplCounts();
	for(int i=0; i<cpShapeCounts; i++)
	{
		cpShape* _shape = shape->getImpl(i);
		cpSpaceRemoveShape(space, _shape);
	}

	delete[] shape->m_cmShape;
}

}
