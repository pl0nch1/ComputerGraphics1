#include <d3d11.h>
#include <d3dx11.h>
#include <d3dcompiler.h>
#include <xnamath.h>
#pragma comment(lib, "d3d11.lib")
#pragma comment(lib, "d3dx11.lib")
#include <string>
#include "geometryData.h"
#include "MeshLoader.h"

HINSTANCE hinst(NULL);
HWND hwnd(NULL);

D3D_FEATURE_LEVEL featureLevels[]
= { D3D_FEATURE_LEVEL_11_0 };


ID3D11Device* g_pd3dDevice(NULL);
ID3D11DeviceContext* g_pImmediateContext(NULL);
IDXGISwapChain* g_pSwapChain(NULL);
ID3D11RenderTargetView* g_pRenderTargetView(NULL);
ID3D11VertexShader* g_pVertexShader(NULL);
ID3D11PixelShader* g_pPixelShader(NULL);
ID3D11InputLayout* g_pVertexLayout(NULL);
ID3D11Buffer* g_pVertexBuffer(NULL);

const int width(640);
const int height(480);
int n = 0;	// кол-во вершин


void InitBuffer() {
	ID3DBlob* pVSBlob = NULL;
	D3DX11CompileFromFile("Sh.fx", NULL, NULL, "vs_main", "vs_4_0", NULL, 0, NULL, &pVSBlob, NULL, NULL);
	g_pd3dDevice->CreateVertexShader(pVSBlob->GetBufferPointer(), pVSBlob->GetBufferSize(), NULL, &g_pVertexShader);
	D3D11_INPUT_ELEMENT_DESC layout[] = {
		{ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "COLOR", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, 12, D3D11_INPUT_PER_VERTEX_DATA, 0 }
	};
	g_pd3dDevice->CreateInputLayout(layout, 2, pVSBlob->GetBufferPointer(), pVSBlob->GetBufferSize(), &g_pVertexLayout);
	pVSBlob->Release();
	g_pImmediateContext->IASetInputLayout(g_pVertexLayout);

	ID3DBlob* pPSBlob(NULL);
	D3DX11CompileFromFile("Sh.fx", NULL, NULL, "ps_main", "ps_4_0", NULL, 0, NULL, &pPSBlob, NULL, NULL);
	g_pd3dDevice->CreatePixelShader(pPSBlob->GetBufferPointer(), pPSBlob->GetBufferSize(), NULL, &g_pPixelShader);
	pPSBlob->Release();

	MeshLoader loader("C:/Users/pivor/source/repos/ComputerGraphics1/ComputerGraphics/FIO.stl");
	loader.load(3, -.15, 0);
	CustomVertex* vertices = new CustomVertex[loader.getTriangleCount() * 3];

	for (int i = 0; i < loader.getTriangleCount(); i++) {
		vertices[i*3] = loader.getTriangles()[i].a;
		vertices[i*3+1] = loader.getTriangles()[i].b;
		vertices[i*3+2] = loader.getTriangles()[i].c;
	}

	n = loader.getTriangleCount() * 3;
	D3D11_BUFFER_DESC bd;
	ZeroMemory(&bd, sizeof(bd));
	bd.ByteWidth = sizeof(CustomVertex) * n;
	bd.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	D3D11_SUBRESOURCE_DATA InitData;
	ZeroMemory(&InitData, sizeof(InitData));
	InitData.pSysMem = vertices;

	g_pd3dDevice->CreateBuffer(&bd, &InitData, &g_pVertexBuffer);
	UINT stride = sizeof(CustomVertex);
	UINT offset = 0;
	g_pImmediateContext->IASetVertexBuffers(0, 1, &g_pVertexBuffer, &stride, &offset);
	g_pImmediateContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
	delete[] vertices;
}

void InitDevice() {
	DXGI_SWAP_CHAIN_DESC sd;
	ZeroMemory(&sd, sizeof(sd));
	sd.BufferCount = 1;
	sd.BufferDesc.Width = width;
	sd.BufferDesc.Height = height;
	sd.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
	sd.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
	sd.OutputWindow = hwnd;
	sd.SampleDesc.Count = 1;
	sd.SampleDesc.Quality = 0;
	sd.Windowed = TRUE;

	D3D11CreateDeviceAndSwapChain(NULL, D3D_DRIVER_TYPE_HARDWARE, NULL, 0, featureLevels, 1, D3D11_SDK_VERSION, &sd, &g_pSwapChain, &g_pd3dDevice, NULL, &g_pImmediateContext);

	ID3D11Texture2D* pBackBuffer = NULL;
	g_pSwapChain->GetBuffer(0, __uuidof(ID3D11Texture2D), (void**)&pBackBuffer);
	g_pd3dDevice->CreateRenderTargetView(pBackBuffer, NULL, &g_pRenderTargetView);
	g_pImmediateContext->OMSetRenderTargets(1, &g_pRenderTargetView, NULL);

	D3D11_VIEWPORT vp;
	vp.Width = width;
	vp.Height = height;
	vp.MinDepth = 0.0f;
	vp.MaxDepth = 1.0f;
	vp.TopLeftX = 0;
	vp.TopLeftY = 0;

	g_pImmediateContext->RSSetViewports(1, &vp);
}

void Render() {
	float ClearColor[4] = { 0.835, 0.976, 1, 1 };
	g_pImmediateContext->ClearRenderTargetView(g_pRenderTargetView, ClearColor);
	g_pImmediateContext->VSSetShader(g_pVertexShader, NULL, 0);
	g_pImmediateContext->PSSetShader(g_pPixelShader, NULL, 0);
	g_pImmediateContext->Draw(n, 0);
	g_pSwapChain->Present(0, 0);
}

void CleanupDevice() {
	if (g_pImmediateContext)
		g_pImmediateContext->ClearState();
	if (g_pVertexBuffer)
		g_pVertexBuffer->Release();
	if (g_pVertexLayout)
		g_pVertexLayout->Release();
	if (g_pVertexShader)
		g_pVertexShader->Release();
	if (g_pPixelShader)
		g_pPixelShader->Release();
	if (g_pRenderTargetView)
		g_pRenderTargetView->Release();
	if (g_pSwapChain)
		g_pSwapChain->Release();
	if (g_pImmediateContext)
		g_pImmediateContext->Release();
	if (g_pd3dDevice)
		g_pd3dDevice->Release();
}

LONG WINAPI WndProc(HWND h, UINT m, WPARAM wp, LPARAM lp) {
	switch (m) {

	case WM_PAINT:
		Render();
		break;
	case WM_DESTROY:
		CleanupDevice();
		PostQuitMessage(0);
		break;
	default:
		return DefWindowProc(h, m, wp, lp);
	}
	return 0;
}

int WINAPI WinMain(HINSTANCE hinstance, HINSTANCE hprevinstance, LPSTR lpcmdline, int ncmdshow) {
	WNDCLASSEX wnd;
	MSG msg;
	wnd.cbSize = sizeof(WNDCLASSEX);
	wnd.style = CS_HREDRAW;;
	wnd.lpfnWndProc = WNDPROC(WndProc);
	wnd.cbClsExtra = 0;
	wnd.cbWndExtra = 0;
	wnd.hInstance = hinstance;
	wnd.hIcon = NULL;
	wnd.hCursor = LoadCursor(NULL, IDC_ARROW);
	wnd.hbrBackground = (HBRUSH)(WHITE_BRUSH);
	wnd.lpszMenuName = NULL;
	wnd.lpszClassName = "WINDOWSCLASS";
	wnd.hIconSm = LoadIcon(NULL, IDI_APPLICATION);

	if (!RegisterClassEx(&wnd)) return 0;
	hinst = hinstance;
	if (!(hwnd = CreateWindowEx(NULL, "WINDOWSCLASS", "Example", WS_OVERLAPPEDWINDOW, 10, 10, width, height, NULL, NULL, hinstance, NULL)))
		return 0;
	InitDevice();
	InitBuffer();
	ShowWindow(hwnd, ncmdshow);
	ZeroMemory(&msg, sizeof(msg));

	while (WM_QUIT != msg.message)
	{
		if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
		{
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}

	}
	return msg.wParam;
}
