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

// структуры
struct ConstantBuffer {
	XMMATRIX mWorld;
	XMMATRIX mView;
	XMMATRIX mProjection;
} cb;

ID3D11Device* g_pd3dDevice(NULL);
ID3D11DeviceContext* g_pImmediateContext(NULL);
IDXGISwapChain* g_pSwapChain(NULL);
ID3D11RenderTargetView* g_pRenderTargetView(NULL);
ID3D11VertexShader* g_pVertexShader(NULL);
ID3D11PixelShader* g_pPixelShader(NULL);
ID3D11InputLayout* g_pVertexLayout(NULL);
ID3D11Buffer* g_pVertexBuffer(NULL);
ID3D11DepthStencilView* g_pDepthStencilView(NULL);
ID3D11Texture2D* g_pDepthStencil(NULL); // текстура
ID3D11Buffer* g_pConstantBuffer(NULL);
ID3D11RasterizerState* g_pRasterState(NULL);
ID3D11ShaderResourceView* g_pTextureRV = NULL; 	// Объект текстуры
ID3D11SamplerState* g_pSamplerLinear = NULL; 	/* Параметры наложения текстуры */



const int width(640);
const int height(480);
int n = 0;	// кол-во вершин
float t = 0.0f;	// угол поворота


// структуры для вспомогательных матриц 4х4 мировго преобразования
XMMATRIX mxWorld, myWorld, mzWorld, mWorld1, mWorld;

// функция для расчета текущих значений матриц преобразований
void SetMatrixes()
{
	// заполнение вспомогательной матрицы поворота вокруг оси X
	mxWorld = XMMatrixScaling(0.5f + sin(t)/8, 0.5f + sin(t)/8, 0.5f + sin(t)/8);
	// заполнение вспомогательной матрицы поворота вокруг оси Y
	myWorld = XMMatrixRotationY(t/4);
	/* присвоение полю константного буфера, которое соответствуют мировой матрице, произведения вспомогательных матриц поворотов вокруг осей Х и У */
	cb.mWorld = XMMatrixMultiply(myWorld, mxWorld);
	/* присвоение полю константного буфера, которое соответствует видовой матрице, транспонированной матрицы вида */
	cb.mView = XMMatrixLookAtLH(
		XMVectorSet(0.0f, 0.0f, -15.0f, 0.0f), XMVectorSet(0.0f, 0.0f, 0.0f, 0.0f),
		XMVectorSet(0.0f, 1.0f, 0.0f, 0.0f));
	/* присвоение полю константного буфера, которое соответствуют проекционной матрице, транспонированной матрицы проекции  */
	cb.mProjection = 
		XMMatrixPerspectiveFovLH(XM_PIDIV4, 640 / 480, 0.01f, 100.0f);
}



void InitBuffer() {
	ID3DBlob* pVSBlob = NULL;
	D3DX11CompileFromFile("Sh.fx", NULL, NULL, "vs_main", "vs_4_0", NULL, 0, NULL, &pVSBlob, NULL, NULL);
	g_pd3dDevice->CreateVertexShader(pVSBlob->GetBufferPointer(), pVSBlob->GetBufferSize(), NULL, &g_pVertexShader);
	D3D11_INPUT_ELEMENT_DESC layout[] = {
		{ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		// { "COLOR", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, 12, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "TEX", 0, DXGI_FORMAT_R32G32_FLOAT, 0, 12, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "NORMAL", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 20, D3D11_INPUT_PER_VERTEX_DATA, 0 },
	};
	g_pd3dDevice->CreateInputLayout(layout, 3, pVSBlob->GetBufferPointer(), pVSBlob->GetBufferSize(), &g_pVertexLayout);
	pVSBlob->Release();
	g_pImmediateContext->IASetInputLayout(g_pVertexLayout);

	ID3DBlob* pPSBlob(NULL);
	D3DX11CompileFromFile("Sh.fx", NULL, NULL, "ps_main", "ps_4_0", NULL, 0, NULL, &pPSBlob, NULL, NULL);
	g_pd3dDevice->CreatePixelShader(pPSBlob->GetBufferPointer(), pPSBlob->GetBufferSize(), NULL, &g_pPixelShader);
	pPSBlob->Release();

	MeshLoader loader("name.stl");
	loader.load(0.4f, .15, 0);
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

	bd.Usage = D3D11_USAGE_DEFAULT;
	bd.ByteWidth = sizeof(ConstantBuffer);
	bd.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	g_pd3dDevice->CreateBuffer(&bd, NULL, &g_pConstantBuffer);

	// загрузка текстуры из файла text1.JPG
	D3DX11CreateShaderResourceViewFromFile(g_pd3dDevice, "pic1.jpg",
		NULL, NULL, &g_pTextureRV, NULL);
	// Создание структуры для описания параметров наложения текстуры
	D3D11_SAMPLER_DESC sampDesc;
	ZeroMemory(&sampDesc, sizeof(sampDesc));
	sampDesc.Filter = D3D11_FILTER_MIN_MAG_MIP_LINEAR;	// Тип фильтрации
	g_pd3dDevice->CreateSamplerState(&sampDesc, &g_pSamplerLinear);

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

	D3D11_VIEWPORT vp;
	vp.Width = width;
	vp.Height = height;
	vp.MinDepth = 0.0f;
	vp.MaxDepth = 1.0f;
	vp.TopLeftX = 0;
	vp.TopLeftY = 0;

	D3D11CreateDeviceAndSwapChain(NULL, D3D_DRIVER_TYPE_HARDWARE, NULL, 0, featureLevels, 1, D3D11_SDK_VERSION, &sd, &g_pSwapChain, &g_pd3dDevice, NULL, &g_pImmediateContext);

	ID3D11Texture2D* pBackBuffer = NULL;
	g_pSwapChain->GetBuffer(0, __uuidof(ID3D11Texture2D), (void**)&pBackBuffer);
	g_pd3dDevice->CreateRenderTargetView(pBackBuffer, NULL, &g_pRenderTargetView);

	D3D11_RASTERIZER_DESC desc;
	ZeroMemory(&desc, sizeof(D3D11_RASTERIZER_DESC));
	desc.FillMode = D3D11_FILL_SOLID;
	desc.CullMode = D3D11_CULL_BACK;
	g_pd3dDevice->CreateRasterizerState(&desc, &g_pRasterState);

	// Переходим к созданию буфера глубин
 // Создаем текстуру-описание буфера глубин
	D3D11_TEXTURE2D_DESC descDepth;     // Структура с параметрами
	ZeroMemory(&descDepth, sizeof(descDepth));
	descDepth.Width = width;            // ширина и
	descDepth.Height = height;    // высота текстуры
	descDepth.MipLevels = 1;            // уровень интерполяции
	descDepth.ArraySize = 1;
	descDepth.Format = DXGI_FORMAT_D24_UNORM_S8_UINT; // формат (размер пикселя)
	descDepth.SampleDesc.Count = 1;
	descDepth.SampleDesc.Quality = 0;
	descDepth.Usage = D3D11_USAGE_DEFAULT;
	descDepth.BindFlags = D3D11_BIND_DEPTH_STENCIL;         // вид - буфер глубин
	descDepth.CPUAccessFlags = 0;
	descDepth.MiscFlags = 0;
	// При помощи заполненной структуры-описания создаем объект текстуры
	g_pd3dDevice->CreateTexture2D(&descDepth, NULL, &g_pDepthStencil);

	// Теперь надо создать сам объект буфера глубин
	D3D11_DEPTH_STENCIL_VIEW_DESC descDSV;            // Структура с параметрами
	ZeroMemory(&descDSV, sizeof(descDSV));
	descDSV.Format = descDepth.Format;         // формат как в текстуре
	descDSV.ViewDimension = D3D11_DSV_DIMENSION_TEXTURE2D;
	descDSV.Texture2D.MipSlice = 0;
	// При помощи заполненной структуры-описания и текстуры создаем объект буфера глубин
	g_pd3dDevice->CreateDepthStencilView(g_pDepthStencil, &descDSV, &g_pDepthStencilView);


	g_pImmediateContext->OMSetRenderTargets(1,
		&g_pRenderTargetView, g_pDepthStencilView);
	// Подключаем вьюпорт к контексту устройства
	g_pImmediateContext->RSSetViewports(1, &vp);
}

void Render() {
	float ClearColor[4] = { 1, 1, 0.7, 1 };
	g_pImmediateContext->ClearRenderTargetView(g_pRenderTargetView, ClearColor);
	g_pImmediateContext->ClearDepthStencilView(
		g_pDepthStencilView, D3D11_CLEAR_DEPTH, 1.0f, 0);
	SetMatrixes();
	g_pImmediateContext->UpdateSubresource(g_pConstantBuffer, 0, NULL, &cb, 0, 0);
	//передача текстуры в пиксельный шейдер
	g_pImmediateContext->PSSetShaderResources(0, 1, &g_pTextureRV);
	//передача сэмплера в пиксельный шейдер
	g_pImmediateContext->PSSetSamplers(0, 1, &g_pSamplerLinear);
	g_pImmediateContext->VSSetConstantBuffers(0, 1, &g_pConstantBuffer);
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
		t += (float)XM_PI * 0.000125f;
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
		else {
			Render();
		}
	}
	return msg.wParam;
}

