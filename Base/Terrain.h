#pragma once

#define TRNMANAGER Terrain::GetInstance()
class Terrain  // 지형,
{
public:
	struct  BinaryMAP
	{
		int		 x = 0;
		int		 y = 0;
		Vector2  position = Vector2(0.0f,0.0f);  // class(x)
		char     imgname[80] = "";
		int      order = 0;
		Vector2  offset = Vector2(0.0f, 0.0f);
		Vector2  offsetSize = Vector2(0.0f, 0.0f);
		int      objectType = 0;
		int      Type = 0;
		int      Flip = 0;
		float    angle = 0.0f;
		Vector2  scale = Vector2(0.0f,0.0f);
	};

	Vector2 TerrainMagnification_ = Vector2(1.0f, 1.0f);	// 기본은 1.0, Scene 따라 배율이 달라지게 하려고 함.
public:
	static  Terrain* GetInstance()
	{
		static Terrain instance;
		return &instance;
	}
public:
	void    Update(Matrix V, Matrix P);
	void    Render();

public:  // Setter	
	void    Clear();
	void    AddTile(int X, int Y, int nOrder, int nType, int nObjectType, wstring strImageFile, Vector2 offset, Vector2 offsetSize);
	void    SetSceneMap(string sceneName, bool minimal = false);
	void    SetOffset(float x, float y) { m_Offset = Vector2(x, y); }
	void    SetOldOffset(float x, float y) { m_oldOffset = Vector2(x, y); }
	// m_oldOffset 갱신( SetOffset 에 있던 기능 분리 )
	void    ApplyOldOffset() { m_oldOffset = m_Offset; }
	void    SetTileSize(float x, float y) { m_Size = Vector2(x, y); }
	void    SetMapXY(int x, int y)        { m_MapXY = POINT{ x,y }; }
	void    EraseTile(int x, int y);
	void    SaveFile(string strFileName);
	void    SaveBinaryFile(string strFileName);
	void    OpenBinaryFile(string strFileName);
	void    SavePNGFile(wstring strPNGFile);
	void    SavePNGFitFile(wstring strPNGFile);
	void    SetOrderMax(int order);
	void    SetMapScale(float x, float y) { TerrainMagnification_.x = x, TerrainMagnification_.y = y;	}
public:  // Getter
	Vector2 GetOffset()					  { return m_Offset; }
	Vector2 GetOffsetWithMag() { return Vector2(m_Offset.x * TerrainMagnification_.x,
		m_Offset.y * TerrainMagnification_.y);
	}
	Vector2 GetOldOffset() { return m_oldOffset; }
	Vector2 GetTileSize()                 { return m_Size; }
	Vector2 GetTileSizeWithMag() { return Vector2(m_Size.x * TerrainMagnification_.x, 
		m_Size.y * TerrainMagnification_.y); }
	POINT   GetMapXY()                    { return m_MapXY; }
	bool    GetMapXY(int& x, int&y, Vector2 position);  
	int     GetOrderMax() { return m_nMaxDisplayOrder; }
	Vector2 GetMapScale() { return TerrainMagnification_; }
	void	MoveTiles();
private:
	class    Tile* FindTile(wstring strMap);
	void     OpenFile(string strFileName, bool minimal = false);
	Vector2  m_Offset = Vector2(-500.0f, 800.0f);
	Vector2  m_Size   = Vector2(100.0f, 100.0f);
	POINT    m_MapXY = POINT{ 6,10 };
	Texture* m_pTexture = nullptr;  // 텍스쳐는 한개만 사용
	Vector2  m_oldOffset = Vector2(0.0f, 0.0f);
	map<wstring, class Tile*>     m_cmTiles;
	int      m_nMaxDisplayOrder = 0;
private:
	Terrain();
public:
	~Terrain();
};

