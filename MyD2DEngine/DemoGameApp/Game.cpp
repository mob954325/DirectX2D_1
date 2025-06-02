#include <windows.h>
#include "DemoGameApp.h"

int APIENTRY wWinMain(_In_ HINSTANCE hInstance,
    _In_opt_ HINSTANCE hPrevInstance,
    _In_ LPWSTR    lpCmdLine,
    _In_ int       nCmdShow)
{
    DemoGameApp demoGameApp;

    demoGameApp.Initialize();
    demoGameApp.Run();
    demoGameApp.Uninitialize();

    return (int)1;
}