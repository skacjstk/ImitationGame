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
	};


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
	void    SetSceneMap(string sceneName);
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
public:  // Getter
	Vector2 GetOffset()					  { return m_Offset; }
	Vector2 GetTileSize()                 { return m_Size; }
	POINT   GetMapXY()                    { return m_MapXY; }
	bool    GetMapXY(int& x, int&y, Vector2 position);  
	int     GetOrderMax() { return m_nMaxDisplayOrder; }
private:
	class    Tile* FindTile(wstring strMap);
	void     OpenFile(string strFileName);
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

