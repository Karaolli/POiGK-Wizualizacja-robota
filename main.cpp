#include "App.h"

#pragma comment(lib, "d3d11.lib")
#pragma comment(lib, "D3DCompiler.lib")

#pragma warning(disable : 28251)


int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE, LPSTR, int nCmdShow) {
    App app;
    return app.Run();
}