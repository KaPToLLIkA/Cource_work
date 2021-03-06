#pragma once
#include "pch.h"
#include "Player.h"
#include "utils.h"
#include "Words.h"

#include <future>






class Game final
{
private:
	

	sf::Texture bot_tex;
	sf::Sprite  bot_sp;

	wchar_t next_letter;
	std::wstring printed_word;
	std::string letter_utf8;
	std::string bot_said;
	size_t bot_scores = 10;
	size_t bot_lvl = 2;

	bool word_printed         = false;
	bool bot_move             = false;

	Words category;

	
	void NextPlayer();
public:
	bool bot_losed            = false;
	bool show_result_of_game  = false;
	bool game_ended           = true;
	bool all_players_disabled = false;
	bool can_t_find_word      = false;
	
	size_t disabled_players_counter = 0;
	uint64_t words_counter = 0;
	std::vector <std::wstring> categories_paths;
	std::vector <std::wstring> categories_names;
	std::vector <std::wstring> saves_paths;
	std::vector <std::wstring> saves_names;
	

	std::vector <Player> all_players;
	std::vector <bool> player_enabled;
	result_list player_and_score;
	
	
	unsigned current_player_id = 0;
	
	
	Game();
	~Game();
	std::string getUtf8Letter();
	


	void SaveGame(std::wstring& file_name);
	bool LoadGame(int idx_of_save);
	void EndGame();
	bool StartGame(sf::Vector2i window_pos,
				   sf::Vector2u window_size, 
		           size_t idx_of_choosed_category, 
		           size_t bot_lvl);
	


	
	bool AddNewPlayer(std::string name);
	void CheckPrintedWord(bool &answer);
	


	void ShowResults();
	void ShowBot();
	void ShowPlayers();


};

