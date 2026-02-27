#pragma once
#include <vector>
#include <d3d11.h>

namespace Craft
{
	// 렌더 명령(Render Command)
	struct RenderCommand
	{
		// 정점(vertex) 버퍼(buffer - 메모리 덩어리)
		ID3D11Buffer* vertexBuffer = nullptr;
		// 색인 버퍼(index buffer) --> 옵션, 상용 엔진은 다 쓴다
		ID3D11Buffer* indexBuffer = nullptr;
		// 입력(정점 데이터)의 생김새
		ID3D11InputLayout* inputLayout = nullptr;

		// 필수 셰이더(VS/PS)
		ID3D11VertexShader* vertexShader = nullptr;
		ID3D11PixelShader* pixelShader = nullptr;

		// 인덱스 개수(index Count)
		// VertexBuffer와 연결된 색인을 어디까지 그릴지 지정
		uint32_t indexCount{};


	};


	// DrawCall 담당
	// RHI - Render Hardward Interface: 그래픽카드를 말하는 것
	class Renderer
	{
	public:
		Renderer();
		~Renderer();

		// 초기화
		void Initialize();

		// DrawCall 처리
		// -> 렌더링 파이프라인 실행(구동)
		void DrawScene();


	private:
		// 렌더 큐(queue)
		std::vector<RenderCommand> renderQueue;
	};
}




