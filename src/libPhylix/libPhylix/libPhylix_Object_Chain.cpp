#include "libPhylix_stdafx.h"
#include "libPhylix_Object_Chain.h"
#include "libPhylix_Object_Joint_Hinge.h"
#include "libPhylix_Object_Entity.h"
#include "libPhylix_ObjectParams.h"

namespace Phylix
{

//-------------------------------------------------------------------------------------------------------------------
ObjectChain::ObjectChain(unsigned int id)
	: Object(id)
	, m_width(0.1f)
	, m_lostFirst(false)
	, m_lostTail(false)
{
}

//-------------------------------------------------------------------------------------------------------------------
ObjectChain::~ObjectChain()
{
}

//-------------------------------------------------------------------------------------------------------------------
void ObjectChain::buildCreateParam(ObjectParams& param)
{
	param.reset(OT_CHAIN);

	//points
	PointVector ptArray;

	//lost head
	param.setBoolean1(m_lostFirst);
	if(m_lostFirst)
	{
		ptArray.push_back(m_firstHingePos);
	}
	else
	{
		ptArray.push_back(m_hingeList[0]->getWorldPos());
	}

	for(int i=1; i<(int)m_hingeList.size()-1; i++)
	{
		ptArray.push_back(m_hingeList[i]->getWorldPos());
	}

	//lost tail
	param.setBoolean2(m_lostTail);
	if(m_lostTail)
	{
		//最后一个节点
		const fVector2& ptLastNode = m_circleList[m_circleList.size()-1]->getTrnasFormMatrix().getOffsetPart();
		ptArray.push_back(2*ptLastNode- ptArray[ptArray.size()-1]);
	}
	else
	{
		ptArray.push_back(m_hingeList[m_hingeList.size()-1]->getWorldPos());
	}


	param.setPoints(ptArray);
	//width
	param.setWidth(m_width);
	//child mass
	ObjectParams::ChildMassCache childMass;
	for(int i=0; i<(int)m_circleList.size(); i++)
	{
		ObjectParams::MassCache mass;
		MassBody* massBody = m_circleList[i]->getMassBody();
		if(massBody) 
		{
			massBody->getCpData(mass);
		}
		childMass.push_back(mass);
	}
	param.setChildMass(childMass);
	param.setZValue(m_circleList[0]->getZOrder());
	param.setMaterial(m_circleList[0]->getMaterial());
	param.setObjectID(getID());
	param.setZValue(getZOrder());
}

//-------------------------------------------------------------------------------------------------------------------
void ObjectChain::draw(IRenderInterface* render)
{
}

//-------------------------------------------------------------------------------------------------------------------
unsigned int ObjectChain::getHeadHinge(void) const
{
	if(m_lostFirst) return 0;
	return m_hingeList[0]->getID();
}

//-------------------------------------------------------------------------------------------------------------------
unsigned int ObjectChain::getTailHinge(void) const
{
	if(m_lostTail) return 0;
	return m_hingeList[m_hingeList.size()-1]->getID();
}

//-------------------------------------------------------------------------------------------------------------------
void ObjectChain::deleteHead(void)
{
	if(m_lostFirst) return;

	m_lostFirst = true;
	m_firstHingePos = m_hingeList[0]->getWorldPos();
	m_hingeList[0] = 0;
}

//-------------------------------------------------------------------------------------------------------------------
void ObjectChain::resumeHead(ObjectHingeJoint* hinge)
{
	if(!m_lostFirst) return;

	m_lostFirst = false;
	if(m_hingeList.empty()) return;
	
	m_hingeList[0] = hinge;
}

//-------------------------------------------------------------------------------------------------------------------
void ObjectChain::deleteTail(void)
{
	if(m_lostTail) return;

	m_lostTail = true;
	m_hingeList[m_hingeList.size()-1] = 0;
}

//-------------------------------------------------------------------------------------------------------------------
void ObjectChain::resumeTail(ObjectHingeJoint* hinge)
{
	if(!m_lostTail) return;

	m_lostTail = false;
	if(m_hingeList.empty()) return;
	
	m_hingeList[m_hingeList.size()-1] = hinge;
}


}
