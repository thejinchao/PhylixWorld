// stdafx.h : include file for standard system include files,
//  or project specific include files that are used frequently, but
//      are changed infrequently
//

#pragma once

// Change these values to use different versions
#define WINVER		0x0500
#define _WIN32_IE	0x0500

#include <atlbase.h>
#include <atlapp.h>
#include <atlwin.h>
#include <atlframe.h>
#include <atlctrls.h>
#include <atlctrlw.h>
#include <atldlgs.h>
#include <atlscrl.h>
#include <atlmisc.h>
#include <atlcrack.h>

extern CAppModule _Module;

// C Runtime library
#include <stdio.h>
#include <tchar.h>
#include <time.h>
#include <math.h>
#include <assert.h>

// CPP Runtime library
#include <string>
#include <vector>
#include <list>
#include <map>
#include <set>

// OpenGL
#include <gl/gl.h>
#include <gl/glu.h>

//chipmunk
#include <chipmunk/chipmunk.h>

//libWorld
#include <libPhylix_TypeDefine.h>
#include <libPhylix_Math.h>
#include <libPhylix_Camera.h>
#include <libPhylix_Scene.h>
#include <libPhylix_RenderInterface.h>
#include <libPhylix_EventManager.h>
#include <libPhylix_MassBodyManager.h>
#include <libPhylix_MaterialManager.h>
#include <libPhylix_ShapeManager.h>
#include <libPhylix_ObjectManager.h>
#include <libPhylix_ActionManager.h>
#include <libPhylix_EventManager.h>
#include <libPhylix_OperatorManager.h>
#include <libPhylix_Object_Entity.h>
#include <libPhylix_Shape.h>
#include <libPhylix_FileStream.h>
#include <libPhylix_Operator.h>
#include <libPhylix_OperatorManager.h>
#include <libPhylix_Operator.h>

