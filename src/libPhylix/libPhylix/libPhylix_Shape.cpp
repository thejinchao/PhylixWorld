#include "libPhylix_stdafx.h"
#include "libPhylix_Shape.h"
#include "libPhylix_ShapeManager.h"
#include "libPhylix_RenderInterface.h"
#include "libPhylix_Material.h"
#include "libPhylix_Scene.h"
#include "libPhylix_MassBody.h"
#include "libPhylix_Object_Entity.h"

namespace Phylix
{

//-------------------------------------------------------------------------------------------------------------------
Shape::Shape(ShapeManager* shapManager, ObjectEntity* ownerObj) 
	: m_shapManager(shapManager)	
	, m_area(0)
	, m_areaDirt(true)
//	, m_material(0)
	, m_moment(0)
	, m_momentDirt(true)
	, m_cmShapeCounts(0)
	, m_cmShape(0)
	, m_objectOwner(ownerObj)
{
}

//-------------------------------------------------------------------------------------------------------------------
Shape::~Shape()
{
}

//-------------------------------------------------------------------------------------------------------------------
Real Shape::getArea(void) const 
{
	if(m_areaDirt) 
	{
		_updateAreaImpl();
	}
	return m_area;
}

//-------------------------------------------------------------------------------------------------------------------
Real Shape::getMoment(void) const
{
	if(m_momentDirt)
	{
		_updateMomentImpl();
	}
	return m_moment;
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

//-------------------------------------------------------------------------------------------------------------------
void Shape_Circle::_updateAreaImpl(void) const
{
	//s = pi*r^2;
	m_area = MATH_PI*m_radius*m_radius;
	m_areaDirt = false;
}

//-------------------------------------------------------------------------------------------------------------------
void Shape_Circle::_updateMomentImpl(void) const
{
	//圆形物体围绕圆心旋转, I=m*R*R/2
	m_moment = m_radius*m_radius/2.0f;
	//偏移(平行轴原理) 
	m_moment += getPosition().square();
	m_momentDirt = false;
}

//-------------------------------------------------------------------------------------------------------------------
void Shape_Circle::draw(IRenderInterface* render, const Material& material, bool selected) const
{
	fMatrix3 trans = m_objectOwner->getTrnasFormMatrix();

	fVector2 axis = trans.transform(m_radius*m_axis);

	render->drawSolidCircle(trans.getOffsetPart(), m_radius, m_axisValid, axis, material, selected);
}

//-------------------------------------------------------------------------------------------------------------------
void Shape_Circle::resetMassOwner(MassBody* body)
{
	//旧的Transform矩阵
	fMatrix3 oldTransForm = m_objectOwner->getTrnasFormMatrix();
	//新的MassBody的Transform矩阵
	fMatrix3 newMassTransForm = body ? body->getTrnasFormMatrix() : fMatrix3::IDENTIFY;
	//新的Shape变换矩阵(相对于新的MassBody)
	newMassTransForm.inverse();
	m_transForm = mulMatrix(oldTransForm, newMassTransForm);

	cpSpace* space = SCENE()->getImpl();

	//从chipmunk空间去除
	cpShape* cmShape = getImpl(0);
	cpSpaceRemoveShape(space, cmShape);

	//调整chipmunk内部数据
	fVector2 newPositon = m_transForm.getOffsetPart();
	((cpCircleShape*)cmShape)->c = cpv(newPositon.x, newPositon.y);

	//重新加入chipmunk空间
	cmShape->body = body ? body->getImpl() : cpSpaceGetStaticBody(space);
	cpSpaceAddShape(space, cmShape);
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

//-------------------------------------------------------------------------------------------------------------------
void Shape_Edge::_updateAreaImpl(void) const
{
	//该函数不应该被执行
	assert(false);
	return;
}

//-------------------------------------------------------------------------------------------------------------------
void Shape_Edge::_updateMomentImpl(void) const
{
	//该函数不应该被执行
	assert(false);
	return;
}

//-------------------------------------------------------------------------------------------------------------------
void Shape_Edge::resetMassOwner(MassBody* body)
{
	//该函数不应该被执行
	assert(false);
	return;
}

//-------------------------------------------------------------------------------------------------------------------
void Shape_Edge::draw(IRenderInterface* render, const Material& material, bool selected) const
{
	if(m_borderDirt) _updateBorder();
	if(m_border.vertices.empty()) return;

	render->drawPolygon(&(m_border.vertices[0]), (unsigned int)m_border.vertices.size(), material, true, true, selected);
}

//-------------------------------------------------------------------------------------------------------------------
void Shape_Edge::_updateBorder(void) const
{
	//计算交界点
	fVector2 ptLeft, ptRight;
	bool ret = boolean_intersection(
		SCENE()->getWorldBounderBox(), 
		Edge(getBasePoint(), getNormalAngle()), m_border, ptLeft, ptRight);

	assert(ret);ret=true;
	m_borderDirt = false;
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//-------------------------------------------------------------------------------------------------------------------
void Shape_Poly::_updateAreaImpl(void) const
{
	m_area = polygonArea(m_poly);
	m_areaDirt = false;
}

//-------------------------------------------------------------------------------------------------------------------
void Shape_Poly::_updateMomentImpl(void) const
{
	m_moment = 0.0;
	std::list< SubConvex >::const_iterator it, end=m_convexList.end();
	for(it=m_convexList.begin(); it!=end; ++it)
	{
		const SubConvex& poly = *it;

		//所占质量系数
		Real percent = poly.area/getArea();

		m_moment += percent*(convexPolygonMoment(poly.convex, poly.center) + poly.center.square());
	}
	//偏移(平行轴原理) 
	m_moment += getPosition().square();
	m_momentDirt = false;
}

//-------------------------------------------------------------------------------------------------------------------
void Shape_Poly::draw(IRenderInterface* render, const Material& material, bool selected) const
{
	if(m_convexList.empty()) return;
	
	fMatrix3 trans = m_objectOwner->getTrnasFormMatrix();

	bool convex_mode = (m_convexList.size()==1);

	std::list< SubConvex >::const_iterator it, end=m_convexList.end();
	for(it=m_convexList.begin(); it!=end; ++it)
	{
		const SubConvex& poly = *it;
		const PointVector& vertices = poly.convex.vertices;

		PointVector ptArray(vertices.size());
		for(unsigned int i=0; i<(unsigned int)vertices.size(); i++)
		{
			ptArray[i] = trans.transform(vertices[i]);
		}

		render->drawPolygon(&(ptArray[0]), (unsigned int)ptArray.size(), material, true, convex_mode ? true : false, selected);
	}

	//draw border
	if(!convex_mode)
	{
		const PointVector& vertices = m_poly.vertices;

		PointVector ptArray(vertices.size());
		for(unsigned int i=0; i<(unsigned int)vertices.size(); i++)
		{
			ptArray[i] = trans.transform(vertices[i]);
		}

		render->drawPolygon(&(ptArray[0]), (unsigned int)ptArray.size(), material, false, true, selected);
	}
}

//-------------------------------------------------------------------------------------------------------------------
void Shape_Poly::resetMassOwner(MassBody* body)
{
	//旧的Transform矩阵
	fMatrix3 oldTransForm = m_objectOwner->getTrnasFormMatrix();
	//新的MassBody的Transform矩阵
	fMatrix3 newMassTransForm = body ? body->getTrnasFormMatrix() : fMatrix3::IDENTIFY;
	//新的Shape变换矩阵(相对于新的MassBody)
	fMatrix3 newMassTransForm_i = newMassTransForm;
	newMassTransForm_i.inverse();
	m_transForm = mulMatrix(oldTransForm, newMassTransForm_i);

	cpSpace* space = SCENE()->getImpl();
	
	//重新设置子凸多边形顶点数据
	std::list< SubConvex >::const_iterator it, end=m_convexList.end();
	for(it=m_convexList.begin(); it!=end; ++it)
	{
		const SubConvex& poly = *it;
		const PointVector& vertices = poly.convex.vertices;

		//删除chipmunk空间数据
		cpPolyShape* cmShape = (cpPolyShape*)poly.cmShape;
		cpSpaceRemoveShape(space, (cpShape*)cmShape);

		//顶点数
		int numVerts = (int)vertices.size();
		for(int i=0; i<numVerts; i++)
		{
			//顶点的世界坐标
			fVector2 vWorldPos = oldTransForm.transform(vertices[i]);
			//相对于新质心坐标
			fVector2 vPos_NewMass = newMassTransForm.solve(vWorldPos);

			//调整chipmunk内部数据_1
			cmShape->verts[i] = cpv(vPos_NewMass.x, vPos_NewMass.y);
		}

		//调整chipmunk内部数据_2
		for(int i=0; i<numVerts; i++)
		{
			const cpVect& a = cmShape->verts[i];
			const cpVect& b = cmShape->verts[(i+1)%numVerts];
			cpVect n = cpvnormalize(cpvperp(cpvsub(b, a)));

			cmShape->planes[i].n = n;
			cmShape->planes[i].d = cpvdot(n, a);
		}

		//重新加入chipmunk空间
		((cpShape*)cmShape)->body = body ? body->getImpl() : cpSpaceGetStaticBody(space);
		cpSpaceAddShape(space, ((cpShape*)cmShape));
	}
}

}

