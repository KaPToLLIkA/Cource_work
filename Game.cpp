#include "Game.h"
#include "pch.h"


bool Game::findLetter(unsigned *idx)
{
	size_t sz = category.getSizeOfTable();
	for (size_t i = 0; i < sz; ++i)
	{
		if (category.WordsSet()[i].size() == 0)
			continue;
		if (category.WordsSet()[i][0][0] == letter_utf8[0])
		{
			std::string letter = utf8_to_string(letter_utf8.c_str(), std::locale(".1251"));
			std::string letter_in_table = utf8_to_string(category.WordsSet()[i][0].c_str(), std::locale(".1251"));

			if (letter_in_table[0] == letter[0])
			{
				if (idx != nullptr) *idx = i;
				return true;
			}

		}


	}
	return false;
	
}

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
		std::string str = getFilename(path);
		str = cp1251_to_utf8(str);
		categories_names.push_back(str);
	}

	for (auto path : saves_paths)
	{
		std::string str = getFilename(path);
		str = cp1251_to_utf8(str);
		saves_names.push_back(str);
	}
	
}


Game::~Game()
{
}

std::string Game::getUtf8Letter()
{
	return letter_utf8;
}

Words & Game::getCategory()
{
	return category;
}



void Game::SaveGame(std::string & file_name)
{
	
	std::string way;
	
	way = createWayToNewSave(file_name);
	saves_paths.push_back(way);
	saves_names.push_back(file_name);


	std::string way_to_categ = category.GetPath();
	std::ofstream save(way, std::ios::trunc);
	std::string last_letter_utf8 = letter_utf8;
	char buf[2];
	buf[0] = next_letter;
	buf[1] = '\0';
	std::string last_letter_1251 = buf;
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

	save << way_to_categ     << std::endl;
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
	


	const std::set <std::string> words = category.GetUsedWords();
	for (auto it = words.begin(); it != words.end(); ++it)
	{
		std::string word = *it;
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
	bot_lvl = 2;
	bot_scores = 10;
	bot_said.clear();
}

bool Game::LoadGame(int idx_of_save)
{
	player_and_score.clear();
	std::stringstream conv;
	std::ifstream save(saves_paths[idx_of_save]);
	if (save.is_open())
	{
		std::string way_to_categ;
		std::getline(save, way_to_categ, '\n');
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
			remove(saves_paths[idx_of_save].c_str());
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
			next_letter = buf_letter[0];


			std::getline(save, number_buffer, '\n');
			decode(number_buffer);
			conv << number_buffer;
			conv >> disabled_players_counter;
			conv.clear();


			std::getline(save, number_buffer, '\n');
			decode(number_buffer);
			conv << number_buffer;
			conv >> current_player_id;
			conv.clear();

			std::getline(save, bot_said, '\n');
			decode(bot_said);

			std::getline(save, number_buffer, '\n');
			decode(number_buffer);
			conv << number_buffer;
			conv >> bot_lvl;
			conv.clear();

			std::getline(save, number_buffer, '\n');
			decode(number_buffer);
			conv << number_buffer;
			conv >> bot_scores;
			conv.clear();

			std::getline(save, number_buffer, '\n');
			decode(number_buffer);
			conv << number_buffer;
			conv >> players_count;
			conv.clear();


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
				conv << score;
				uint64_t s;
				conv >> s;
				all_players[i].setScore(s);
				conv.clear();
				
				conv << enable;
				conv >> enable_;
				conv.clear();
				
				
				for (size_t i = 0; i < 11; ++i)
				{
					std::getline(save, colors, '\n');
					conv << colors;
					conv >> col[i];
					conv.clear();

				}



				all_players[i].setColors(&col[0], &col[4], &col[8]);

				player_enabled[i] = enable_;
			}
			category.setPathAndLoadData(way_to_categ);
			game_ended = false;

			std::string buffer;
			while (1)
			{
				std::getline(save, buffer, '\n');
				if (save.eof()) break;

				decode(buffer);
				category.useThisWord(buffer);
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
			remove(saves_paths[idx_of_save].c_str());
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
	for (size_t i = 0; i < category.getSizeOfTable(); ++i)
	{
		if (!category.WordsSet()[i].empty())
		{
			std::string word = utf8_to_string(category.WordsSet()[i][0].c_str(), std::locale(".1251"));
			if (word.size() >= 1)
			{
				next_letter = word[0];
				letter_utf8 = convert_1251char_to_utf8(next_letter);
				break;
			}
		}

	}

	if (category.getWordsCount() < 100)
	{
		game_ended = true;
		this->bot_lvl = 2;
		
		category.unloadData();
		return false;
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
		std::string str = utf8_to_string(printed_word.c_str(), std::locale(".1251"));
		
		std::transform(str.begin(), str.end(),  str.data(), tolower);
		
		char*buffer = new char[str.size() * 4];
		ZeroMemory(buffer, str.size() * 4);
		cp1251_to_utf8(buffer, str.c_str());
		
		std::memcpy(printed_word.data(), buffer, strlen(buffer));
		

		delete[] buffer;

		if (!findLetter())
		{
			can_t_find_word = true;
			return;
		}

		if (category.findWord(printed_word) && next_letter == str[0]) {
			

			category.useThisWord();
			category.getLastChar(str, next_letter);

			
			letter_utf8 = convert_1251char_to_utf8(next_letter);

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
			/*unsigned idx = category.safeHashF(letter_utf8);

			std::string word;
			while (true) {
				if (category.WordsSet()[idx].size() > 0)
				{

					word = *category.WordsSet()[idx].begin();
					std::string str = utf8_to_string(word.c_str(), std::locale(".1251"));
					if (next_letter == str[0])
					{
						category.useThisWord(word);
						++bot_scores;
						category.getLastChar(str, next_letter);
						letter_utf8 = convert_1251char_to_utf8(next_letter);

						break;
					}
					else
					{
						can_t_find_word = true;
						break;
					}
				}
				idx++;
				if (idx >= category.getSizeOfTable())
				{
					can_t_find_word = true;
					break;
				}
			}*/
			unsigned idx = 0;
			if (findLetter(&idx))
			{
				bot_said = *category.WordsSet()[idx].begin();
				category.useThisWord(bot_said);
				std::string str = utf8_to_string(bot_said.c_str(), std::locale(".1251"));
				category.getLastChar(str, next_letter);
				letter_utf8 = convert_1251char_to_utf8(next_letter);
				++bot_scores;

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

