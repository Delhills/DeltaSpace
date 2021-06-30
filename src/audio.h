#pragma once
#ifndef AUDIO_H
#define AUDIO_H
#include "extra/bass.h"
#include <map> 
#include <string>
#include "utils.h"


constexpr auto numSongs = 7;

static char* Songs[numSongs] =
{
	"data/sound/baile.mp3",
	"data/sound/level1.mp3",
	"data/sound/choque.mp3",
	"data/sound/level2.mp3",
	"data/sound/level3.mp3",
	"data/sound/credits.mp3",
	"data/sound/boost.mp3"
};

enum eSongs {
	DANCE,
	SAILOR,
	COLISION,
	EVA,
	UNDER,
	CREDITS,
	BOOST
};

class Audio
{
public:
	HSAMPLE sample; //aqui guardamos el handler del sample que retorna BASS_SampleLoad

	Audio();
	Audio(HSAMPLE sample); //importante poner sample a cero aqui
	//~Audio(); //aqui deberiamos liberar el sample con BASS_SampleFree

	//HCHANNEL play(float volume); //lanza el audio y retorna el channel donde suena
	bool load(const std::string& filename);
	//static void Stop(HCHANNEL channel); //para parar un audio necesitamos su channel
	static Audio* Get(const char* filename); //manager de audios 
	static HCHANNEL* Play(const char* filename); //version estática para ir mas rapido
	static void Stop();
	static void Pause();
	static void UnPause();
};

static std::map<std::string, Audio*> sLoadedAudios;
#endif