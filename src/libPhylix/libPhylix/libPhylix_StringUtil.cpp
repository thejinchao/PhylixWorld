#include "libPhylix_stdafx.h"
#include "libPhylix_StringUtil.h"

namespace Phylix
{

//-------------------------------------------------------------------------------------------------------------------
static int	convertStringToVector(const char* strStrintgSource, std::vector< std::string >& vRet, const char* szKey, bool bOneOfKey, bool bIgnoreEmpty)
{
	vRet.clear();

	//合法性
	if(!strStrintgSource || strStrintgSource[0] == '\0') return 0;
	
	std::string strSrc = strStrintgSource;

	//查找第一个分割点
	std::string::size_type nLeft = 0;
	std::string::size_type nRight;
	if(bOneOfKey)
	{
		nRight = strSrc.find_first_of(szKey);
	}
	else
	{
		nRight = strSrc.find(szKey);
	}

	if(nRight == std::string::npos)
	{
		nRight = strSrc.length();
	}
	while(true)
	{
		std::string strItem = strSrc.substr(nLeft, nRight-nLeft);
		if(strItem.length() > 0 || !bIgnoreEmpty)
		{
			vRet.push_back(strItem);
		}

		if(nRight == strSrc.length())
		{
			break;
		}

		nLeft = nRight + (bOneOfKey ? 1 : strlen(szKey));
		
		if(bOneOfKey)
		{
			std::string strTemp = strSrc.substr(nLeft);
			nRight = strTemp.find_first_of(szKey);
			if(nRight != std::string::npos) nRight += nLeft;
		}
		else
		{
			nRight = strSrc.find(szKey, nLeft);
		}

		if(nRight == std::string::npos)
		{
			nRight = strSrc.length();
		}
	}

	return (int)vRet.size();
}

//-------------------------------------------------------------------------------------------------------------------
std::string toString(const fVector2& point)
{
	char szTemp[32]={0};
	snprintf(szTemp, 32, "%f %f", point.x, point.y);

	return std::string(szTemp);
}

//-------------------------------------------------------------------------------------------------------------------
std::string toString(const Material& material)
{
	char szTemp[256]={0};
	snprintf(szTemp, 256, "%f %f %f %f %f %f %f", 
		material.density, material.fricition, material.restitution, 
		material.color.r, material.color.g, material.color.b, material.color.a);

	return std::string(szTemp);
}

//-------------------------------------------------------------------------------------------------------------------
std::string toString(const MassBody::cpDataCache& massBody)
{
	char szTemp[1024]={0};
	snprintf(szTemp, 1024, "%f %f %f %f %f %f %f %f %f %f %f %d",
		massBody.p.x, massBody.p.y,
		massBody.v.x, massBody.v.y,
		massBody.f.x, massBody.f.y,
		massBody.a, massBody.w, massBody.t, 
		massBody.rot.x, massBody.rot.y, 
		massBody.massBodyID);

	return std::string(szTemp);
}

//-------------------------------------------------------------------------------------------------------------------
std::string toString(const PointVector& points)
{
	std::string ret;
	char szTemp[32]={0};
	for(int i=0; i<(int)points.size(); i++)
	{
		const fVector2& pt = points[i];
		snprintf(szTemp, 32, "%f %f%s", pt.x, pt.y, (i==(int)points.size()-1) ? "" : ";");

		ret += szTemp;
	}

	return ret;
}

//-------------------------------------------------------------------------------------------------------------------
std::string toString(const ObjectParams::PARAM_TYPE& paramType)
{
	switch(paramType)
	{
	case ObjectParams::PT_POSITION_1: return std::string("Position1");
	case ObjectParams::PT_POSITION_2: return std::string("Position2");
	case ObjectParams::PT_ANGLE: return std::string("Angle");
	case ObjectParams::PT_SIZE: return std::string("Size");
	case ObjectParams::PT_RADIUS: return std::string("Radius");
	case ObjectParams::PT_WIDTH: return std::string("Width");
	case ObjectParams::PT_POINTS: return std::string("Points");
	case ObjectParams::PT_MATERIAL: return std::string("Material");
	case ObjectParams::PT_OBJECT_ID: return std::string("ObjectID");
	case ObjectParams::PT_MASSBODY: return std::string("MassCache");
	case ObjectParams::PT_Z_VALUE: return std::string("ZValue");
	default: return std::string("<ERROR>");
	}
}

//-------------------------------------------------------------------------------------------------------------------
std::string toString(const Object::Type& objType)
{
	switch(objType)
	{
	case Object::OT_ENTITY_EDGE: return std::string("Edge");
	case Object::OT_ENTITY_BOX: return std::string("Box");
	case Object::OT_ENTITY_CIRCLE: return std::string("Circle");
	case Object::OT_ENTITY_POLYGON: return std::string("Polygon");
	case Object::OT_FIXED_JOINT: return std::string("Fixed");
	case Object::OT_HINGE_JOINT: return std::string("Hinge");
	case Object::OT_SPRING_JOINT: return std::string("Spring");

	default: return std::string("<ERROR>");
	}
}

//-------------------------------------------------------------------------------------------------------------------
std::string toString(const Angle& angle)
{
	char szTemp[16]={0};
	snprintf(szTemp, 16, "%f", angle.radian);

	return std::string(szTemp);
}

//-------------------------------------------------------------------------------------------------------------------
std::string toString(const Real& real)
{
	char szTemp[16]={0};
	snprintf(szTemp, 16, "%f", real);

	return std::string(szTemp);
}

//-------------------------------------------------------------------------------------------------------------------
std::string toString(const unsigned int& value)
{
	char szTemp[16]={0};
	snprintf(szTemp, 16, "%d", value);

	return std::string(szTemp);
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

//-------------------------------------------------------------------------------------------------------------------
void fromString(const std::string& str, fVector2& point)
{
	sscanf(str.c_str(), "%f %f", &(point.x), &(point.y));
}

//-------------------------------------------------------------------------------------------------------------------
void fromString(const std::string& str, Material& material)
{
	sscanf(str.c_str(), "%f %f %f %f %f %f %f",
		&(material.density), &(material.fricition), &(material.restitution), 
		&(material.color.r), &(material.color.g), &(material.color.b), &(material.color.a));
}

//-------------------------------------------------------------------------------------------------------------------
void fromString(const std::string& str, PointVector& points)
{
	std::vector< std::string > ptVector;
	convertStringToVector(str.c_str(), ptVector, ";", true, true);
	for(int i=0; i<(int)ptVector.size(); i++)
	{
		fVector2 point;
		sscanf(ptVector[i].c_str(), "%f %f", &(point.x), &(point.y));

		points.push_back(point);
	}
}

//-------------------------------------------------------------------------------------------------------------------
void fromString(const std::string& str, MassBody::cpDataCache& massBody)
{
	sscanf(str.c_str(), "%f %f %f %f %f %f %f %f %f %f %f %d",
		&(massBody.p.x), &(massBody.p.y),
		&(massBody.v.x), &(massBody.v.y),
		&(massBody.f.x),&( massBody.f.y),
		&(massBody.a), &(massBody.w), &(massBody.t), 
		&(massBody.rot.x), &(massBody.rot.y), 
		&(massBody.massBodyID));
}

//-------------------------------------------------------------------------------------------------------------------
void fromString(const std::string& str, ObjectParams::PARAM_TYPE& paramType)
{
	if(strcasecmp(str.c_str(), "Position1")==0) paramType = ObjectParams::PT_POSITION_1;
	else if(strcasecmp(str.c_str(), "Position2")==0) paramType = ObjectParams::PT_POSITION_2;
	else if(strcasecmp(str.c_str(), "Angle")==0) paramType = ObjectParams::PT_ANGLE;
	else if(strcasecmp(str.c_str(), "Size")==0) paramType = ObjectParams::PT_SIZE;
	else if(strcasecmp(str.c_str(), "Radius")==0) paramType = ObjectParams::PT_RADIUS;
	else if(strcasecmp(str.c_str(), "Width")==0) paramType = ObjectParams::PT_WIDTH;
	else if(strcasecmp(str.c_str(), "Points")==0) paramType = ObjectParams::PT_POINTS;
	else if(strcasecmp(str.c_str(), "Material")==0) paramType = ObjectParams::PT_MATERIAL;
	else if(strcasecmp(str.c_str(), "ObjectID")==0) paramType = ObjectParams::PT_OBJECT_ID;
	else if(strcasecmp(str.c_str(), "MassCache")==0) paramType = ObjectParams::PT_MASSBODY;
	else if(strcasecmp(str.c_str(), "ZValue")==0) paramType = ObjectParams::PT_Z_VALUE;
	else paramType = ObjectParams::PT_COUNTS;

}

//-------------------------------------------------------------------------------------------------------------------
void fromString(const std::string& str, Object::Type& objType)
{
	if(strcasecmp(str.c_str(), "Edge")==0) objType = Object::OT_ENTITY_EDGE;
	else if(strcasecmp(str.c_str(), "Box")==0) objType = Object::OT_ENTITY_BOX;
	else if(strcasecmp(str.c_str(), "Circle")==0) objType = Object::OT_ENTITY_CIRCLE;
	else if(strcasecmp(str.c_str(), "Polygon")==0) objType = Object::OT_ENTITY_POLYGON;
	else if(strcasecmp(str.c_str(), "Fixed")==0) objType = Object::OT_FIXED_JOINT;
	else if(strcasecmp(str.c_str(), "Hinge")==0) objType = Object::OT_HINGE_JOINT;
	else if(strcasecmp(str.c_str(), "Spring")==0) objType = Object::OT_SPRING_JOINT;
	else objType = Object::OT_NULL;
}

//-------------------------------------------------------------------------------------------------------------------
void fromString(const std::string& str, Angle& angle)
{
	sscanf(str.c_str(), "%f", &(angle.radian));
}

//-------------------------------------------------------------------------------------------------------------------
void fromString(const std::string& str, Real& real)
{
	sscanf(str.c_str(), "%f", &real);
}

//-------------------------------------------------------------------------------------------------------------------
void fromString(const std::string& str, unsigned int& value)
{
	sscanf(str.c_str(), "%d", &value);
}

}
