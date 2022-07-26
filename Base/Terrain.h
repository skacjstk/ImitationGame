#pragma once

#define TRNMANAGER Terrain::GetInstance()
class Terrain  // 지형,
{
public:
	struct  BinaryMAP
	{
		int		 x;
		int		 y;
		Vector2  position;  // class(x)
		char     imgname[80];
		int      order;
		Vector2  offset;
		Vector2  offsetSize;
		int      objectType;
		int      Type;
		int      Flip;
		float    angle;
		Vector2  scale;
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
	void    AddTile(int X, int Y, int nOrder, int nType, int nObjectType,
		wstring strImageFile,
		Vector2 offset, Vector2 offsetSize);
	void    SetSceneMap(string sceneName, bool minimal = false);
	void    SetOffset(float x, float y)   { m_Offset = Vector2(x, y); }
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
	Vector2 GetTileSize()                 { return m_Size; }
	Vector2 GetTileSizeWithMag() { return Vector2(m_Size.x * TerrainMagnification_.x, 
		m_Size.y * TerrainMagnification_.y); }
	POINT   GetMapXY()                    { return m_MapXY; }
	bool    GetMapXY(int& x, int&y, Vector2 position);  
	int     GetOrderMax() { return m_nMaxDisplayOrder; }
	Vector2 GetMapScale() { return TerrainMagnification_; }
private:
	class    Tile* FindTile(wstring strMap);
	void     OpenFile(string strFileName, bool minimal = false);
	Vector2  m_Offset = Vector2(-500.0f, 800.0f);
	Vector2  m_Size   = Vector2(100.0f, 100.0f);
	POINT    m_MapXY = POINT{ 6,10 };
	Texture* m_pTexture = nullptr;  // 텍스쳐는 한개만 사용
	map<wstring, class Tile*>     m_cmTiles;
	int      m_nMaxDisplayOrder = 0;
private:
	Terrain();
public:
	~Terrain();
};

