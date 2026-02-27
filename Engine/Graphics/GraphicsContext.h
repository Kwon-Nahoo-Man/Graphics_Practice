#pragma once

#include <d3d11.h>	// Direct3D 11 일반적인 내용
#include <dxgi.h>	// DXGI 장치 관련(스왑체인)
#include <string>


// 장치(그래픽카드)
// Direct3D에서 "장치"로 다루는 객체는 3개
// Device: 리소스 생성
// DeviceContext: 생성된 리소스를 그래픽카드에 할당(바인딩), Draw call을 함
// SwapChain(스왑체인): 백버퍼 - 프론트버퍼 교환

namespace Craft
{
	// 전방선언
	class Win32Window;

	// 그래픽스 관련 기능을 제공 및 리소스 관리
	class GraphicsContext
	{
	public:
		GraphicsContext();
		~GraphicsContext();

		void Initialize(const Win32Window& window);

		// 프레임 준비 함수
		void BeginScene(float red, float green, float blue);

		// 나중에 REnderer에서 DrawScene(또는 DrawFrame) 함수가 호출될 예정

		// 완성된 프레임을 모니터에 보내는(체인 변경) 함수
		// vsync가 0이면 수직동기화 안함. 1이면 모니터 수직 동기화 사용
		void EndScene(uint32_t vsnyc);

		// Getter
		inline ID3D11Device& GetDevice() const { return *device; }
		inline ID3D11DeviceContext& GetDeviceContext() const { return *context; }
		static GraphicsContext& Get();

	private:
		void CreateDevice();
		void CreateSwapChain(const Win32Window& window);
		void CreateViewport(const Win32Window& window);

		// 렌더 타겟 뷰 생성
		void CreateRenderTargetView();

	private:
		// 장치류(디바이스, 디바이스 컨텍스트, 스왑체인)
		// Direct3D 리소스는 포인터로만 다룰 수 있음
		// 생성, 해제를 API에 요청해야함
		ID3D11Device* device = nullptr;
		ID3D11DeviceContext* context = nullptr;
		IDXGISwapChain* swapChain = nullptr;

		// 리소스
		// RTV - 그리기 대상 리소스
		ID3D11RenderTargetView* renderTargetView{};

		// 뷰포트
		D3D11_VIEWPORT viewport{};

		// 화면 크기
		uint32_t width{};
		uint32_t height{};

		// 전역변수
		static GraphicsContext* instance;

	};
}


