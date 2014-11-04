#include "libPhylix_stdafx.h"
#include "libPhylix_Object_Joint.h"

namespace Phylix
{

//-------------------------------------------------------------------------------------------------------------------
ObjectJoint::ObjectJoint(unsigned int id)
	: Object(id)
	, m_firstObj(0)
	, m_secondObj(0)
{
}

//-------------------------------------------------------------------------------------------------------------------
ObjectJoint::~ObjectJoint()
{
}


}
