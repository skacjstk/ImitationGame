#include  "ImitationGame/framework.h"
#include  "S18_AStarDemo.h"
#include  "Base/Camera.h"
#include  "./Object/Line.h"
#include  "./Object/AStar.h"
#include  "./Object/Rect.h"

float  X;
float  Y ;
S18_AStarDemo::S18_AStarDemo()
{
	SetActive(false);
	m_strSceneName = "AstarDemo";

	X = -MAIN->GetWidth()*0.5f;
	Y = -MAIN->GetHeight()*0.5f;

	X = Y = 0.0f;

	// 수평데이터
	float gapX = MAIN->GetWidth()  / 15.0f;
	float gapY = MAIN->GetHeight() / 15.0f;

	float gap = gapX;
	if( gapX > gapY)
		gap = gapY;

	gap = 48;

	wstring  strShader = SHADER_FOLDER; strShader += L"Texture.hlsl";
	wstring  strImage = IMAGE_FOLDER; strImage += L"Flag.png";

	m_pChocobo = new Texture(strImage, strShader,0,0,100,100);
	m_pChocobo->SetScale(0.5f, 0.5f);
	m_pChocobo->SetPosition(Vector2(gap * 8 - 24, gap * 8 - 24));

	m_pStart   = new Rect();	// Chocobo 가 Start의 역할을 대신함.
	m_pStart->SetPosition(Vector2(gap * 15 - 24, gap * 15 - 24));
	
	m_pStart->SetColor(Color(1, 1, 0,0));
	m_pStart->SetScale(Vector2(48, 48));

	m_pGoal = new Rect();
	m_pGoal->SetPosition(Vector2(gap* 1  - 24,  gap*1  - 24));
	m_pGoal->SetColor(Color(1, 1, 0,0));
	m_pGoal->SetScale(Vector2(48, 48));



	for (int i = 0; i < 16; i++)
	{
		Line *pLine = new Line();
		pLine->AddLine(0.0f, gap*i, gap * 15, gap*i);
		pLine->CreateVertexBuffer();
		m_cvGrid.push_back(pLine);
	}


	for (int i = 0; i < 16; i++)
	{
		Line *pLine = new Line();
		pLine->AddLine(gap*i, 0.0, gap*i, gap * 15);
		pLine->CreateVertexBuffer();
		m_cvGrid.push_back(pLine);
	}


}

S18_AStarDemo::~S18_AStarDemo()
{
	
}
void S18_AStarDemo::Update()
{
	if (KEYBOARD->Down(VK_NUMPAD0))
	{
		ExitScene();
		// ChangeScene 자체를 ExitScene에 옮김.
	}
	Matrix V, P;
	//
	// 카메라를 먼저 Update한후 View, Projection Matrix얻어온다
	// player에 의한 카메라 위치 변환
	CAMERA->Update();
	CAMERA->SetPosition(300.0f, 300.0f);


	V = CAMERA->GetViewMatrix();
	P = CAMERA->GetProjectionMatrix();



	// 마우스 위치 가져오기 
	Vector2 pos = Mouse->GetPosition();
	CAMERA->WCtoVC(pos);

	if ( Mouse->Down(0) || Mouse->Press(0))
	{
		int x = (int)((pos.x) / 48) ;
		int y = (int)((pos.y) / 48) ;

		pos.x = (float)x * 48.0f + 24.0f ;
		pos.y = (float)y * 48.0f + 24.0f ;

		if (!ExistWall(pos))
		{
			if (!(x > 14 || y > 14))
			{
				Rect *rect = new Rect();
				rect->SetPosition(pos);
				rect->SetColor(Color(1, 0, 0,0.0f));
				rect->SetScale(Vector2(48, 48));
				m_cvWalls.push_back(rect);
			}
		}
	}

	if (Mouse->Down(1))
	{
		int x = (int)((pos.x) / 48);
		int y = (int)((pos.y) / 48);

		pos.x = (float)x * 48.0f + 24.0f;
		pos.y = (float)y * 48.0f + 24.0f;
		m_pGoal->SetPosition(pos);

	}

	if (Mouse->Down(2))
	{
		int x = (int)((pos.x) / 48);
		int y = (int)((pos.y) / 48);

		pos.x = (float)x * 48.0f + 24.0f;
		pos.y = (float)y * 48.0f + 24.0f;
		m_pChocobo->SetPosition(pos);

	}

	m_pChocobo->Update(V, P);
//	m_pStart->SetPosition(m_pChocobo->GetPosition());
//	m_pStart->Update(V, P);
	m_pGoal->Update(V, P);
	for (auto a : m_cvGrid)
		a->Update(V, P);

	for (auto a : m_cvWalls)
		a->Update(V, P);

	for (auto a : m_cvPaths)
		a->Update(V, P);

	CAMERA->Update(V, P);
}

void S18_AStarDemo::Render()
{



	for (auto a : m_cvWalls)
		a->Render();

	for (auto a : m_cvPaths)
		a->Render();

	for (auto a : m_cvGrid)
		a->Render();
	
//	m_pStart->Render();
	m_pChocobo->Render();
	m_pGoal->Render();

	CAMERA->Render();


	//ImGui NewFrame
	ImGui::GetIO().NavActive = false;
	ImGui::GetIO().WantCaptureMouse = true;

	ImGui_ImplDX11_NewFrame();
	ImGui_ImplWin32_NewFrame();
	ImGui::NewFrame();

	if (ImGui::Button("Clear"))
	{
		ClearWall();
		ClearPath();
	}
	ImGui::SameLine();
	if (ImGui::Button(" A Star"))
	{
		RunAStar();

	}

	ImGui::Render();
	ImGui_ImplDX11_RenderDrawData(ImGui::GetDrawData());


	
}

void S18_AStarDemo::ChangeScene()
{
	SetActive(true);
}

void S18_AStarDemo::ExitScene()
{
	SCENEMANAGER->ChangeScene("Intro");
}

void S18_AStarDemo::ClearWall()
{
	if (m_cvWalls.size())
	{
		for (UINT i = 0; i < m_cvWalls.size(); i++)
		{
			delete m_cvWalls[i];
		}
		m_cvWalls.erase(m_cvWalls.begin(), m_cvWalls.begin() + m_cvWalls.size());
	}
	

}

void S18_AStarDemo::ClearPath()
{
	if (m_cvPaths.size())
	{
		for (UINT i = 0; i < m_cvPaths.size(); i++)
		{
			delete m_cvPaths[i];
		}
		m_cvPaths.erase(m_cvPaths.begin(), m_cvPaths.begin() + m_cvPaths.size());
	}

}
// Astar 핵심 알고리즘.
void S18_AStarDemo::RunAStar()
{
	AStar  astar;

	ClearPath();


	astar.SetWorldSize(15, 15);  // Grid Size 
	astar.SetHeuristic(1);


	Vector2 pos = m_pChocobo->GetPosition();  // start
	int    sx = (int)(pos.x - 24 ) / 48;
	int    sy = (int)(pos.y - 24 ) / 48;

	pos = m_pGoal->GetPosition();             // end

	int    ex = (int)(pos.x - 24) / 48;
	int    ey = (int)(pos.y - 24) / 48;

	// Wall

	for (UINT i = 0; i < m_cvWalls.size(); i++)
	{
		Vector2 pos = m_cvWalls[i]->GetPosition();
		int x = (int)(pos.x - 24) / 48;
		int y = (int)(pos.y - 24) / 48;
		astar.AddCollision(x, y);
	}





	auto result = astar.FindPath(sx, sy, ex, ey);

	for (auto s : result)
	{
		Vector2 pos;

		pos.x = s.x*48.0f + 24.0f;
		pos.y = s.y*48.0f + 24.0f;
		Rect *rect = new Rect();
		rect->SetPosition(pos);
		rect->SetColor(Color(1, 0, 1,0.0f));
		rect->SetScale(Vector2(48, 48));
		m_cvPaths.push_back(rect);

	}





}

bool S18_AStarDemo::ExistWall(Vector2 pos)
{
	bool Value = false;

	for (UINT i = 0; i < m_cvWalls.size(); i++)
	{
		if (!m_cvWalls[i]->IsPtInRect(pos))
			continue;
		Value = true;
		break;
	}
	return Value;
}
