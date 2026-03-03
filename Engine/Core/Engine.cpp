
#include "Engine.h"
#include "Core/Win32Window.h"
#include "Graphics/GraphicsContext.h"
#include "Graphics/Renderer.h"

namespace Craft
{
	Engine::Engine()
	{

	}

	Engine::~Engine()
	{

	}

	bool Engine::Initialize(HINSTANCE instance)
	{
		// 창 객체 생성
		window = std::make_unique<Win32Window>(
			setting.width,
			setting.height,
			setting.title,
			instance,
			Win32MessageProcedure
		);

		// 창 초기화
		if (!window->Initialize())
		{
			__debugbreak();
			return false;
		}

		// 컨텍스트 객체 생성
		graphicsContext = std::make_unique<GraphicsContext>();
		
		// 초기화
		graphicsContext->Initialize(*window);

		renderer = std::make_unique<Renderer>();
		renderer->Initialize();

		return false;
	}

	void Engine::Run()
	{
		// 메세지 처리 루프
		// 창에서 발생하는 메세지 처리 루프
		// GetMessage - 동기 방식(Blocking 방식)
		// PeekMessage - 비동기 방식(Non Blocking 방식)
		// 동기: 처리되기 전까지 다른 동작 안함
		// 비동기: 처리되지 않아도 넘어감
		MSG msg = { };

		// 창 종료 메세지가 발생할 때까지 실행
		while (msg.message != WM_QUIT)
		{
			// 창 메세지 처리
			// 비동기 방식으로 메세지가 들어왔는지 확인
			if (PeekMessage(&msg, nullptr, 0, 0, PM_REMOVE))
			{
				//메세지 해석, 메세지를 적절하게 변환
				TranslateMessage(&msg);

				// 변환한 메세지를 처리함수에 전달
				DispatchMessage(&msg);
			}

			// 시스템 메세지가 없는 경우(창 메세지가 없으면 엔진 로직 처리)
			else
			{
				// 프레임 처리
				graphicsContext->BeginScene(0.6f, 0.7f, 0.8f);

				// Draw call, Scene 그리기
				renderer->DrawScene();

				graphicsContext->EndScene(setting.vsync);

			}
		}
	}

	LRESULT Engine::Win32MessageProcedure(HWND handle, UINT message, WPARAM wparam, LPARAM lparam)
	{

		switch (message)
		{
		case WM_DESTROY:
			PostQuitMessage(0);
			return 0;

		case WM_PAINT:
		{
			PAINTSTRUCT ps;
			HDC hdc = BeginPaint(handle, &ps);

			// All painting occurs here, between BeginPaint and EndPaint.

			FillRect(hdc, &ps.rcPaint, (HBRUSH)(COLOR_WINDOW + 1));

			EndPaint(handle, &ps);
		}
		return 0;

		}
		return DefWindowProc(handle, message, wparam, lparam);
	}

}