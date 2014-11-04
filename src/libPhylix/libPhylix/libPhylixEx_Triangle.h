/*
 from Box2D http://code.google.com/p/box2d
*/
#ifndef __PHYSICS_WORLD_LIBEX_TRIANGLE_H__
#define __PHYSICS_WORLD_LIBEX_TRIANGLE_H__
/*
 * Copyright (c) 2007 Eric Jordan
 *
 * This software is provided 'as-is', without any express or implied
 * warranty.  In no event will the authors be held liable for any damages
 * arising from the use of this software.
 * Permission is granted to anyone to use this software for any purpose,
 * including commercial applications, and to alter it and redistribute it
 * freely, subject to the following restrictions:
 * 1. The origin of this software must not be misrepresented; you must not
 * claim that you wrote the original software. If you use this software
 * in a product, an acknowledgment in the product documentation would be
 * appreciated but is not required.
 * 2. Altered source versions must be plainly marked as such, and must not be
 * misrepresented as being the original software.
 * 3. This notice may not be removed or altered from any source distribution.
 */

namespace Phylix
{

class b2Triangle
{
public:
	Real x[3];
    Real y[3];
	b2Triangle();
	b2Triangle(Real x1, Real y1, Real x2, Real y2, Real x3, Real y3);
    ~b2Triangle();
	bool IsInside(Real _x, Real _y);
	void Set(const b2Triangle& toMe);

};

}

#endif
