#include "Plane.h"

Plane::Plane()
{
}

Plane::~Plane()
{
	//ReleaseDeviceDependentResources();
}

void Plane::Create(ID3D11Device &dev, XMFLOAT4X4 &camera, ID3D11DeviceContext &Con)
{
	m_dev = &dev;
	m_camera = &camera;
	m_devCon = &Con;
}

void Plane::LoadFBXFromFile(bool overwrite)
{
	FBXtoBinary("../Original Assets/Plane.fbx", "../Exports/Plane.bin", overwrite);
	Skeleton skelly;
	std::vector<PNTIWVertex> verts;
	ReadBinary("../Exports/Plane.bin", &skelly, &indexes, &verts);
	for (size_t i = 0; i < verts.size(); i++)
	{
		positions.push_back(verts[i].mPosition);
	}

	XMStoreFloat4x4(m_constantBufferData.World, XMMatrixIdentity());
	CreateDeviceDependentResources();
}

void Plane::CreateDeviceDependentResources(void)
{
	//Setting Vertex Buffer
	D3D11_BUFFER_DESC m_vertexBufferDesc;
	ZeroMemory(&m_vertexBufferDesc, sizeof(D3D11_BUFFER_DESC));
	m_vertexBufferDesc.Usage = D3D11_USAGE_IMMUTABLE;
	m_vertexBufferDesc.ByteWidth = sizeof(XMFLOAT3) * positions.size();
	m_vertexBufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	m_vertexBufferDesc.CPUAccessFlags = 0;
	m_vertexBufferDesc.StructureByteStride = 0;
	m_vertexBufferDesc.MiscFlags = 0;

	D3D11_SUBRESOURCE_DATA m_vertexBufferData;
	ZeroMemory(&m_vertexBufferData, sizeof(D3D11_SUBRESOURCE_DATA));
	m_vertexBufferData.pSysMem = positions.data();
	m_vertexBufferData.SysMemPitch = 0;
	m_vertexBufferData.SysMemSlicePitch = 0;
	m_dev->CreateBuffer(&m_vertexBufferDesc, &m_vertexBufferData, &m_vertexBuffer);

	//Setting the Constant Buffer
	CD3D11_BUFFER_DESC AnimateConstantBufferDesc(sizeof(MVP), D3D11_BIND_CONSTANT_BUFFER);
	m_dev->CreateBuffer(&AnimateConstantBufferDesc, NULL, &m_constantBuffer);


	//Setting Input Layout
	static const D3D11_INPUT_ELEMENT_DESC vertDesc[] =
	{
		{ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 }
	};
	HRESULT t = m_dev->CreateInputLayout(vertDesc, ARRAYSIZE(vertDesc), &_SphereVShader, sizeof(_SphereVShader), &m_inputLayout);

	//Setting the Shaders
	m_dev->CreateVertexShader(&_SphereVShader, sizeof(_SphereVShader), nullptr, &m_vertexShader);
	m_dev->CreatePixelShader(&_SpherePShader, sizeof(_SpherePShader), nullptr, &m_pixelShader);

	CreateWindowSizeDependentResources();
}

// Initializes view parameters when the window size changes.
void Plane::CreateWindowSizeDependentResources(void)
{
	XMStoreFloat4x4(&m_constantBufferData.Pro, XMMatrixPerspectiveFovRH(verticalFOV, aspectratio, zNear, zFar));
}

void Plane::Render()
{
	XMStoreFloat4x4(&m_constantBufferData.View, XMMatrixInverse(nullptr, XMLoadFloat4x4(m_camera)));

	//Sending the Constant Buffer
	m_devCon->UpdateSubresource(m_constantBuffer, 0, NULL, &m_constantBufferData, 0, 0);
	m_devCon->VSSetConstantBuffers(0, 1, &m_constantBuffer);

	//Sending the Vertex Buffer
	UINT stride = sizeof(XMFLOAT3);
	UINT offset = 0;
	m_devCon->IASetVertexBuffers(0, 1, &m_vertexBuffer, &stride, &offset);

	//Sending the Input Layout
	m_devCon->IASetInputLayout(m_inputLayout);

	//Setting the Shaders
	m_devCon->VSSetShader(m_vertexShader, nullptr, 0);
	m_devCon->PSSetShader(m_pixelShader, nullptr, 0);

	//Draw
	m_devCon->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
	m_devCon->DrawInstanced(positions.size(), 1, 0, 0);
}

void Plane::ReleaseDeviceDependentResources(void)
{
	if (m_inputLayout)
		m_inputLayout->Release();
	if (m_vertexBuffer)
		m_vertexBuffer->Release();
	if (m_vertexShader)
		m_vertexShader->Release();
	if (m_constantBuffer)
		m_constantBuffer->Release();
}