#ifndef __PHYSICS_WORLD_LIB_STRING_UTIL_H__
#define __PHYSICS_WORLD_LIB_STRING_UTIL_H__

#include "libPhylix_ObjectParams.h"

namespace Phylix
{

std::string toString(const fVector2& point);
std::string toString(const Material& material);
std::string toString(const PointVector& points);
std::string toString(const MassBody::cpDataCache& massBody);
std::string toString(const ObjectParams::PARAM_TYPE& paramType);
std::string toString(const Object::Type& objType);
std::string toString(const Angle& angle);
std::string toString(const Real& real);
std::string toString(const unsigned int& value);


void fromString(const std::string& str, fVector2& point);
void fromString(const std::string& str, Material& material);
void fromString(const std::string& str, PointVector& points);
void fromString(const std::string& str, MassBody::cpDataCache& massBody);
void fromString(const std::string& str, ObjectParams::PARAM_TYPE& paramType);
void fromString(const std::string& str, Object::Type& objType);
void fromString(const std::string& str, Angle& angle);
void fromString(const std::string& str, Real& real);
void fromString(const std::string& str, unsigned int& value);

}

#endif

