#include "Win32Window.h"

namespace Craft
{

	Win32Window::Win32Window(uint32_t width, uint32_t height, const std::wstring& title, HINSTANCE instance, WNDPROC messageProcedure)
		: width(width),height(height),title(title), instance(instance), messageProcedure(messageProcedure)
	{
	}

	Win32Window::~Win32Window()
	{
		// 창 등록 해제
		UnregisterClass(className.c_str(), instance);
	}

	bool Win32Window::Initialize()
	{
        // 창 생성에 필요한 정보(구조체) 채우기
        WNDCLASS wc = { };
        wc.lpfnWndProc = messageProcedure;
        wc.hInstance = instance;
        wc.lpszClassName = className.c_str();

        // 클래스 등록 (성공/실패)
        if (!RegisterClass(&wc))
        {
            return false;
        }

        // 창 크기 구하기
        RECT rect{};
        rect.left = 0;
        rect.top = 0;
        rect.right = width;
        rect.bottom = height;
        AdjustWindowRect(&rect, WS_OVERLAPPEDWINDOW, FALSE);

        // 창 크기
        uint32_t windowWidth = rect.right - rect.left;
        uint32_t windowHeight = rect.bottom - rect.top;

        // 창 생성 위치 가운데로
        uint32_t positionX = (GetSystemMetrics(SM_CXSCREEN) - windowWidth) / 2;
        uint32_t positionY = (GetSystemMetrics(SM_CXSCREEN) - windowHeight) / 2;

        // 창 객체 생성
        handle = CreateWindow(
            className.c_str(),              // Window class
            title.c_str(),                  // Window text
            WS_OVERLAPPEDWINDOW,            // Window style

            // position and size
            positionX, positionY, windowWidth, windowHeight,

            NULL,       // Parent window    
            NULL,       // Menu
            instance,  // Instance handle
            NULL        // Additional application data
        );

        // 창 생성 실패 처리
        if (handle == nullptr)
        {
            return false;
        }

        //// 창의 클라이언트 영역
        //RECT rect;
        //GetClientRect(hwnd, &rect);

        // 창 보이기 설정
        ShowWindow(handle, SW_SHOW);

		return true;
	}

}