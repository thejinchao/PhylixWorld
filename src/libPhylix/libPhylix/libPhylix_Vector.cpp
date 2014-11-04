#include "libPhylix_stdafx.h"
#include "libPhylix_Vector.h"
#include "libPhylix_Math.h"

namespace Phylix
{

//-------------------------------------------------------------------------------------------------------------------
const fVector2 fVector2::ZERO(0, 0);

//-------------------------------------------------------------------------------------------------------------------
Real fVector2::length(void) const 
{
	return sqrt(x*x+y*y);
}

//-------------------------------------------------------------------------------------------------------------------
Real fVector2::normalize(void)
{
	Real len = sqrt(x*x+y*y);
	if(floatCompare(len, 0)<=0) return 0.0;

	Real inv_len = 1.0f/len;

	x *= inv_len;
	y *= inv_len;

	return len;
}

}
