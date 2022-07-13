#include "ImitationGame/framework.h"
#include "S04_Extra01.h"
#include "./Object/Line.h"
// #include "./Object/Rect.h"
#include "./ImGUI/imgui.h"
#include <shellapi.h>
#include <commdlg.h>
//#include <codecvt>

bool IsDrawingLine = false;

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
//	if (ImGui::GetIO().WantCaptureMouse)  return;

	if (Mouse->Down(0))
	{
		m_nMousePick = 1;
		m_pRubberBand->ClearLine();

		Vector2 line = Mouse->GetPosition();
		CAMERA->WCtoVC(line);
		m_pRubberBand->AddLine(line.x, line.y, line.x, line.y);
		m_pRubberBand->EndLine();
	}
	if (Mouse->Up(0) )  // 데이터를 저장
	{
		m_pLine2->ClearVertexBuffer();
		Vector2 start = m_pRubberBand->GetStartPoint(0);
		Vector2 end = m_pRubberBand->GetEndPoint(0);

		m_pLine2->AddLine(start.x, start.y, end.x, end.y);
		m_pLine2->EndLine();

		m_nMousePick = -1;
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

S04_Extra01::S04_Extra01()
{
	m_strSceneName = "MapEditor";
	SetActive(false);

	m_pLine2 = new Line();
	m_pRubberBand = new Line();
	/*

	TRNMANAGER->SetSceneMap("SceneEX01");
	m_cvImageFiles.push_back(L"./Image/Floor/Floor1.png");
	m_cvImageFiles.push_back(L"./Image/Floor/Floor2.png");
	m_cvImageFiles.push_back(L"./Image/Floor/Floor3.png");
	m_cvImageFiles.push_back(L"./Image/Floor/Floor4.png");
	m_cvImageFiles.push_back(L"./Image/Floor/Floor5.png");
	m_cvImageFiles.push_back(L"./Image/Floor/Floor6.png");
	m_cvImageFiles.push_back(L"./Image/Floor/Floor7.png");
	m_cvImageFiles.push_back(L"./Image/Floor/Floor8.png");
	m_cvImageFiles.push_back(L"./Image/Floor/SideFrost1.png");
	m_cvImageFiles.push_back(L"./Image/Floor/SideFrost3.png");
	m_cvImageFiles.push_back(L"./Image/Floor/SideFrost4.png");
	m_cvImageFiles.push_back(L"./Image/Floor/SideFrost5.png");
	m_cvImageFiles.push_back(L"./Image/Floor/SideFrost6.png");
	m_cvImageFiles.push_back(L"./Image/Floor/SideFrost7.png");
	m_cvImageFiles.push_back(L"./Image/Floor/SideFrost8.png");
	m_cvImageFiles.push_back(L"./Image/Floor/Wall.png");

	*/


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
	TRNMANAGER->Update(V, P);

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
		if ((m_bImGuiWindow == false) && Mouse->Down(0)) // 1번튼이 눌려진 경우
		{
			Vector2 pos = Mouse->GetPosition();  // Window
			CAMERA->WCtoVC(pos);                 // Widow coord --> DirectX coord
			int x = -1;
			int y = -1;
			if (TRNMANAGER->GetMapXY(x, y, pos))
			{
				wstring strImageFile = m_pMoveTexture->GetImageFile();
				Vector2 offset    = m_pMoveTexture->GetOffset();
				Vector2 offsetSize = m_pMoveTexture->GetOffsetSize();
				TRNMANAGER->AddTile(x, y, m_nDisplayOrder, m_nTileType, m_nObjectType,
					strImageFile, offset, offsetSize);
				m_nSelectMapX = x;
				m_nSelectMapY = y;
			}
		}
		if ((m_bImGuiWindow == false) && Mouse->Press(0)) // 1버튼이튼이 지속적 눌려진 경우
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

			m_pLine2->EraseLine(pos, 2.0f);
			m_pRubberBand->ClearLine();
		}
	}

	m_pMoveTexture->Update(V, P);
	m_pLine->Update(V, P);
	m_pLine2->Update(V, P);;
	m_pRubberBand->Update(V, P);
}

void S04_Extra01::Render()
{

	TRNMANAGER->Render();
	m_pLine->Render();
	m_pMoveTexture->Render();
	CAMERA->Render();
	m_pLine2->Render();
	m_pRubberBand->Render();
	
	
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
			}
			ImGui::Separator();
			if (ImGui::MenuItem("Save Map File"))
			{
				//this->FileOpenDialog();				
				TRNMANAGER->SaveFile("./test.txt");
			}
			if (ImGui::MenuItem("Save PNG File"))
			{
				wstring  saveFile = GetSaveFile();
				if (saveFile != L"")
				TRNMANAGER->SavePNGFile(saveFile);
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

	static Vector2  Origin = Vector2(100.0f, 100.0f);
	static int PixelXY[2] = { 100,100 };
	static int MapXY[4] = { 5,10,20,20 };
	static float scaleXY[2] = { 1.0f, 1.0f };


	sprintf(buf, "%s", u8"Setting");

	if (!ImGui::CollapsingHeader(buf))
		return;

	// Scene선택용 콤보박스
	{
		const char*  items[] = { "Chapter1","Chapter2","Chapter3" };
		static int   nCurrentScene = 2;
		ImGui::Text(u8"Scene선택");
		ImGui::SameLine(120.0f, 0.0f);

		ret = ImGui::Combo("##Scene", &nCurrentScene, items, IM_ARRAYSIZE(items));
		if (ret)
			printf(" %d\n", nCurrentScene);
	}
	// MAP 크기
	{
		ImGui::Text(u8"MAP 크기");
		ImGui::SameLine(120.0f, 0.0f);

		// 파일이나, 데이터베이스 읽어오는 것이 처리

		ret = ImGui::InputInt2("##1", MapXY);
		if (ret)
		{
			if (MapXY[0] < 1)
				MapXY[0] = 1;
			if (MapXY[1] < 1)
				MapXY[1] = 1;
		}
	}
	// Pixel 크기
	{
		ImGui::Text(u8"Pixel 크기");
		ImGui::SameLine(120.0f, 0.0f);

		// 파일이나, 데이터베이스 읽어오는 것이 처리
		ret = ImGui::InputInt2("##2", PixelXY);
		if (ret)
		{
			if (PixelXY[0] < 1)
				PixelXY[0] = 1;
			if (PixelXY[1] < 1)
				PixelXY[1] = 1;
		}		
	}
	// Offset
	{
		ImGui::Text(u8"원점");
		ImGui::SameLine(120.0f, 0.0f);

		// 파일이나, 데이터베이스 읽어오는 것이 처리
		ret =  ImGui::InputFloat2("##3", Origin, "%0.2f",0);
		if (ret)
		{
	
		}
	}
	// 배율 크기
	{
		ImGui::Text(u8"맵 배율 설정");
		ImGui::SameLine(120.0f, 0.0f);

		// 파일이나, 데이터베이스 읽어오는 것이 처리
		ret = ImGui::InputFloat2("##4", scaleXY);
		if (ret)
		{
			if (scaleXY[0] < 0.0f)
				scaleXY[0] = 0.0001f;
			if (scaleXY[1] < 0.0f)
				scaleXY[1] = 0.0001f;
			TRNMANAGER->SetMapScale(scaleXY[0], scaleXY[1]);
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
			MapXY[0] = MAIN->GetWidth();
			MapXY[1] = MAIN->GetHeight();
			PixelXY[0] = 1;
			PixelXY[1] = 1;
		}

		ImGui::SameLine();


		ImGui::PushStyleColor(ImGuiCol_Button, (ImVec4)ImColor::HSV(1.0f, 0.6f, 0.6f));
		ImGui::PushStyleColor(ImGuiCol_ButtonHovered, (ImVec4)ImColor::HSV(1.0f, 0.7f, 0.7f));
		ImGui::PushStyleColor(ImGuiCol_ButtonActive, (ImVec4)ImColor::HSV(1.0f, 0.8f, 0.7f));
		ret = ImGui::Button(u8"데이터저장");
		ImGui::PopStyleColor(3);
		
		if (ret)
		{
			TRNMANAGER->SetMapXY(MapXY[0], MapXY[1]);
			TRNMANAGER->SetTileSize(PixelXY[0], PixelXY[1]);
			TRNMANAGER->SetOffset(Origin.x, Origin.y);
			CreateGrid();
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
			TRNMANAGER->SetOffset(Origin.x, Origin.y);
			CreateGrid();
		}

	}

	// Button
// Combo
	{
		const char* items[] = { "None","Line" };
		static int combo;
		int  oldcombo = combo;
		ImGui::Combo("Background", &combo, items, ARRAYSIZE(items));

		if (oldcombo != combo)
			m_nMousePick = -1;

		if (combo == 1)
			EditLine();

	}

	// Button
	{
		ImGui::SameLine();
		ret = ImGui::Button(u8"저장하기");

		if (ret)
		{
			m_pLine2->SaveLine("./testcoord.txt");
		}

	}

	// Button
	{
		ImGui::SameLine();
		ret = ImGui::Button(u8"LOAD하기");

		if (ret)
		{
			m_pLine2->LoadLine("./testcoord.txt");
		}

	}

}

void S04_Extra01::SelectTexture()
{
	bool ret = false;
	if (!ImGui::CollapsingHeader("Tile Slect"))
		return;
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
			ImVec2(size.x*0.3, size.y*0.3), ImVec2(0.0f, 0.0f), ImVec2(1.0f, 1.0f));
		Count++;
		if (Count < 9)
			ImGui::SameLine();
		else
		{
			Count = 0;
		}
		
		if (ret)  // Button이 Click
		{
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


