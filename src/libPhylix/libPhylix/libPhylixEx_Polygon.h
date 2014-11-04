/*
 from Box2D http://code.google.com/p/box2d
*/
#ifndef __PHYSICS_WORLD_LIBEX_POLYGON_H__
#define __PHYSICS_WORLD_LIBEX_POLYGON_H__

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

#include "libPhylixEx_Triangle.h"

namespace Phylix
{

class b2Polygon;

int remainder(int x, int modulus);
int TriangulatePolygon(Real* xv, Real* yv, int vNum, b2Triangle* results);
bool IsEar(int i, Real* xv, Real* yv, int xvLength); //Not for external use
int PolygonizeTriangles(b2Triangle* triangulated, int triangulatedLength, std::list< b2Polygon >& polys);
int DecomposeConvex(b2Polygon* p, std::list< b2Polygon >& results);
b2Polygon ConvexHull(fVector2* v, int nVert);
b2Polygon ConvexHull(Real* cloudX, Real* cloudY, int nVert);
void ReversePolygon(Real* x, Real* y, int n);

b2Polygon TraceEdge(b2Polygon* p); //For use with self-intersecting polygons, finds outline

class b2Polygon {
	
public:
    //const static int maxVerticesPerPolygon = b2_maxPolygonVertices;

    Real* x; //vertex arrays
    Real* y;
    int nVertices;
	
	Real area;
	bool areaIsSet;
	
    b2Polygon(Real* _x, Real* _y, int nVert);
    b2Polygon(fVector2* v, int nVert);
	b2Polygon();
    ~b2Polygon();
	
	Real GetArea();
	
	void MergeParallelEdges(Real tolerance);
    fVector2* GetVertexVecs();
    b2Polygon(b2Triangle& t);
    void Set(const b2Polygon& p);
    bool IsConvex();
	bool IsCCW();
	bool IsUsable(bool printError);
	bool IsUsable();
    bool IsSimple();
	
    b2Polygon* Add(b2Triangle& t);

	void print(){
		printFormatted();
//		for (int i=0; i<nVertices; ++i){
//			printf("i: %d, x:%f, y:%f\n",i,x[i],y[i]);
//		}
	}

	void printFormatted(){
		printf("float xv[] = {");
		for (int i=0; i<nVertices; ++i){
			printf("%ff,",x[i]);
		}
		printf("};\nfloat yv[] = {");
		for (int i=0; i<nVertices; ++i){
			printf("%ff,",y[i]);
		}
		printf("};\n");
	}
    
	b2Polygon(const b2Polygon& p){
		nVertices = p.nVertices;
		area = p.area;
		areaIsSet = p.areaIsSet;
		x = new Real[nVertices];
		y = new Real[nVertices];
		memcpy(x, p.x, nVertices * sizeof(Real));
		memcpy(y, p.y, nVertices * sizeof(Real));
	}

	
};

const int MAX_CONNECTED = 32;
const Real COLLAPSE_DIST_SQR = FLT_EPSILON*FLT_EPSILON;//0.1f;//1000*FLT_EPSILON*1000*FLT_EPSILON;
	
class b2PolyNode{
public:
	fVector2 position;
	b2PolyNode* connected[MAX_CONNECTED];
	int nConnected;
	bool visited;

	b2PolyNode(fVector2& pos);
	b2PolyNode();
	void AddConnection(b2PolyNode& toMe);
	void RemoveConnection(b2PolyNode& fromMe);
	void RemoveConnectionByIndex(int index);
	bool IsConnectedTo(b2PolyNode& me);
	b2PolyNode* GetRightestConnection(b2PolyNode* incoming);
	b2PolyNode* GetRightestConnection(fVector2& incomingDir);
};

}

#endif