#include  "ImitationGame/framework.h"
#include  "Physics/clipper.hpp"
#include  "./Render/ColorShader.h"
#include  "Collider.h"

#define MAX(x,y) ( ( ((x) >= (y)) ? (x):(y)) )
#define MIN(x,y) ( ( ((x) <  (y)) ? (x):(y)) )
#define ABS(x) ( ( ((x) >  (0)) ? (x):(-(x))) )
#define PI 3.141592f

using namespace clipper;

enum poly_color_type { pctSubject, pctClip, pctSolution };
ClipType     CT = ctIntersection;      //  { ctIntersection, ctUnion, ctDifference, ctXor };
PolyFillType PFT = pftNonZero;


/////////////////////////////////////////////////////////////////////
//  생성자
/////////////////////////////////////////////////////////////////////
Collider::Collider()
{
	wstring strShader = SHADER_FOLDER;
	strShader += L"Color.hlsl";

	m_pShader = new ColorShader(strShader);
	CreateBuffer();
	D3DXMatrixIdentity(&m_World);
}

Collider::~Collider()
{
	SAFE_DELETE(m_pShader);
	SAFE_RELEASE(m_pVertexBuffer);

	if (m_cvPolygons.size())
	{
		for (UINT i = 0; i < m_cvPolygons.size(); i++)
			delete m_cvPolygons[i];
		m_cvPolygons.erase(m_cvPolygons.begin(), m_cvPolygons.begin() + m_cvPolygons.size());

	}
}
void Collider::Update(Matrix V, Matrix P)
{
	Matrix W, S, T, R;

	D3DXMatrixIdentity(&S);
	D3DXMatrixIdentity(&T);
	D3DXMatrixIdentity(&R);

	D3DXMatrixTranslation(&T, m_Position.x, m_Position.y, 0.0f);
	D3DXMatrixScaling(&S, m_Scale.x, m_Scale.y, 0.0f);
	D3DXMatrixRotationYawPitchRoll(&R, m_Rotation.y*PI / 180.0f, m_Rotation.x*PI / 180.0f, m_Rotation.z*PI / 180.0f);
	W = S * R *  T;
	m_World = S * R * T;
	m_pShader->Update(W, V, P);

}

void Collider::Render()
{
	UINT  stride = sizeof(Vertex);
	UINT  offset = 0;

	if (!g_bVisibleCollisionCheck)
		return;


	if (m_bCollisionCheck)
		UpdateBlock(Color(1, 0, 0, 1));
	else
		UpdateBlock(Color(0, 1, 0, 1));

	// IA단계  Input Assembly
	DeviceContext->IASetVertexBuffers(0, 1, &this->m_pVertexBuffer, &stride, &offset);
	DeviceContext->IASetInputLayout(m_pShader->GetLayout());
	DeviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_LINESTRIP);

	// VS-OM
	m_pShader->Draw(5, 0);

}
bool Collider::IntersectOBB(SHAPE sA, SHAPE sB)
{
	Vector2 dist = sA.position - sB.position;
	Vector2 vec[4] = { GetUpVector(sA), GetUpVector(sB),
					   GetRightVector(sA),GetRightVector(sB) };
	Vector2 unit;
	for (int i = 0; i < 4; i++)
	{
		double sum = 0.0f;
		unit = GetUnitVector(vec[i]);
		for (int j = 0; j < 4; j++)
		{
			sum = sum + abs(vec[j].x*unit.x + vec[j].y*unit.y);
			if (abs(dist.x*unit.x + dist.y*unit.y) > sum)
				return false;
		}


	}

	return true;
}
bool Collider::IntersectOBB(Collider * A, Collider * B)
{
	SHAPE sA;
	SHAPE sB;

	sA.position = A->GetPosition();
	sA.scale = A->GetScale();
	sA.rot = A->GetRotation().z;
	sA.min.x = A->GetPosition().x - A->GetScale().x*0.5f;
	sA.min.y = A->GetPosition().y - A->GetScale().y*0.5f;
	sA.max.x = A->GetPosition().x + A->GetScale().x*0.5f;
	sA.max.y = A->GetPosition().y + A->GetScale().y*0.5f;

	sB.position = B->GetPosition();
	sB.scale = B->GetScale();
	sB.rot = B->GetRotation().z;
	sB.min.x = B->GetPosition().x - B->GetScale().x*0.5f;
	sB.min.y = B->GetPosition().y - B->GetScale().y*0.5f;
	sB.max.x = B->GetPosition().x + B->GetScale().x*0.5f;
	sB.max.y = B->GetPosition().y + B->GetScale().y*0.5f;

	return (IntersectOBB(sA, sB));

}
bool Collider::IntersectAABB(class Collider *A, class Collider *B)
{
	Matrix world1 = A->GetWorld();
	Matrix world2 = B->GetWorld();

	// world1영역

	float halfWidth  = fabs(world1._11 *0.5f);
	float halfHeight = fabs(world1._22 *0.5f);
	float leftA      = world1._41  - halfWidth;
	float rightA     = world1._41 + halfWidth;
	float topA       = world1._42 + halfHeight;
	float bottomA    = world1._42 - halfHeight;

	// world2영역

		halfWidth = fabs(world2._11 *0.5f);
		halfHeight = fabs(world2._22 *0.5f);
	float leftB = world2._41 - halfWidth;
	float rightB = world2._41 + halfWidth;
	float topB = world2._42 + halfHeight;
	float bottomB = world2._42 - halfHeight;

	// 충돌 Check
	if (rightA < leftB || leftA > rightB)
		return false;
	if (topA < bottomB || bottomA > topB)
		return false;
	return true;

}
//////////////////////////////////////////////////////////////////////////
// CCW를 이용한 Line Itersection Check
// https://gaussian37.github.io/math-algorithm-line_intersection/
//////////////////////////////////////////////////////////////////////////
bool Collider::InterSectionLine(Vector2 A, Vector2 B, Vector2 C, Vector2 D)
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
//////////////////////////////////////////////////////////////
// 1점이 내부에 있는가 Check
/////////////////////////////////////////////////////////////
bool Collider::PointInRect(Matrix world, Vector2 pos)
{
	// world1 영역
	float halfWidth = fabsf(world._11 * 0.5f);
	float halfHeight = fabsf(world._22 * 0.5f);

	float minX = world._41 - halfWidth;
	float maxX = world._41 + halfWidth;
	float maxY = world._42 + halfHeight;
	float minY = world._42 - halfHeight;

	if (!(maxX >= pos.x && minX <= pos.x && maxY >= pos.y && minY <= pos.y))
		return false;

	return true;
}
//////////////////////////////////////////////////////////////
// 1점이 내부에 있는가 Check
/////////////////////////////////////////////////////////////
bool Collider::Aabb(Matrix world, Vector2 pos)
{
	// world1 영역
	float halfWidth  = fabsf(world._11 * 0.5f);
	float halfHeight = fabsf(world._22 * 0.5f);

	float minX   = world._41 - halfWidth;
	float maxX   = world._41 + halfWidth;
	float maxY   = world._42 + halfHeight;
	float minY   = world._42 - halfHeight;

	if (!(maxX >= pos.x && minX <= pos.x && maxY >= pos.y && minY <= pos.y))
		return false;

	return true;
}
/////////////////////////////////////////////////////////////
// Polygon Operation 
////////////////////////////////////////////////////////////
bool Collider::InterSect(Collider * A, Collider * B)
{
	return(IntersectAABB(A, B));


	bool  return_value = false;

	vector<Vector2*> PolygonA = A->GetAreas();
	vector<Vector2*> PolygonB = B->GetAreas();

	// clipper 사용방법
	// 변수를 선언
	Clipper  Engine;
	Polygons  clip, sub, solution;

	clip.resize(1);  // sA + sB -->2 다수의 size
	sub.resize(1);   // 


	clip[0].resize(PolygonA.size());  // 실제 데이터 Size
	sub[0].resize(PolygonB.size());   // 실제 데이터 Size

	// Clipper데이터 타입 integer
	for (UINT i = 0; i < PolygonA.size(); i++)
	{
		clip[0][i].X = (int)(PolygonA[i]->x*1000.0f);
		clip[0][i].Y = (int)(PolygonA[i]->y*1000.0f);

	}

	for (UINT i = 0; i < PolygonB.size(); i++)
	{
		sub[0][i].X = (int)(PolygonB[i]->x*1000.0f);
		sub[0][i].Y = (int)(PolygonB[i]->y*1000.0f);

	}

	Engine.AddPolygons(sub, ptSubject);        // Polygon Subject
	Engine.AddPolygons(clip, ptClip);          // Polygon Clip
	Engine.Execute(CT, solution, PFT, PFT);    // AND,OR


	int nCount = 0;

	if (solution.size() == 0)
		return false;

	return true;


	// 만약 겹쳐진 영역을 구하려면

	POINT  pt[1000];
	int    no = 0;

	for (UINT i = 0; i < solution.size(); ++i)
	{
		nCount = nCount + (int)(solution[i].size());
		no = 0;
		for (UINT j = 0; j < solution[i].size(); j++)
		{
			int x = (int)(solution[i][j].X);
			int y = (int)(solution[i][j].Y);

			pt[no].x = x;
			pt[no].y = y;
			no++;
		}
		if (no)
		{
			//Win2DGraphic  graphic;
			//for (int j = 0; j < (no - 1); j++)
			//{
			//	graphic.DrawLine(pt[j].x, pt[j].y, pt[j + 1].x, pt[j + 1].y, 5, RGB(255, 0, 0));
			//
			//}
		    // Draw Line
		}
	}
	return true;
}
/*
- input : p1 = (p1.x, p1.y), p2 = (p2.x, p2.y), p3 = (p3.x, p3.y)
- output : 1 (counter clockwise), 0 (collinear), -1 (clockwise)
※ vector v1 = (p2 - p1), vector v2 = (p3 - p1)
*/
int Collider::CCW(Vector2 p1, Vector2 p2, Vector2 p3)
{
	float cross_product = (p2.x - p1.x)*(p3.y - p1.y) - (p3.x - p1.x)*(p2.y - p1.y);

	if (cross_product > 0) 
		return 1;

	else if (cross_product < 0) 
		return -1;
	else 
		return 0;
}
// right가 left 보다 크면 true를 반환합니다.
// if right is greater than left, it is true
int Collider::IsLeft(Vector2 left, Vector2 right)
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



vector<Vector2*> Collider::GetAreas()
{
	// 기존데이터 지움
	if (m_cvPolygons.size())
	{
		for (UINT i = 0; i < m_cvPolygons.size(); i++)
			delete m_cvPolygons[i];
		m_cvPolygons.erase(m_cvPolygons.begin(), m_cvPolygons.begin() + m_cvPolygons.size());

	}

	Matrix  matrix = GetWorld();  // sacle, trans, rotate , position
	Vector2 posA[5];
	
	posA[0] = Vector2(-0.5f, -0.5f);
	posA[1] = Vector2(+0.5f, -0.5f);
	posA[2] = Vector2(+0.5f, +0.5f);
	posA[3] = Vector2(-0.5f, +0.5f);
	posA[4] = Vector2(-0.5f, -0.5f);

	for (int i = 0; i < 5; i++)
	{
		D3DXVec2TransformCoord(&posA[i], &posA[i], &matrix);
		m_cvPolygons.push_back(new Vector2(posA[i]));
	}
	return m_cvPolygons;
}
Vector2 Collider::GetUpVector(SHAPE s)
{
	Vector2 ret;

	ret.x =  s.scale.y * sinf(s.rot*PI / 180.0f)*0.5f;
	ret.y = -s.scale.y * cosf(s.rot*PI / 180.0f)*0.5f;
	return ret;
}
Vector2 Collider::GetRightVector(SHAPE s)
{
	Vector2 ret;

	ret.x = s.scale.x * cosf(s.rot*PI / 180.0f)*0.5f;
	ret.y = -s.scale.x * sinf(s.rot*PI / 180.0f)*0.5f;
	return ret;

}
// 단위벡터
Vector2 Collider::GetUnitVector(Vector2 s)
{
	Vector2 ret;
	float size = sqrt(s.x*s.x + s.y*s.y);
	ret.x = s.x / size;
	ret.y = s.y / size;

	return ret;
}
/////////////////////////////////////////////////////
//
//  정점은 시계방향으로 만듬 --> OpenGL : 사계반대 방향
// 랜더링 파이프라인 IA단계 : IA->VS->RS->PS->OM
//
//  OpenGL/DirectX  --> 실제좌표는  -0.5 ~ 0.5 값 : Normal값
//
//    랜더링 파이프라인  ( IA 단계)  IA>VS->RS->PS->OM
//    * 기본데이터가 들어가는 조립단계
//    
//     1    |   (3)
//          |
//    -------------
//     0    |    2         0 : -0.5f, -0.5f, 0.0f
//          |              1 : -0.5f, +0.5f, 0.0f
//                         2 : +0.5f, -0.5f, 0.0f
//
//  Rectangle ---> Triangle + Triangle
//                 0 1 2      3 4 5
/////////////////////////////////////////////////////////
void Collider::CreateBuffer()
{
	// 1. 정점을 만든다  --> 시계방향으로
	Vertex vertices[5];

	vertices[0].Position = Vector3(-0.5f, -0.5f, 0.0f);  // 0
	vertices[1].Position = Vector3(-0.5f, +0.5f, 0.0f);  // 1
	vertices[2].Position = Vector3(+0.5f, +0.5f, 0.0f);  // 3
	vertices[3].Position = Vector3(+0.5f, -0.5f, 0.0f);  // 2
	vertices[4].Position = Vector3(-0.5f, -0.5f, 0.0f);  // 0

	vertices[0].Color = Color(1.0f,1.0f,1.0f,0.7f);
	vertices[1].Color = Color(1.0f, 1.0f, 1.0f, 0.7f);
	vertices[2].Color = Color(1.0f, 1.0f, 1.0f, 0.7f);
	vertices[3].Color = Color(1.0f, 1.0f, 1.0f, 0.7f);
	vertices[4].Color = Color(1.0f, 1.0f, 1.0f, 0.7f);


	// 2. Device(CPU) 데이터를 생성
    // 2. CPU자원을 GPU(IA단계)에 넘겨 주기위한 변환 작업을 한다
	{

		D3D11_BUFFER_DESC  desc;

		ZeroMemory(&desc, sizeof(D3D11_BUFFER_DESC)); // 초기화 

		desc.Usage = D3D11_USAGE_DYNAMIC;             // Buffer를 수정할 수 있다
		desc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
		desc.ByteWidth = sizeof(Vertex) * 5;          // Class가 안됩니까? 
		desc.BindFlags = D3D11_BIND_VERTEX_BUFFER;    // Buffer -> Constant ( Matrix )

		D3D11_SUBRESOURCE_DATA data;
		ZeroMemory(&data, sizeof(D3D11_SUBRESOURCE_DATA)); // 초기화 
		data.pSysMem = vertices;                       // vertices의 시스템 포인터

		// 자원은 디바이스로 부터 만든다  ( Device --> CPU )
		HRESULT hr = Device->CreateBuffer(&desc, &data, &this->m_pVertexBuffer);
		assert(SUCCEEDED(hr));
	}
}

void Collider::UpdateBlock(Color color)
{
	Vertex  vertices[5];

	vertices[0].Position = Vector3(-0.5f, -0.5f, 0.0f);  // 0
	vertices[1].Position = Vector3(-0.5f, +0.5f, 0.0f);  // 1
	vertices[2].Position = Vector3(+0.5f, +0.5f, 0.0f);  // 3
	vertices[3].Position = Vector3(+0.5f, -0.5f, 0.0f);  // 2
	vertices[4].Position = Vector3(-0.5f, -0.5f, 0.0f);  // 0

	vertices[0].Color = color;
	vertices[1].Color = color;
	vertices[2].Color = color;
	vertices[3].Color = color;
	vertices[4].Color = color;
	// MAP과 UNMAP사이에서 데이터 변경, CPU_ACCESS_WRITE

	D3D11_MAPPED_SUBRESOURCE mapped_subresource;
	DeviceContext->Map(this->m_pVertexBuffer, 0, D3D11_MAP_WRITE_DISCARD,
		0, &mapped_subresource);

	// 데이터를 변경
	memcpy(mapped_subresource.pData, vertices, sizeof(Vertex) * 5);
	DeviceContext->Unmap(this->m_pVertexBuffer, 0);



}


