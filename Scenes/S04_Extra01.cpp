#include "ImitationGame/framework.h"
#include "S04_Extra01.h"
#include "./Object/Line.h"
// #include "./Object/Rect.h"
#include "./ImGUI/imgui.h"
#include <shellapi.h>
#include <commdlg.h>
//#include <codecvt>

 #define   MAPEDITOR

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
	// ImGUI�����쿡 ���콺�� Capture�Ǿ� ������ ���� ����
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
	if (Mouse->Up(0) )  // �����͸� ����
	{
		m_pLine2->ClearVertexBuffer();
		Vector2 start = m_pRubberBand->GetStartPoint(0);
		Vector2 end = m_pRubberBand->GetEndPoint(0);

		m_pLine2->AddLine(start.x, start.y, end.x, end.y);
		m_pLine2->EndLine();

		m_nMousePick = -1;
	}
	// ���콺�� �����̴� ����
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

	// �ſ� ���
	
	P = CAMERA->GetProjectionMatrix();
	V = CAMERA->GetViewMatrix();

	CAMERA->Update(V, P);
	TRNMANAGER->Update(V, P);

#ifdef MAPEDITOR


	// Mouse Move

	if (m_bImGuiWindow == false)
	{
		Vector2 pos = Mouse->GetPosition();  // Window
		CAMERA->WCtoVC(pos);                 // Widow coord --> DirectX coord
		m_pMoveTexture->SetPosition(pos);
	}
	if ((m_bImGuiWindow == false) && Mouse->Down(0)) // 1��ư�� ������ ���
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
	if ((m_bImGuiWindow == false) && Mouse->Press(0)) // 1��ư��ư�� ������ ������ ���
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
	if ((!m_bImGuiWindow) && Mouse->Press(2))	// ��Ŭ��(3��) ��ư�� ��� ������ ���
	{
		Vector2 pos = Mouse->GetPosition();
		CAMERA->WCtoVC(pos);		// W��ǥ -> V(DX)��ǥ 
		int x = -1;
		int y = -1;
		if (TRNMANAGER->GetMapXY(x, y, pos)) {
			TRNMANAGER->EraseTile(x, y);
			printf("��Ŭ������ Ÿ�� �����. %d %d\n", x, y);
		}
	}

#endif
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

void S04_Extra01::AddDrops(HDROP hDrop)
{
	//printf("AAAAAAAAAAAAAAAAAAAA\n");

}

void S04_Extra01::ShowGUI()
{
	ImGui::GetIO().NavActive = false;
	ImGui::GetIO().WantCaptureMouse = true;


	// Window����
	ImGui::Begin(u8"Map Editor",&m_bpush,ImGuiWindowFlags_MenuBar);

	// Menubar����
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
			if (ImGui::MenuItem(u8"PNG ���� ����(Fit)"))
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


	sprintf(buf, "%s", u8"Setting");

	if (!ImGui::CollapsingHeader(buf))
		return;

	// Scene���ÿ� �޺��ڽ�
	{
		const char*  items[] = { "Chapter1","Chapter2","Chapter3" };
		static int   nCurrentScene = 2;
		ImGui::Text(u8"Scene����");
		ImGui::SameLine(120.0f, 0.0f);

		ret = ImGui::Combo("##Scene", &nCurrentScene, items, IM_ARRAYSIZE(items));
		if (ret)
			printf(" %d\n", nCurrentScene);
	}
	// MAP ũ��
	{


		ImGui::Text(u8"MAP ũ��");
		ImGui::SameLine(120.0f, 0.0f);

		// �����̳�, �����ͺ��̽� �о���� ���� ó��

		ret = ImGui::InputInt2("##1", MapXY);
		if (ret)
		{
			if (MapXY[0] < 1)
				MapXY[0] = 1;
			if (MapXY[1] < 1)
				MapXY[1] = 1;
		}
	}
	// Pixel ũ��
	{
		

		ImGui::Text(u8"Pixel ũ��");
		ImGui::SameLine(120.0f, 0.0f);

		// �����̳�, �����ͺ��̽� �о���� ���� ó��

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

		ImGui::Text(u8"����");
		ImGui::SameLine(120.0f, 0.0f);

		// �����̳�, �����ͺ��̽� �о���� ���� ó��

		ret =  ImGui::InputFloat2("##3", Origin, "%0.2f",0);
		if (ret)
		{
	
		}

	}

	// Button
	{
		ImGui::Text(u8"����");
		ImGui::SameLine(120.0f, 0.0f);


		ImGui::PushStyleColor(ImGuiCol_Button, (ImVec4)ImColor::HSV(1.0f, 0.6f, 0.6f));
		ImGui::PushStyleColor(ImGuiCol_ButtonHovered, (ImVec4)ImColor::HSV(1.0f, 0.7f, 0.7f));
		ImGui::PushStyleColor(ImGuiCol_ButtonActive, (ImVec4)ImColor::HSV(1.0f, 0.8f, 0.7f));
		ret = ImGui::Button(u8"Rest");
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
		ret = ImGui::Button(u8"����������");
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
		ret = ImGui::Button(u8"�����");
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
		ret = ImGui::Button(u8"�����ϱ�");

		if (ret)
		{
			m_pLine2->SaveLine("./testcoord.txt");
		}

	}

	// Button
	{
		ImGui::SameLine();
		ret = ImGui::Button(u8"LOAD�ϱ�");

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
	// Display Order ���ÿ� �޺��ڽ�
	{
		const char*  items[] = { "0","1","2","3","4","5" };
		static int   nOrder = 0;

		nOrder = m_nDisplayOrder;
		ImGui::Text(u8"Display Order����");
		ImGui::SameLine(120.0f, 0.0f);

		ret = ImGui::Combo("##Display Order", &nOrder, items, IM_ARRAYSIZE(items));
		if (ret)
			m_nDisplayOrder = nOrder;
	}
	//���õ� Map ����
	{
		static int   items[] = { -1,-1 };

		items[0] = m_nSelectMapX;
		items[1] = m_nSelectMapY;
		ImGui::Text(u8"���õ� Map");
		ImGui::SameLine(120.0f, 0.0f);

		ret = ImGui::InputInt2("##Display Order2", items);
		if (ret)
		{
			m_nSelectMapX = items[0];
			m_nSelectMapY = items[1];
		}
		// button
		{
			ImGui::Text(u8"����");
			ImGui::SameLine(120.0f, 0.0f);

			ImGui::PushStyleColor(ImGuiCol_Button, (ImVec4)ImColor::HSV(1.0f, 0.6f, 0.6f));
			ImGui::PushStyleColor(ImGuiCol_ButtonHovered, (ImVec4)ImColor::HSV(1.0f, 0.7f, 0.7f));
			ImGui::PushStyleColor(ImGuiCol_ButtonActive, (ImVec4)ImColor::HSV(1.0f, 0.8f, 0.7f));
			ret = ImGui::Button(u8"�����������");
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
		
		if (ret)  // Button�� Click
		{
			Vector2 pos = Vector2(0.0f+size.x*1.5f , MAIN->GetHeight()*0.5f);    // Window ��ǥ
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




	// ���μ�

	Vector2  start = offset;
	start.x = start.x - size.x * 0.85f;
	start.y = start.y + size.y * 0.5f;
	for (int i = 0; i <= (y + 1); i++)
	{
		m_pLine->AddLine(start.x, start.y, offset.x + size.x*x, start.y);
		start.y = start.y - size.y;
	}

	// ���μ�

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
// WinApi���� GetOpenFileName()�� Call
// include �׸�   OPENFILENAME      --> #include  <shellapi.h>
//               GetOpenFileName()  --> #include  <commdlg.h> 
// MODAL Dialog���� --> �ҷ��� â������ ��� �����쵵 Active�ȵ�
////////////////////////////////////////////////////
void S04_Extra01::FileOpenDialog()
{
	OPENFILENAME OFN;
	WCHAR lpstrFile[2562] = L"";  // ��Ƽ �ִ빮��
	WCHAR lpCurrentDiretory[1024];
	wstring   CurrentDirectory;

	// ./sfsf/sfsf/aa.png
	GetCurrentDirectory(1024, lpCurrentDiretory);
	CurrentDirectory = lpCurrentDiretory;


	// Replace�� �ڵ�
	Replace(CurrentDirectory, L"\\", L"/");

	memset(&OFN, 0, sizeof(OPENFILENAME));
	OFN.lStructSize = sizeof(OPENFILENAME);
	OFN.hwndOwner = MAIN->GetWindowHandler();  // Modal(Wait)
	OFN.lpstrFilter = L"png File(*.png)\0*.png\0Text File\0*.png;*.png\0";  // ������ ����
	OFN.lpstrFile = lpstrFile;  // ���õǸ� ������ ������� ����
	OFN.nMaxFile = 2562;
	OFN.lpstrInitialDir = lpCurrentDiretory;
	OFN.Flags = OFN_HIDEREADONLY | OFN_NOCHANGEDIR | OFN_EXPLORER | OFN_ALLOWMULTISELECT;

	// OFN_HIDEREADONLY --> GetOpenFile
	// OFN_NOCHANGEDIR  --> Api����ϸ� �⺻������ ���� ������ ��ġ�� 
	//                      ����--> �ȵǰ�
	// OFN_EXPLORER | OFN_ALLOWMULTISELECT --> Multi Mode & ���̾�α�

	if (GetOpenFileName(&OFN) == 0)  // Modal
		return;
	const UINT    nMaxLen = 2562;
	WCHAR        szPath[nMaxLen];
	WCHAR        szFile[nMaxLen];


	UINT id = wcslen(OFN.lpstrFile) + 1;

	// ex) "C:/dds/sfsf/select.png\0select1.png\0"....

	// ���õ� ������ 1���� ���
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
	// ���õ� ������ 2�� �̻��� ���

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
	OFN.lpstrFilter = L"png File(*.png)\0*.png\0Text File\0*.png;*.png\0"; // ����
	OFN.lpstrFile = lpstrFile;  // ������ ������ Ȯ�ι�ư�� �������� ���õ� ������ 
	OFN.nMaxFile = 2562;        // Multi/Single --> 2562
	OFN.Flags = OFN_HIDEREADONLY | OFN_NOCHANGEDIR ;
	OFN.lpstrInitialDir = bufCurrentDirectory;

	// OFN_HIDEREADONLY   --> Read Only
	// OFN_NOCHANGEDIR    --> ���ϴ��̿÷��� ���� �����ϸ�, ���� ���丮 ��ġ ���� �ȵǰԲ�
	// OFN_EXPLORER ,OFN_ALLOWMULTISELECT --> Explore ���� �ְ�
	if (GetSaveFileName(&OFN) == 0)
		return L"";


	wstring strSaveFile = OFN.lpstrFile;
	return strSaveFile;
}


