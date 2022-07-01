#pragma once
class TileOrder
{
public:
	TileOrder() {}
	~TileOrder() { printf(" �Ҹ��� ȣ��\n"); }
public:
	int order = 0;	// ������ displayOrder
	wstring imageFile = L"";
	Vector2 offset = Vector2(0.0f, 0.0f);
	Vector2 offsetSize = Vector2(0.0f, 0.0f);
	int Flip = 0;	// X Flip, Y Flip (������ ����)
	float Angle = 0.0f;	// ���� or Degree 
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
		Vector2 offset, Vector2 offsetSize, int Flip, float Angle);
	TileOrder* FindTileOrder(int order);
private:	// Fields
	vector<TileOrder*> _orders;
	int _order = 0;		// �ٸ� ������ � Order �� Rendering �� �� ���ϴ� ����
	Texture* _pTexture = nullptr;
};