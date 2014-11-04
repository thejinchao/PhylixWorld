#include "stdafx.h"
#include "Phylix_MainFrame.h"
#include "Phylix_System.h"

CAppModule _Module;

//-------------------------------------------------------------------------------------------------------------------
int WINAPI _tWinMain(HINSTANCE hInstance, HINSTANCE /*hPrevInstance*/, LPTSTR lpstrCmdLine, int nCmdShow)
{
	//init srand
	srand(::GetTickCount());

	//init common control
	INITCOMMONCONTROLSEX iccx;
	iccx.dwSize = sizeof(iccx);
	iccx.dwICC = ICC_COOL_CLASSES | ICC_BAR_CLASSES;
	BOOL bRet = ::InitCommonControlsEx(&iccx);
	bRet;
	ATLASSERT(bRet);

	//init wtl module
	HRESULT hRes = _Module.Init(NULL, hInstance);
	hRes;
	ATLASSERT(SUCCEEDED(hRes));

	//prepare mesage loop
	CGLMessageLoop theLoop;
	_Module.AddMessageLoop(&theLoop);

	//crate main frame
	Phylix::CMainFrame wndMain;
	if(wndMain.CreateEx() == NULL)
	{
		ATLTRACE(_T("Main window creation failed!\n"));
		return -1;
	}
	wndMain.ShowWindow(nCmdShow);

	//enter message loop
	int nRet = theLoop.Run();

	//end!
	_Module.RemoveMessageLoop();
	_Module.Term();
	return nRet;
}

