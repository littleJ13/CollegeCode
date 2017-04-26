#pragma once
#include "Globals.h"
#include "FBXRenderer.h"
#include "Plane.h"
#include "Camera.h"
#include "SkyBoxRenderer.h"
#include <d3d11.h>
#pragma comment (lib, "d3d11.lib")

LRESULT CALLBACK WindowProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);
bool WindowsSetup(HWND& WindowHandle, HINSTANCE hInstance, int nShowCmd);
bool Init(HWND &hWnd);
bool InitGraphics(HWND& hWnd);
void CleanD3D();

D3D11_VIEWPORT ViewPort;
IDXGISwapChain *SwapChain;
ID3D11Device *Device;
ID3D11DeviceContext *DeviceContext;
ID3D11RenderTargetView* BackBuffer;
ID3D11DepthStencilView* DSV;
Camera camera;

FBXRenderer box;
FBXRenderer bear;
FBXRenderer wizard;

Plane plane;
SkyBoxRenderer SkyBox;

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nShowCmd)
{
	HWND WindowHandle;
	WindowsSetup(WindowHandle, hInstance, nShowCmd);

	Init(WindowHandle);

	/////////////////////////////////////////////////////////////////////////////////////////
	//Filling the Sampler description
	D3D11_SAMPLER_DESC samplerDesc;
	samplerDesc.Filter = D3D11_FILTER_MIN_MAG_LINEAR_MIP_POINT;
	samplerDesc.AddressU = D3D11_TEXTURE_ADDRESS_CLAMP;
	samplerDesc.AddressV = D3D11_TEXTURE_ADDRESS_CLAMP;
	samplerDesc.AddressW = D3D11_TEXTURE_ADDRESS_CLAMP;
	samplerDesc.MaxAnisotropy = 1;
	samplerDesc.MipLODBias = 1.0f;
	ID3D11SamplerState *samplerS;

	Device->CreateSamplerState(&samplerDesc, &samplerS);
	DeviceContext->PSSetSamplers(0, 1, &samplerS);

	MSG msg = { 0 };
	while (TRUE)
	{
		DeviceContext->OMSetRenderTargets(1, &BackBuffer, DSV);
		DeviceContext->RSSetViewports(1, &ViewPort);
		DeviceContext->ClearRenderTargetView(BackBuffer, DirectX::Colors::Peru);
		DeviceContext->ClearDepthStencilView(DSV, D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1.0f, 0);

		if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
		{
			TranslateMessage(&msg);
			DispatchMessage(&msg);
			if (msg.message == WM_QUIT)
				break;
		}

		camera.UpdateCamera(25.0f, 10.0f);
		//Black Magic to make camera smooth, do not touch
		if (GetAsyncKeyState(VK_RBUTTON))
		{
			SetCursorPos(width / 2, height / 2);
			camera.right = true;
		}
		else
		{
			camera.right = false;
		}

		SkyBox.Render();
		box.Update();
		bear.Update();
		wizard.Update();

		box.Render();
		bear.Render();
		wizard.Render();
		plane.Render();
		SwapChain->Present(0, 0);
	}
	CleanD3D();
}

LRESULT CALLBACK WindowProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	switch (message)
	{
	case WM_SIZE:
	{
		if (SwapChain)
		{
			ID3D11RenderTargetView* nullViews[] = { nullptr };
			DeviceContext->OMSetRenderTargets(ARRAYSIZE(nullViews), nullViews, nullptr);
			DeviceContext->Flush();

			// Release all outstanding references to the swap chain's buffers.
			BackBuffer->Release();

			HRESULT hr;
			// Preserve the existing buffer count and format.
			// Automatically choose the width and height to match the client rect for HWNDs.
			hr = SwapChain->ResizeBuffers(0, 0, 0, DXGI_FORMAT_UNKNOWN, 0);

			// Perform error handling here!

			// Get buffer and create a render-target-view.
			ID3D11Texture2D* pBuffer;
			hr = SwapChain->GetBuffer(0, __uuidof(ID3D11Texture2D),
				(void**)&pBuffer);
			// Perform error handling here!

			hr = Device->CreateRenderTargetView(pBuffer, NULL,
				&BackBuffer);
			// Perform error handling here!
			pBuffer->Release();

			
			//DeviceContext->OMSetRenderTargets(1, &BackBuffer, NULL);

			// Set up the viewport.
			D3D11_VIEWPORT vp;
			vp.TopLeftX = 0;
			vp.TopLeftY = 0;
			vp.Width = width;
			vp.Height = height;
			vp.MinDepth = 0.0f;
			vp.MaxDepth = 1.0f;
		}
		return 1;
	}
	case WM_DESTROY:
	{
		PostQuitMessage(0);
		return 0;
	} break;
	}

	return DefWindowProc(hWnd, message, wParam, lParam);
}

bool WindowsSetup(HWND& WindowHandle, HINSTANCE hInstance, int nShowCmd)
{

	WNDCLASSEX WindowsClassDesc;
	ZeroMemory(&WindowsClassDesc, sizeof(WNDCLASSEX));
	WindowsClassDesc.cbSize = sizeof(WNDCLASSEX);
	WindowsClassDesc.style = CS_HREDRAW | CS_VREDRAW;
	WindowsClassDesc.lpfnWndProc = WindowProc;
	WindowsClassDesc.hInstance = hInstance;
	WindowsClassDesc.hCursor = LoadCursor(NULL, IDC_ARROW);
	WindowsClassDesc.hbrBackground = (HBRUSH)COLOR_WINDOW;
	WindowsClassDesc.lpszClassName = L"WindowClass1";
	RegisterClassEx(&WindowsClassDesc);

	RECT wr = { 0, 0, width, height };
	AdjustWindowRect(&wr, WS_OVERLAPPEDWINDOW, FALSE);
	WindowHandle = CreateWindowEx(NULL, L"WindowClass1", L"Milestone 3 Baby", WS_OVERLAPPEDWINDOW, 50, 50, wr.right - wr.left, wr.bottom - wr.top, NULL, NULL, hInstance, NULL);
	ShowWindow(WindowHandle, nShowCmd);
	return true;
}

bool Init(HWND& hWnd)
{
	InitGraphics(hWnd);

	//Initialize Camera
	camera.Create(XMFLOAT4X4(
		-1.0f, 0, 0, 0,
		0, -1.0f, 0, 0,
		0, 0, 1.0f, 0,
		0, 0, 20.0f, 1.0f
	));

	//Initialize fbx objects
	box.Create(*Device, camera, *DeviceContext);
	box.LoadFBXFromFile(
		"../Original Assets/AnimatedBox/Box_Idle.fbx",
		"../Exports/Box_Idle.bin",
		L"../Original Assets/AnimatedBox/Box_Idle.fbm/TestCube.dds"
	);

	box.LoadAdditionalAnimations(
		"../Original Assets/AnimatedBox/Box_Attack.fbx",
		"../Exports/Box_Attack.bin"
	);
	box.LoadAdditionalAnimations(
		"../Original Assets/AnimatedBox/Box_Jump.fbx",
		"../Exports/Box_Jump.bin"
	);
	box.LoadAdditionalAnimations(
		"../Original Assets/AnimatedBox/Box_Walk.fbx",
		"../Exports/Box_Walk.bin"
	);

	bear.Create(*Device, camera, *DeviceContext);
	bear.LoadFBXFromFile(
		"../Original Assets/Teddy/Teddy_Idle.fbx",
		"../Exports/Teddy_Idle.bin",
		L"../Original Assets/Teddy/Teddy_Idle.fbm/Teddy_D.dds"
	);

	bear.LoadAdditionalAnimations(
		"../Original Assets/Teddy/Teddy_Attack1.fbx",
		"../Exports/Teddy_Attack1.bin"
	);
	bear.LoadAdditionalAnimations(
		"../Original Assets/Teddy/Teddy_Attack2.fbx",
		"../Exports/Teddy_Attack2.bin"
	);
	bear.LoadAdditionalAnimations(
		"../Original Assets/Teddy/Teddy_Run.fbx",
		"../Exports/Teddy_Run.bin"
	);

	wizard.Create(*Device, camera, *DeviceContext);
	wizard.LoadFBXFromFile(
		"../Original Assets/Mage/Battle Mage with Rig and textures.fbx",
		"../Exports/Battle Mage with Rig and textures.bin",
		L"../Original Assets/Mage/Battle Mage with Rig and textures.fbm/PPG_3D_Player_D.dds"
	);
	wizard.LoadAdditionalAnimations(
		"../Original Assets/Mage/Idle.fbx",
		"../Exports/Idle.bin"
	);
	wizard.LoadAdditionalAnimations(
		"../Original Assets/Mage/Death.fbx",
		"../Exports/Death.bin"
	);
	wizard.LoadAdditionalAnimations(
		"../Original Assets/Mage/Run.fbx",
		"../Exports/Run.bin"
	);
	wizard.LoadAdditionalAnimations(
		"../Original Assets/Mage/Walk.fbx",
		"../Exports/Walk.bin"
	);

	plane.Create(*Device, camera.m_camera, *DeviceContext);
	plane.LoadFBXFromFile(true);
	SkyBox.Create(*Device, camera.m_camera, *DeviceContext, *DSV);
	SkyBox.LoadTextureFromFile();
	return true;
}

bool InitGraphics(HWND& hWnd)
{
	DXGI_SWAP_CHAIN_DESC SwapChainDesc;
	ZeroMemory(&SwapChainDesc, sizeof(DXGI_SWAP_CHAIN_DESC));
	SwapChainDesc.BufferCount = 1;
	SwapChainDesc.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
	SwapChainDesc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
	SwapChainDesc.OutputWindow = hWnd;
	SwapChainDesc.SampleDesc.Count = 1;
	SwapChainDesc.Windowed = TRUE;
	D3D11CreateDeviceAndSwapChain(NULL, D3D_DRIVER_TYPE_HARDWARE, NULL, D3D11_CREATE_DEVICE_DEBUG, NULL, NULL, D3D11_SDK_VERSION, &SwapChainDesc, &SwapChain, &Device, NULL, &DeviceContext);

	ID3D11Texture2D *BackBufferTexture;
	SwapChain->GetBuffer(0, __uuidof(ID3D11Texture2D), (LPVOID*)&BackBufferTexture);
	Device->CreateRenderTargetView(BackBufferTexture, NULL, &BackBuffer);

	// Create a depth stencil view for use with 3D rendering if needed.
	ID3D11Texture2D* DepthStencil = NULL;
	D3D11_TEXTURE2D_DESC descDepth;
	ZeroMemory(&descDepth, sizeof(D3D11_TEXTURE2D_DESC));
	descDepth.Width = width;
	descDepth.Height = height;
	descDepth.MipLevels = 1;
	descDepth.ArraySize = 1;
	descDepth.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
	descDepth.SampleDesc.Count = 1;
	descDepth.SampleDesc.Quality = 0;
	descDepth.Usage = D3D11_USAGE_DEFAULT;
	descDepth.BindFlags = D3D11_BIND_DEPTH_STENCIL;
	descDepth.CPUAccessFlags = 0;
	descDepth.MiscFlags = 0;
	HRESULT hr = Device->CreateTexture2D(&descDepth, NULL, &DepthStencil);

	D3D11_DEPTH_STENCIL_DESC dsDesc;
	ZeroMemory(&dsDesc, sizeof(D3D11_DEPTH_STENCIL_DESC));

	// Depth test parameters
	dsDesc.DepthEnable = true;
	dsDesc.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ALL;
	dsDesc.DepthFunc = D3D11_COMPARISON_LESS;

	// Stencil test parameters
	dsDesc.StencilEnable = true;
	dsDesc.StencilReadMask = 0xFF;
	dsDesc.StencilWriteMask = 0xFF;

	// Stencil operations if pixel is front-facing
	dsDesc.FrontFace.StencilFailOp = D3D11_STENCIL_OP_KEEP;
	dsDesc.FrontFace.StencilDepthFailOp = D3D11_STENCIL_OP_INCR;
	dsDesc.FrontFace.StencilPassOp = D3D11_STENCIL_OP_KEEP;
	dsDesc.FrontFace.StencilFunc = D3D11_COMPARISON_ALWAYS;

	// Stencil operations if pixel is back-facing
	dsDesc.BackFace.StencilFailOp = D3D11_STENCIL_OP_KEEP;
	dsDesc.BackFace.StencilDepthFailOp = D3D11_STENCIL_OP_DECR;
	dsDesc.BackFace.StencilPassOp = D3D11_STENCIL_OP_KEEP;
	dsDesc.BackFace.StencilFunc = D3D11_COMPARISON_ALWAYS;

	// Create depth stencil state
	ID3D11DepthStencilState * DSState;
	Device->CreateDepthStencilState(&dsDesc, &DSState);


	D3D11_DEPTH_STENCIL_VIEW_DESC descDSV;
	ZeroMemory(&descDSV, sizeof(D3D11_DEPTH_STENCIL_VIEW_DESC));
	descDSV.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
	descDSV.ViewDimension = D3D11_DSV_DIMENSION_TEXTURE2D;
	descDSV.Texture2D.MipSlice = 0;

	// Create the depth stencil view

	hr = Device->CreateDepthStencilView(DepthStencil, // Depth stencil texture
		&descDSV, // Depth stencil desc
		&DSV);  // [out] Depth stencil view

				// Bind the depth stencil view
	DeviceContext->OMSetDepthStencilState(DSState, 1);

	ZeroMemory(&ViewPort, sizeof(ViewPort));
	ViewPort.TopLeftX = 0;
	ViewPort.TopLeftY = 0;
	ViewPort.Width = width;
	ViewPort.Height = height; 
	ViewPort.MinDepth = 0.0f;
	ViewPort.MaxDepth = 1.0f;
	DeviceContext->RSSetViewports(1, &ViewPort);

	return true;
}

void CleanD3D()
{
	box.ReleaseDeviceDependentResources();
	bear.ReleaseDeviceDependentResources();
	wizard.ReleaseDeviceDependentResources();
	plane.ReleaseDeviceDependentResources();
	SwapChain->Release();
	BackBuffer->Release();
	Device->Release();
	DeviceContext->Release();
	DSV->Release();
}