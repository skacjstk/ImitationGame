#pragma once
class TileOrder
{
public:
	TileOrder() {}
	~TileOrder() { printf(" 소멸자 호출\n"); }
public:
	int order = 0;	// 추정상 displayOrder
	wstring imageFile = L"";
	Vector2 offset = Vector2(0.0f, 0.0f);
	Vector2 offsetSize = Vector2(0.0f, 0.0f);
	Vector2 scale = Vector2(1.0f, 1.0f);
	int Flip = 0;	// X Flip, Y Flip (뒤집기 기준)
	float Angle = 0.0f;	// 라디안 or Degree 
};
class Tile : public GameObject
{
public:
	Tile();
	~Tile();

	void Update(Matrix V, Matrix P) override;
	void Render() override;
	void Reset() override;
public:	// Getter
	UINT GetTileOrderSize() { return (UINT)_orders.size(); }
	TileOrder* GetTile(UINT id);
public:	// Setter
	void SetOrder(int order) { _order = order; }
	void SetOrder(Texture* pTexture, int order, wstring strImageFile,
		Vector2 offset, Vector2 offsetSize, int Flip, float Angle, Vector2 scale);
	TileOrder* FindTileOrder(int order);
private:	// Fields
	vector<TileOrder*> _orders;
	int _order = 0;		// 다른 곳에서 어떤 Order 를 Rendering 할 지 정하는 변수
	Texture* _pTexture = nullptr;
};