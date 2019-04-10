#pragma once
#include "pch.h"

class Music final
{
private:
	std::vector <std::string>* music_path;
	sf::Music                  background_music;
	bool                       can_play = false;
	bool                       is_playing = false;
	size_t                     current_track_idx = 0;
	float                      volume;
public:
	
	//initialization
	Music();
	Music(std::vector <std::string> *music_path);
	void init(std::vector <std::string> *music_path);

	//operations
	void play();

	void pause();

	void stop();

	void switchTrack();
	
	void nextTrack();
	void prevTrack();
	
	//get-set
	void setTrack(size_t id);

	void setVolume(float volume);

	float getVolume() const;


	bool isPlaying() const;
	void setPlayingFlag(bool flag);

	size_t getCurrentTrkIdx() const;
	void setCurrentTrkIdx(size_t idx);
};
