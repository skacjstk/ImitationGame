#include "ImitationGame/framework.h"
#include "ImitationGame/MainWindow.h"
#include "Tile.h"

Tile::Tile()
{
}

Tile::~Tile()
{
	for (UINT i = 0; i < _orders.size(); ++i)
		delete _orders[i];	
	_orders.erase(_orders.begin(), _orders.end());
}
// Tile�� ���� _order �� Texture�� ã�� �ش� Texture�� ��ǥ�� �����ϰ� Update ���� 
void Tile::Update(Matrix V, Matrix P)
{
	return;
	TileOrder* pTileOrder = FindTileOrder(_order);
	if (!pTileOrder)
		return;
	// Render ���� ó����. ������ �ļ� 
//	_pTexture->SetImageFile(pTileOrder->imageFile);
//	_pTexture->SetOffset(pTileOrder->offset);
//	Vector2 size = SRVMANAGER->FindSpriteSize(pTileOrder->imageFile);
//	if (pTileOrder->offsetSize.x < 0.1f) {
//		pTileOrder->offsetSize.x = size.x;
//	}
//	if (pTileOrder->offsetSize.y < 0.1f) {
//		pTileOrder->offsetSize.y = size.y;
//	}
//	_pTexture->SetOffsetSize(pTileOrder->offsetSize);
//	_pTexture->SetPosition(GetPosition());	
//	_pTexture->Update(V, P);
}

void Tile::Render()
{
	TileOrder* pTileOrder = FindTileOrder(_order);
	if (!pTileOrder)
		return;
	Matrix V, P, R;
	P = CAMERA->GetProjectionMatrix();
	V = CAMERA->GetViewMatrix();

	_pTexture->SetImageFile(pTileOrder->imageFile);
	_pTexture->SetOffset(pTileOrder->offset);
	Vector2 size = SRVMANAGER->FindSpriteSize(pTileOrder->imageFile);
	if (pTileOrder->offsetSize.x < 0.1f) {
		pTileOrder->offsetSize.x = size.x;
	}
	if (pTileOrder->offsetSize.y < 0.1f) {
		pTileOrder->offsetSize.y = size.y;
	}
	_pTexture->SetOffsetSize(pTileOrder->offsetSize);
	_pTexture->SetRotation(0.0f, 0.0f, pTileOrder->Angle);
	_pTexture->SetScale(pTileOrder->scale.x * TRNMANAGER->GetMapScale().x,
		pTileOrder->scale.y * TRNMANAGER->GetMapScale().y);
	_pTexture->SetPosition(GetPosition().x * TRNMANAGER->GetMapScale().x, GetPosition().y * TRNMANAGER->GetMapScale().y);
	_pTexture->Update(V, P);
	_pTexture->Render();

	// �ӽ���ġ�� �ȹٲ� ����: eye up at �� �⺻������ �ٲٰ� Update�� �߾���.
}

void Tile::Reset()
{
}

TileOrder* Tile::GetTile(UINT id)
{
	if (id >= _orders.size() || id < 0)
		return nullptr;
	return _orders[id];
}

void Tile::SetOrder(Texture* pTexture, int order, wstring strImageFile, Vector2 offset, Vector2 offsetSize, int Flip, float Angle, Vector2 scale, int objectType)
{
	_pTexture = pTexture;
	TileOrder* pTileOrder = FindTileOrder(order);
	if (!pTileOrder) {	// ���� �ƴ� ��� ���� ����, ���̸� �̹� �ִ°Ŵ� ������ ������
		// TileOrder �� 
		pTileOrder = new TileOrder();
		_orders.push_back(pTileOrder);
	}
	pTileOrder->order = order;
	pTileOrder->imageFile = strImageFile;
	pTileOrder->offset = offset;
	pTileOrder->offsetSize = offsetSize;
	pTileOrder->scale = scale;
	pTileOrder->Flip = Flip;
	pTileOrder->Angle = Angle;
	pTileOrder->objectType = objectType;
}
// �ȳ����� nullptr ��ȯ. 
TileOrder* Tile::FindTileOrder(int order)
{
	TileOrder* pTileOrder = nullptr;

	for (UINT i = 0; i < _orders.size(); ++i) {
		if (order != _orders[i]->order)
			continue;
		pTileOrder = _orders[i];
		break;
	}
	return pTileOrder;
}