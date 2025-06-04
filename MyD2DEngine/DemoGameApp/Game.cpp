#include <windows.h>
#include "DemoGameApp.h"

int APIENTRY wWinMain(_In_ HINSTANCE hInstance,
    _In_opt_ HINSTANCE hPrevInstance,
    _In_ LPWSTR    lpCmdLine,
    _In_ int       nCmdShow)
{
    CoInitialize(nullptr);  // COM 초기화

    DemoGameApp demoGameApp;

    demoGameApp.Initialize();
    demoGameApp.Run();
    demoGameApp.Uninitialize();

    CoUninitialize();        // COM 해제
    return (int)1;
}