#include "ImitationGame/framework.h"
#include "S04_Extra01.h"
#include "./Object/Line.h"
// #include "./Object/Rect.h"
#include "./ImGUI/imgui.h"
#include "Base/EditorObjects.h"
#include <shellapi.h>
#include <commdlg.h>
//#include <codecvt>

bool IsDrawingLine = false;
bool saftyPress = true;		// F1 키: 이제 line 수가 0이면 안그리게 했으니까 더미
	typedef struct MapEditorOption
	{
		Vector2  Origin = Vector2(0.0f, 0.0f);
		int PixelXY[2] = { 100,100 };
		int MapXY[4] = { 5,10,20,20 };
		float scaleXY[2] = { 1.0f, 1.0f };
	}MapEditorOption;
static MapEditorOption OPT;

static int coords[2] = { 0,0 };
static int Floor = 1;
static int enemyCombo;

void S04_Extra01::Replace(wstring & str, wstring comp, wstring rep)
{
	wstring temp = str;

	size_t start_pos = 0;
	while ((start_pos = temp.find(comp, start_pos)) != wstring::npos)
	{
		temp.replace(start_pos, comp.length(), rep);
		start_pos += rep.length();
	}
	str = temp;

}

void S04_Extra01::EditLine()
{
	// ImGUI윈도우에 마우스가 Capture되어 있으면 동작 안함
//	if (ImGui::GetIO().WantCaptureMouse)  
//		return;

	if (Mouse->Down(0))
	{
		m_nMousePick = 1;
		m_pRubberBand->ClearLine();
		
		Vector2 line = Mouse->GetPosition();
		CAMERA->WCtoVC(line);
		m_pRubberBand->AddLine(line.x, line.y, line.x, line.y);
		m_pRubberBand->EndLine();
	}
	if (Mouse->Up(0))  // 데이터를 저장
	{
		m_pGroundLine->ClearVertexBuffer();
		Vector2 start = m_pRubberBand->GetStartPoint(0);
		Vector2 end = m_pRubberBand->GetEndPoint(0);
		m_pGroundLine->AddLine(start.x, start.y, end.x, end.y);
		m_pGroundLine->EndLine();
		

		m_nMousePick = -1;

		m_pRubberBand->ClearLine();
	}
	// 마우스가 움직이는 상태
	if (m_nMousePick == 1)
	{
		Vector2 start = m_pRubberBand->GetStartPoint(0);
		Vector2 end = Mouse->GetPosition();
		CAMERA->WCtoVC(end);
		m_pRubberBand->ClearLine();
		m_pRubberBand->AddLine(start.x, start.y, end.x, end.y);
		m_pRubberBand->EndLine();
	}
}

void S04_Extra01::EditCeilingLine()
{
	// ImGUI윈도우에 마우스가 Capture되어 있으면 동작 안함
//	if (ImGui::GetIO().WantCaptureMouse)  
//		return;

	if (Mouse->Down(0))
	{
		m_nMousePick = 1;
		m_pRubberBand->ClearLine();

		Vector2 line = Mouse->GetPosition();
		CAMERA->WCtoVC(line);
		m_pRubberBand->AddLine(line.x, line.y, line.x, line.y);
		m_pRubberBand->EndLine();
	}
	if (Mouse->Up(0) && saftyPress)  // 데이터를 저장
	{
		m_pCeilingLine->ClearVertexBuffer();
		Vector2 start = m_pRubberBand->GetStartPoint(0);
		Vector2 end = m_pRubberBand->GetEndPoint(0);

		m_pCeilingLine->AddLine(start.x, start.y, end.x, end.y);
		m_pCeilingLine->EndLine();

		m_nMousePick = -1;
		m_pRubberBand->ClearLine();
	}
	// 마우스가 움직이는 상태
	if (m_nMousePick == 1)
	{
		Vector2 start = m_pRubberBand->GetStartPoint(0);
		Vector2 end = Mouse->GetPosition();
		CAMERA->WCtoVC(end);
		m_pRubberBand->ClearLine();
		m_pRubberBand->AddLine(start.x, start.y, end.x, end.y);
		m_pRubberBand->EndLine();
	}
}

void S04_Extra01::EditPlatformLine()
{
	if (Mouse->Down(0))
	{
		m_nMousePick = 1;
		m_pRubberBand->ClearLine();

		Vector2 line = Mouse->GetPosition();
		CAMERA->WCtoVC(line);
		m_pRubberBand->AddLine(line.x, line.y, line.x, line.y);
		m_pRubberBand->EndLine();
	}
	if (Mouse->Up(0) && saftyPress)  // 데이터를 저장
	{
		m_pPlatformLine->ClearVertexBuffer();
		Vector2 start = m_pRubberBand->GetStartPoint(0);
		Vector2 end = m_pRubberBand->GetEndPoint(0);

		m_pPlatformLine->AddLine(start.x, start.y, end.x, end.y);
		m_pPlatformLine->EndLine();

		m_nMousePick = -1;
		m_pRubberBand->ClearLine();
	}
	// 마우스가 움직이는 상태
	if (m_nMousePick == 1)
	{
		Vector2 start = m_pRubberBand->GetStartPoint(0);
		Vector2 end = Mouse->GetPosition();
		CAMERA->WCtoVC(end);
		m_pRubberBand->ClearLine();
		m_pRubberBand->AddLine(start.x, start.y, end.x, end.y);
		m_pRubberBand->EndLine();
	}
}

void S04_Extra01::ResetLineScale(Vector2 trnMag)
{
	m_pLine->lineScale_ = trnMag;
//	m_pGroundLine->lineScale_ = trnMag;
//	m_pCeilingLine->lineScale_ = trnMag;
//	m_pRubberBand->lineScale_ = trnMag;
//	m_pPlatformLine->lineScale_ = trnMag;
}

S04_Extra01::S04_Extra01()
{
	m_strSceneName = "MapEditor";
	SetActive(false);
	m_pGroundLine = new Line();
	m_pCeilingLine = new Line();
	m_pRubberBand = new Line();
	m_pPlatformLine = new Line();
	m_pGroundLine->SetColor(Color(1.0f, 1.0f, 0.0f, 1.0f));
	m_pCeilingLine->SetColor(Color(0.0f, 1.0f, 1.0f, 1.0f));
	m_pRubberBand->SetColor(Color(1.0f,0.0f,1.0f, 1.0f));
	m_pPlatformLine->SetColor(Color(0.0f, 0.5f, 0.5f, 1.0f));

	wstring strImage = IMAGE_FOLDER;
	strImage += L"Button/GameStart.png";
	wstring  strShader = SHADER_FOLDER;
		strShader += L"Texture.hlsl";
	m_pMoveTexture = new Texture(strImage, strShader);
	m_pLine = new Line();
	CreateGrid();
}

S04_Extra01::~S04_Extra01()
{
	
}

void S04_Extra01::Update()
{
	Matrix V, P;

	// 신에 사용

	P = CAMERA->GetProjectionMatrix();
	V = CAMERA->GetViewMatrix();

	CAMERA->Update(V, P);
//	TRNMANAGER->Update(V, P);

	if (KEYBOARD->Down(VK_TAB)) {
		IsDrawingLine = !IsDrawingLine;
	}

	if(IsDrawingLine == false){

	// Mouse Move
		if (m_bImGuiWindow == false)
		{
			Vector2 pos = Mouse->GetPosition();  // Window
			CAMERA->WCtoVC(pos);                 // Widow coord --> DirectX coord
			m_pMoveTexture->SetPosition(pos);
		}
		if ((m_bImGuiWindow == false) && Mouse->Press(0)) // 1버튼이튼이 지속적 눌려진 경우: Down 제거했음.
		{
			Vector2 pos = Mouse->GetPosition();  // Window
			CAMERA->WCtoVC(pos);                 // Widow coord --> DirectX coord
			int x = -1;
			int y = -1;
			if (TRNMANAGER->GetMapXY(x, y, pos))
			{
				wstring strImageFile = m_pMoveTexture->GetImageFile();
				Vector2 offset = m_pMoveTexture->GetOffset();
				Vector2 offsetSize = m_pMoveTexture->GetOffsetSize();
								
				// 0728 기능 추가: object일 경우: 
				TRNMANAGER->AddTile(x, y, m_nDisplayOrder, m_nTileType, m_nObjectType,
					strImageFile, offset, offsetSize);
				
				m_nSelectMapX = x;
				m_nSelectMapY = y;	
			}
		}
		if ((!m_bImGuiWindow) && Mouse->Press(2))	// 우클릭(3번) 버튼이 계속 눌리는 경우
		{
			Vector2 pos = Mouse->GetPosition();
			CAMERA->WCtoVC(pos);		// W좌표 -> V(DX)좌표 
				int x = -1;
			int y = -1;
			if (TRNMANAGER->GetMapXY(x, y, pos)) {
				TRNMANAGER->EraseTile(x, y);
				printf("우클릭으로 타일 지우기. %d %d\n", x, y);
			}
			
		}
	}
	else if (IsDrawingLine == true) {
		if ((!m_bImGuiWindow) && Mouse->Press(2))	// 우클릭(3번) 버튼이 계속 눌리는 경우
		{
			Vector2 pos = Mouse->GetPosition();
			CAMERA->WCtoVC(pos);		// W좌표 -> V(DX)좌표 

			m_pGroundLine->EraseLine(pos, 2.0f);
			m_pCeilingLine->EraseLine(pos, 2.0f);
			m_pPlatformLine->EraseLine(pos, 2.0f);
			m_pRubberBand->ClearLine();
		}
	}
	m_pMoveTexture->Update(V, P);
	m_pLine->Update(V, P);
	m_pGroundLine->Update(V, P);
	m_pCeilingLine->Update(V, P);
	m_pPlatformLine->Update(V, P);
	m_pRubberBand->Update(V, P);
}

void S04_Extra01::Render()
{

	TRNMANAGER->Render();
	m_pLine->Render();
	m_pMoveTexture->Render();
	CAMERA->Render();
	m_pGroundLine->Render();	// 검산을 이 함수 내부로 옮김
	m_pCeilingLine->Render();
	m_pRubberBand->Render();
	m_pPlatformLine->Render();

	
	// imGui Rendering
	{
		ImGui_ImplDX11_NewFrame();
		ImGui_ImplWin32_NewFrame();
		ImGui::NewFrame();
	}
	ShowGUI();
	{
		ImGui::Render();
		ImGui_ImplDX11_RenderDrawData(ImGui::GetDrawData());
	}


}

void S04_Extra01::ChangeScene()
{
	TRNMANAGER->SetSceneMap("test");
	LoadEditorOption("./Option.txt", true);	// 이 시점에서 Floor, coords도 같이 초기화됨.
	TRNMANAGER->SetSceneObj(Floor, coords[0], coords[1]);
	SetActive(true);
}

void S04_Extra01::ExitScene()
{
	SetActive(false);
}

void S04_Extra01::AddDrops(HDROP hDrop)
{
	//printf("AAAAAAAAAAAAAAAAAAAA\n");

}

void S04_Extra01::ShowGUI()
{
	ImGui::GetIO().NavActive = false;
	ImGui::GetIO().WantCaptureMouse = true;


	// Window생성
	ImGui::Begin(u8"Map Editor",&m_bpush,ImGuiWindowFlags_MenuBar);

	// Menubar생성
	if (ImGui::BeginMenuBar())
	{
		if (ImGui::BeginMenu("File"))
		{
			if (ImGui::MenuItem("Load PNG File"))
			{
				this->FileOpenDialog();
				//m_cvImageFiles 에 넣어짐 
			}
			ImGui::Separator();
			if (ImGui::MenuItem("Save Map File"))
			{
				//this->FileOpenDialog();		
				string coord = to_string(coords[0]) + to_string(coords[1]);
			//	int coord = (coords[0] * 10) + coords[1];	// 10 미만의 위치가 뻑남
				string objName = TEMPROOM_FOLDER;
				objName += to_string(Floor) + "F/";
				objName += coord + "ObjectDesc.txt";
				TRNMANAGER->SaveFile("./test.txt", objName);	// 0728 기능추가: object도 같이 저장되게
				
				SaveEditorOption("./Option.txt");	// 중복 저장되지 않게 주의!
			}
			if (ImGui::MenuItem("Save PNG File"))
			{
				int coord = (coords[0] * 10) + coords[1];
				wstring objName = TEMPROOM_FOLDER_W;
				objName += to_wstring(Floor) + L"F/";
				objName += to_wstring(coord) + L"Terrain.txt";
				TRNMANAGER->SavePNGFile(objName);
				/*
				wstring  saveFile = GetSaveFile();
				if (saveFile != L"")
					TRNMANAGER->SavePNGFile(saveFile);
				*/
			}
			if (ImGui::MenuItem(u8"PNG 파일 저장(Fit)"))
			{
				wstring  saveFile = GetSaveFile();
				if(saveFile!=L"")
					TRNMANAGER->SavePNGFitFile(saveFile);
			}
			ImGui::EndMenu();
		}
		ImGui::EndMenuBar();
	}

	if (ImGui::IsWindowHovered())
		m_bImGuiWindow = true;
	else
		m_bImGuiWindow = false;
	

	SettingMenu();
	SelectTexture();
	if(ImGui::IsAnyItemHovered())
		m_bImGuiWindow = true;
	else
		m_bImGuiWindow = false;
	ImGui::End();

}

void S04_Extra01::SettingMenu()
{
	char  buf[120];
	bool  ret = false;

	sprintf(buf, "%s", u8"Setting");

	if (!ImGui::CollapsingHeader(buf))
		return;

	// Scene선택용 콤보박스
	{
	//	
		ImGui::Text(u8"현재 던전 좌표: ");
		ret = ImGui::InputInt2("##5", coords);
		if (ret)
		{
			if (coords[0] < 1)
				coords[0] = 1;
			if (coords[1] < 1)
				coords[1] = 1;
		}
	}
	{
		ImGui::Text(u8"현재 층: ");
		ret = ImGui::InputInt("##6", &Floor);
		if (ret)
		{
			Floor = max(Floor, 1);	// 1보다 작을 수 없음.
		}
	}
	// MAP 크기
	{
		ImGui::Text(u8"MAP 크기");
		ImGui::SameLine(120.0f, 0.0f);

		// 파일이나, 데이터베이스 읽어오는 것이 처리

		ret = ImGui::InputInt2("##1", OPT.MapXY);
		if (ret)
		{
			if (OPT.MapXY[0] < 1)
				OPT.MapXY[0] = 1;
			if (OPT.MapXY[1] < 1)
				OPT.MapXY[1] = 1;
		}
	}
	// Pixel 크기
	{
		ImGui::Text(u8"Pixel 크기");
		ImGui::SameLine(120.0f, 0.0f);

		// 파일이나, 데이터베이스 읽어오는 것이 처리
		ret = ImGui::InputInt2("##2", OPT.PixelXY);
		if (ret)
		{
			if (OPT.PixelXY[0] < 16)
				OPT.PixelXY[0] = 16;
			if (OPT.PixelXY[1] < 16)
				OPT.PixelXY[1] = 16;
		}		
	}
	// Offset
	{
		ImGui::Text(u8"원점");
		ImGui::SameLine(120.0f, 0.0f);

		// 파일이나, 데이터베이스 읽어오는 것이 처리
		ret =  ImGui::InputFloat2("##3", OPT.Origin, "%0.2f",0);
		if (ret)
		{
	
		}
	}
	// 배율 크기
	{
		ImGui::Text(u8"맵 배율 설정");
		ImGui::SameLine(120.0f, 0.0f);

		// 파일이나, 데이터베이스 읽어오는 것이 처리
		ret = ImGui::InputFloat2("##4", OPT.scaleXY);
		if (ret)
		{
			if (OPT.scaleXY[0] < 0.0f)
				OPT.scaleXY[0] = 0.0001f;
			if (OPT.scaleXY[1] < 0.0f)
				OPT.scaleXY[1] = 0.0001f;
		}
	}
	// Button
	{
		ImGui::Text(u8"적용");
		ImGui::SameLine(120.0f, 0.0f);


		ImGui::PushStyleColor(ImGuiCol_Button, (ImVec4)ImColor::HSV(1.0f, 0.6f, 0.6f));
		ImGui::PushStyleColor(ImGuiCol_ButtonHovered, (ImVec4)ImColor::HSV(1.0f, 0.7f, 0.7f));
		ImGui::PushStyleColor(ImGuiCol_ButtonActive, (ImVec4)ImColor::HSV(1.0f, 0.8f, 0.7f));
		ret = ImGui::Button(u8"Reset");
		ImGui::PopStyleColor(3);

		if (ret)
		{
			TRNMANAGER->ApplyOldOffset();
			TRNMANAGER->ApplyOldSize();
			LoadEditorOption("./Option.txt");
			m_pMoveTexture->SetScale(OPT.scaleXY[0], OPT.scaleXY[1]);
		}

		ImGui::SameLine();


		ImGui::PushStyleColor(ImGuiCol_Button, (ImVec4)ImColor::HSV(1.0f, 0.6f, 0.6f));
		ImGui::PushStyleColor(ImGuiCol_ButtonHovered, (ImVec4)ImColor::HSV(1.0f, 0.7f, 0.7f));
		ImGui::PushStyleColor(ImGuiCol_ButtonActive, (ImVec4)ImColor::HSV(1.0f, 0.8f, 0.7f));
		ret = ImGui::Button(u8"세팅값 적용");
		ImGui::PopStyleColor(3);
		
		if (ret)
		{
			TRNMANAGER->ApplyOldOffset();
			TRNMANAGER->ApplyOldSize();
			ApplyEditorOption();
			m_pMoveTexture->SetScale(OPT.scaleXY[0], OPT.scaleXY[1]);
		}

	}
	// Button
	{
		ImGui::SameLine();

		ImGui::PushStyleColor(ImGuiCol_Button, (ImVec4)ImColor::HSV(1.0f, 0.6f, 0.6f));
		ImGui::PushStyleColor(ImGuiCol_ButtonHovered, (ImVec4)ImColor::HSV(1.0f, 0.7f, 0.7f));
		ImGui::PushStyleColor(ImGuiCol_ButtonActive, (ImVec4)ImColor::HSV(1.0f, 0.8f, 0.7f));
		ret = ImGui::Button(u8"지우기");
		ImGui::PopStyleColor(3);

		if (ret)
		{

			TRNMANAGER->Clear();
			TRNMANAGER->SetOldOffset(OPT.Origin.x, OPT.Origin.y);
			TRNMANAGER->SetOffset(OPT.Origin.x, OPT.Origin.y);
			CreateGrid();
		}

	}

	// Button
// Combo
	{
		const char* items[] = { "None","Ground","Ceiling", "Platform"};
		static int combo;
		int  oldcombo = combo;
		ImGui::Combo("Background", &combo, items, ARRAYSIZE(items));

		if (oldcombo != combo)
			m_nMousePick = -1;

		if (IsDrawingLine == true) {
			if (combo == 1)
				EditLine();
			if (combo == 2)
				EditCeilingLine();
			if (combo == 3)
				EditPlatformLine();
		}
	}
	// LineCombo
	{
		const char* items[] = { "None","Ground","Ceiling", "Platform" };
		static int combo;
		int  oldcombo = combo;
		ImGui::Combo(u8"층과 좌표기준 옵션", &combo, items, ARRAYSIZE(items));
	
		if (oldcombo != combo)
			m_nMousePick = -1;
	
		if (combo == 1)
		{ 
			{
				ImGui::SameLine();
				ret = ImGui::Button(u8"G: 저장하기");
	
				if (ret)
				{
					int coord = (coords[0] * 10) + coords[1];
					string objName = TEMPROOM_FOLDER;
					objName += to_string(Floor) + "F/";
					objName += to_string(coord) + "GroundLine.txt";
					m_pGroundLine->SaveLine(objName);
				}
			}
			// Button
			{
				ImGui::SameLine();
				ret = ImGui::Button(u8"G: LOAD하기");
	
				if (ret)
				{
					int coord = (coords[0] * 10) + coords[1];
					string objName = TEMPROOM_FOLDER;
					objName += to_string(Floor) + "F/";
					objName += to_string(coord) + "GroundLine.txt";
					m_pGroundLine->LoadLine(objName);
				}
			}
		}// end combo1
		if (combo == 2)
		{	
			// Button
			{
				ImGui::SameLine();
				ret = ImGui::Button(u8"C: 저장하기");
	
				if (ret)
				{
					int coord = (coords[0] * 10) + coords[1];
					string objName = TEMPROOM_FOLDER;
					objName += to_string(Floor) + "F/";
					objName += to_string(coord) + "CeilingLine.txt";
					m_pCeilingLine->SaveLine(objName);
				}
			}
			// Button
			{
				ImGui::SameLine();
				ret = ImGui::Button(u8"C: LOAD하기");
	
				if (ret)
				{
					int coord = (coords[0] * 10) + coords[1];
					string objName = TEMPROOM_FOLDER;
					objName += to_string(Floor) + "F/";
					objName += to_string(coord) + "CeilingLine.txt";
					m_pCeilingLine->LoadLine(objName);
				}
			}
		}// end combo2
		if (combo == 3)
		{
			// Button
			{
				ImGui::SameLine();
				ret = ImGui::Button(u8"P: 저장하기");
	
				if (ret)
				{
					int coord = (coords[0] * 10) + coords[1];
					string objName = TEMPROOM_FOLDER;
					objName += to_string(Floor) + "F/";
					objName += to_string(coord) + "PlatformLine.txt";
					m_pPlatformLine->SaveLine(objName);
				}
			}
			// Button
			{
				ImGui::SameLine();
				ret = ImGui::Button(u8"P: LOAD하기");
	
				if (ret)
				{
					int coord = (coords[0] * 10) + coords[1];
					string objName = TEMPROOM_FOLDER;
					objName += to_string(Floor) + "F/";
					objName += to_string(coord) + "PlatformLine.txt";
					m_pPlatformLine->LoadLine(objName);
				}
			}
		}// end combo3
	}//end LineOption Combo

	// 오브젝트 배치 모드
	{
		const char* items[] = { "None","SkelDog","BigWhiteSkel", "LittleGhost" };
		// EnemyCombo를 SelectTexture() 내부에서 참조하기 위해 전역 static 화 
		int  oldcombo = enemyCombo;
		ImGui::Combo("Enemy", &enemyCombo, items, ARRAYSIZE(items));

		if (oldcombo != enemyCombo) {
			m_nMousePick = -1;
			m_nObjectType = enemyCombo;
			// moveTexture 교체 및 배율적용
			if (enemyCombo != 0) {
				m_pMoveTexture = enemyDB.FindActorTexture(m_nObjectType);
				m_pMoveTexture->SetPosition(Mouse->GetPosition());
				m_pMoveTexture->SetScale(OPT.scaleXY[0], OPT.scaleXY[1]);
			}
		}
	}
}

void S04_Extra01::SelectTexture()
{
	bool ret = false;
	if (!ImGui::CollapsingHeader("Tile Slect"))
		return;	//m_nMaxDisplayOrder
	// Display Order 선택용 콤보박스
	{
		const char*  items[] = { "0","1","2","3","4","5" };
		static int   nOrder = 0;
		nOrder = m_nDisplayOrder;
		ImGui::Text(u8"Display Order선택");
		ImGui::SameLine(120.0f, 0.0f);

		ret = ImGui::Combo("##Display Order", &nOrder, items, IM_ARRAYSIZE(items));
		if (ret)
			m_nDisplayOrder = nOrder;
	}
	//선택된 Map 보기
	{
		static int   items[] = { -1,-1 };

		items[0] = m_nSelectMapX;
		items[1] = m_nSelectMapY;
		ImGui::Text(u8"선택된 Map");
		ImGui::SameLine(120.0f, 0.0f);

		ret = ImGui::InputInt2("##Display Order2", items);
		if (ret)
		{
			m_nSelectMapX = items[0];
			m_nSelectMapY = items[1];
		}
		// button
		{
			ImGui::Text(u8"적용");
			ImGui::SameLine(120.0f, 0.0f);

			ImGui::PushStyleColor(ImGuiCol_Button, (ImVec4)ImColor::HSV(1.0f, 0.6f, 0.6f));
			ImGui::PushStyleColor(ImGuiCol_ButtonHovered, (ImVec4)ImColor::HSV(1.0f, 0.7f, 0.7f));
			ImGui::PushStyleColor(ImGuiCol_ButtonActive, (ImVec4)ImColor::HSV(1.0f, 0.8f, 0.7f));
			ret = ImGui::Button(u8"데이터지우기");
			ImGui::PopStyleColor(3);
			if (ret)
			{
				TRNMANAGER->EraseTile(items[0], items[1]);

			}


		}

	}
	// Button
	





	ImVec2  scroll_size = ImVec2(0.0f,
		ImGui::GetFrameHeightWithSpacing() * 14 + 30);

	// Scroll 
	ImGui::BeginChild("scroll texture", scroll_size, true,
		ImGuiWindowFlags_HorizontalScrollbar);

	int Count = 0;
	for (UINT i = 0; i < m_cvImageFiles.size(); i++)
	{
		Vector2 size = SRVMANAGER->FindSpriteSize(m_cvImageFiles[i]);
		ret = ImGui::ImageButton(SRVMANAGER->CreateShaderResourceView(m_cvImageFiles[i]),
			ImVec2(size.x*2.0, size.y*2.0), ImVec2(0.0f, 0.0f), ImVec2(1.0f, 1.0f));
		Count++;
		if (Count < 9)
			ImGui::SameLine();
		else
		{
			Count = 0;
		}
		
		if (ret)  // Button이 Click
		{
			enemyCombo = 0;	// EnemyCombo를 0으로 바꿔, moveTexture가 선택한게 Tile이 되도록 함.
			Vector2 pos = Vector2(0.0f+size.x*1.5f , MAIN->GetHeight()*0.5f);    // Window 좌표
			CAMERA->WCtoVC(pos);
			m_pMoveTexture->SetOffsetSize(size);
			m_pMoveTexture->SetImageFile(m_cvImageFiles[i]);
			m_pMoveTexture->SetPosition(pos);
			m_pMoveTexture->Update(CAMERA->GetViewMatrix(),
				CAMERA->GetProjectionMatrix());
		}
			
	}
	ImGui::EndChild();



}
void S04_Extra01::CreateGrid()
{
	m_pLine->ClearLine();

	Vector2 offset = TRNMANAGER->GetOffset();
	Vector2 size = TRNMANAGER->GetTileSize();
	int   x = TRNMANAGER->GetMapXY().x;
	int   y = TRNMANAGER->GetMapXY().y;

	if (size.x < 16 || size.y < 16)
	{
		Vector2  start = offset;
		start.x = start.x - size.x * 0.85f;
		start.y = start.y + size.y * 0.5f;
		m_pLine->AddLine(start.x, start.y, offset.x + size.x*x, start.y);
		m_pLine->AddLine(start.x, start.y-size.y*y, offset.x + size.x*x, start.y-size.y*y);

		start = offset;
		start.x = start.x - size.x * 0.5f;
		start.y = start.y + size.y * 0.5f;

		m_pLine->AddLine(start.x, start.y, start.x , offset.y - size.y*(y + 1));
	
		
		m_pLine->CreateVertexBuffer();
		return;

	}




	// 가로선

	Vector2  start = offset;
	start.x = start.x - size.x * 0.85f;
	start.y = start.y + size.y * 0.5f;
	for (int i = 0; i <= (y + 1); i++)
	{
		m_pLine->AddLine(start.x, start.y, offset.x + size.x*x, start.y);
		start.y = start.y - size.y;
	}

	// 세로선

	start = offset;
	start.x = start.x - size.x * 0.5f;
	start.y = start.y + size.y * 0.5f;

	for (int i = 0; i <= x; i++)
	{
		m_pLine->AddLine(start.x, start.y, start.x, offset.y - size.y*(y + 1));
		start.x = start.x + size.x;
	}
	m_pLine->CreateVertexBuffer();
}
////////////////////////////////////////////////////
// FileSelectionDialog
// WinApi에서 GetOpenFileName()를 Call
// include 항목   OPENFILENAME      --> #include  <shellapi.h>
//               GetOpenFileName()  --> #include  <commdlg.h> 
// MODAL Dialog생성 --> 불러진 창에서는 어떠한 윈도우도 Active안됨
////////////////////////////////////////////////////
void S04_Extra01::FileOpenDialog()
{
	OPENFILENAME OFN;
	WCHAR lpstrFile[2562] = L"";  // 멀티 최대문자
	WCHAR lpCurrentDiretory[1024];
	wstring   CurrentDirectory;

	// ./sfsf/sfsf/aa.png
	GetCurrentDirectory(1024, lpCurrentDiretory);
	CurrentDirectory = lpCurrentDiretory;


	// Replace용 코드
	Replace(CurrentDirectory, L"\\", L"/");

	memset(&OFN, 0, sizeof(OPENFILENAME));
	OFN.lStructSize = sizeof(OPENFILENAME);
	OFN.hwndOwner = MAIN->GetWindowHandler();  // Modal(Wait)
	OFN.lpstrFilter = L"png File(*.png)\0*.png\0Text File\0*.png;*.png\0";  // 우측에 필터
	OFN.lpstrFile = lpstrFile;  // 선택되면 내요이 담겨지는 버퍼
	OFN.nMaxFile = 2562;
	OFN.lpstrInitialDir = lpCurrentDiretory;
	OFN.Flags = OFN_HIDEREADONLY | OFN_NOCHANGEDIR | OFN_EXPLORER | OFN_ALLOWMULTISELECT;

	// OFN_HIDEREADONLY --> GetOpenFile
	// OFN_NOCHANGEDIR  --> Api사용하면 기본적으러 현재 디렉토이 위치가 
	//                      변경--> 안되게
	// OFN_EXPLORER | OFN_ALLOWMULTISELECT --> Multi Mode & 다이얼로그

	if (GetOpenFileName(&OFN) == 0)  // Modal
		return;
	const UINT    nMaxLen = 2562;
	WCHAR        szPath[nMaxLen];
	WCHAR        szFile[nMaxLen];


	UINT id = wcslen(OFN.lpstrFile) + 1;

	// ex) "C:/dds/sfsf/select.png\0select1.png\0"....

	// 선택된 파일이 1개인 경우
	if (OFN.lpstrFile[id] == NULL)
	{
		wcscpy(szFile, OFN.lpstrFile);
		wstring str = szFile;

		int oldSize;
		Replace(str, L"\\", L"/");
		oldSize = str.length();
		Replace(str, CurrentDirectory, L"");
		if (oldSize != str.length())
			str = L"." + str;
		m_cvImageFiles.push_back(str);
		return;
	}
	// 선택된 파일이 2개 이상인 경우

	m_cvImageFiles.clear();

	wcscpy(szPath, OFN.lpstrFile);
	wstring  strDir = szPath;
	for (int i = wcslen(OFN.lpstrFile) + 1; NULL != OFN.lpstrFile[i]; i += (wcslen(OFN.lpstrFile + i) + 1))
	{
		wcscpy(szFile, OFN.lpstrFile + i);
		wstring str = szFile;
		wstring file = strDir + L"/" + str;

		int oldSize;
		Replace(file, L"\\", L"/");
		oldSize = file.length();
		Replace(file, CurrentDirectory, L"");
		if (oldSize != file.length())
			file = L"." + file;
		m_cvImageFiles.push_back(file);
	}
}

void S04_Extra01::SaveEditorOption(string strFileName)
{
	FILE* op = NULL;

	op = fopen(strFileName.c_str(), "w");
	if (op == NULL)
		return;

	fprintf(op, "#Floor coordsX Y	OriginX	Y,	PixelX	Y,	MapX	Y,	scaleX	Y\n");
	fprintf(op, "%d %d %d %f %f %d %d %d %d %f %f\0",
		Floor,
		coords[0],
		coords[1],
		OPT.Origin.x,
		OPT.Origin.y,
		OPT.PixelXY[0],
		OPT.PixelXY[1],
		OPT.MapXY[0],
		OPT.MapXY[1],
		OPT.scaleXY[0],
		OPT.scaleXY[1]
	);
	fclose(op);
}
// 최초 적용 여부 판별: 기본값: false. true 위치는 ChangeScene에 있음.
void S04_Extra01::LoadEditorOption(string strFileName, bool isTheFirst)
{
	FILE* fp = NULL;

	fp = fopen(strFileName.c_str(), "r");
	if (fp == NULL)
		return;

	const int size = 120;
	char buf[size] = { 0 };
	int readCount = 1;	// 크기가 커서 다 못읽으니, 의미없는 변수
	readCount = fread(buf, sizeof(buf), 1, fp);
	
	char* value = NULL;	
	// 읽은 버퍼에 끝내기가 없다면
	if (buf[size - 1] != '\0')
		return;

	value = strstr(buf, "\n");
	value = value + 1;	// 개행문자 바로 다음으로

	if (value != NULL) {
		sscanf_s(value, "%d %d %d %f %f %d %d %d %d %f %f",
			&Floor,
			&coords[0],
			&coords[1],
			&OPT.Origin.x,
			&OPT.Origin.y,
			&OPT.PixelXY[0],
			&OPT.PixelXY[1],
			&OPT.MapXY[0],
			&OPT.MapXY[1],
			&OPT.scaleXY[0],
			&OPT.scaleXY[1]
		);
	}
	if(isTheFirst == true)	// oldOffset이 곧 newOffset이기에 이렇게 함.
		TRNMANAGER->SetOldOffset(OPT.Origin.x, OPT.Origin.y);
	ApplyEditorOption(isTheFirst);	// 받아온 opt들을 적용
	// 참고로, 여긴 타일 움직이기가 없음.
	fclose(fp);
}

void S04_Extra01::ApplyEditorOption(bool isTheFirst)
{	// 최초 적용시엔 Load할때 
	TRNMANAGER->SetMapXY(OPT.MapXY[0], OPT.MapXY[1]);
	TRNMANAGER->SetTileSize(OPT.PixelXY[0], OPT.PixelXY[1]);
	TRNMANAGER->SetOffset(OPT.Origin.x, OPT.Origin.y);
	TRNMANAGER->SetMapScale(OPT.scaleXY[0], OPT.scaleXY[1]);
	if(isTheFirst == false)	// 처음엔 MoveTiles 안해도 됨.
		TRNMANAGER->MoveTiles();	// 오프셋의 변화량을 모든 Tile에 적용, 이후 offset과 old Offset을 동기화
	ResetLineScale(Vector2(OPT.scaleXY[0], OPT.scaleXY[1]));	// m_pLine만 바꾼다. 나머진 저장할때 나눠져
	CreateGrid();
}

wstring S04_Extra01::GetSaveFile()
{
	OPENFILENAME OFN;
	WCHAR lpstrFile[2562] = L"";

	WCHAR  bufCurrentDirectory[1024];
	GetCurrentDirectory(1024, bufCurrentDirectory);


	memset(&OFN, 0, sizeof(OPENFILENAME));
	OFN.lStructSize = sizeof(OPENFILENAME);
	OFN.hwndOwner = MAIN->GetWindowHandler();  // Modal
	OFN.lpstrFilter = L"png File(*.png)\0*.png\0Text File\0*.png;*.png\0"; // 필터
	OFN.lpstrFile = lpstrFile;  // 파일을 선택후 확인버튼이 눌려지면 선택된 내역이 
	OFN.nMaxFile = 2562;        // Multi/Single --> 2562
	OFN.Flags = OFN_HIDEREADONLY | OFN_NOCHANGEDIR ;
	OFN.lpstrInitialDir = bufCurrentDirectory;

	// OFN_HIDEREADONLY   --> Read Only
	// OFN_NOCHANGEDIR    --> 파일다이올러그 열고 선택하면, 현재 디렉토리 위치 변경 안되게끔
	// OFN_EXPLORER ,OFN_ALLOWMULTISELECT --> Explore 보여 주게
	if (GetSaveFileName(&OFN) == 0)
		return L"";


	wstring strSaveFile = OFN.lpstrFile;
	return strSaveFile;
}



/*
const char*  items[] = { "Chapter1","Chapter2","Chapter3" };
static int   nCurrentScene = 2;
ImGui::Text(u8"Scene선택");
ImGui::SameLine(120.0f, 0.0f);

ret = ImGui::Combo("##Scene", &nCurrentScene, items, IM_ARRAYSIZE(items));
if (ret)
	printf(" %d\n", nCurrentScene);
*/