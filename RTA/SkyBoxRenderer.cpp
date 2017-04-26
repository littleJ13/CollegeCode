#include "SkyBoxRenderer.h"

SkyBoxRenderer::SkyBoxRenderer()
{

}

SkyBoxRenderer::~SkyBoxRenderer()
{

}

void SkyBoxRenderer::Create(ID3D11Device &dev, XMFLOAT4X4 &camera, ID3D11DeviceContext &Con, ID3D11DepthStencilView &dsv)
{
	m_dev = &dev;
	m_camera = &camera;
	m_devCon = &Con;
	DSV = &dsv;
}

void SkyBoxRenderer::LoadTextureFromFile()
{
	CreateDDSTextureFromFile(m_dev,
		L"../Original Assets/SkyboxOcean.dds",
		(ID3D11Resource**)&m_texture2D,
		&m_texView);

	XMStoreFloat4x4(m_constantBufferData.World, XMMatrixIdentity());
	CreateDeviceDependentResources();
}

void SkyBoxRenderer::CreateDeviceDependentResources(void)
{
	//Building the Cube
	static XMFLOAT3 cubeVertices[] =
	{
		XMFLOAT3(-0.5f, -0.5f, -0.5f),
		XMFLOAT3(-0.5f, -0.5f,  0.5f),
		XMFLOAT3(-0.5f,  0.5f, -0.5f),
		XMFLOAT3(-0.5f,  0.5f,  0.5f),
		XMFLOAT3(0.5f, -0.5f, -0.5f), 
		XMFLOAT3(0.5f, -0.5f,  0.5f), 
		XMFLOAT3(0.5f,  0.5f, -0.5f), 
		XMFLOAT3(0.5f,  0.5f,  0.5f), 
	};

	D3D11_SUBRESOURCE_DATA vertexBufferData = { 0 };
	vertexBufferData.pSysMem = cubeVertices;
	vertexBufferData.SysMemPitch = 0;
	vertexBufferData.SysMemSlicePitch = 0;
	CD3D11_BUFFER_DESC vertexBufferDesc(sizeof(cubeVertices), D3D11_BIND_VERTEX_BUFFER);
	m_dev->CreateBuffer(&vertexBufferDesc, &vertexBufferData, &m_vertexBuffer);

	//Flip the Cube indices
	static const unsigned short cubeIndices[] =
	{
		2,1,0, // -x
		2,3,1,

		5,6,4, // +x
		7,6,5,

		1,5,0, // -y
		5,4,0,

		6,7,2, // +y
		7,3,2,

		4,6,0, // -z
		6,2,0,

		3,7,1, // +z
		7,5,1,
	};

	m_indexCount = ARRAYSIZE(cubeIndices);

	//Index Buffer
	D3D11_SUBRESOURCE_DATA indexBufferData = { 0 };
	indexBufferData.pSysMem = cubeIndices;
	indexBufferData.SysMemPitch = 0;
	indexBufferData.SysMemSlicePitch = 0;
	CD3D11_BUFFER_DESC indexBufferDesc(sizeof(cubeIndices), D3D11_BIND_INDEX_BUFFER);
	m_dev->CreateBuffer(&indexBufferDesc, &indexBufferData, &m_indexBuffer);

	//Constant Buffer
	CD3D11_BUFFER_DESC ConstantBufferDesc(sizeof(MVP), D3D11_BIND_CONSTANT_BUFFER);
	m_dev->CreateBuffer(&ConstantBufferDesc, NULL, &m_constantBuffer);

	//Create Input Layout
	static const D3D11_INPUT_ELEMENT_DESC vertexDesc[] =
	{
		{ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 }
	};

	UINT FBXNumElements = ARRAYSIZE(vertexDesc);
	m_dev->CreateInputLayout(vertexDesc, FBXNumElements, &CubeVertexShader, sizeof(CubeVertexShader), &m_inputLayout);

	//Set the Shaders
	m_dev->CreateVertexShader(&CubeVertexShader, sizeof(CubeVertexShader), nullptr, &m_vertexShader);
	m_dev->CreatePixelShader(&CubePixelShader, sizeof(CubePixelShader), nullptr, &m_pixelShader);

	CreateWindowSizeDependentResources();
}

void SkyBoxRenderer::CreateWindowSizeDependentResources(void)
{
	XMStoreFloat4x4(&m_constantBufferData.Pro, XMMatrixPerspectiveFovRH(verticalFOV, aspectratio, zNear, zFar));
}

// Renders one frame using the vertex and pixel shaders.
void SkyBoxRenderer::Render()
{
	XMStoreFloat4x4(&m_constantBufferData.World[0], XMMatrixScaling(100, 100, 100) * XMMatrixTranslation(m_camera->_41, m_camera->_42, m_camera->_43));
	XMStoreFloat4x4(&m_constantBufferData.View, XMMatrixInverse(nullptr, XMLoadFloat4x4(m_camera)));

	// Prepare the constant buffer to send it to the graphics device.
	m_devCon->UpdateSubresource(m_constantBuffer, 0, NULL, &m_constantBufferData, 0, 0);
	m_devCon->VSSetConstantBuffers(0, 1, &m_constantBuffer);

	//Sending the Vertex Buffer 
	UINT stride = sizeof(XMFLOAT3);
	UINT offset = 0;
	m_devCon->IASetVertexBuffers(0, 1, &m_vertexBuffer, &stride, &offset);

	// Each index is one 16-bit unsigned integer (short).
	m_devCon->IASetIndexBuffer(m_indexBuffer, DXGI_FORMAT_R16_UINT, 0);
	m_devCon->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
	m_devCon->IASetInputLayout(m_inputLayout);

	// Attach our vertex shader.
	m_devCon->VSSetShader(m_vertexShader, nullptr, 0);
	m_devCon->PSSetShader(m_pixelShader, nullptr, 0);

	//Texture
	ID3D11ShaderResourceView* texViews[] = { m_texView };
	m_devCon->PSSetShaderResources(0, 1, texViews);

	// Draw the objects.
	m_devCon->DrawIndexed(m_indexCount, 0, 0);
	m_devCon->ClearDepthStencilView(DSV, D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1.0f, 0);
}

void SkyBoxRenderer::ReleaseDeviceDependentResources(void)
{
	if (m_inputLayout)
		m_inputLayout->Release();
	if (m_vertexBuffer)
		m_vertexBuffer->Release();
	if (m_vertexShader)
		m_vertexShader->Release();
	if (m_constantBuffer)
		m_constantBuffer->Release();
	if (m_indexBuffer)
		m_indexBuffer->Release();
}