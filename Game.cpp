#include "Game.h"
#include "pch.h"






void Game::NextPlayer()
{
	do
	{
		++current_player_id;
		if (current_player_id >= all_players.size())
		{
			bot_move = true;
			current_player_id = 0;
		}
	} while (!player_enabled[current_player_id]);
}

Game::Game()
{
	bot_tex.loadFromFile(".\\data\\textures\\bot.png");
	bot_sp.setTexture(bot_tex);
	GetCategoriesPathes(categories_paths);
	GetSavesPathes(saves_paths);

	for (auto path : categories_paths)
	{	
		categories_names.push_back(getFilename(path));
	}

	for (auto path : saves_paths)
	{
		
		saves_names.push_back(getFilename(path));
	}
	
}


Game::~Game()
{
}

std::string Game::getUtf8Letter()
{
	return letter_utf8;
}




void Game::SaveGame(std::wstring & file_name)
{
	
	std::wstring way;
	
	way = createWayToNewSave(file_name);
	saves_paths.push_back(way);
	saves_names.push_back(file_name);


	std::wstring way_to_categ = category.GetPath();
	std::ofstream save(way, std::ios::trunc);
	save.imbue(std::locale("rus_rus.866"));
	std::string last_letter_utf8 = letter_utf8;
	wchar_t buf[2];
	buf[0] = next_letter;
	buf[1] = L'\0';
	std::string last_letter_1251 = wstring_to_utf8(buf);
	std::string disabled_count   = std::to_string(disabled_players_counter);
	std::string current_player   = std::to_string(current_player_id);
	std::string bot_said_        = bot_said;
	std::string bot_lvl_         = std::to_string(bot_lvl);
	std::string bot_score_       = std::to_string(bot_scores);
	std::string players_count    = std::to_string(all_players.size());

	encode(way_to_categ);
	encode(last_letter_utf8);
	encode(last_letter_1251);
	encode(disabled_count);
	encode(current_player);
	encode(bot_said_);
	encode(bot_lvl_);
	encode(bot_score_);
	encode(players_count);

	save << wstring_to_utf8(way_to_categ.c_str())     << std::endl;
	save << last_letter_utf8 << std::endl;
	save << last_letter_1251 << std::endl;
	save << disabled_count   << std::endl;
	save << current_player   << std::endl;
	save << bot_said_        << std::endl;
	save << bot_lvl_         << std::endl;
	save << bot_score_       << std::endl;
	save << players_count    << std::endl;

	for (size_t i = 0; i < all_players.size(); ++i)
	{
		std::string name = all_players[i].getName();
		std::string score = std::to_string(all_players[i].getScore());
		float* colors = all_players[i].getColors();
		
		std::string enable = std::to_string(player_enabled[i]);
		

		encode(name);
		encode(score);
		encode(enable);
		
		save << name  << std::endl;
		save << score << std::endl;
		save << enable<< std::endl;
		for (size_t i = 0; i < 11; ++i) save << colors[i] << std::endl;
	}
	


	const std::set <wstring> words = category.GetUsedWords();
	for (auto it = words.begin(); it != words.end(); ++it)
	{
		std::string word = wstring_to_utf8(it->c_str());
		encode(word);
		save << word << std::endl;
	}

	save.close();

	for (size_t i = 0; i < all_players.size(); ++i)
	{
		all_players[i].setScore(5);
		all_players[i].ResetStyle();
		player_enabled[i] = true;


	}
	category.clearUsedWordsList();
	category.unloadData();
	current_player_id = 0;
	disabled_players_counter = 0;
	all_players_disabled = false;
	can_t_find_word = false;
	bot_losed = false;
	game_ended = true;
	show_result_of_game = false;
	bot_lvl = 2;
	bot_scores = 10;
	bot_said.clear();
}

bool Game::LoadGame(int idx_of_save)
{
	player_and_score.clear();
	
	std::ifstream save(saves_paths[idx_of_save]);
	save.imbue(std::locale("rus_rus.866"));
	if (save.is_open())
	{
		std::wstring way_to_categ;
		std::string tmp;
		std::getline(save, tmp, '\n');
		way_to_categ = utf8_to_wstring(tmp.c_str());
		decode(way_to_categ);
		bool exist = false;
		for (size_t i = 0; i < categories_paths.size(); ++i)
		{
			if (categories_paths[i] == way_to_categ)
			{
				
				exist = true;
				break;
			}
		}
		if (!exist)
		{
			save.close();
			std::filesystem::remove(std::filesystem::path(saves_paths[idx_of_save].c_str()));
			saves_names.erase(saves_names.begin() + idx_of_save);
			saves_paths.erase(saves_paths.begin() + idx_of_save);
			return false;
		}
		std::string buf_letter;
		std::string number_buffer;

		size_t players_count;
		try
		{
			std::getline(save, letter_utf8, '\n');
			decode(letter_utf8);


			std::getline(save, buf_letter, '\n');
			decode(buf_letter);
			next_letter = utf8_to_wstring(buf_letter.c_str())[0];


			std::getline(save, number_buffer, '\n');
			decode(number_buffer);
			
			disabled_players_counter = std::atol(number_buffer.c_str());
			


			std::getline(save, number_buffer, '\n');
			decode(number_buffer);
			current_player_id = std::atol(number_buffer.c_str());
			

			std::getline(save, bot_said, '\n');
			decode(bot_said);

			std::getline(save, number_buffer, '\n');
			decode(number_buffer);
			bot_lvl = std::atol(number_buffer.c_str());
			
			std::getline(save, number_buffer, '\n');
			decode(number_buffer);
			bot_scores = std::atol(number_buffer.c_str());
			
			std::getline(save, number_buffer, '\n');
			decode(number_buffer);
			players_count = std::atol(number_buffer.c_str());
			

			all_players.clear();
			all_players.resize(players_count);
			player_enabled.clear();
			player_enabled.resize(players_count);
			

			std::string name;
			std::string score;
			std::string colors;
			std::string enable;
			for (size_t i = 0; i < players_count; ++i)
			{
				std::getline(save, name, '\n');
				std::getline(save, score, '\n');
				std::getline(save, enable, '\n');
				

				decode(name);
				decode(score);
				decode(enable);
				

				float col[11];
				bool enable_;

				Player player(name, &printed_word);
				all_players[i] = player;
				
				uint64_t s = std::atoll(score.c_str());;
				
				all_players[i].setScore(s);
				
				enable_ = std::atoi(enable.c_str());;
				
				
				for (size_t i = 0; i < 11; ++i)
				{
					std::getline(save, colors, '\n');
					col[i] = (float)std::atof(colors.c_str());;
				}



				all_players[i].setColors(&col[0], &col[4], &col[8]);

				player_enabled[i] = enable_;
			}
			category.setPathAndLoadData(way_to_categ);
			game_ended = false;

			std::wstring str;
			std::string buffer;
			while (1)
			{
				std::getline(save, buffer, '\n');
				if (save.eof()) break;
				
				decode(buffer);
				str = utf8_to_wstring(buffer.c_str());
				category.useThisWord(str);
			}

			all_players_disabled = true;
			for(size_t i = 0; i < player_enabled.size(); ++i)
				if (player_enabled[i])
				{
					all_players_disabled = false;
					break;
				}
		}
		catch (...)
		{
			save.close();
			std::filesystem::remove(std::filesystem::path(saves_paths[idx_of_save].c_str()));
			saves_names.erase(saves_names.begin() + idx_of_save);
			saves_paths.erase(saves_paths.begin() + idx_of_save);
			all_players.clear();
			player_enabled.clear();

			category.clearUsedWordsList();
			category.unloadData();
			current_player_id = 0;
			disabled_players_counter = 0;
			all_players_disabled = false;
			show_result_of_game = true;
			can_t_find_word = false;
			bot_losed = false;
			bot_lvl = 2;
			bot_scores = 10;
			bot_said.clear();
			return false;
		}

		save.close();
	}
	game_ended = false;

	return true;
}

void Game::EndGame()
{
	uint64_t score = bot_scores;
	std::string name = "Terminator";

	player_and_score.push_back(std::pair(name, score));

	
	score = all_players[0].getScore();
	name = all_players[0].getName();

	if (score <= player_and_score[0].second)
		player_and_score.push_back(std::pair(name, score));
	else
		player_and_score.insert(player_and_score.begin(), std::pair(name, score));


	for (size_t i = 1; i < all_players.size(); ++i)
	{
		size_t size = player_and_score.size();
		score = all_players[i].getScore();
		name = all_players[i].getName();
		for (size_t x = 0; x < size; ++x)
		{
			if (x == 0 && score >= player_and_score[x].second)
			{
				player_and_score.insert(player_and_score.begin(), std::pair(name, score));
				break;
			}
			if (x == size - 1) 
			{
				player_and_score.push_back(std::pair(name, score));
				break;
			}
			if (player_and_score[x].second >= score && player_and_score[x + 1].second <= score)
			{
				player_and_score.insert(player_and_score.begin() + x + 1, std::pair(name, score));
				break;
			}
		}
	}
	


	for(size_t i = 0; i < all_players.size(); ++i)
	{
		all_players[i].setScore(5);
		all_players[i].ResetStyle();
		player_enabled[i] = true;


	}
	category.clearUsedWordsList();
	category.unloadData();
	current_player_id = 0;
	disabled_players_counter = 0;
	all_players_disabled = false;
	show_result_of_game = true;
	can_t_find_word = false;
	bot_losed = false;
	bot_lvl = 2;
	bot_scores = 10;
	printed_word.clear();
	word_printed = false;
	bot_said.clear();
}

bool Game::StartGame(sf::Vector2i window_pos,
	                 sf::Vector2u window_size,
	                 size_t idx_of_choosed_category,
	                 size_t bot_lvl)
{
	player_and_score.clear();
	
	this->bot_lvl = bot_lvl;
	

	game_ended = false;

	bool loading_window_started = true;
	auto f = std::async(std::launch::async, LoadingWindow,
		window_pos,
		window_size,
		std::ref(loading_window_started));

	category.setPathAndLoadData(categories_paths[idx_of_choosed_category]);
	loading_window_started = false;
	words_counter = category.getWordsCount();
	if (category.getWordsCount() < 100)
	{
		game_ended = true;
		this->bot_lvl = 2;

		category.unloadData();
		return false;
	}

	for (size_t i = 0; i < category.getSizeOfTable(); ++i)
	{
		
		if (category.WordsSet()[i].size() >= 1)
		{
			next_letter = category.WordsSet()[i][0];
			letter_utf8 = wchar_to_utf8(next_letter);
			break;
		}
		

	}

	
	return true;
}



bool Game::AddNewPlayer(std::string name)
{
	for (size_t i = 0; i < all_players.size(); i++) 
	{
		if (all_players[i].getName() == name) 
		{
			return false;
		}

	}
	Player new_player(name, &printed_word);
	all_players.push_back(new_player);
	player_enabled.push_back(true);
	return true;
}

void Game::CheckPrintedWord(bool &answer)
{
	if(word_printed)
	{

		if (!category.findLetter(this->next_letter))
		{
			can_t_find_word = true;
			return;
		}

		if (category.findWord(printed_word) && next_letter == printed_word[0]) {
			

			category.useThisWord();
			category.getLastChar(printed_word, next_letter);

			
			letter_utf8 = wchar_to_utf8(next_letter);

			all_players[current_player_id].incScore();

			NextPlayer();

			printed_word.clear();
			answer = true;
			
		}
		else 
		{
			all_players[current_player_id].decScore();
			if (all_players[current_player_id].getScore() == 0)
			{
				++disabled_players_counter;
				player_enabled[current_player_id] = false;

				

				if (disabled_players_counter == all_players.size())
					all_players_disabled = true;
				else
					NextPlayer();
			}
			else
				NextPlayer();


			printed_word.clear();


			
			answer = false;
			
			
		}
		
		if (category.getWordsCount() == 0) can_t_find_word = true;
		word_printed = false;
	}
	
}


void Game::ShowResults()
{
	ImGui::SetNextWindowPosCenter(ImGuiCond_::ImGuiCond_FirstUseEver);
	ImGui::PushFont(ImGui::GetIO().Fonts->Fonts[1]);
	ImGui::Begin(u8"Результаты игры", nullptr, { 350, 500 }, 1.0f,
		ImGuiWindowFlags_::ImGuiWindowFlags_NoResize |
		ImGuiWindowFlags_::ImGuiWindowFlags_NoSavedSettings |
		ImGuiWindowFlags_::ImGuiWindowFlags_NoCollapse);
	
	
	
	
	
	ImGui::BeginChild(u8"##show_results", { 0, 380 }, true, 
		ImGuiWindowFlags_::ImGuiWindowFlags_HorizontalScrollbar);
	ImGui::PopFont();
	ImGui::PushFont(ImGui::GetIO().Fonts->Fonts[0]);
	ImGui::Columns(2, "results");
	ImGui::TextWrapped(u8"Имя:");
	ImGui::NextColumn();
	ImGui::TextWrapped(u8"Очки:");
	ImGui::Separator();
	ImGui::NextColumn();

	
	
	
	for (size_t i = 0; i < player_and_score.size(); i++)
	{
		ImGui::TextWrapped(player_and_score[i].first.c_str());
		
		ImGui::NextColumn();
		ImGui::PopFont();
		ImGui::PushFont(ImGui::GetIO().Fonts->Fonts[2]);
		ImGui::TextWrapped(std::to_string(player_and_score[i].second).c_str());
		ImGui::Separator();
		ImGui::NextColumn();
		ImGui::PopFont();
		ImGui::PushFont(ImGui::GetIO().Fonts->Fonts[0]);
	}
	ImGui::EndChild();





	ImGui::NewLine();
	if (ImGui::Button("Ok", {120, 35}))
	{
		player_and_score.clear();
		show_result_of_game = false;
	}





	ImGui::End();
	ImGui::PopFont();
}

void Game::ShowBot()
{
	ImGui::PushStyleColor(ImGuiCol_::ImGuiCol_WindowBg,
		{ 0.f, 0.5176f, 1.f, 1.f });
	ImGui::SetNextWindowPosCenter(ImGuiCond_::ImGuiCond_FirstUseEver);
	ImGui::PushFont(ImGui::GetIO().Fonts->Fonts[0]);
	ImGui::Begin("AI (Terminator)", nullptr, { 220, 210 }, 1.0f,
		ImGuiWindowFlags_::ImGuiWindowFlags_NoResize |
		ImGuiWindowFlags_::ImGuiWindowFlags_NoSavedSettings |
		ImGuiWindowFlags_::ImGuiWindowFlags_NoCollapse);
	ImGui::Image(bot_sp, {200.f, 100.f});
	


	if (bot_move && !bot_losed)
	{
		if ((rand() % (bot_lvl * bot_lvl - (bot_lvl - 1) * 2)) == 0)
		{
			unsigned idx = 0;
			if (category.findLetter(this->next_letter, &idx))
			{
				bot_said = wstring_to_utf8(category.WordsSet()[idx].c_str());
				
				if (bot_said.empty())
				{
					bot_said = u8"Я ошибся.";
					--bot_scores;
					if (bot_scores == 0)
					{
						bot_losed = true;
						bot_said = u8"Я проиграл.";
					}
				}
				else
				{
					std::wstring str = category.WordsSet()[idx].c_str();
					category.useThisWord(str);
					category.getLastChar(str, next_letter);
					letter_utf8 = wchar_to_utf8(next_letter);
					++bot_scores;
				}

			}
			else can_t_find_word = true;
		}
		else
		{
			bot_said = u8"Я ошибся.";
			--bot_scores;
			if (bot_scores == 0)
			{
				bot_losed = true;
				bot_said = u8"Я проиграл.";
			}
		}
		bot_move = false;
		if (category.getWordsCount() == 0) can_t_find_word = true;
	}
	else
		bot_move = false;
	ImGui::Text(("said:  " + bot_said).c_str());
	ImGui::Text(("score: " + std::to_string(bot_scores)).c_str());
	ImGui::End();
	ImGui::PopFont();
	ImGui::PopStyleColor();
}

void Game::ShowPlayers()
{
	for (size_t i = 0; i < all_players.size(); i++) 
	{
		ImGui::PushID(i);
		if (player_enabled[i]) 
		{
			if (current_player_id == i) all_players[i].ShowLight(word_printed, letter_utf8);
			else all_players[i].ShowDark();
		}
		else
		{
			
			ImGui::PushFont(ImGui::GetIO().Fonts->Fonts[1]);
			ImGui::Begin(u8"Список проигравших игроков", nullptr, { 420, 300 }, 1.0f,
				ImGuiWindowFlags_::ImGuiWindowFlags_NoResize |
				ImGuiWindowFlags_::ImGuiWindowFlags_NoSavedSettings |
				ImGuiWindowFlags_::ImGuiWindowFlags_HorizontalScrollbar);
			ImGui::PopFont();
			ImGui::PushFont(ImGui::GetIO().Fonts->Fonts[0]);
			ImGui::Text(all_players[i].getName().c_str());
			ImGui::Separator();
			ImGui::End();
			ImGui::PopFont();
		}
		ImGui::PopID();
	}
}

