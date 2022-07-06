#pragma once

using namespace FMOD;

class CAudio
{
public:
	CAudio();
	~CAudio();

	void Update();

	void AddSound(string name, string file, bool bLoop = false);
	void AddSound(string name, wstring file, bool bLoop = false);
	void Play(string name, float volume = 1.0f);
	void Stop(string name);
	void Pause(string name);
	void Resume(string name);

	bool Playing(string name);
	bool Paused(string name);

	void Volume(string name, float volume);
	float Volume() { return volume; }


private:
	System* system;
	Sound** sound;
	Channel** channel;

	UINT count;

	map<string, Sound**> sounds;
	map<string, Sound**>::iterator iter;

	float volume;

};