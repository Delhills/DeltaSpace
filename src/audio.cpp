#include "audio.h"



Audio::Audio() {
	
}

Audio::Audio(HSAMPLE sample) {
	this->sample = sample;
}


Audio* Audio::Get(const char* filename)
{
	//check if loaded
	auto it = sLoadedAudios.find(filename);

	if (it != sLoadedAudios.end())
		return it->second;

	//load it
	Audio* audio = new Audio();
	
	if (!audio->load(filename))
	{
		delete audio;
		return NULL;
	}

	return audio;
}

bool Audio::load(const std::string& filename)
{
	std::cout << " +Audio: " << filename << std::endl;
	
	HSAMPLE hsample= BASS_SampleLoad( false, filename.c_str(),0, 0, 3, 0);

	if (hsample == 0) {
		std::cout << " ERROR LOADING : " << filename << std::endl;
		return false;
	}

	this->sample = hsample;
	sLoadedAudios.insert({ filename, this });
	
	return true;
}

HCHANNEL* Audio::Play(const char* filename)
{
	Audio* audio = Get(filename);

	HCHANNEL hSampleChannel = BASS_SampleGetChannel(audio->sample,false);

	BASS_ChannelPlay(hSampleChannel,true);

	return &hSampleChannel;
}

void Audio::Stop()
{
	BASS_Pause();
	BASS_Stop();
	BASS_Start();
}

void Audio::Pause()
{
	BASS_Pause();
}

void Audio::UnPause()
{
	BASS_Start();
}
