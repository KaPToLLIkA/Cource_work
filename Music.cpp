#include "Music.h"
#include "pch.h"

//initialization
Music::Music() {};
Music::Music(std::vector <std::wstring> *music_path) :
	music_path(music_path)
{
	if (music_path->size() > 0)
	{
		can_play = true;
		background_music.openFromFile(utf8_to_string(music_path->at(0), std::locale(".1251")));

	}
};
void Music::init(std::vector <std::wstring> *music_path)
{
	this->music_path = music_path;
	if (music_path->size() > 0)
	{
		can_play = true;
		background_music.openFromFile(utf8_to_string(music_path->at(0), std::locale(".1251")));

	}
}

//operations
void Music::play()
{
	if (can_play
		&& background_music.getStatus() != background_music.Playing
		|| background_music.getStatus() == background_music.Paused)
	{
		background_music.play();
		this->is_playing = true;
	}
}

void Music::pause()
{
	if (can_play)
	{
		background_music.pause();
		this->is_playing = false;
	}
}

void Music::stop()
{
	background_music.stop();
	this->is_playing = false;
}

void Music::switchTrack()
{
	if (can_play && is_playing
		&& background_music.getStatus() == background_music.Stopped) {
		current_track_idx++;

		if (current_track_idx >= music_path->size()) current_track_idx = 0;
		background_music.openFromFile(utf8_to_string(music_path->at(current_track_idx), std::locale(".1251")));
		background_music.play();
	}
}

void Music::nextTrack()
{
	if (can_play)
	{
		if (background_music.getStatus() == background_music.Playing
			|| background_music.getStatus() == background_music.Paused)
			background_music.stop();
		current_track_idx++;

		if (current_track_idx >= music_path->size()) current_track_idx = 0;

		background_music.openFromFile(utf8_to_string(music_path->at(current_track_idx), std::locale(".1251")));
		background_music.play();
		this->is_playing = true;
	}
}
void Music::prevTrack()
{
	if (can_play)
	{
		if (background_music.getStatus() == background_music.Playing
			|| background_music.getStatus() == background_music.Paused)
			background_music.stop();
		current_track_idx--;

		if (current_track_idx >= music_path->size())
			current_track_idx = music_path->size() - 1;

		background_music.openFromFile(utf8_to_string(music_path->at(current_track_idx), std::locale(".1251")));
		background_music.play();
		this->is_playing = true;
	}
}

//get-set
void Music::setTrack(size_t id)
{
	if (can_play)
	{
		background_music.stop();
		current_track_idx = id;

		background_music.openFromFile(utf8_to_string(music_path->at(current_track_idx), std::locale(".1251")));
		background_music.play();
		this->is_playing = true;
	}

}

void Music::setVolume(float volume)
{
	this->volume = volume;
	background_music.setVolume(volume);
}

float Music::getVolume() const
{
	return volume;
}

bool Music::isPlaying() const
{
	return is_playing;
}

void Music::setPlayingFlag(bool flag)
{
	this->is_playing = flag;
}

size_t Music::getCurrentTrkIdx() const
{
	return current_track_idx;
}

void Music::setCurrentTrkIdx(size_t idx)
{
	current_track_idx = idx;
}
