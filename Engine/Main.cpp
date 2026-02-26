
#include "Core/Engine.h"

using namespace Craft;

// 엔진 실행 함수
void LaunchEngineStartUp(HINSTANCE instance)
{
	Engine engine;
	engine.Initialize(instance);
	engine.Run();
}

int main()
{
    LaunchEngineStartUp(GetModuleHandle(nullptr));
}

//#ifdef _WIN64
//// 창모드 메인(WinMain)
//int WINAPI WinMain(
//    _In_ HINSTANCE hInstance,
//    _In_opt_ HINSTANCE hPrevInstance,
//    _In_ LPSTR lpCmdLine,
//    _In_ int nShowCmd
//)
//{
//    LaunchEngineStartUp(hInstance);
//
//    return 0;
//}
//#else
//int main()
//{
//    LaunchEngineStartUp(GetModuleHandle(nullptr));
//}
//#endif