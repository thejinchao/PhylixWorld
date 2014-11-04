#include "StdAfx.h"
#include "Phylix_Canvas.h"
#include "Phylix_TextureUtil.h"

namespace Phylix
{
//-------------------------------------------------------------------------------------------------------------------
Canvas* Canvas::s_pSingleton = 0; 

//-------------------------------------------------------------------------------------------------------------------
Canvas::Canvas()
{
	s_pSingleton = this;
}

//-------------------------------------------------------------------------------------------------------------------
Canvas::~Canvas()
{
	s_pSingleton = 0;
}

//-------------------------------------------------------------------------------------------------------------------
void Canvas::drawPolygon(const fVector2* vertices, unsigned int vertexCount, const Material& material, bool solid, bool border, bool selected)
{
	TextureInfo* texInfo = (TextureInfo*)(material.pTemplate ?  material.pTemplate->proxy : 0);
	fColor color = material.color;

	if(solid)
	{
		if(texInfo) 
		{
			glDisable(GL_BLEND);
			glBindTexture(GL_TEXTURE_2D, texInfo->textureID);
			glColor4f(1.0, 1.0, 1.0, 0.0);
		}
		else 
		{
			glDisable(GL_BLEND);
			//glBlendFunc (GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
			glColor4f(color.r, color.g, color.b, color.a);
		}

		glBegin(GL_TRIANGLE_FAN);
		for (unsigned int i = 0; i < vertexCount; ++i)
		{	
			if(texInfo)
			{
				Real u = vertices[i].x / (CAMERA()->convertScreenSizeToWorld(CAMERA()->getViewZoom() * texInfo->width));
				Real v = vertices[i].y / (CAMERA()->convertScreenSizeToWorld(CAMERA()->getViewZoom() * texInfo->height));

				glTexCoord2f(u, v);
			}
			glVertex2d(vertices[i].x, vertices[i].y);
		}
		glEnd();

		glDisable(GL_BLEND);
	}

	if(border)
	{
		glDisable(GL_BLEND);

		if(selected)
			glColor4f(1.0f, 1.0f, 1.0f, color.a);
		else
			glColor4f(0.5f * color.r, 0.5f * color.g, 0.5f * color.b, color.a);

		//glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
		glLineWidth(2.0);
		glBegin(GL_LINE_LOOP);
		for (unsigned int i = 0; i < vertexCount; ++i)
		{
			glVertex2d(vertices[i].x, vertices[i].y);
		}
		glEnd();
	}

	glDisable(GL_BLEND);
}

//-------------------------------------------------------------------------------------------------------------------
void Canvas::drawCircle(const fVector2& center, Real radius, const fColor& color)
{
	const float k_segments = 32.0f;
	const float k_increment = (float)MATH_PI_DOUBLE / k_segments;
	float theta = 0.0f;

	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glLineWidth(1.5);

	glColor4f(color.r, color.g, color.b, color.a);

	glBegin(GL_LINE_LOOP);
	for (int i = 0; i < k_segments; ++i)
	{
		fVector2 v = center + radius * fVector2(cosf(theta), sinf(theta));
		glVertex2d(v.x, v.y);
		theta += k_increment;
	}
	glEnd();

}

//-------------------------------------------------------------------------------------------------------------------
void Canvas::drawSolidCircle(const fVector2& center, Phylix::Real radius, bool drawAxis, const fVector2& axis, const Material& material, bool selected)
{
	TextureInfo* texInfo = (TextureInfo*)(material.pTemplate ?  material.pTemplate->proxy : 0);
	fColor color = material.color;

	const float k_segments = 32.0f;
	const float k_increment = (float)MATH_PI_DOUBLE / k_segments;
	float theta = 0.0f;

	glEnable(GL_BLEND);
	glBlendFunc (GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glColor4f(color.r, color.g, color.b, color.a);
	glBegin(GL_TRIANGLE_FAN);

	if(texInfo) 
	{
		glBindTexture(GL_TEXTURE_2D, texInfo->textureID);
	}

	for (int i = 0; i < k_segments; ++i)
	{
		fVector2 p = center + radius * fVector2(cosf(theta), sinf(theta));

		if(texInfo)
		{
			Real u = p.x / (CAMERA()->convertScreenSizeToWorld(CAMERA()->getViewZoom() * texInfo->width));
			Real v = p.y / (CAMERA()->convertScreenSizeToWorld(CAMERA()->getViewZoom() * texInfo->height));

			glTexCoord2f(u, v);
		}

		glVertex2d(p.x, p.y);
		theta += k_increment;
	}
	glEnd();

	theta = 0.0f;
	if(selected)
		glColor4f(1.0f, 1.0f, 1.0f, color.a);
	else
		glColor4f(0.5f * color.r, 0.5f * color.g, 0.5f * color.b, color.a);

	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glLineWidth(1.5);

	glBegin(GL_LINE_LOOP);
	for (int i = 0; i < k_segments; ++i)
	{
		fVector2 v = center + radius * fVector2(cosf(theta), sinf(theta));
		glVertex2d(v.x, v.y);
		theta += k_increment;
	}
	glEnd();

	if(drawAxis)
	{
		fVector2 p = fVector2(axis.x, axis.y);
		glBegin(GL_LINES);
		glVertex2d(center.x, center.y);
		glVertex2d(p.x, p.y);
		glEnd();
	}
	glDisable(GL_BLEND);
}

//-------------------------------------------------------------------------------------------------------------------
void Canvas::drawSegment(const fVector2& p1, const fVector2& p2, Real width, const fColor& color)
{
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glLineWidth((float)width);

	glColor3f(color.r, color.g, color.b);
	glBegin(GL_LINES);
	glVertex2d(p1.x, p1.y);
	glVertex2d(p2.x, p2.y);
	glEnd();

	glDisable(GL_BLEND);
}

//-------------------------------------------------------------------------------------------------------------------
void Canvas::drawCross(const fVector2& point, Real size, const fColor& color)
{
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glLineWidth((float)1.0);

	glColor3f(color.r, color.g, color.b);
	glBegin(GL_LINES);
	glVertex2d(point.x-size/2.0, point.y);
	glVertex2d(point.x+size/2.0, point.y);
	glVertex2d(point.x, point.y-size/2.0);
	glVertex2d(point.x, point.y+size/2.0);
	glEnd();

	glDisable(GL_BLEND);
}

//-------------------------------------------------------------------------------------------------------------------
void Canvas::drawSegmentArrary(const fVector2* vertices, unsigned int vertexCount, const fColor& color)
{
	glColor3f(color.r, color.g, color.b);
	glLineWidth(1.f);
	glEnable(GL_LINE_STIPPLE);
	glLineStipple(1, 0x00FF);
	glBegin(GL_LINE_STRIP);
	for (unsigned int i = 0; i < vertexCount; ++i)
	{
		glVertex2d(vertices[i].x, vertices[i].y);
	}
	glEnd();
	glDisable(GL_LINE_STIPPLE);
}

//-------------------------------------------------------------------------------------------------------------------
void Canvas::drawSpring(const fVector2& p1, const fVector2& p2, const fColor& color, Real width, Real targetHeight, bool selected)
{
	glEnable(GL_BLEND);
	glBlendFunc (GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	fVector2 delta = p2-p1;
	Real cell_height = 2.0f*width/sqrt(3.0f);
	int cell_counts = (int)(targetHeight/cell_height);
	Real realLength = delta.length();
	cell_height = realLength/cell_counts;

	if(selected)
		glColor4f(1.0f, 1.0f, 1.0f, color.a);
	else
		glColor4f(color.r, color.g, color.b, color.a);

	glPushMatrix(); {
		GLdouble x = p1.x;
		GLdouble y = p1.y;
		GLdouble cos = delta.x/delta.length();
		GLdouble sin = delta.y/delta.length();

		const GLdouble matrix[] = {
			cos,	sin,	0.0, 0.0,
			-sin,	cos,	0.0, 0.0,
			0.0,	0.0,	1.0, 0.0,
			x,		y,		0.0, 1.0,
		};
		glMultMatrixd(matrix);

		glBegin(GL_LINE_STRIP);

		glVertex2d(0.0, 0.0);
		glVertex2d(cell_height/4.0, width/2.0);

		int half_counts = cell_counts*2-1;
		for(int i=0; i<half_counts; i++)
		{
			glVertex2d(cell_height/4.0 + (i+1)*cell_height/2.0, (((i&1)*2)-1)*width/2.0);
		}

		glVertex2d(realLength, 0.0);
		glEnd();

	} glPopMatrix();
}

//-------------------------------------------------------------------------------------------------------------------
void Canvas::drawArrow(const fVector2& start, const fVector2& end, const fColor& color)
{
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	glColor3f(color.r, color.g, color.b);

	fVector2 delta = end-start;
	Real len = delta.length();

	glPushMatrix(); {
		Real x = start.x;
		Real y = start.y;
		Real cos = delta.x/delta.length();
		Real sin = delta.y/delta.length();

		const Real matrix[] = {
			cos,	sin,	0.0, 0.0,
			-sin,	cos,	0.0, 0.0,
			0.0,	0.0,	1.0, 0.0,
			x,		y,		0.0, 1.0,
		};
		glMultMatrixf(matrix);

		glBegin(GL_TRIANGLES);

		glVertex2d(  0.0	, -0.08*len);
		glVertex2d(  0.0	,  0.08*len);
		glVertex2d(  0.6*len, -0.08*len);

		glVertex2d(  0.0	,  0.08*len);
		glVertex2d(  0.6*len, -0.08*len);
		glVertex2d(  0.6*len,  0.08*len);

		glVertex2d(  0.6*len, 0.3*len);
		glVertex2d(  1.0*len, 0.0);
		glVertex2d(  0.6*len, -0.3*len);

		glEnd();

	} glPopMatrix();


	glEnd();

	glDisable(GL_BLEND);

}


//-------------------------------------------------------------------------------------------------------------------
/*
void Canvas::drawTransform(const fMatrix3& xf)
{
	fVector2 p1 = xf.position, p2;
	const float k_axisScale = 0.4f;
	glBegin(GL_LINES);
	
	glColor3f(1.0f, 0.0f, 0.0f);
	glVertex2f(p1.x, p1.y);
	p2 = p1 + k_axisScale * xf.R.col1;
	glVertex2f(p2.x, p2.y);

	glColor3f(0.0f, 1.0f, 0.0f);
	glVertex2f(p1.x, p1.y);
	p2 = p1 + k_axisScale * xf.R.col2;
	glVertex2f(p2.x, p2.y);

	glEnd();
}
*/

//-------------------------------------------------------------------------------------------------------------------
//void Canvas::drawPoint(const fVector2& p, Phylix::Real size, const fColor& color)
//{
//	glPointSize((float)size);
//	glBegin(GL_POINTS);
//	glColor3f(color.r, color.g, color.b);
//	glVertex2d(p.x, p.y);
//	glEnd();
//	glPointSize(1.0f);
//}

}

