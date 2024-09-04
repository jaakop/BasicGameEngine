#include "../core/include/IApplication.h"

/// <summary>
/// Windows main -> APIENTRY due to windows library being in c and it will be called from there (?)
/// </summary>
/// <param name="inst">Handle to instance</param>
/// <param name="prevInst">Handle to previous instance: The app that launched this app</param>
/// <param name="cmdLine">Long pointer string -> Command line params</param>
/// <param name="cmdShow">Deprecated window option</param>
/// <returns></returns>
int APIENTRY WinMain(HINSTANCE inst, HINSTANCE prevInst, LPSTR cmdLine, int cmdShow) 
{
	auto app = std::make_unique<IApplication>();

	if (!app->Create(1280, 720, "SuperGame")) 
	{
		IApplication::Debug("App start failed!");
		return 0;
	}

	app->Run();

	return 0;
}