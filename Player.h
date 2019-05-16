#pragma once
#include "pch.h"
#include "utils.h"




class Player final
{
private:
	float background_col[4] =	{ 0.f, 0.f, 0.f, 0.7f };
	float text_col[3] =			{ 1.f, 1.f, 1.f };
	float header_col[4] =		{ 0.f, 0.f, 0.f, 0.85f };
	
	std::wstring* printed_word = nullptr;
	std::array <char, 128>  buffer;
	bool keyboard_focus = false;
	bool first_show_light = true;
	
	std::string name;
	uint64_t    score = 5;
public:
	const std::string& getName() const;
	
	uint64_t getScore() const;
	void setScore(uint64_t score);
	void incScore();
	void decScore();

	Player();
	Player(	std::string  name, 
			std::wstring* printed_word);
	Player(const Player &obj);

	Player &operator=(const Player &right);

	~Player();

	void ResetStyle();
	void ShowLight(bool &word_printed, std::string letter);
	void ShowDark();
	float* getColors();
	void setColors(float* header, float* background, float* text);
};

