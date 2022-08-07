#include "ImitationGame/framework.h"
#include "S04_Extra01.h"
#include "./Object/Line.h"
// #include "./Object/Rect.h"
#include "./ImGUI/imgui.h"
#include "Database/ObjectDB.h"
#include <shellapi.h>
#include <commdlg.h>
//#include <codecvt>

bool IsDrawingLine = false;		// Tab Ű: ������ �׸����� ������
bool DrawDivideLine = true;		// F1 Ű: ���м��� �׸����� ������
bool Div2Position = false;	// F2 Ű: ������ ���� ������ ���� ��ġ 0.5 ������ ���� ��.// ��ġ���� ��ĭ ���
int IsAxis = 0;	// ������ X���ΰ�?  X, Y, XY ������ �Űܰ�
float axisX = 0;
float axisY = 0;
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
static std::array<int, 2> objCombo;

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
	if (Mouse->Up(0))  // �����͸� ����
	{
		m_pGroundLine->ClearVertexBuffer();
		Vector2 start = m_pRubberBand->GetStartPoint(0);
		Vector2 end = m_pRubberBand->GetEndPoint(0);
		m_pGroundLine->AddLine(start.x, start.y, end.x, end.y);
		m_pGroundLine->EndLine();
		

		m_nMousePick = -1;

		m_pRubberBand->ClearLine();
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

void S04_Extra01::EditCeilingLine()
{
	// ImGUI�����쿡 ���콺�� Capture�Ǿ� ������ ���� ����
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
	if (Mouse->Up(0))  // �����͸� ����
	{
		m_pCeilingLine->ClearVertexBuffer();
		Vector2 start = m_pRubberBand->GetStartPoint(0);
		Vector2 end = m_pRubberBand->GetEndPoint(0);

		m_pCeilingLine->AddLine(start.x, start.y, end.x, end.y);
		m_pCeilingLine->EndLine();

		m_nMousePick = -1;
		m_pRubberBand->ClearLine();
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
	if (Mouse->Up(0))  // �����͸� ����
	{
		m_pPlatformLine->ClearVertexBuffer();
		Vector2 start = m_pRubberBand->GetStartPoint(0);
		Vector2 end = m_pRubberBand->GetEndPoint(0);

		m_pPlatformLine->AddLine(start.x, start.y, end.x, end.y);
		m_pPlatformLine->EndLine();

		m_nMousePick = -1;
		m_pRubberBand->ClearLine();
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
	strImage += L"LongSword.png";
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
	CheckOptionKeyboard();

	if(IsDrawingLine == false){

	// Mouse Move
		if (m_bImGuiWindow == false)
		{
			Vector2 pos = Mouse->GetPosition();  // Window
			CAMERA->WCtoVC(pos);                 // Widow coord --> DirectX coord

			if (Div2Position == true) {
				if (IsAxis == 0) {
				//	axisX = OPT.PixelXY[0] * (OPT.scaleXY[0] * 0.5f);
					axisX = OPT.PixelXY[0] * 0.5f * OPT.scaleXY[0];
					m_pMoveTexture->SetPosition(pos.x + axisX, pos.y);
					axisY = 0.0f;
				}
				else if (IsAxis == 1){
					axisY = OPT.PixelXY[1] * 0.5f * OPT.scaleXY[1];
					m_pMoveTexture->SetPosition(pos.x, pos.y - axisY);
					axisX = 0.0f;
				}
				else if (IsAxis == 2) {
					axisX = OPT.PixelXY[0] * 0.5f * OPT.scaleXY[0];
					axisY = OPT.PixelXY[1] * 0.5f * OPT.scaleXY[1];
					m_pMoveTexture->SetPosition(pos.x + axisX, pos.y - axisY);
				}
			}else
				m_pMoveTexture->SetPosition(pos);
		}
		if ((m_bImGuiWindow == false) && Mouse->Press(0)) // 1��ư��ư�� ������ ������ ���: Down ��������.
		{
			Vector2 pos = Mouse->GetPosition();  // Window
			CAMERA->WCtoVC(pos);                 // Widow coord --> DirectX coord
			int x = -1;
			int y = -1;
			if (Div2Position == false) {
				if (TRNMANAGER->GetMapXY(x, y, pos))
				{
					wstring strImageFile = m_pMoveTexture->GetImageFile();
					Vector2 offset = m_pMoveTexture->GetOffset();
					Vector2 offsetSize = m_pMoveTexture->GetOffsetSize();

					// 0728 ��� �߰�: object�� ��� ObjectDesc �� �и�. 0729Next: int ������ ���� ��ġ��� �߰��ϱ�
					TRNMANAGER->AddTile(x, y, m_nDisplayOrder, m_nTileType, m_nObjectType,
						strImageFile, offset, offsetSize, m_pMoveTexture->GetRotation());

					m_nSelectMapX = x;
					m_nSelectMapY = y;
				}
			}
			else  // Div2Position Ÿ�� Add
			{
				if (TRNMANAGER->GetMapXY(x, y, pos))
				{
					wstring strImageFile = m_pMoveTexture->GetImageFile();
					Vector2 offset = m_pMoveTexture->GetOffset();
					Vector2 offsetSize = m_pMoveTexture->GetOffsetSize();
					int half1 = 0;
					int half2 = 0;
					if (IsAxis == 0) 
						half1 = (int)(axisX / OPT.scaleXY[0]);
					else if(IsAxis == 1)
						half2 = (int)(axisY / OPT.scaleXY[1]);
					else if (IsAxis == 2) {
						half1 = (int)(axisX / OPT.scaleXY[0]);
						half2 = (int)(axisY / OPT.scaleXY[1]);
					}
					// 0728 ��� �߰�: object�� ��� ObjectDesc �� �и�. 0729Next: int ������ ���� ��ġ��� �߰��ϱ�
					TRNMANAGER->AddTileHalf(x, y, half1, half2, m_nDisplayOrder, m_nTileType, m_nObjectType,
						strImageFile, offset, offsetSize, m_pMoveTexture->GetRotation(), IsAxis);

					m_nSelectMapX = x;
					m_nSelectMapY = y;
				}
			}
		}
		if ((!m_bImGuiWindow) && Mouse->Press(2))	// ��Ŭ��(3��) ��ư�� ��� ������ ���
		{
			Vector2 pos = Mouse->GetPosition();
			CAMERA->WCtoVC(pos);		// W��ǥ -> V(DX)��ǥ 
				int x = -1;
			int y = -1;

			// ����°� �Ȱ��� �����.
			if (TRNMANAGER->GetMapXY(x, y, pos)) {
					TRNMANAGER->EraseTile(x, y);
					printf("��Ŭ������ Ÿ�� �����. %d %d\n", x, y);
			}
			
			
		}
	}
	else if (IsDrawingLine == true) {
		if ((!m_bImGuiWindow) && Mouse->Press(2))	// ��Ŭ��(3��) ��ư�� ��� ������ ���
		{
			Vector2 pos = Mouse->GetPosition();
			CAMERA->WCtoVC(pos);		// W��ǥ -> V(DX)��ǥ 

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
	if (DrawDivideLine == true)
		m_pLine->Render();
	m_pMoveTexture->Render();
	CAMERA->Render();
	m_pGroundLine->Render();	// �˻��� �� �Լ� ���η� �ű�
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
	LoadEditorOption("./Option.txt", true);	// �� �������� Floor, coords�� ���� �ʱ�ȭ��.
	TRNMANAGER->SetSceneObj(Floor, coords[0], coords[1]);
	m_pMoveTexture->SetScale(OPT.scaleXY[0], OPT.scaleXY[1]);
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
// Ű �����Ҹ��� ���� ����ۿ�
void S04_Extra01::CheckOptionKeyboard()
{

	if (KEYBOARD->Down(VK_TAB)) {
		IsDrawingLine = !IsDrawingLine;
	}
	// ���� �� �׸��� ����
	if (KEYBOARD->Down(VK_F1)) {
		DrawDivideLine = !DrawDivideLine;
	}
	if (Mouse->Down(4)) {	// ���콺 5��ư(��)
		Div2Position = !Div2Position;
	}
	if (KEYBOARD->Down('R')) {
		float z = m_pMoveTexture->GetRotation().z;
		m_pMoveTexture->SetRotation(0.0f, 0.0f, z + 90.0f);
	}
	if (Mouse->Down(3)) {	// ���콺 4��ư(�Ʒ�)
		++IsAxis %= 3;
	}
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
				//m_cvImageFiles �� �־��� 
			}
			ImGui::Separator();
			if (ImGui::MenuItem("Save Map File"))
			{
				//this->FileOpenDialog();		
				string coord = to_string(coords[0]) + to_string(coords[1]);
			//	int coord = (coords[0] * 10) + coords[1];	// 10 �̸��� ��ġ�� ����
				string objName = TEMPROOM_FOLDER;
				string mapName;
				objName += to_string(Floor) + "F/";
				mapName = objName;
				objName += coord + "ObjectDesc.txt";
				mapName += coord + "TileMap.txt";
				TRNMANAGER->SaveFile("./test.txt", mapName, objName);	// 0728 ����߰�: object�� ���� ����ǰ�
				
				SaveEditorOption("./Option.txt");	// �ߺ� ������� �ʰ� ����!
			}
			if (ImGui::MenuItem("Save PNG File"))
			{
				wstring coord = to_wstring(coords[0]) + to_wstring(coords[1]);
				wstring objName = TEMPROOM_FOLDER_W;
				objName += to_wstring(Floor) + L"F/";
				objName += coord + L"Terrain.png";
				TRNMANAGER->SavePNGFile(objName);
				/*
				wstring  saveFile = GetSaveFile();
				if (saveFile != L"")
					TRNMANAGER->SavePNGFile(saveFile);
				*/
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

	sprintf(buf, "%s", u8"Setting");

	if (!ImGui::CollapsingHeader(buf))
		return;

	// Scene���ÿ� �޺��ڽ�
	{
	//	
		ImGui::Text(u8"���� ���� ��ǥ: ");
		ret = ImGui::InputInt2("##5", coords);
		if (ret)
		{
			if (coords[0] < 0)
				coords[0] = 0;
			if (coords[1] < 0)
				coords[1] = 0;
		}
	}
	{
		ImGui::Text(u8"���� ��: ");
		ret = ImGui::InputInt("##6", &Floor);
		if (ret)
		{
			Floor = max(Floor, 1);	// 1���� ���� �� ����.
		}
	}
	// MAP ũ��
	{
		ImGui::Text(u8"MAP ũ��");
		ImGui::SameLine(120.0f, 0.0f);

		// �����̳�, �����ͺ��̽� �о���� ���� ó��

		ret = ImGui::InputInt2("##1", OPT.MapXY);
		if (ret)
		{
			if (OPT.MapXY[0] < 1)
				OPT.MapXY[0] = 1;
			if (OPT.MapXY[1] < 1)
				OPT.MapXY[1] = 1;
		}
	}
	// Pixel ũ��
	{
		ImGui::Text(u8"Pixel ũ��");
		ImGui::SameLine(120.0f, 0.0f);

		// �����̳�, �����ͺ��̽� �о���� ���� ó��
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
		ImGui::Text(u8"����");
		ImGui::SameLine(120.0f, 0.0f);

		// �����̳�, �����ͺ��̽� �о���� ���� ó��
		ret =  ImGui::InputFloat2("##3", OPT.Origin, "%0.2f",0);
		if (ret)
		{
	
		}
		ImGui::Text(u8"���� ��ǥ�� ��ȯ");
		ImGui::SameLine(120.0f, 0.0f);
		ImGui::PushStyleColor(ImGuiCol_Button, (ImVec4)ImColor::HSV(1.0f, 0.6f, 0.6f));
		ImGui::PushStyleColor(ImGuiCol_ButtonHovered, (ImVec4)ImColor::HSV(1.0f, 0.7f, 0.7f));
		ImGui::PushStyleColor(ImGuiCol_ButtonActive, (ImVec4)ImColor::HSV(1.0f, 0.8f, 0.8f));
		ret = ImGui::Button(u8"�߽� ��ǥ�� ��ȯ");
		ImGui::PopStyleColor(3);
		if (ret)
		{	// �� ������ �Ȱ��ص� �ȴ�. ������ ���� GetOldMapXY�� ����.
			OPT.Origin[0] = OPT.Origin[0] - (OPT.MapXY[0] * OPT.PixelXY[0]) * 0.5f;
			OPT.Origin[1] = OPT.Origin[1] + (OPT.MapXY[1] * OPT.PixelXY[1]) * 0.5f;
		}

		ImGui::SameLine();
		ImGui::PushStyleColor(ImGuiCol_Button, (ImVec4)ImColor::HSV(1.0f, 0.6f, 0.6f));
		ImGui::PushStyleColor(ImGuiCol_ButtonHovered, (ImVec4)ImColor::HSV(1.0f, 0.7f, 0.7f));
		ImGui::PushStyleColor(ImGuiCol_ButtonActive, (ImVec4)ImColor::HSV(1.0f, 0.8f, 0.8f));
		ret = ImGui::Button(u8"Window ��ǥ�� ��ȯ");
		ImGui::PopStyleColor(3);
		if (ret)
		{
			OPT.Origin[0] = OPT.Origin[0] + (OPT.MapXY[0] * OPT.PixelXY[0]) * 0.5f;
			OPT.Origin[1] = OPT.Origin[1] - (OPT.MapXY[1] * OPT.PixelXY[1]) * 0.5f;
		}
	}
	// ���� ũ��
	{
		ImGui::Text(u8"�� ���� ����");
		ImGui::SameLine(120.0f, 0.0f);

		// �����̳�, �����ͺ��̽� �о���� ���� ó��
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
		ImGui::Text(u8"����");
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
			TRNMANAGER->ApplyOldMagnification();
			LoadEditorOption("./Option.txt");
			m_pMoveTexture->SetScale(OPT.scaleXY[0], OPT.scaleXY[1]);
		}

		ImGui::SameLine();


		ImGui::PushStyleColor(ImGuiCol_Button, (ImVec4)ImColor::HSV(1.0f, 0.6f, 0.6f));
		ImGui::PushStyleColor(ImGuiCol_ButtonHovered, (ImVec4)ImColor::HSV(1.0f, 0.7f, 0.7f));
		ImGui::PushStyleColor(ImGuiCol_ButtonActive, (ImVec4)ImColor::HSV(1.0f, 0.8f, 0.7f));
		ret = ImGui::Button(u8"���ð� ����");
		ImGui::PopStyleColor(3);
		
		if (ret)
		{
			TRNMANAGER->ApplyOldOffset();
			TRNMANAGER->ApplyOldSize();
			TRNMANAGER->ApplyOldMagnification();
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
		ret = ImGui::Button(u8"�����");
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
		ImGui::Combo(u8"���� ��ǥ���� �ɼ�", &combo, items, ARRAYSIZE(items));
	
		if (oldcombo != combo)
			m_nMousePick = -1;
	
		if (combo == 1)
		{ 
			{
				ImGui::SameLine();
				ret = ImGui::Button(u8"G: �����ϱ�");
	
				if (ret)
				{
					string coord = to_string(coords[0]) + to_string(coords[1]);
					//	int coord = (coords[0] * 10) + coords[1];	// 10 �̸��� ��ġ�� ����
					string objName = TEMPROOM_FOLDER;
					objName += to_string(Floor) + "F/";
					objName += coord + "GroundLine.txt";
					m_pGroundLine->SaveLine(objName);
				}
			}
			// Button
			{
				ImGui::SameLine();
				ret = ImGui::Button(u8"G: LOAD�ϱ�");
	
				if (ret)
				{
					string coord = to_string(coords[0]) + to_string(coords[1]);
					//	int coord = (coords[0] * 10) + coords[1];	// 10 �̸��� ��ġ�� ����
					string objName = TEMPROOM_FOLDER;
					objName += to_string(Floor) + "F/";
					objName += coord + "GroundLine.txt";
					m_pGroundLine->LoadLine(objName);
				}
			}
		}// end combo1
		if (combo == 2)
		{	
			// Button
			{
				ImGui::SameLine();
				ret = ImGui::Button(u8"C: �����ϱ�");
	
				if (ret)
				{
					string coord = to_string(coords[0]) + to_string(coords[1]);
					//	int coord = (coords[0] * 10) + coords[1];	// 10 �̸��� ��ġ�� ����
					string objName = TEMPROOM_FOLDER;
					objName += to_string(Floor) + "F/";
					objName += coord + "CeilingLine.txt";
					m_pCeilingLine->SaveLine(objName);
				}
			}
			// Button
			{
				ImGui::SameLine();
				ret = ImGui::Button(u8"C: LOAD�ϱ�");
	
				if (ret)
				{
					string coord = to_string(coords[0]) + to_string(coords[1]);
					//	int coord = (coords[0] * 10) + coords[1];	// 10 �̸��� ��ġ�� ����
					string objName = TEMPROOM_FOLDER;
					objName += to_string(Floor) + "F/";
					objName += coord + "CeilingLine.txt";
					m_pCeilingLine->LoadLine(objName);
				}
			}
		}// end combo2
		if (combo == 3)
		{
			// Button
			{
				ImGui::SameLine();
				ret = ImGui::Button(u8"P: �����ϱ�");
	
				if (ret)
				{
					string coord = to_string(coords[0]) + to_string(coords[1]);
					//	int coord = (coords[0] * 10) + coords[1];	// 10 �̸��� ��ġ�� ����
					string objName = TEMPROOM_FOLDER;
					objName += to_string(Floor) + "F/";
					objName += coord + "PlatformLine.txt";
					m_pPlatformLine->SaveLine(objName);
				}
			}
			// Button
			{
				ImGui::SameLine();
				ret = ImGui::Button(u8"P: LOAD�ϱ�");
	
				if (ret)
				{
					string coord = to_string(coords[0]) + to_string(coords[1]);
					//	int coord = (coords[0] * 10) + coords[1];	// 10 �̸��� ��ġ�� ����
					string objName = TEMPROOM_FOLDER;
					objName += to_string(Floor) + "F/";
					objName += coord + "PlatformLine.txt";
					m_pPlatformLine->LoadLine(objName);
				}
			}
		}// end combo3
	}//end LineOption Combo

	// ������Ʈ Enemy��ġ ���
	{
		const char* items[] = { "None","SkelDog","BigWhiteSkel", "LittleGhost" };
		// EnemyCombo�� SelectTexture() ���ο��� �����ϱ� ���� ���� static ȭ 
		int  oldcombo = objCombo[0];
		ImGui::Combo("Enemy", &objCombo[0], items, ARRAYSIZE(items));

		if (oldcombo != objCombo[0]) {
			m_nMousePick = -1;
			m_nObjectType = objCombo[0];
			// moveTexture ��ü �� ��������
			if (objCombo[0] != 0) {
				SetOtherComboZero(0, objCombo[0]);
				m_pMoveTexture = objectDB.FindActorTexture(m_nObjectType);
				m_pMoveTexture->SetPosition(Mouse->GetPosition());
				m_pMoveTexture->SetScale(OPT.scaleXY[0], OPT.scaleXY[1]);
				m_pMoveTexture->SetRotation(0.0f, 0.0f, 0.0f);		// 0729 ȸ�� �ʱ�ȭ
				IsAxis = 0;
			}
		}
	}
	// ������Ʈ NPC��ġ ���
	{
		const char* items[] = { "None","Door", "Stele"};
		// EnemyCombo�� SelectTexture() ���ο��� �����ϱ� ���� ���� static ȭ 
		int  oldcombo = objCombo[1];
		ImGui::Combo("NPC", &objCombo[1], items, ARRAYSIZE(items));

		if (oldcombo != objCombo[1]) {
			m_nMousePick = -1;
			m_nObjectType = objCombo[1];
			m_nObjectType += 100;	// ��� ��ü�ڵ尡 101������. �׷��� 100�� ������� ��.
			// moveTexture ��ü �� ��������
			if (objCombo[1] != 0) {
				SetOtherComboZero(1, objCombo[1]);
				m_pMoveTexture = objectDB.FindActorTexture(m_nObjectType);
				m_pMoveTexture->SetPosition(Mouse->GetPosition());
				m_pMoveTexture->SetScale(OPT.scaleXY[0], OPT.scaleXY[1]);
				m_pMoveTexture->SetRotation(0.0f, 0.0f, 0.0f);
			}
		}
	}
}

void S04_Extra01::SelectTexture()
{
	bool ret = false;
	if (!ImGui::CollapsingHeader("Tile Slect"))
		return;	//m_nMaxDisplayOrder
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
			ImVec2(size.x*2.0, size.y*2.0), ImVec2(0.0f, 0.0f), ImVec2(1.0f, 1.0f));
		Count++;
		if (Count < 9)
			ImGui::SameLine();
		else
		{
			Count = 0;
		}
		
		if (ret)  // Button�� Click
		{
			SetOtherComboZero(-1, 0);		//EnemyCombo�� 0���� �ٲ�, moveTexture�� �����Ѱ� Tile�� �ǵ��� ��.
			Vector2 pos = Vector2(0.0f+size.x*1.5f , MAIN->GetHeight()*0.5f);    // Window ��ǥ
			CAMERA->WCtoVC(pos);
			m_nObjectType = 0;
			m_pMoveTexture->SetOffsetSize(size);
			m_pMoveTexture->SetImageFile(m_cvImageFiles[i]);
			m_pMoveTexture->SetPosition(pos);
			m_pMoveTexture->SetRotation(0.0f, 0.0f, 0.0f);
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
// ���� ���� ���� �Ǻ�: �⺻��: false. true ��ġ�� ChangeScene�� ����.
void S04_Extra01::LoadEditorOption(string strFileName, bool isTheFirst)
{
	FILE* fp = NULL;

	fp = fopen(strFileName.c_str(), "r");
	if (fp == NULL)
		return;

	const int size = 120;
	char buf[size] = { 0 };
	int readCount = 1;	// ũ�Ⱑ Ŀ�� �� ��������, �ǹ̾��� ����
	readCount = fread(buf, sizeof(buf), 1, fp);
	
	char* value = NULL;	
	// ���� ���ۿ� �����Ⱑ ���ٸ�
	if (buf[size - 1] != '\0')
		return;

	value = strstr(buf, "\n");
	value = value + 1;	// ���๮�� �ٷ� ��������

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
	if(isTheFirst == true)	// oldOffset�� �� newOffset�̱⿡ �̷��� ��.
		TRNMANAGER->SetOldOffset(OPT.Origin.x, OPT.Origin.y);
	ApplyEditorOption(isTheFirst);	// �޾ƿ� opt���� ����
	// �����, ���� Ÿ�� �����̱Ⱑ ����.
	fclose(fp);
}

void S04_Extra01::ApplyEditorOption(bool isTheFirst)
{	// ���� ����ÿ� Load�Ҷ� 
	TRNMANAGER->SetMapXY(OPT.MapXY[0], OPT.MapXY[1]);
	TRNMANAGER->SetTileSize(OPT.PixelXY[0], OPT.PixelXY[1]);
	TRNMANAGER->SetOffset(OPT.Origin.x, OPT.Origin.y);
	TRNMANAGER->SetMapScale(OPT.scaleXY[0], OPT.scaleXY[1]);
	if(isTheFirst == false)	// ó���� MoveTiles ���ص� ��.
		TRNMANAGER->MoveTiles();	// �������� ��ȭ���� ��� Tile�� ����, ���� offset�� old Offset�� ����ȭ
	ResetLineScale(Vector2(OPT.scaleXY[0], OPT.scaleXY[1]));	// m_pLine�� �ٲ۴�. ������ �����Ҷ� ������
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
/// <summary>
/// objCombo[exceptIndex] = value �� �����ϰ� �������� 0���� �����.
/// </summary>
/// <param name="exceptIndex">: ��ȭ���� ���� index ��ȣ, 0���� ���� ���� ������ ��� 0���� �ٲ��.</param>
/// <param name="value">: �� index�� ��.(�� ���� ���� �ؾ� ��)</param>
void S04_Extra01::SetOtherComboZero(int exceptIndex, int value)
{
	objCombo.fill(0);	// ���� 0���� �ٲٰ�
	// index��ȣ�� 0 �̻��̸� �־��ش�.
	if(exceptIndex >= 0)	
		objCombo[exceptIndex] = value;
}



/*
const char*  items[] = { "Chapter1","Chapter2","Chapter3" };
static int   nCurrentScene = 2;
ImGui::Text(u8"Scene����");
ImGui::SameLine(120.0f, 0.0f);

ret = ImGui::Combo("##Scene", &nCurrentScene, items, IM_ARRAYSIZE(items));
if (ret)
	printf(" %d\n", nCurrentScene);
*/