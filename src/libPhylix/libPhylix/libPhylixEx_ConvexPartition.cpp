#include "libPhylix_stdafx.h"
#include "libPhylixEx_ConvexPartition.h"
#include "libPhylixEx_Polygon.h"

namespace Phylix
{

//-------------------------------------------------------------------------------------------------------------------
void convexPartition(Polygon& polygon, std::list< Polygon >& convexList, bool cw)
{
	b2Polygon input(&(polygon.vertices[0]), (int)polygon.size());

	//首先调整成逆时针(ccw)
	bool _is_cw = !input.IsCCW();
	if(_is_cw)
	{
		ReversePolygon(input.x, input.y, input.nVertices);
	}

	std::list< b2Polygon > output;
	DecomposeConvex(&input, output);

	//导出数据
	for(std::list< b2Polygon >::iterator it=output.begin(); it!=output.end(); ++it)
	{
		b2Polygon& _part = *it;

		{
			Polygon newPoly;
			convexList.push_back(newPoly);
		}
		Polygon& newPoly = *(convexList.rbegin());

		//调整顺序
		if(cw && _part.IsCCW())
		{
			ReversePolygon(_part.x, _part.y, _part.nVertices);
		}

		for(int i=0; i<_part.nVertices; i++)
		{
			newPoly.vertices.push_back(fVector2(_part.x[i], _part.y[i]));
		}
	}

	//需要反转数据
	if(cw!=_is_cw)
	{
	    PointVector::iterator it_begin = polygon.vertices.begin();
		std::reverse(++it_begin, polygon.vertices.end());
	}
}

#if 0

//CGAL
#include <CGAL/Exact_predicates_inexact_constructions_kernel.h>
#include <CGAL/Polygon_2.h>
#include <CGAL/point_generators_2.h>
#include <CGAL/partition_2.h>
#include <CGAL/Partition_traits_2.h>
#include <CGAL/centroid.h>

//CGAL Basic type
typedef CGAL::Exact_predicates_inexact_constructions_kernel		cgalKernel;
typedef cgalKernel::Point_2										cgalPoint;
typedef cgalKernel::Segment_2									cgalSegment;
typedef cgalKernel::Line_2										cgalLine;
typedef CGAL::Polygon_2<cgalKernel, std::list<cgalPoint> >		cgalPolygon; 
typedef std::list< cgalPolygon >								cgalPolygonList;

//-------------------------------------------------------------------------------------------------------------------
void convexPartition(Polygon& polygon, std::list< Polygon >& convexList, bool cw)
{
	cgalPolygon _poly;
	for(int i=0; i<(int)polygon.vertices.size(); i++)
	{
		_poly.push_back(cgalPoint(polygon.vertices[i].x, polygon.vertices[i].y));
	}

	//首先调整成逆时针(ccw)，这是CGAL的需求
	bool _is_cw;
	if(_is_cw=_poly.is_clockwise_oriented()) _poly.reverse_orientation();

	bool isconvex = _poly.is_convex();

	//调用cgal的优化分割算法
	/*
		基于论文 "Approximate Convex Decomposition of Polygons" by Jyh-Ming Lien & Nancy M.Amato
	*/
	cgalPolygonList partPolys;
	CGAL::optimal_convex_partition_2(_poly.vertices_begin(), _poly.vertices_end(),
		std::back_inserter(partPolys));

	//导出数据
	for(cgalPolygonList::iterator it=partPolys.begin(); it!=partPolys.end(); ++it)
	{
		cgalPolygon& _part = *it;

		{
			Polygon newPoly;
			convexList.push_back(newPoly);
		}
		Polygon& newPoly = *(convexList.rbegin());

		//调整顺序
		if(cw && _part.is_counterclockwise_oriented() )
		{
			_part.reverse_orientation();
		}

		for(cgalPolygon::Vertex_const_iterator vi = _part.vertices_begin(); vi!=_part.vertices_end(); ++vi)
		{
			const cgalPoint& v = *vi;
			newPoly.vertices.push_back(fVector2(v.x(), v.y()));
		}
	}

	//需要反转数据
	if(cw!=_is_cw)
	{
	    PointVector::iterator it_begin = polygon.vertices.begin();
		std::reverse(++it_begin, polygon.vertices.end());
	}
}
#endif


}
