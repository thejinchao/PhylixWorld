#include "libPhylix_stdafx.h"
#include "libPhylix_MassBody.h"

namespace Phylix
{
//-------------------------------------------------------------------------------------------------------------------
MassBody::MassBody(unsigned int id, const fVector2& pos)
	: m_id(id)
	, m_cmBody(0)
	, m_transForm(Angle(0), pos)
{
	
}

//-------------------------------------------------------------------------------------------------------------------
MassBody::~MassBody()
{
}

//-------------------------------------------------------------------------------------------------------------------
void MassBody::updateFromImpl(void)
{
	if(m_cmBody)
	{
		m_transForm = fMatrix3(Angle((Real)(m_cmBody->a)), fVector2((Real)(m_cmBody->p.x), (Real)(m_cmBody->p.y)));
	}
}

//-------------------------------------------------------------------------------------------------------------------
void MassBody::attachObject(ObjectEntity* obj)
{
	m_setObject.insert(obj);
}

//-------------------------------------------------------------------------------------------------------------------
void MassBody::getCpData(cpDataCache& data)
{
	cpBody* cmBody = getImpl();
	data.p = cmBody->p;
	data.v = cmBody->v;
	data.f = cmBody->f;
	data.a = cmBody->a;
	data.w = cmBody->w;
	data.t = cmBody->t;
	data.rot = cmBody->rot;
	data.massBodyID = getID();
}

//-------------------------------------------------------------------------------------------------------------------
void MassBody::updateParam(Real mass, Real moment)
{
	m_mass = mass;

	cpBody* cmBody = getImpl();
	cpBodySetMass(cmBody, mass);
	cpBodySetMoment(cmBody, moment);
}

}
