#include "./ImitationGame/framework.h"
#include "Intro.h"

Intro::Intro()
{
	m_strSceneName = "Intro";//SCENEMANAGER->sceneNames_[0];	// Intro
	wstring strImage = IMAGE_FOLDER; strImage += L"Intro/Title.png";
	wstring strShader = SHADER_FOLDER; strShader += L"Texture.hlsl";
	wstring strHoverImage;
	titleImage_ = new Texture(strImage, strShader);
	titleImage_->SetPosition(0.0f, 200.0f);

	strImage = IMAGE_FOLDER; strImage += L"Intro/backGround.png";
	strShader = SHADER_FOLDER;  strShader += L"TextureColor.hlsl";
	backGround_ = new Texture(strImage, strShader);
	backGround_->SetScale(19.20f * WSCALEX, 10.80f * WSCALEY );
	backGround_->UpdateColorBuffer(Color(121.0f / 255.0f, 185.0f / 255.0f, 1.0f, 1.0f), 1);	// Color 값만 받기( 하늘색 )

	strImage = IMAGE_FOLDER;	strImage += L"Button/GameStart.png";
	strHoverImage = IMAGE_FOLDER;	strHoverImage += L"Button/GameStartHover.png";
	introButton_[0] = new Button(strImage, strHoverImage, strShader);
	introButton_[0]->SetButtonName(L"");

	strImage = IMAGE_FOLDER;	strImage += L"Button/Option.png";
	strHoverImage = IMAGE_FOLDER;	strHoverImage += L"Button/OptionHover.png";
	introButton_[1] = new Button(strImage, strHoverImage, strShader);
	introButton_[1]->SetButtonName(L"");

	strImage = IMAGE_FOLDER;	strImage += L"Button/Quit.png";
	strHoverImage = IMAGE_FOLDER;	strHoverImage += L"Button/QuitHover.png";
	introButton_[2] = new Button(strImage, strHoverImage, strShader);
	introButton_[2]->SetButtonName(L"");

	introButton_[0]->SetActivate(true);
	introButton_[1]->SetActivate(false);
	introButton_[2]->SetActivate(false);

	// TabGroup
	introButton_[0]->SetBottomTabGroup(introButton_[1]);
	introButton_[1]->SetBottomTabGroup(introButton_[2]);
	introButton_[2]->SetBottomTabGroup(introButton_[0]);

	introButton_[1]->SetTopTabGroup(introButton_[0]);
	introButton_[2]->SetTopTabGroup(introButton_[1]);
	introButton_[0]->SetTopTabGroup(introButton_[2]);

	// Callback 설정
	std::function<void(int)> fp;	// 반환값(인자) 형태 
	fp = bind(&Intro::OnButton, this, std::placeholders::_1);	// ph_1 은 아규먼트 1개 콜백 설정

	for (int i = 0; i < sizeof(introButton_) / sizeof(introButton_[0]); ++i) {
		introButton_[i]->SetID(i);
		introButton_[i]->SetCallback(fp);
	}

	// Position
	introButton_[0]->SetPosition(0.0f, -100.0f);
	introButton_[1]->SetPosition(0.0f, -170.0f);
	introButton_[2]->SetPosition(0.0f, -240.0f);

	for (int i = 0; i < _countof(introButton_); ++i)
		introButton_[i]->SetShaderInputType(11);	// alpha값만 받게

	// 오디오 추가
	wstring introBGM = AUDIO_FOLDER;
	introBGM += L"title.wav";
	Audio->AddSound("IntroBGM", introBGM, true);
	Audio->Play("IntroBGM", 1.0f);
}

Intro::~Intro()
{
	SAFE_DELETE(titleImage_);
}

void Intro::Update()
{
	Matrix V, P;
	V = CAMERA->GetViewMatrix();
	P = CAMERA->GetProjectionMatrix();

	if (KEYBOARD->Down(VK_NUMPAD9))
	{
		ExitScene();
		SCENEMANAGER->ChangeScene("MapEditor");
		return;
	}
	CAMERA->Update(V, P);

	backGround_->Update(V, P);
	titleImage_->Update(V, P);
	for (int i = 0; i < _countof(introButton_); ++i)
		introButton_[i]->Update(V, P);
}

void Intro::Render()
{
	backGround_->Render();
	titleImage_->Render();
	CAMERA->Render();
	for (int i = 0; i < _countof(introButton_); ++i)
		introButton_[i]->Render();
}

void Intro::ChangeScene()
{
	SetActive(true);
	Audio->Play("IntroBGM", 1.0f);
}

void Intro::ExitScene()
{
	Audio->Stop("IntroBGM");
}

void Intro::OnButton(int id)
{
	switch (id)
	{
	case 0:	// 게임 시작 ( 저장 슬롯 3개 ) ( 현재는 바로 씬 바꿔보자 )
		this->setNextName(SCENEMANAGER->sceneNames_[1]);	// Town
		ExitScene();
		SCENEMANAGER->ChangeScene(this->GetNextName());		
		break;
	case 1:	// 옵션 창
		break;
	case 2:
		PostQuitMessage(0);
		break;
	default:
		MessageBoxW(MAIN->GetWindowHandler(), L"그런 버튼 없다.", L"Intro 버튼id 범위 벗어남", MB_OK);
		break;
	}
}

/*
		Scene* scene = (S02_LoadingScene*)SCENEMANAGER->GetScene(HELLTAKERMAP->HelltakerEventScene[0]);	// S02_Loading
		if (scene) {
			scene->setNextName(HELLTAKERMAP->HelltakerChapterScene[id - 1]);
			scene->SetExam(0);
			SCENEMANAGER->ChangeScene(this->GetNextName());
		}
*/