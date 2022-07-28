
#include  "ImitationGame/framework.h"
#include  "Terrain.h"
#include "./Scenes/Room.h"
#include  "./Object/Tile.h"
#include  "./Render/Graphic2D.h"

///////////////////////////////////////////////////////////
// ������
//////////////////////////////////////////////////////////
Terrain::Terrain()
{
	// ���ſ� ���� Texture��  ���������� ����ϱ� ���Ͽ� 
	// MAPX * MAPY Size�� ����

	wstring  strImage = IMAGE_FOLDER;
	strImage += L"char/Adventurer/CharHand.png";
	wstring strShader = SHADER_FOLDER;
	strShader += L"Texture.hlsl";

	// Texture�� �ϳ��� ������ ��밡��

	m_pTexture = new Texture(strImage, strShader);
//	SetSceneMap("test");
}

Terrain::~Terrain()
{
	SAFE_DELETE(m_pTexture);
	for (auto v : m_cmTiles)
	{
		SAFE_DELETE(v.second);
	}
	m_cmTiles.clear();

}
void Terrain::Clear()
{
	for (auto v : m_cmTiles)
	{
		SAFE_DELETE(v.second);
	}
	m_cmTiles.clear();
}

void Terrain::Update(Matrix V, Matrix P)
{
	for (int i = 0; i <= m_nMaxDisplayOrder; i++)  // order max��
	{
		for (auto v : m_cmTiles)
		{
			v.second->SetOrder(i);
			v.second->Update(V, P);
		}	
	}
}

void Terrain::Render()
{
	for (int i = 0; i <= m_nMaxDisplayOrder; i++)  // order max��
	{
		for (auto v : m_cmTiles)
		{
			v.second->SetOrder(i);
			v.second->Render();
		}
	}

}



void Terrain::AddTile(int X, int Y, int nOrder, int nType, int nObjectType, wstring strImageFile, Vector2 offset, Vector2 offsetSize)
{
	Tile   *pTile = nullptr;
	wstring strMap = to_wstring(X) + L"," + to_wstring(Y);
	Vector2 position = Vector2(m_Offset.x + X * m_Size.x,
		m_Offset.y - Y * m_Size.y);

	pTile = FindTile(strMap);
	if (!pTile)
	{
		pTile = new Tile();
		m_cmTiles.insert(make_pair(strMap, pTile));
	}

	if (m_nMaxDisplayOrder < nOrder)
		m_nMaxDisplayOrder = nOrder;

	// nType�� TileType�ε� �ǹ� ����.


	SRVMANAGER->CreateShaderResourceView(strImageFile);
	pTile->SetPosition(position);
	pTile->SetOrder(m_pTexture, nOrder, strImageFile, offset, offsetSize, 0, 0.0f, Vector2(1.0f, 1.0f), nObjectType);
	

}
// flag�� True �� ���, Map�� Magnification( ��Ÿ �ʼ��ɼ� )�� �޾ƿ�.
void Terrain::SetSceneMap(string sceneName, bool minimal)
{
	string str = "./" + sceneName + ".txt";
	OpenFile(str, minimal);
}
// �ɼ����Ͽ� floor, x, y �־ �ž�, ��� �ʿ����Ϳ��̴� temp�� �����;���
void Terrain::SetSceneObj(int floor, int x, int y)
{
	string fileName = TEMPROOM_FOLDER;
	fileName += to_string(floor) + "F/";
	fileName += to_string(x) + to_string(y) + "ObjectDesc.txt";
	LoadObjFile(fileName);
}
void Terrain::EraseTile(int x, int y)
{
	wstring strMap = to_wstring(x) + L"," + to_wstring(y);
	m_cmTiles.erase(strMap);

}
/////////////////////////////////////////////////////////
// ��������
/////////////////////////////////////////////////////////
void Terrain::SaveFile(string strFileName, string objFileName)
{
	FILE *op = NULL;
	FILE* opObj = NULL;
	op = fopen(strFileName.c_str(), "w");
	opObj = fopen(objFileName.c_str(), "w");
	if (op == NULL || opObj == NULL) 
		return;

	fprintf(op, "#X		Y		POSITION	IMAGE		DISPLAYORDER	OFFSET	OFFSETSIZE	SCALEX	SCALEY	OBJECTYPE\n");
	fprintf(opObj, "#X		Y		POSITION	IMAGE		DISPLAYORDER	OFFSET	OFFSETSIZE	SCALEX	SCALEY	OBJECTYPE\n");
	fprintf(op, "MAGNIFICATION:%f %f\n", TerrainMagnification_.x, TerrainMagnification_.y);
	for (auto v : m_cmTiles)
	{
		// map
		wstring strMap1 = v.first.c_str();
		string  strMap2;
		strMap2.assign(strMap1.begin(), strMap1.end());
		int x;
		int y;
		// scanf  : �Է�(console)
		// sscanf : stearm  char buf[100];
		//    sscanf(buf,"%d%*c......
		sscanf(strMap2.c_str(), "%d%*c%d", &x, &y);	// ó�� XY ���ϱ�
	

		for (UINT i = 0; i < v.second->GetTileOrderSize(); i++)
		{
			TileOrder *pOrder = v.second->GetTile(i);
			if (!pOrder)
				continue;
			if(pOrder->objectType == 0){
				// MAP X, Y
				fprintf(op, "%-3d %-3d ", x, y);
				// POSITION
				fprintf(op, "%-3.2f %-3.2f |", v.second->GetPosition().x, v.second->GetPosition().y);
				// IMAGE
				string tmp;
				tmp.assign(pOrder->imageFile.begin(), pOrder->imageFile.end());
				tmp = tmp.replace(0, tmp.find("Resources"), "../");	// ����� ���� �Ϸ�
				fprintf(op, "%s|", tmp.c_str());
				// DIAPLAY ORDER
				fprintf(op, "%3d ", pOrder->order);
				// Offset
				fprintf(op, "%.f %.f ", pOrder->offset.x, pOrder->offset.y);
				// Offset Size
				fprintf(op, "%.f %.f ", pOrder->offsetSize.x, pOrder->offsetSize.y);
				// SCALE
				fprintf(op, "%.f %.f ", pOrder->scale.x, pOrder->scale.y);
				// ������Ʈ Ÿ��
				fprintf(op, "%d", pOrder->objectType);
				fprintf(op, "\n");
			} // end 0 
			else // ������Ʈ�� �ֱ�
			{
				// MAP X, Y
				fprintf(opObj, "%-3d %-3d ", x, y);
				// POSITION
				fprintf(opObj, "%-3.2f %-3.2f |", v.second->GetPosition().x, v.second->GetPosition().y);
				// IMAGE
				string tmp;
				tmp.assign(pOrder->imageFile.begin(), pOrder->imageFile.end());
				tmp = tmp.replace(0, tmp.find("Resources"), "../");	// ����� ���� �Ϸ�
				fprintf(opObj, "%s|", tmp.c_str());
				// DIAPLAY ORDER
				fprintf(opObj, "%3d ", pOrder->order);
				// Offset
				fprintf(opObj, "%.f %.f ", pOrder->offset.x, pOrder->offset.y);
				// Offset Size
				fprintf(opObj, "%.f %.f ", pOrder->offsetSize.x, pOrder->offsetSize.y);
				// SCALE
				fprintf(opObj, "%.f %.f ", pOrder->scale.x, pOrder->scale.y);
				// ������Ʈ Ÿ��
				fprintf(opObj, "%d", pOrder->objectType);
				fprintf(opObj, "\n");
			}
		}
	
	}



	fclose(op);
	fclose(opObj);
	string t = "NotePad.exe " + objFileName;
	const char* ptr = t.c_str();
	system(ptr);
//	system("NotePad.exe ./test.txt");

	// ���̳ʸ� ����
	return;
	SaveBinaryFile("./test.bi");

}
void Terrain::SetRoomObj(Room* room)
{
	string fileName = ROOM_FOLDER;
	fileName += to_string(room->currentFloor_) + "F/";
	fileName += to_string(room->myIndex[0]) + to_string(room->myIndex[1]) + "ObjectDesc.txt";
	LoadObjFile(fileName, room);
}
// �̰� Room �� ������ ��°Ŷ� �׷���.
void Terrain::LoadObjFile(string strFileName, Room* room)
{
}
// m_cmTiles�� ������ �ֱ�(�� �����Ϳ�)
void Terrain::LoadObjFile(string strFileName)
{
	FILE* fp;

	// Texture�� Setting

//	OpenBinaryFile("./test.bi");
//	return;

	printf("%s\n", strFileName.c_str());
	fp = fopen(strFileName.c_str(), "r");
	if (fp == NULL)
		return;

	// Ascii File�� ��� 1 Line�� �����͸� read

	while (1)
	{
		char buf[1024];
		int  X, Y;
		char imgBuf[200];
		int  nOrder = 0;
		int objectType = 0;
		Vector2 Offset = Vector2(0.0f, 0.0f);
		Vector2 OffsetSize = Vector2(0.0f, 0.0f);
		Vector2 scale = Vector2(1.0f, 1.0f);
		int   nFlip = 0;
		float nAngle = 0.0f;
		int success = 0;

		if (!fgets(buf, 1024, fp))
			break;
		//	buf[strlen(buf) - 1] = '\0'; // NewLine������ ����
		if (strlen(buf) < 10)
			continue;
		if (strstr(buf, "#"))
			continue;

		//	0   10 -500.00 -200.00 | . / Image//Floor/Wall7.png|       1 0 0 100 112 
		float FX, FY;

		success = sscanf(buf, "%d %d %f %f", &X, &Y, &FX, &FY);
		char* p = strstr(buf, "|");
		p++;
		strcpy(imgBuf, p);
		p = strstr(imgBuf, "|");
		*p = '\0';

		string   str = imgBuf;
		wstring  str2;
		str2.assign(str.begin(), str.end());

		p = strstr(buf, "|");
		p++;

		p = strstr(p, "|");
		p++;

		Vector2 offset;
		Vector2 offsetSize;
		success = sscanf_s(p, "%d %f %f %f %f %f %f %d", &nOrder, &offset.x, &offset.y, &offsetSize.x, &offsetSize.y, &scale.x, &scale.y, &objectType);
		//_s�� %d, %f���� �⺻����� ũ������ �����൵ ��.


		// imgBuf�� ������ SRVMANAGER�� ����
		wstring strImage = str2;
		// Texture�� ����

		// 6 * 10
		wstring strMap = to_wstring(X) + L"," + to_wstring(Y);


		Vector2 position = Vector2(FX, FY);

		SRVMANAGER->CreateShaderResourceView(strImage);

		Tile* pTile = FindTile(strMap);

		// �űԻ���
		if (!pTile)
		{
			pTile = new Tile();
			m_cmTiles.insert(make_pair(strMap, pTile));
		}

		if (m_nMaxDisplayOrder < nOrder)
			m_nMaxDisplayOrder = nOrder;

		pTile->SetPosition(position);
		pTile->SetOrder(m_pTexture, nOrder, strImage, offset, offsetSize, nFlip, nAngle, scale, objectType);
	}
	fclose(fp);
}

void Terrain::SaveBinaryFile(string strFileName)
{
	FILE  *op; 
	op = fopen(strFileName.c_str(), "wb");
	if (op == NULL)
		return;

	int Count = 0;
	for (auto v : m_cmTiles)
	{
		for (UINT i = 0; i < v.second->GetTileOrderSize(); i++)
		{
			TileOrder *pOrder = v.second->GetTile(i);
			if (!pOrder)
				continue;
			Count++;
		}

	}
	string tmpStr = "MAGNIFICATION:";
	fwrite(&Count, sizeof(int), 1, op);
	fwrite(&tmpStr, sizeof(tmpStr), 1, op);
	fwrite(&TerrainMagnification_, sizeof(Vector2), 1, op);
	BinaryMAP tmp;
	for (auto v : m_cmTiles)
	{
		// map
		wstring strMap1 = v.first.c_str();
		string  strMap2;
		strMap2.assign(strMap1.begin(), strMap1.end());
		int x;
		int y;
		// scanf  : �Է�( console)
		// sscanf : stearm  char buf[100];
		//    sscanf(buf,"%d%*c......
		sscanf(strMap2.c_str(), "%d%*c%d", &x, &y);


		for (UINT i = 0; i < v.second->GetTileOrderSize(); i++)
		{
			TileOrder *pOrder = v.second->GetTile(i);
			if (!pOrder)
				continue;
			ZeroMemory(&tmp, sizeof(tmp));
		
			tmp.x = x;
			tmp.y = y;
			tmp.position = v.second->GetPosition();
			tmp.offset = pOrder->offset;
			tmp.offsetSize = pOrder->offsetSize;
			tmp.order = pOrder->order;
			tmp.objectType = 0;
			tmp.scale = pOrder->scale;

			string str;
			str.assign(pOrder->imageFile.begin(), pOrder->imageFile.end());
			strcpy(tmp.imgname, str.c_str());
			fwrite(&tmp, sizeof(BinaryMAP), 1, op);
		}
	}
	fclose(op);
}
void Terrain::OpenBinaryFile(string strFileName)
{
	// ���̳ʸ� ������ ������ fclose ���� �߻� ���� ���� �� �� ���� ����� ���� �Ѵ� Next
	FILE *fp = NULL;

	fp = fopen(strFileName.c_str(), "rb");
	if (fp == NULL)
		return;

	int   Count;
	BinaryMAP tmp;

	string tempStr;
	string tempStr2 = "MAGNIFICATION:";
	fread(&Count, sizeof(int), 1, fp);
	fread(&tempStr, sizeof(tempStr2), 1, fp);
	fread(&TerrainMagnification_, sizeof(Vector2), 1, fp);
	for (int i = 0; i < Count; i++)
	{
		fread(&tmp, sizeof(BinaryMAP), 1, fp);

		// imgBuf�� ������ SRVMANAGER�� ����
		string  strImage2 = tmp.imgname;
		wstring strImage;
		strImage.assign(strImage2.begin(), strImage2.end());
		wstring strMap = to_wstring(tmp.x) + L"," + to_wstring(tmp.y);
		Vector2 position = tmp.position;

		SRVMANAGER->CreateShaderResourceView(strImage);

		Tile *pTile = FindTile(strMap);

		// �űԻ���
		if (!pTile)
		{
			pTile = new Tile();
			m_cmTiles.insert(make_pair(strMap, pTile));
		}
		if (m_nMaxDisplayOrder < tmp.order)
			m_nMaxDisplayOrder = tmp.order;
		pTile->SetPosition(position);
		pTile->SetOrder(m_pTexture, tmp.order, strImage, tmp.offset, tmp.offsetSize, tmp.Flip, tmp.angle, tmp.scale, tmp.objectType);

	}

	fclose(fp);

}
/////////////////////////////////////////////////////////
// Map�� �׷��� �ִ� �����͸� GdiPlus�� �̿��Ͽ� png���Ϸ� ����
/////////////////////////////////////////////////////////
void Terrain::SavePNGFile(wstring strPNGFile)
{
	Graphic2D *graphic = new Graphic2D();
	// 1. GdiPlus�� ����Ҽ� �ְ�
	graphic->GdiPlusStartUp();
	
	int width  = this->GetMapXY().x * this->GetTileSize().x;
	int height = this->GetMapXY().y * this->GetTileSize().y;

	// 2. memGC ����
	graphic->CreateMemoryDC(width, height);

	// 3. Rendering
	// �������: GetTileSize() �� �Ⱦ���, offsetX, offsetY, offsetSize�� ���� ����
	// ���� ������ �ٸ��� ������.
	for (UINT i = 0; i <= m_nMaxDisplayOrder; i++)  // order max��
	{
		for (auto v : m_cmTiles)
		{
			TileOrder *pOrder = v.second->FindTileOrder(i);
			if (!pOrder)
				continue;
			Vector2 position = v.second->GetPosition();

			printf("%f %f %f %f\n", GetOffset().x, GetOffset().y,position.x, position.y);

			float OffsetX = -(GetOffset().x - m_Size.x*0.5f);
			float OffsetY =  (GetOffset().y + m_Size.y*0.5f);

			Vector2 offsetSize = pOrder->offsetSize;
		//	position.x = position.x - GetOffset().x - GetTileSize().x * 0.5f;
		//	position.y = GetOffset().y - position.y - GetTileSize().y * 0.5f;
			position.x = OffsetX + position.x - offsetSize.x*0.5f;
			position.y = OffsetY - position.y - offsetSize.y*0.5f;

			graphic->DrawImage(pOrder->imageFile, position, pOrder->offset, pOrder->offsetSize);
		}
	}

	// 4
	graphic->SavePNG(strPNGFile);
	graphic->GdiPlusShutDown();
	SAFE_DELETE(graphic);

	MessageBox(MAIN->GetWindowHandler(), L"png������ ���� �Ǿ����ϴ�", L"Game", MB_OK);

}
//Fit ������ ������ ����.
void Terrain::SavePNGFitFile(wstring strPNGFile)
{

	Graphic2D *graphic = new Graphic2D();
	// 1. GdiPlus�� ����Ҽ� �ְ�
	graphic->GdiPlusStartUp();


	float XMIN = FLT_MAX;
	float XMAX = FLT_MIN;
	float YMIN = FLT_MAX;
	float YMAX = FLT_MIN;

	for (UINT i = 0; i <= m_nMaxDisplayOrder; i++)  // order max��
	{
		for (auto v : m_cmTiles)
		{
			TileOrder *pOrder = v.second->FindTileOrder(i);
			if (!pOrder)
				continue;
			Vector2 position = v.second->GetPosition();
			//position.x = position.x -  pOrder->offsetSize.x*0.5f;
			//position.y = position.y  - pOrder->offsetSize.y*0.5f;
			XMIN = min(XMIN, position.x  - pOrder->offsetSize.x*0.5f);
			XMAX = max(XMAX, position.x + pOrder->offsetSize.x*0.5f);
			YMIN = min(YMIN, position.y - pOrder->offsetSize.y*0.5f);
			YMAX = max(YMAX, position.y - pOrder->offsetSize.y*0.5f);
		}
	}

	int width  = (int)((XMAX - XMIN)+0.5f);
	int height = (int)((YMAX - YMIN) + 0.5f);
	graphic->CreateMemoryDC(width, height);

	// 2. memGC ����
	graphic->CreateMemoryDC(width, height);

	// 3. Rendering

	for (UINT i = 0; i <= m_nMaxDisplayOrder; i++)  // order max��
	{
		for (auto v : m_cmTiles)
		{
			TileOrder *pOrder = v.second->FindTileOrder(i);
			if (!pOrder)
				continue;
			Vector2 position = v.second->GetPosition();
			position.x = fabs(XMIN) + position.x;// -pOrder->offsetSize.x*0.5f;
			position.y = fabs(YMAX) - position.y;// -pOrder->offsetSize.y*0.5f;
			graphic->DrawImage(pOrder->imageFile, position, pOrder->offset, pOrder->offsetSize);
		}
	}

	// 4
	graphic->SavePNG(strPNGFile);
	graphic->GdiPlusShutDown();
	SAFE_DELETE(graphic);

	MessageBox(MAIN->GetWindowHandler(), L"png������ ���� �Ǿ����ϴ�", L"Game", MB_OK);
}
void Terrain::SetOrderMax(int order)
{
	m_nMaxDisplayOrder = order;
}
/////////////////////////////////////////////////////////
// ��ǥ���� �־�����  Map������ row,column�� ã�� �Լ�
// �Ʒ� Old�� GameObject�� ��ǥ�� �������⿡, ���� �׷��Ȱ� ���� ��ġ�� �ٸ�����
// �� �Լ��� �ַ� position �� Mouse��ǥ�� ���� ������ ���� ��ġ�� ���ƾ� ��. �׷��� ������.
/////////////////////////////////////////////////////////
bool Terrain::GetMapXY(int & x, int & y, Vector2 position)
{
	bool bExist = false;
	Vector2 Min;
	Vector2 Max;

	for (int i = 0; i < this->GetMapXY().x; i++)
	{
		Min.x = -GetTileSizeWithMag().x*0.5f + GetOffsetWithMag().x + (i* GetTileSizeWithMag().x);
		Max.x =  Min.x + GetTileSizeWithMag().x;

		for (int j = 0; j <= GetMapXY().y; j++)
		{
			Max.y = GetTileSizeWithMag().y*0.5f + GetOffsetWithMag().y - (j* GetTileSizeWithMag().y);
			Min.y = Max.y - GetTileSizeWithMag().y;
			if ((position.x >= Min.x && position.x <= Max.x) &&
				(position.y >= Min.y && position.y <= Max.y))
			{
				x = i;
				y = j;
				bExist = true;
				break;
			}
		}
		if (bExist)
			break;
	}
	return bExist;
}

/////////////////////////////////////////////////////////
// MoveTiles() ���� ����ϴ�, ��ǥ ���� ���� XY�� ã�� �Լ�
// ������: Magnification�� ������ ����.
// ����: Tile::Update()���� SetPosition�� �� �� Magni�� ���ϴµ�, 
// �׷��������� ������ ���� �׷�������, ���� ��ü�� ��ǥ�� Magni�� ������ ���� ��ġ�� ����.
// �׷��� �� ������: ���� ���߿� ������ �ٲٰ� ���� �� �������� �ʰ� �ϱ� ���ؼ���. (txt���� ������ �ٲٸ� Scene���� �޾ƿ� �� ������ �ٲ�)
/////////////////////////////////////////////////////////
bool Terrain::GetOldMapXY(int& x, int& y, Vector2 position)
{
	bool bExist = false;
	Vector2 Min;
	Vector2 Max;

	for (int i = 0; i < this->GetMapXY().x; i++)
	{
		Min.x = -GetOldTileSize().x * 0.5f + GetOldOffset().x + (i * GetOldTileSize().x);
		Max.x = Min.x + GetOldTileSize().x;

		for (int j = 0; j <= GetMapXY().y; j++)
		{
			Max.y = GetOldTileSize().y * 0.5f + GetOldOffset().y - (j * GetOldTileSize().y);
			Min.y = Max.y - GetOldTileSize().y;
			if ((position.x >= Min.x && position.x <= Max.x) &&
				(position.y >= Min.y && position.y <= Max.y))
			{
				x = i;
				y = j;
				bExist = true;
				break;
			}
		}
		if (bExist)
			break;
	}
	return bExist;
}

void Terrain::MoveTiles()
{
	// TileSize�� ���� �� �־�� ��.
	Vector2 changeAmount = m_Offset - m_oldOffset;
	Vector2 changeSize = m_Size - m_oldSize;
	Vector2 changeMag = OldTerrainMag_ - TerrainMagnification_;
	Vector2 tempPos = Vector2(0.0f, 0.0f);
	if (fabsf(changeAmount.x + changeAmount.y) < 0.01f && fabsf(changeSize.x + changeSize.y) < 0.01f && fabsf(changeMag.x + changeMag.y) < 0.01f){
		printf("�ּ�ũ�� 0.01f �̸� �̵� ����\n");
		return;
	}
//	printf("//////////////////\n");
	
	int index[2] = { 0,0 };
	for (auto iter = m_cmTiles.begin(); iter != m_cmTiles.end(); ++iter) {
		tempPos = iter->second->GetPosition();
		GetOldMapXY(index[0], index[1], tempPos);
		printf("coord: %d %d\n", index[0], index[1]);
	//	iter->second->SetPosition(tempPos + changeAmount);
		iter->second->SetPosition(tempPos.x + changeAmount.x + changeSize.x * (float)index[0],
			tempPos.y + changeAmount.y  - changeSize.y * (float)index[1]);
	}
	m_oldOffset = m_Offset;	// ����ȭ
}

Tile * Terrain::FindTile(wstring strMap)
{
	if (m_cmTiles.find(strMap) != m_cmTiles.end())
		return m_cmTiles.find(strMap)->second;
	return nullptr;

}
// �ּҿɼ�(SetSceneMap ���� �޾ƿ�)
void Terrain::OpenFile(string strFileName, bool minimal)
{
	FILE  *fp;

	// Texture�� Setting

//	OpenBinaryFile("./test.bi");
//	return;

	printf("%s\n", strFileName.c_str());
	fp = fopen(strFileName.c_str(), "r");
	if (fp == NULL)
		return;

	// Ascii File�� ��� 1 Line�� �����͸� read

	while (1)
	{
		char buf[1024];
		int  X, Y;
		char imgBuf[200];
		int  nOrder = 0;
		int objectType = 0;
		Vector2 Offset = Vector2(0.0f,0.0f);
		Vector2 OffsetSize = Vector2(0.0f, 0.0f);
		Vector2 scale = Vector2(1.0f, 1.0f);
		int   nFlip = 0;
		float nAngle = 0.0f;	
		int success = 0;

		if (!fgets(buf, 1024, fp)) 
			break;
	//	buf[strlen(buf) - 1] = '\0'; // NewLine������ ����
		if (strstr(buf, "MAGNIFICATION:")) {
			success = sscanf_s(buf, "MAGNIFICATION:%f %f", &scale.x, &scale.y);
			// �⺻ Ÿ���� �ƴ� ���� _s �� �ش� ���� �ڿ� ũ�⸦ �������ָ� ��. ( char, float�� ����, string�� ũ�� ���� ���)
			TerrainMagnification_ = scale;
			////////////////////////////////
			// 220725 �߰�: minimal �� true �� ��� �ּҿɼǸ� �����ϰ� �������� )
			////////////////////////////////
			if (minimal == true)
				break;
			else
				continue;
		}
		if (strlen(buf) < 10)
			continue;
		if(strstr(buf,"#"))
			continue;

	//	0   10 -500.00 -200.00 | . / Image//Floor/Wall7.png|       1 0 0 100 112 
		float FX, FY;

		success = sscanf(buf, "%d %d %f %f", &X, &Y,&FX,&FY);
		char *p = strstr(buf, "|");
		p++;
		strcpy(imgBuf, p);
		p = strstr(imgBuf, "|");
		*p = '\0';

		string   str = imgBuf;
		wstring  str2;
		str2.assign(str.begin(), str.end());

		p = strstr(buf, "|");
		p++;

		p = strstr(p, "|");
		p++;

		Vector2 offset;
		Vector2 offsetSize;
		success = sscanf_s(p, "%d %f %f %f %f %f %f %d", &nOrder, &offset.x, &offset.y, &offsetSize.x, &offsetSize.y, &scale.x, &scale.y, &objectType);
		//_s�� %d, %f���� �⺻����� ũ������ �����൵ ��.


		// imgBuf�� ������ SRVMANAGER�� ����
		wstring strImage = str2;
		// Texture�� ����
	
		// 6 * 10
		wstring strMap = to_wstring(X) + L"," + to_wstring(Y);
	

		Vector2 position = Vector2(FX,FY);

		SRVMANAGER->CreateShaderResourceView(strImage);

		Tile *pTile = FindTile(strMap);

		// �űԻ���
		if (!pTile)
		{
			pTile = new Tile();
			m_cmTiles.insert(make_pair(strMap, pTile));
		}

		if (m_nMaxDisplayOrder < nOrder)
			m_nMaxDisplayOrder = nOrder;

		pTile->SetPosition(position);
		pTile->SetOrder(m_pTexture, nOrder, strImage, offset, offsetSize, nFlip, nAngle, scale, objectType);
	}
	fclose(fp);
}


