#include "ImitationGame/framework.h"
#include "Audio.h"

void CheckError(FMOD_RESULT result_)
{
	if (result_ != FMOD_OK)
		MessageBoxW(MAIN->GetWindowHandler(), L"Audio 에러", L"Audio", MB_OK);
}
CAudio::CAudio()
	: count(150)
	, volume(1.0f)
{
	result = System_Create(&system);
	CheckError(result);
	result = system->init(count, FMOD_INIT_NORMAL, NULL);
	CheckError(result);

	sound = new Sound*[count];
	channel = new Channel*[count];

	ZeroMemory(sound, sizeof(Sound*) * count);
	ZeroMemory(channel, sizeof(Channel*) * count);
}

CAudio::~CAudio()
{
	if (channel != NULL)
	{
		for (UINT i = 0; i < count; i++)
			if (channel[i]) channel[i]->stop();
	}

	if (sound != NULL)
	{
		for (UINT i = 0; i < count; i++)
			if (sound[i]) sound[i]->release();
	}

	SAFE_DELETE_ARRAY(channel);
	SAFE_DELETE_ARRAY(sound);

	if (system != NULL)
	{ 
		system->release();
		system->close();
	}

	sounds.clear();
}

void CAudio::Update()
{
	system->update();
}

void CAudio::AddSound(string name, string file, bool bLoop)
{
	if (bLoop == true)
		system->createStream(file.c_str(), FMOD_LOOP_NORMAL, NULL, &sound[sounds.size()]);
	else
		system->createStream(file.c_str(), FMOD_DEFAULT, NULL, &sound[sounds.size()]);

	sounds[name] = &sound[sounds.size()];
}

void CAudio::Play(string name, float volume)
{
	int count = 0;
	iter = sounds.begin();
	for (iter; iter != sounds.end(); iter++, count++)
	{
		if (name == iter->first)
		{
			system->playSound(FMOD_CHANNEL_FREE, *iter->second, false, &channel[count]);
			channel[count]->setVolume(volume);
			break;
		}
	}
}

void CAudio::Stop(string name)
{
	int count = 0; 
	iter = sounds.begin();	
	for (iter; iter != sounds.end(); iter++, count++)
	{
		if (name == iter->first)
		{
			channel[count]->stop();
			break;
		}
	}
	
}

void CAudio::Pause(string name)
{
	int count = 0;
	iter = sounds.begin();
	for (iter; iter != sounds.end(); iter++, count++)
	{
		if (name == iter->first)
		{
			channel[count]->setPaused(true);
			break;
		}
	}
}

void CAudio::Resume(string name)
{
	int count = 0;
	iter = sounds.begin();
	for (iter; iter != sounds.end(); iter++, count++)
	{
		if (name == iter->first)
		{
			channel[count]->setPaused(false);
			break;
		}
	}
}

bool CAudio::Playing(string name)
{
	bool bPlay = false;
	int count = 0;
	iter = sounds.begin();
	for (iter; iter != sounds.end(); iter++, count++)
	{
		if (name == iter->first)
		{
			channel[count]->isPlaying(&bPlay);
			break;
		}
	}

	return bPlay;
}

bool CAudio::Paused(string name)
{
	bool bPaused = false;
	int count = 0;
	iter = sounds.begin();
	for (iter; iter != sounds.end(); iter++, count++)
	{
		if (name == iter->first)
		{
			channel[count]->getPaused(&bPaused);
			break;
		}
	}

	return bPaused;
}
// SceneManager의 최초 기동 시점에 호출(생성자)
void CAudio::AddSoundAll()
{
	string fileType = ".wav";

	// 사운드를 한번에 넣는 걸로 변경해봄
	string strSound = "1.JailField";
	Audio->AddSound("Floor_1BGM", AUDIO_FOLDER + strSound + fileType, true);

	strSound = "0.Town";
	Audio->AddSound("Town", AUDIO_FOLDER + strSound + fileType, true);

	strSound = "title";
	Audio->AddSound("IntroBGM", AUDIO_FOLDER + strSound + fileType, true);

	strSound = "1.JailBoss";
	Audio->AddSound("JailBoss", AUDIO_FOLDER + strSound + fileType, true);

	strSound = "etc-sound0033_swap";
	Audio->AddSound("SwapItem", AUDIO_FOLDER + strSound + fileType, false);

	strSound = "Hit_Player";
	Audio->AddSound("Hit_Player", AUDIO_FOLDER + strSound + fileType, false);

	strSound = "ui-sound-13-dash";
	Audio->AddSound("Dash_Player", AUDIO_FOLDER + strSound + fileType, false);

	strSound = "swing2 #211045";
	Audio->AddSound("swing2", AUDIO_FOLDER + strSound + fileType, false);

	strSound = "Hit_Monster";
	Audio->AddSound("Hit_Monster", AUDIO_FOLDER + strSound + fileType, false);

	strSound = "MonsterDie";
	Audio->AddSound("MonsterDie", AUDIO_FOLDER + strSound + fileType, false);
	
	strSound = "Explosion";	// 플레이어 죽을때
	Audio->AddSound("Explosion", AUDIO_FOLDER + strSound + fileType, false);

	strSound = "Explosion_02";	// 유령류 몹 죽을때
	Audio->AddSound("Explosion2", AUDIO_FOLDER + strSound + fileType, false);

	strSound = "Jail Metal Door Sound Effect";
	Audio->AddSound("JailClose", AUDIO_FOLDER + strSound + fileType, false);

	strSound = "stoneDoor";
	Audio->AddSound("stoneDoor", AUDIO_FOLDER + strSound + fileType, false);

	strSound = "beliallaugh_rev";	// SkellBoss 벨리알 웃음소리
	Audio->AddSound("Blaugh", AUDIO_FOLDER + strSound + fileType, false);

	strSound = "BelialBullet";	// 단 한알 벨리알 총알
	Audio->AddSound("BBullet", AUDIO_FOLDER + strSound + fileType, false);

	strSound = "SpawnMonster";
	Audio->AddSound("SpawnMonster", AUDIO_FOLDER + strSound + fileType, false);
	// 레이저 발사 못찾음. 대체재임.
	strSound = "summonBelialHand";
	Audio->AddSound("BossLaser", AUDIO_FOLDER + strSound + fileType, false);

}

void CAudio::Volume(string name, float volume)
{
	int count = 0;
	iter = sounds.begin();
	for (iter; iter != sounds.end(); iter++, count++)
	{
		if (name == iter->first)
		{
			channel[count]->setVolume(volume);
			break;
		}
	}
}
