#include "ImitationGame/framework.h"
#include "Line.h"
#include "./Physics/Collider.h"
#include "./Render/ColorShader.h"

#define   dot(u,v)   ( (u).x*(v).x + (u).y*(v).x)  // AdotB   = |A*B|cos() 내적
#define   cross(u,v) ( (u).x*(v).y - (u).y*(v).x)  // AcrossB = |A*B|sin() 외적

Line::Line()
{
	wstring wstr = SHADER_FOLDER;
	wstr += L"Color.hlsl";
	m_pShader = new ColorShader(wstr);
}

Line::~Line()
{
	// COM I/F 생성된 변수는 DELETE로 해제되는 것이 아니고 
    // Release로 해제하여야 한다
	SAFE_RELEASE(m_pVertexBuffer);   // c++정점->ID3D11Buffer(Device)->pipeline(DeviceContext)
	SAFE_DELETE(m_pShader);

	for (UINT i = 0; i < m_cvLines.size(); i++)
	{
		SAFE_DELETE(m_cvLines[i]);
	}
	m_cvLines.erase(m_cvLines.begin(), m_cvLines.end());
}

void Line::ClearLine()
{
	SAFE_RELEASE(m_pVertexBuffer);

	for (UINT i = 0; i < m_cvLines.size(); i++)
	{
		SAFE_DELETE(m_cvLines[i]);
	}
	m_cvLines.erase(m_cvLines.begin(), m_cvLines.end());
}

Vector2 Line::GetStartPoint(int no)
{
	// 0 : start, 1: end

	Vector2 pos = Vector2(m_cvLines[no * 2]->x, m_cvLines[no * 2]->y);
	return pos;
}

Vector2 Line::GetEndPoint(int no)
{
	Vector2 pos = Vector2(m_cvLines[no * 2 +1]->x, m_cvLines[no * 2+1]->y);
	return pos;
}

bool Line::LoadLine(string fileName)
{
	ClearLine();

	FILE *fp = fopen(fileName.c_str(), "rb");
	if (fp == NULL)
		return false;
	int Count = 0;
	fread(&Count, sizeof(int), 1, fp);

	for (int i = 0; i < Count; i++)
	{
		Vector2 pos1;
		Vector2 pos2;
		fread(&pos1, sizeof(Vector2), 1, fp);
		fread(&pos2, sizeof(Vector2), 1, fp);
		AddLine(pos1.x * TRNMANAGER->TerrainMagnification_.x, 
			pos1.y* TRNMANAGER->TerrainMagnification_.y, 
			pos2.x * TRNMANAGER->TerrainMagnification_.x, 
			pos2.y * TRNMANAGER->TerrainMagnification_.y);
	}
	EndLine();
	fclose(fp);
	return true;
}

bool Line::SaveLine(string fileName)
{

	FILE *fp = fopen(fileName.c_str(), "wb");
	if (fp == NULL)
		return false;
	int Count = this->GetCountLine();
	fwrite(&Count, sizeof(int), 1, fp);

	for (int i = 0; i < Count; i++)
	{
		Vector2 pos1 = this->GetStartPoint(i);
		Vector2 pos2 = this->GetEndPoint(i);
		pos1.x /= TRNMANAGER->TerrainMagnification_.x;
		pos1.y /= TRNMANAGER->TerrainMagnification_.y;
		pos2.x /= TRNMANAGER->TerrainMagnification_.x;
		pos2.y /= TRNMANAGER->TerrainMagnification_.y;
		fwrite(&pos1, sizeof(Vector2), 1, fp);
		fwrite(&pos2, sizeof(Vector2), 1, fp);
	}
	fclose(fp);
	return true;

}

void Line::EraseLine(Vector2 pos, float gap)
{
	Vector2 AreaMin = Vector2(pos.x - gap, pos.y - gap);
	Vector2 AreaMax = Vector2(pos.x + gap, pos.y + gap);
	bool    bClipping = false;


	for (int i = 0; i < this->GetCountLine(); i++)
	{
		Vector2 start = GetStartPoint(i);
		Vector2 end = GetEndPoint(i);
		if (Clipping(start, end, AreaMin, AreaMax))
		{
			bClipping = true;
			delete m_cvLines[i * 2];
			delete m_cvLines[i * 2+1];
			m_cvLines.erase(m_cvLines.begin() + (i * 2), m_cvLines.begin() + (i * 2) + 2);
			m_cvLines.shrink_to_fit();
		}
	}
	if (!bClipping)
		return;
	this->ClearVertexBuffer();
	this->EndLine();

}

void Line::Update(Matrix V, Matrix P)
{
	if (m_cvLines.size() == 0)
		return;

	Vertex *vertices = new Vertex[m_cvLines.size()];

	for (UINT i = 0; i < m_cvLines.size(); i++)
	{
		Vector2 *p = m_cvLines[i];
		vertices[i].Position = Vector3(p->x * lineScale_.x, p->y * lineScale_.y, 0.0f);
		vertices[i].Color    = m_Color;
	}


	// GPU에 vertex내용을 Update
	// MAP -> UNMAP사이에서 수정
	D3D11_MAPPED_SUBRESOURCE  subResource;
	DeviceContext->Map(m_pVertexBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &subResource);
	{
		memcpy(subResource.pData, vertices, sizeof(Vertex) * m_cvLines.size());
	}
	DeviceContext->Unmap(m_pVertexBuffer, 0);

	// 자기자신에 대한 Matrix

	Matrix W,T,S;

	D3DXMatrixTranslation(&T, 0.0f, 0.0f, 0.0f);
	D3DXMatrixScaling(&S, 1.0f, 1.0f, 1.0f);

	W = S * T;

	D3DXMatrixIdentity(&W);
	m_pShader->Update(W, V, P);
}

void Line::Render()
{
	if (m_cvLines.size() == 0)
		return;
	UINT stride = sizeof(Vertex);
	UINT offset = 0;

	// IA단계  : Device
	DeviceContext->IASetVertexBuffers(0,   // slot
		1,   // buffer의 갯수
		&this->m_pVertexBuffer,
		&stride,
		&offset);
	DeviceContext->IASetInputLayout(m_pShader->GetLayout());

	// D3D11_PRIMITIVE_TOPOLOGY_LINESTRIP
	// D3D11_PRIMITIVE_TOPOLOGY_LINELIST
	// D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST
	DeviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_LINELIST);

	// VS _OM단계
	m_pShader->Draw(m_cvLines.size(), 0);
}

void Line::AddLine(float x1, float y1, float x2, float y2)
{
	Vector2 *pPoint = new Vector2(x1, y1);
	m_cvLines.push_back(pPoint);
	pPoint = new Vector2(x2, y2);
	m_cvLines.push_back(pPoint);
}
////////////////////////////////////////////////////////////////////////////////
// Vector를 이용하여 계산하는 법
// http://www.gisdeveloper.co.kr/?p=89 
// https://gaussian37.github.io/math-algorithm-line_intersection/
/////////////////////////////////////////////////////////////////
bool Line::IntersectionLine(Vector2 AP1, Vector2 AP2, Vector2 BP1, Vector2 BP2, Vector2 & result)
{
	double t;
	double s;
	// cross
	double under = (BP2.y - BP1.y)*(AP2.x - AP1.x) - 
		           (BP2.x - BP1.x)*(AP2.y - AP1.y);

	if (!CheckInterSectionLine(AP1, AP2, BP1, BP2))
		return false;

	// 동일한 선인지 확인하는 Check
	if (under == 0)
	{
		// CCW을 이용하여 Check
		float x1 = AP2.x - AP1.x;
		float y1 = AP2.y - AP1.y;

		float x2 = BP1.x - AP1.x;
		float y2 = BP1.y - AP1.y;

		Vector2 v1 = Vector2(x1, y1);
		Vector2 v2 = Vector2(x2, y2);

		float c = cross(v1, v2);
		if (c == 0)
		{
			if (dot(v1, v2) > 0)
				result = BP1;
			else
				result = AP1;
			return true;
		}
		return false;
	}


	double _t = (double)(BP2.x - BP1.x)*(AP1.y - BP1.y) - (BP2.y - BP1.y)*(AP1.x - BP1.x);
	double _s = (double)(AP2.x - AP1.x)*(AP1.y - BP1.y) - (AP2.y - AP1.y)*(AP1.x - BP1.x);

	t = _t / under;
	s = _s / under;


	if (t<0.0 || t>1.0 || s<0.0 || s>1.0)
		return false;
	if (_t == 0 && _s == 0) return false;

	result.x = (float)(AP1.x + t * (double)(AP2.x - AP1.x));
	result.y = (float)(AP1.y + t * (double)(AP2.y - AP1.y));

	return true;

}

void Line::CreateVertexBuffer()
{
	if (m_cvLines.size() == 0)
		return;
	if (m_pVertexBuffer)
		SAFE_RELEASE(m_pVertexBuffer);

	Vertex *vertices = new Vertex[m_cvLines.size()];

	for (UINT i = 0; i < m_cvLines.size(); i++)
	{
		Vector2 *p = m_cvLines[i];
		vertices[i].Position = Vector3(p->x, p->y, 0.0f);
		vertices[i].Color = Color(1.0f, 1.0f, 0.0f, 0.0f);
	}

	// 위에 선언된 데이터를 VertextBuffer에 Setting
	// DX에서 Craete,Get()를 사용할때 변수명칭 앞에 "I"자가 있으면
	// 무조건 Desctrpion이 붙는다
	D3D11_BUFFER_DESC         desc;    // input
	{
		ZeroMemory(&desc, sizeof(desc));              // 변수가 0로 초기화
		desc.Usage = D3D11_USAGE_DYNAMIC;             // *생성한후에 변경할수  한다
		desc.CPUAccessFlags = D3D10_CPU_ACCESS_WRITE; // *생성한후에 변경할수  한다
		desc.ByteWidth = sizeof(Vertex)*m_cvLines.size();              // Vector3,Color ( 3*4 + 4*4)
		desc.BindFlags = D3D11_BIND_VERTEX_BUFFER;    // 12 +16 = 28

		//* D3D11_BIND_VERTEX_BUFFER = 0x1L,     정점
		//* D3D11_BIND_INDEX_BUFFER = 0x2L,      중복되는 정점을 Index로 구분하여 사용
		//* D3D11_BIND_CONSTANT_BUFFER = 0x4L,   상수Buffer, User가 HLSL에 데이터 보낼때
		// D3D11_BIND_SHADER_RESOURCE = 0x8L,    ImageFile
	}

	D3D11_SUBRESOURCE_DATA    data;    // input
	{
		ZeroMemory(&data, sizeof(D3D11_SUBRESOURCE_DATA));
		data.pSysMem = vertices;
	}

	HRESULT hr = Device->CreateBuffer(&desc, &data, &m_pVertexBuffer);
	assert(SUCCEEDED(hr));
}

int Line::OutCode(Vector2 pt, Vector2 AreaMin, Vector2 AreaMax)
{
	const int LEFT = 1;
	const int RIGHT = 2;
	const int TOP = 8;
	const int BOTTOM = 4;

	int Code = 0;

	if (pt.x < AreaMin.x)
		Code |= LEFT;
	else if (pt.x > AreaMax.x)
		Code |= RIGHT;
	if (pt.y < AreaMin.y)
		Code |= BOTTOM;
	else if (pt.y > AreaMax.y)
		Code |= TOP;
	return Code;
}

//////////////////////////////////////////////////////////////
// 코헨-서덜랜드 클립핑  알고리즘
//
//   ------------------------
//    1001 |  1001 | 1010
//   ------------------------ 
//    0001 |  0000 | 0010
//   ------------------------
//    0101 |  0100  | 0110
//   ------------------------

bool Line::Clipping(Vector2 & start, Vector2 & end, Vector2 AreaMin, Vector2 AreaMax)
{
	const int LEFT = 1;
	const int RIGHT = 2;
	const int TOP = 8;
	const int BOTTOM = 4;

	bool  Value = false;
	int   C1, C2, C;
	float tmpx, tmpy;

	while (1)
	{
		// Bit연산
		C1 = OutCode(start, AreaMin, AreaMax);
		C2 = OutCode(end, AreaMin, AreaMax);

		// 2점이 사각형 내부에 있는 것을 Check
		if (C1 == 0 && C2 == 0)
		{
			Value = true;
			break;
		}
		// 사각형 외부에 데이터 있는것을 Check
		if (C1 & C2)
			break;
		// C1,C2중 하나를 선택하여 Clipping
		if (C1 == 0)
			C = C2;
		else
			C = C1;

		// LEFT -> RIGHT -> BOTTOM -> TOP
		// y = mx+c         한점을 알고 있을때의 방정식
		// y = y1 + m(x-x1) 두점을 알고 있을때의 방정식
		if (C & LEFT)
		{
			tmpx = AreaMin.x;
			tmpy = start.y + ((end.y - start.y) / (end.x - start.x))*(AreaMin.x - start.x);
		}
		if (C & RIGHT)
		{
			tmpx = AreaMax.x;
			tmpy = start.y + ((end.y - start.y) / (end.x - start.x))*(AreaMax.x - start.x);
		}
		if (C & BOTTOM)
		{
			tmpx = start.x + ((end.x - start.x) / (end.y - start.y))*(AreaMin.y - start.y);
			tmpy = AreaMin.y;
		}
		if (C & TOP)
		{
			tmpx = start.x + ((end.x - start.x) / (end.y - start.y))*(AreaMax.y - start.y);
			tmpy = AreaMax.y;
		}

		// C가 변경 되었는지 Check
		if (C == C1)
			start = Vector2(tmpx, tmpy);
		else
			end = Vector2(tmpx, tmpy);

	}

	return Value;
}


bool Line::Clipping(Vector2 start, Vector2 end, Collider * pCollider)
{	

	const int LEFT = 1;
	const int RIGHT = 2;
	const int TOP = 8;
	const int BOTTOM = 4;

	bool  Value = false;
	int   C1, C2, C;
	float tmpx, tmpy;
	tmpx = tmpy = 0.0f;	// 초기화 해달라고해서 초기화함.
	Vector2 AreaMin = pCollider->GetPosition() - (pCollider->GetScale() * 0.5f);
	Vector2 AreaMax = pCollider->GetPosition() + (pCollider->GetScale() * 0.5f);

	while (1)
	{
		// Bit연산
		C1 = OutCode(start, AreaMin, AreaMax);
		C2 = OutCode(end, AreaMin, AreaMax);

		// 2점이 사각형 내부에 있는 것을 Check
		if (C1 == 0 && C2 == 0)
		{
			Value = true;
			break;
		}
		// 사각형 외부에 데이터 있는것을 Check
		if (C1 & C2)
			break;
		// C1,C2중 하나를 선택하여 Clipping
		if (C1 == 0)
			C = C2;
		else
			C = C1;

		// LEFT -> RIGHT -> BOTTOM -> TOP
		// y = mx+c         한점을 알고 있을때의 방정식
		// y = y1 + m(x-x1) 두점을 알고 있을때의 방정식
		if (C & LEFT)
		{
			tmpx = AreaMin.x;
			tmpy = start.y + ((end.y - start.y) / (end.x - start.x)) * (AreaMin.x - start.x);
		}
		if (C & RIGHT)
		{
			tmpx = AreaMax.x;
			tmpy = start.y + ((end.y - start.y) / (end.x - start.x)) * (AreaMax.x - start.x);
		}
		if (C & BOTTOM)
		{
			tmpx = start.x + ((end.x - start.x) / (end.y - start.y)) * (AreaMin.y - start.y);
			tmpy = AreaMin.y;
		}
		if (C & TOP)
		{
			tmpx = start.x + ((end.x - start.x) / (end.y - start.y)) * (AreaMax.y - start.y);
			tmpy = AreaMax.y;
		}

		// C가 변경 되었는지 Check
		if (C == C1)
			start = Vector2(tmpx, tmpy);
		else
			end = Vector2(tmpx, tmpy);

	}
	return Value;	

	
}
/*
// https://blog.naver.com/japgo/40196455578 참조 ( 구현 실패, 선의 길이 관련이 없어짐)
bool Line::IntersectionColliderToLine(Collider* pCollider, Vector2& start, Vector2& end)
{
	int count = 0;
	bool Value = false;
	Vector2 pos = pCollider->GetPosition();
	Vector2 scale = pCollider->GetScale();

	// 0~3 까지 상하좌우로 할꺼임.
	RECT interRect;
	interRect.top = pos.y + scale.y * 0.5f;		//상
	interRect.bottom = pos.y - scale.y * 0.5f;	//하
	interRect.left = pos.x - scale.x * 0.5f;	//좌
	interRect.right = pos.x + scale.x * 0.5f;	//우

//	LONG swapF = 0.0f;
//	// Rotation 때문에 넣은거같은데(뒤집어져 있으면 바꾸게) 우린 현재 땡 사각형밖에 없어서 필요없음.
//	if (interRect.left > interRect.right) {
//		swapF = interRect.left;
//		interRect.left = interRect.right;
//		interRect.right = swapF;
//	}
//	if (interRect.bottom > interRect.top) {
//		swapF = interRect.bottom;
//		interRect.bottom = interRect.top;
//		interRect.top = swapF;
//	}



	if (GetDistancePointToline(interRect.left, interRect.top, start.x, start.y, end.x, end.y) > 0.0f) {
		++count;
	}
	else
		--count;

	if (GetDistancePointToline(interRect.right, interRect.top, start.x, start.y, end.x, end.y) > 0.0f) {
		++count;
	}
	else
		--count;

	if (GetDistancePointToline(interRect.left, interRect.bottom, start.x, start.y, end.x, end.y) > 0.0f) {
		++count;
	}
	else
		--count;

	if (GetDistancePointToline(interRect.right, interRect.bottom, start.x, start.y, end.x, end.y) > 0.0f) {
		++count;
	}
	else
		--count;


	if (count == 4 || count == -4)
		Value = false;
	else
		Value = true;

	return Value;
}
*/
// xy는 한 점, lt rb 는 선의 xy xy.
float Line::GetDistancePointToline(LONG& x, LONG& y, FLOAT& l, FLOAT& t, FLOAT& r, FLOAT& b)
{
	float distance = 0.0f;
	// 캐스팅 한 값을 저장한건 아닌데, 참조라서 이게 바뀌지 않나?
	distance = ((b - t) * x + (l - r) * y + (t * r - l * b)) / (sqrt(pow((b - t), 2) + pow((l - r), 2)));
	return distance;
}

// right가 left 보다 크면 true를 반환합니다.
// if right is greater than left, it is true
int Line::IsLeft(Vector2 left, Vector2 right)
{
	int ret;
	if (left.x == right.x) {
		ret = (left.y <= right.y);
	}
	else {
		ret = (left.x <= right.x);
	}
	return ret;
}

//////////////////////////////////////////////////////////////////////////
// CCW를 이용한 Line Itersection Check
// https://gaussian37.github.io/math-algorithm-line_intersection/
//////////////////////////////////////////////////////////////////////////
bool Line::CheckInterSectionLine(Vector2 A, Vector2 B, Vector2 C, Vector2 D)
{
	int ret;
	// l1(A-B)을 기준으로 l2(C-D)가 교차하는 지 확인한다.
	// Check if l2 intersects based on l1.
	int l1_l2 = CCW(A, B, C) * CCW(A, B, D);
	// l2를 기준으로 l1이 교차하는 지 확인한다.
	// Check if l1 intersects based on l2.
	int l2_l1 = CCW(C, D, A) * CCW(C, D, B);


	// l1 과 l2가 일직선 상에 있는 경우
	// l1 and l2 are in same line.
	if (l1_l2 == 0 && l2_l1 == 0)
	{
		// Line1의 점의 크기 순서를 p1 < p2 순서로 맞춘다.
		// Set the order of the points on Line1 in the order p1 < p2.
		if (IsLeft(B, A)) std::swap(A, B);
		// Line2의 점의 크기 순서를 p1 < p2 순서로 맞춘다.
		// Set the order of the points on Line2 in the order p1 < p2.
		if (IsLeft(D, C)) std::swap(C, D);

		// A -----------B
		//         C -----------D
		// 위 조건을 만족하는 지 살펴본다.
		// See if the above conditions are met.
		ret = (IsLeft(C, B)) && (IsLeft(A, D));
	}
	// l1과 l2가 일직선 상에 있지 않는 경우
	// l1 and l2 are not in same line.
	else {
		ret = (l1_l2 <= 0) && (l2_l1 <= 0);
	}
	return ret;

}
/*
- input : p1 = (p1.x, p1.y), p2 = (p2.x, p2.y), p3 = (p3.x, p3.y)
- output : 1 (counter clockwise), 0 (collinear), -1 (clockwise)
※ vector v1 = (p2 - p1), vector v2 = (p3 - p1)
*/
int Line::CCW(Vector2 p1, Vector2 p2, Vector2 p3)
{
	float cross_product = (p2.x - p1.x)*(p3.y - p1.y) - (p3.x - p1.x)*(p2.y - p1.y);

	if (cross_product > 0)
		return 1;

	else if (cross_product < 0)
		return -1;
	else
		return 0;
}




