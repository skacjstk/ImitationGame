#pragma once
class S04_Extra01 : public Scene
{
public: // override
	void  Update()  override;
	void  Render()  override;
	void  ChangeScene() override;
	void  ExitScene() override;
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
	class    Line*  m_pGroundLine = nullptr;		// 최종적으로 Line 데이터가 저장되는 곳
	class    Line* m_pCeilingLine = nullptr;	// 추가: 벽과 천장용 Line
	class    Line*  m_pRubberBand = nullptr; // 마우스를 떼기 전까지 존재하는 라인 데이터(가장 마지막에 그린 선이기도 함)


public: // 생성자 & 소멸자
	S04_Extra01();
	~S04_Extra01();
public:
	void    EditLine();
	void	EditCelingLine();
};
