#pragma once
#include "Globals.h"
#include "../FBX Exporter/FBX Exporter.h"

using namespace DirectX;
struct Plane
{
	Plane();
	~Plane();
	void Create(ID3D11Device &dev, XMFLOAT4X4 &camera, ID3D11DeviceContext &Con);
	void CreateDeviceDependentResources(void);
	void CreateWindowSizeDependentResources(void);
	void LoadFBXFromFile(bool overwrite = true);
	void ReleaseDeviceDependentResources(void);
	void Render();
	MVP	m_constantBufferData;

private:
	ID3D11Device				*m_dev;
	ID3D11DeviceContext			*m_devCon;
	ID3D11InputLayout			*m_inputLayout;
	ID3D11Buffer				*m_vertexBuffer;
	ID3D11VertexShader			*m_vertexShader;
	ID3D11PixelShader			*m_pixelShader;
	ID3D11Buffer				*m_constantBuffer;

	XMFLOAT4X4* m_camera;


	std::vector<XMFLOAT3> positions;
	std::vector<unsigned int> indexes;
};