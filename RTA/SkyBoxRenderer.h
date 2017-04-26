#pragma once
#include "Globals.h"
#include "../FBX Exporter/FBX Exporter.h"

using namespace DirectX;

struct SkyBoxRenderer
{
	SkyBoxRenderer();
	~SkyBoxRenderer();
	void Create(ID3D11Device &dev, XMFLOAT4X4 &camera, ID3D11DeviceContext &Con, ID3D11DepthStencilView &dsv);
	void CreateDeviceDependentResources(void);
	void CreateWindowSizeDependentResources(void);
	void LoadTextureFromFile();
	void ReleaseDeviceDependentResources(void);
	void Render();
	MVP	m_constantBufferData;

private:
	ID3D11Device				*m_dev;
	ID3D11DeviceContext			*m_devCon;
	ID3D11Texture2D				*m_texture2D;
	ID3D11ShaderResourceView	*m_texView;
	ID3D11InputLayout			*m_inputLayout;
	ID3D11Buffer				*m_vertexBuffer;
	ID3D11Buffer				*m_indexBuffer;
	ID3D11VertexShader			*m_vertexShader;
	ID3D11PixelShader			*m_pixelShader;
	ID3D11Buffer				*m_constantBuffer;
	ID3D11DepthStencilView      *DSV;

	XMFLOAT4X4* m_camera;
	int	m_indexCount;
};




