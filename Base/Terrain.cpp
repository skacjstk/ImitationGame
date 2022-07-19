
#include  "ImitationGame/framework.h"
#include  "Terrain.h"
#include  "./Object/Tile.h"
#include  "./Render/Graphic2D.h"

///////////////////////////////////////////////////////////
// 생성자
//////////////////////////////////////////////////////////
Terrain::Terrain()
{
	// 각신에 사용될 Texture를  공통적으로 사용하기 위하여 
	// MAPX * MAPY Size로 구성

	wstring  strImage = IMAGE_FOLDER;
	strImage += L"char/Adventurer/CharHand.png";
	wstring strShader = SHADER_FOLDER;
	strShader += L"Texture.hlsl";

	// Texture는 하나만 가지고도 사용가능

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
	for (int i = 0; i <= m_nMaxDisplayOrder; i++)  // order max값
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
	for (int i = 0; i <= m_nMaxDisplayOrder; i++)  // order max값
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



	SRVMANAGER->CreateShaderResourceView(strImageFile);
	pTile->SetPosition(position);
	pTile->SetOrder(m_pTexture, nOrder, strImageFile, offset, offsetSize, 0, 0.0f, Vector2(1.0f,1.0f));
}

void Terrain::SetSceneMap(string sceneName)
{
	string str = "./" + sceneName + ".txt";
	OpenFile(str);
}
void Terrain::EraseTile(int x, int y)
{
	wstring strMap = to_wstring(x) + L"," + to_wstring(y);
	m_cmTiles.erase(strMap);

}
/////////////////////////////////////////////////////////
// 파일저장
/////////////////////////////////////////////////////////
void Terrain::SaveFile(string strFileName)
{
	FILE *op = NULL;

	op = fopen(strFileName.c_str(), "w");
	if (op == NULL) 
		return;

	fprintf(op, "#X		Y		POSITION	IMAGE		DISPLAYORDER	OFFSET	OFFSETSIZE	SCALEX	SCALEY\n");
	fprintf(op, "MAGNIFICATION:%f %f\n", TerrainMagnification_.x, TerrainMagnification_.y);
	for (auto v : m_cmTiles)
	{
		// map
		wstring strMap1 = v.first.c_str();
		string  strMap2;
		strMap2.assign(strMap1.begin(), strMap1.end());
		int x;
		int y;
		// scanf  : 입력(console)
		// sscanf : stearm  char buf[100];
		//    sscanf(buf,"%d%*c......
		sscanf(strMap2.c_str(), "%d%*c%d", &x, &y);
	

		for (UINT i = 0; i < v.second->GetTileOrderSize(); i++)
		{
			TileOrder *pOrder = v.second->GetTile(i);
			if (!pOrder)
				continue;
			// MAP X, Y
			fprintf(op, "%-3d %-3d ", x, y);
			// POSITION
			fprintf(op, "%-3.2f %-3.2f |", v.second->GetPosition().x, v.second->GetPosition().y);
			// IMAGE
			string tmp;
			tmp.assign(pOrder->imageFile.begin(), pOrder->imageFile.end());
			tmp = tmp.replace(0 , tmp.find("Resources"), "../");	// 상대경로 변경 완료
			fprintf(op, "%s|", tmp.c_str());
			// DIAPLAY ORDER
			fprintf(op, "%3d ", pOrder->order);
			// Offset
			fprintf(op, "%.f %.f ", pOrder->offset.x, pOrder->offset.y);
			// Offset Size
			fprintf(op, "%.f %.f ", pOrder->offsetSize.x, pOrder->offsetSize.y);
			// SCALE
			fprintf(op, "%.f %.f ", pOrder->scale.x, pOrder->scale.y );
			fprintf(op, "\n");	
		}
	
	}



	fclose(op);
	system("NotePad.exe ./test.txt");
	SaveBinaryFile("./test.bi");

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
		// scanf  : 입력( console)
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
	// 바이너리 포인터 문제로 fclose 오류 발생 읽을 때와 쓸 때 전부 면밀이 봐야 한다 Next
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

		// imgBuf의 내용은 SRVMANAGER에 저장
		string  strImage2 = tmp.imgname;
		wstring strImage;
		strImage.assign(strImage2.begin(), strImage2.end());
		wstring strMap = to_wstring(tmp.x) + L"," + to_wstring(tmp.y);
		Vector2 position = tmp.position;

		SRVMANAGER->CreateShaderResourceView(strImage);

		Tile *pTile = FindTile(strMap);

		// 신규생성
		if (!pTile)
		{
			pTile = new Tile();
			m_cmTiles.insert(make_pair(strMap, pTile));
		}
		if (m_nMaxDisplayOrder < tmp.order)
			m_nMaxDisplayOrder = tmp.order;
		pTile->SetPosition(position);
		pTile->SetOrder(m_pTexture, tmp.order, strImage, tmp.offset, tmp.offsetSize, tmp.Flip, tmp.angle, tmp.scale);

	}

	fclose(fp);

}
/////////////////////////////////////////////////////////
// Map에 그려져 있는 데이터를 GdiPlus를 이용하여 png파일로 저장
/////////////////////////////////////////////////////////
void Terrain::SavePNGFile(wstring strPNGFile)
{
	Graphic2D *graphic = new Graphic2D();
	// 1. GdiPlus를 사용할수 있게
	graphic->GdiPlusStartUp();
	
	int width  = this->GetMapXY().x * this->GetTileSize().x;
	int height = this->GetMapXY().y * this->GetTileSize().y;

	// 2. memGC 생성
	graphic->CreateMemoryDC(width, height);

	// 3. Rendering
	// 변경사항: GetTileSize() 를 안쓰고, offsetX, offsetY, offsetSize를 새로 구해
	// 기존 계산식을 다르게 적용함.
	for (UINT i = 0; i <= m_nMaxDisplayOrder; i++)  // order max값
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

	MessageBox(MAIN->GetWindowHandler(), L"png파일이 생성 되었습니다", L"Game", MB_OK);

}
//Fit 파일은 기존과 같음.
void Terrain::SavePNGFitFile(wstring strPNGFile)
{

	Graphic2D *graphic = new Graphic2D();
	// 1. GdiPlus를 사용할수 있게
	graphic->GdiPlusStartUp();


	float XMIN = FLT_MAX;
	float XMAX = FLT_MIN;
	float YMIN = FLT_MAX;
	float YMAX = FLT_MIN;

	for (UINT i = 0; i <= m_nMaxDisplayOrder; i++)  // order max값
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

	// 2. memGC 생성
	graphic->CreateMemoryDC(width, height);

	// 3. Rendering

	for (UINT i = 0; i <= m_nMaxDisplayOrder; i++)  // order max값
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

	MessageBox(MAIN->GetWindowHandler(), L"png파일이 생성 되었습니다", L"Game", MB_OK);
}
void Terrain::SetOrderMax(int order)
{
	m_nMaxDisplayOrder = order;
}
/////////////////////////////////////////////////////////
// 좌표점을 주었을때  Map에서의 row,column을 찾는 함수
/////////////////////////////////////////////////////////
bool Terrain::GetMapXY(int & x, int & y, Vector2 position)
{
	bool bExist = false;
	Vector2 Min;
	Vector2 Max;

	for (int i = 0; i < this->GetMapXY().x; i++)
	{
		Min.x = -GetTileSize().x*0.5f + GetOffset().x + (i*GetTileSize().x);
		Max.x =  Min.x +GetTileSize().x;

		for (int j = 0; j <= GetMapXY().y; j++)
		{
			Max.y = GetTileSize().y*0.5f + GetOffset().y - (j*GetTileSize().y);
			Min.y = Max.y - GetTileSize().y;
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

Tile * Terrain::FindTile(wstring strMap)
{
	if (m_cmTiles.find(strMap) != m_cmTiles.end())
		return m_cmTiles.find(strMap)->second;
	return nullptr;

}

void Terrain::OpenFile(string strFileName)
{
	FILE  *fp;

	// Texture값 Setting

//	OpenBinaryFile("./test.bi");
//	return;

	printf("%s\n", strFileName.c_str());
	fp = fopen(strFileName.c_str(), "r");
	if (fp == NULL)
		return;

	// Ascii File인 경우 1 Line씩 데이터를 read

	while (1)
	{
		char buf[1024];
		int  X, Y;
		char imgBuf[200];
		int  nOrder =0;
		Vector2 Offset = Vector2(0.0f,0.0f);
		Vector2 OffsetSize = Vector2(0.0f, 0.0f);
		Vector2 scale = Vector2(1.0f, 1.0f);
		int   nFlip = 0;
		float nAngle = 0.0f;	
		int success = 0;

		if (!fgets(buf, 1024, fp)) 
			break;
	//	buf[strlen(buf) - 1] = '\0'; // NewLine데이터 삭제
		if (strstr(buf, "MAGNIFICATION:")) {
			success = sscanf_s(buf, "MAGNIFICATION:%f %f", &scale.x, &scale.y);
			// 기본 타입이 아닐 때만 _s 에 해당 인자 뒤에 크기를 전달해주면 됨. ( char, float은 괜찮, string은 크기 전달 등등)
			TerrainMagnification_ = scale;
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
		success = sscanf(p, "%d %f %f %f %f %f %f", &nOrder, &offset.x, &offset.y, &offsetSize.x, &offsetSize.y, &scale.x, &scale.y);



		// imgBuf의 내용은 SRVMANAGER에 저장
		wstring strImage = str2;
		// Texture에 저장
	
		// 6 * 10
		wstring strMap = to_wstring(X) + L"," + to_wstring(Y);
	

		Vector2 position = Vector2(FX,FY);

		SRVMANAGER->CreateShaderResourceView(strImage);

		Tile *pTile = FindTile(strMap);

		// 신규생성
		if (!pTile)
		{
			pTile = new Tile();
			m_cmTiles.insert(make_pair(strMap, pTile));
		}

		if (m_nMaxDisplayOrder < nOrder)
			m_nMaxDisplayOrder = nOrder;

		pTile->SetPosition(position);
		pTile->SetOrder(m_pTexture, nOrder, strImage, offset, offsetSize, nFlip, nAngle, scale);
	}
	fclose(fp);
}


