#include "ImitationGame/framework.h"
#include "Audio.h"

CAudio::CAudio()
	: count(150)
	, volume(1.0f)
{
	System_Create(&system);

	system->init(count, FMOD_INIT_NORMAL, NULL);

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

void CAudio::AddSound(string name, wstring file, bool bLoop)
{
	string cfile;
	cfile.assign(file.begin(), file.end());
	
	if (sounds.find(name) != sounds.end()) {
		printf("AddSound 내용물 중복\n");
		return;
	}

	if (bLoop == true)
		system->createStream(cfile.c_str(), FMOD_LOOP_NORMAL, NULL, &sound[sounds.size()]);
	else
		system->createStream(cfile.c_str(), FMOD_DEFAULT, NULL, &sound[sounds.size()]);
	
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
