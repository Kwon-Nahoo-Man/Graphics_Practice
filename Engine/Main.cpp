#include "Core/Win32Window.h"
#include "Graphics/GraphicsContext.h"

// 콜백 함수
// 창 메세지 처리할 때 사용
LRESULT CALLBACK WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam);

// winMain은 창 프로그램의 진입점이다.
// 콘솔 진입점 main은 콘솔창을 생성한다. 여기에서 window 창은 부가적으로 만든 것.
// 결국 콘솔창과 window창 2개가 생성되는 것
// 디버그 시 콘솔창도 같이 뜨는 것이 편함(콘솔 출력으로 디버그 시 체크하기 좋기 때문)
// winMain으로 하면 콘솔창은 안뜨고 window창만 뜸
// 그래서 디버그 시에는 main으로하고 release에서는 winMain으로 하게 한 번 감쌀 수 있음 --> 현업에서 많이 사용
int main()
{
	// 그래픽스 프로그래밍
	// 창(window)를 생성하고 API를 활용해서 창에 그림을 그리는 프로그램
	// Win32 API 활용

    // 운영체제에 창 등록할 때 사용할 클래스 이름 - 유일해야함
    std::wstring className = L"Craft Engine Class name";
    std::wstring title = L"Craft Engine";

    // 창크기 지정
    uint32_t width = 1280;
    uint32_t height = 800;

    HINSTANCE hInstance = GetModuleHandleA(nullptr);

    // 창 생성
    Craft::Win32Window window(width, height, hInstance, WindowProc);

    // 초기화(초기화 실패 시 프로그램 종료)
    if (window.Initialize() == false)
    {
        return -1;
    }

    Craft::GraphicsContext context;
    context.Initialize(width, height, window);

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
            // 엔진 로직
        }
    }

}

LRESULT CALLBACK WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
    switch (uMsg)
    {
    case WM_DESTROY:
        PostQuitMessage(0);
        return 0;

    case WM_PAINT:
    {
        PAINTSTRUCT ps;
        HDC hdc = BeginPaint(hwnd, &ps);

        // All painting occurs here, between BeginPaint and EndPaint.

        FillRect(hdc, &ps.rcPaint, (HBRUSH)(COLOR_WINDOW + 1));

        EndPaint(hwnd, &ps);
    }
    return 0;

    }
    return DefWindowProc(hwnd, uMsg, wParam, lParam);
}