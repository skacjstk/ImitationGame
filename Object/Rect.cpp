#include "ImitationGame/framework.h"
#include "Render/ColorShader.h"
#include "Rect.h"

Rect::Rect(wstring strShaderFile)
{
	m_pShader = new ColorShader(strShaderFile);
	CreateVertexBuffer();
	CreateIndexBuffer();
}
Rect::Rect()
{
	wstring strShaderFile = SHADER_FOLDER; strShaderFile += L"Color.hlsl";
	m_pShader = new ColorShader(strShaderFile);
	CreateVertexBuffer();
	CreateIndexBuffer();
}
Rect::~Rect()
{
	SAFE_DELETE(m_pShader);
	SAFE_RELEASE(this->m_pVertexBuffer);
	SAFE_RELEASE(this->m_pIndexBuffer);
}

void Rect::Update(Matrix V, Matrix P)
{
	Matrix W, S, T, R;

	D3DXMatrixIdentity(&S);
	D3DXMatrixIdentity(&T);
	D3DXMatrixIdentity(&R);

	D3DXMatrixTranslation(&T, m_Position.x, m_Position.y, 0.0f);
	D3DXMatrixScaling(&S, m_Scale.x, m_Scale.y, 0.0f);

	W = S * T;

	UpdateBlock(m_Color);

	m_pShader->Update(W, V, P);
}

void Rect::Render()
{
	UINT stride = sizeof(Vertex);  // 정점크기에 대한 정보
	UINT  offset = 0;

	// IA단계  Input Assembly
	DeviceContext->IASetVertexBuffers(0, 1, &this->m_pVertexBuffer, &stride, &offset);
	DeviceContext->IASetIndexBuffer(m_pIndexBuffer, DXGI_FORMAT_R32_UINT, 0);
	DeviceContext->IASetInputLayout(m_pShader->GetLayout());
	DeviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);


	// VS-OM
	m_pShader->DrawIndexed(6, 0, 0);


}
void Rect::UpdateBlock(Color color)
{
	Vertex  vertices[4];

	vertices[0].Position = Vector3(-0.5f, -0.5f, 0.0f);
	vertices[1].Position = Vector3(-0.5f, +0.5f, 0.0f);
	vertices[2].Position = Vector3(+0.5f, +0.5f, 0.0f);
	vertices[3].Position = Vector3(+0.5f, -0.5f, 0.0f);

	vertices[0].Color = color;
	vertices[1].Color = color;
	vertices[2].Color = color;
	vertices[3].Color = color;

	// MAP과 UNMAP사이에서 데이터 변경, CPU_ACCESS_WRITE

	D3D11_MAPPED_SUBRESOURCE mapped_subresource;
	DeviceContext->Map(this->m_pVertexBuffer, 0, D3D11_MAP_WRITE_DISCARD,
		0, &mapped_subresource);

	// 데이터를 변경
		memcpy(mapped_subresource.pData, vertices, sizeof(Vertex)*4);
	DeviceContext->Unmap(this->m_pVertexBuffer, 0);


}
bool Rect::IsPtInRect(Vector2 pos)
{
	Vector2 min = Vector2(m_Position.x - m_Scale.x*0.5f, m_Position.y - m_Scale.y*0.5f);
	Vector2 max = Vector2(m_Position.x + m_Scale.x*0.5f, m_Position.y + m_Scale.y*0.5f);

	if (max.x >= pos.x && min.x <= pos.x && max.y >= pos.y && min.y <= pos.y)
		return true;
	return false;
}
///////////////////////////////////////////
//
//            1    |  2
//        ----------------
//            0    |  3 
void Rect::CreateVertexBuffer()
{
	Vertex  vertices[4];

	vertices[0].Position = Vector3(-0.5f, -0.5f, 0.0f);
	vertices[1].Position = Vector3(-0.5f, +0.5f, 0.0f);
	vertices[2].Position = Vector3(+0.5f, +0.5f, 0.0f);
	vertices[3].Position = Vector3(+0.5f, -0.5f, 0.0f);

	vertices[0].Color = Color(1.0f, 0.0f, 0.0f, 1.0f);
	vertices[1].Color = Color(1.0f, 0.0f, 0.0f, 1.0f);
	vertices[2].Color = Color(1.0f, 0.0f, 0.0f, 1.0f);
	vertices[3].Color = Color(1.0f, 0.0f, 0.0f, 1.0f);

	// Buffer에 변환

	D3D11_BUFFER_DESC     desc;

	ZeroMemory(&desc, sizeof(D3D11_BUFFER_DESC));

	desc.Usage = D3D11_USAGE_DYNAMIC;  // CPU-Write --> GPU Read
	desc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	desc.ByteWidth = sizeof(Vertex) * 4;
	desc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;


	D3D11_SUBRESOURCE_DATA data;
	ZeroMemory(&data, sizeof(D3D11_SUBRESOURCE_DATA));
	data.pSysMem = vertices;
	HRESULT hr = Device->CreateBuffer(&desc, &data, &this->m_pVertexBuffer);

	assert(SUCCEEDED(hr));
}

void Rect::CreateIndexBuffer()
{
	UINT indecies[] = { 0,1,2,3,0,2 };


	D3D11_BUFFER_DESC     desc;

	ZeroMemory(&desc, sizeof(D3D11_BUFFER_DESC));

	desc.Usage = D3D11_USAGE_DYNAMIC;  // CPU-Write --> GPU Read
	desc.BindFlags = D3D11_BIND_INDEX_BUFFER;
	desc.ByteWidth = sizeof(UINT) * 6;
	desc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;


	D3D11_SUBRESOURCE_DATA data;
	ZeroMemory(&data, sizeof(D3D11_SUBRESOURCE_DATA));
	data.pSysMem = indecies;
	HRESULT hr = Device->CreateBuffer(&desc, &data, &this->m_pIndexBuffer);

	assert(SUCCEEDED(hr));
}

