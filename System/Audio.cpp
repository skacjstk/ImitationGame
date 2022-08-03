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

	// 사운드를 한번에 넣는 걸로 변경해봄
	string strSound = "1.JailField.wav";
	Audio->AddSound("Floor_1BGM", AUDIO_FOLDER + strSound, true);

	strSound = "0.Town.wav";
	Audio->AddSound("Town", AUDIO_FOLDER + strSound, true);

	strSound = "title.wav";
	Audio->AddSound("IntroBGM", AUDIO_FOLDER + strSound, true);

	strSound = "etc-sound0033_swap.wav";
	Audio->AddSound("SwapItem", AUDIO_FOLDER + strSound, false);

	strSound = "Hit_Player.wav";
	Audio->AddSound("Hit_Player", AUDIO_FOLDER + strSound, false);

	strSound = "ui-sound-13-dash.wav";
	Audio->AddSound("Dash_Player", AUDIO_FOLDER + strSound, false);

	strSound = "swing2 #211045.wav";
	Audio->AddSound("swing2", AUDIO_FOLDER + strSound, false);
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
