#pragma once
class S04_Extra01 : public Scene
{
public: // override
	void  Update()  override;
	void  Render()  override;
	void  ChangeScene() override;
	void  AddDrops(HDROP hDrop);

private:
	void  ShowGUI();
	void  SettingMenu();
	void  SelectTexture();
	void  CreateGrid();
	void  FileOpenDialog();
	wstring  GetSaveFile();
private:
	vector<wstring>  m_cvImageFiles;
	Texture  *m_pMoveTexture = nullptr;
	class    Line     *m_pLine = nullptr;
	bool     m_bImGuiWindow = false;
	int      m_nDisplayOrder = 0;
	int      m_nTileType = 0;  // Water, Wall
	int      m_nObjectType = 0; // 0 tile, 1 object
	int      m_nSelectMapX = -1;
	int      m_nSelectMapY = -1;
	bool     m_bpush = false;
	void     Replace(wstring& str, wstring comp, wstring rep);
	int      m_nMousePick = -1;
	void     EditLine();
	class    Line*  m_pLine2 = nullptr;
	class    Line*  m_pRubberBand = nullptr;


public: // 持失切 & 社瑚切
	S04_Extra01();
	~S04_Extra01();
};
