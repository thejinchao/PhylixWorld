#include "libPhylix_stdafx.h"
#include "libPhylix_Object_Entity.h"
#include "libPhylix_Shape.h"
#include "libPhylix_MassBody.h"
#include "libPhylix_MassBodyManager.h"
#include "libPhylix_ObjectManager.h"
#include "libPhylix_Scene.h"
#include "libPhylix_ObjectParams.h"
#include "libPhylix_Material.h"
#include "libPhylix_RenderInterface.h"
#include "libPhylix_Camera.h"

namespace Phylix
{

//-------------------------------------------------------------------------------------------------------------------
ObjectEntity::ObjectEntity(unsigned int id, Type type)
	: Object(id)
	, m_type(type)
	, m_shape(0)
	, m_massBody(0)
{
}

//-------------------------------------------------------------------------------------------------------------------
ObjectEntity::~ObjectEntity()
{
	//TODO: 销毁引用数据
}

//-------------------------------------------------------------------------------------------------------------------
void ObjectEntity::draw(IRenderInterface* render)
{
	if(!m_shape) return;

	bool selected = (SCENE()->getSelectedObject()==getID());

	//显示所拥有的Shape
	m_shape->draw(render, getMaterial(), selected);

	//debug: draw mass center
	if(getMassBody())
	{
		render->drawCross(getMassBody()->getPosition(), (Real)(1.0/(CAMERA()->getViewZoom()*16.0)), fColor(1.0, 1.0, 0.0, 1.0));
	}
}

//-------------------------------------------------------------------------------------------------------------------
fMatrix3 ObjectEntity::getTrnasFormMatrix(void) const
{
	if(m_massBody)
		return mulMatrix(m_shape->getTrnasFormMatrix(), m_massBody->getTrnasFormMatrix());
	else
		return m_shape->getTrnasFormMatrix();
}

//-------------------------------------------------------------------------------------------------------------------
void ObjectEntity::buildCreateParam(ObjectParams& param)
{
	switch(getType())
	{
	case OT_ENTITY_EDGE:	//边界
		{
			param.reset(OT_ENTITY_EDGE);

			Shape_Edge* shape = (Shape_Edge*)getShape();
			param.setPosition1(shape->getBasePoint());
			param.setAngle(shape->getNormalAngle());

			param.setMaterial(getMaterial());
			param.setObjectID(getID());
			param.setZValue(getZOrder());
		}
		break;
	case OT_ENTITY_BOX:		// 盒子
	case OT_ENTITY_POLYGON:	// 多边形
		{
			param.reset(OT_ENTITY_POLYGON);

			Shape_Poly* shape = (Shape_Poly*)getShape();
			param.setPoints(shape->getPolygonData().vertices);

			if(getMassBody())
			{
				MassBody::cpDataCache massData;
				getMassBody()->getCpData(massData);
				param.setMassBody(massData);
			}

			param.setMaterial(getMaterial());
			param.setObjectID(getID());
			param.setZValue(getZOrder());
		}
		break;
	case OT_ENTITY_CIRCLE:	// 圆形
		{
			param.reset(OT_ENTITY_CIRCLE);

			param.setPosition1(getTrnasFormMatrix().getOffsetPart());

			Shape_Circle* shape = (Shape_Circle*)getShape();
			param.setRadius(shape->getRadius());

			fVector2 axis = shape->getAxis();
			param.setAngle(fastAtan2(axis.y, axis.x));

			if(getMassBody())
			{
				MassBody::cpDataCache massData;
				getMassBody()->getCpData(massData);
				param.setMassBody(massData);
			}

			param.setMaterial(getMaterial());
			param.setObjectID(getID());
			param.setZValue(getZOrder());
		}
		break;
            
    default: break;
	}
}

//-------------------------------------------------------------------------------------------------------------------
void ObjectEntity::updateMaterial(const Material& material)
{
	m_material = material;

	//重新计算质量体参数
	if(m_massBody)
	{
		Real mass = m_shape->getArea() * m_material.density;
		Real moment = mass * m_shape->getMoment();
		m_massBody->updateParam(mass, moment);
	}

	//重新计算shape参数
	int counts = m_shape->getImplCounts();
	for(int i=0; i<counts; i++)
	{
		cpShape* shape = m_shape->getImpl(i);
		shape->e = material.restitution;
		shape->u = material.fricition;
	}
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
CircleObject::CircleObject(unsigned int id, Type type)
	: ObjectEntity(id, type)
	, m_chainID(0)
{
}

//-------------------------------------------------------------------------------------------------------------------
CircleObject::~CircleObject()
{

}

//-------------------------------------------------------------------------------------------------------------------
void CircleObject::notifySelected(void)
{
	if(m_chainID!=0)
	{
		SCENE()->selectObject(m_chainID);
	}
}

//-------------------------------------------------------------------------------------------------------------------
void CircleObject::draw(IRenderInterface* render)
{
	if(!m_shape) return;

	unsigned int selectID = SCENE()->getSelectedObject();
	bool selected = (selectID!=0 && (selectID==getID() || selectID==m_chainID));

	//显示所拥有的Shape
	m_shape->draw(render, getMaterial(), selected);
}

}
