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
// Tile이 가진 _order 로 Texture를 찾아 해당 Texture의 좌표를 설정하고 Update 해줌 
void Tile::Update(Matrix V, Matrix P)
{
	TileOrder* pTileOrder = FindTileOrder(_order);
	if (!pTileOrder)
		return;
	// Render 에서 처리함. 일종의 꼼수 
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
	_pTexture->SetPosition(GetPosition());
	_pTexture->Update(V, P);
	_pTexture->Render();

	// 임시조치때 안바뀐 이유: eye up at 을 기본값으로 바꾸고 Update를 했었음.
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

void Tile::SetOrder(Texture* pTexture, int order, wstring strImageFile, Vector2 offset, Vector2 offsetSize, int Flip, float Angle)
{
	_pTexture = pTexture;
	TileOrder* pTileOrder = FindTileOrder(order);
	if (!pTileOrder) {	// 참이 아닐 경우 새로 생성, 참이면 이미 있는거니 데이터 수정만
		// TileOrder 의 
		pTileOrder = new TileOrder();
		_orders.push_back(pTileOrder);
	}
	pTileOrder->order = order;
	pTileOrder->imageFile = strImageFile;
	pTileOrder->offset = offset;
	pTileOrder->offsetSize = offsetSize;
	pTileOrder->Flip = Flip;
	pTileOrder->Angle = Angle;
}
// 안나오면 nullptr 반환. 
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