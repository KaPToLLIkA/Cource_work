#include "pch.h"
#include <condition_variable>
#include <iostream>
#include <future>
#include <chrono>


#include "portable-file-dialogs.h"

#include "utils.h"
#include "Player.h"
#include "Words.h"
#include "Game.h"
#include "msgBoxes.h"
#include "Music.h"


static std::string user_name_str;
static std::string words_counter;
static std::string letter_utf8;
static std::array <char, 512> user_name_arr;

static Game  game;

static std::string save_name_str;
static std::array<char, 512> save_name_arr;

static bool load_save = false;
static bool del_save = false;
static size_t idx_of_save = 0;



static bool fullscreen           = false;
static bool window_closed        = false;
static bool window_resized       = false;
static bool static_background    = false;
static bool start_loading_window = false;
static bool is_right_word        = true;
static bool keyboard_focus       = false; 

static int frame_count = 0;
static size_t current_background = 0;
static size_t choosed_background = 0;


namespace categEditor
{
	static bool word_added   = false;
	static bool word_deleted = false;
	static bool exit_redo    = false;
	static bool do_duplicate = false;
	

	static size_t choosed_category = 0;
	static Words  changeable_category;
	static std::string category_name_str;
	static std::array <char, 512> category_name_arr;
	static std::string rename_buf_str;
	static std::array <char, 512> rename_buf_arr;


}
namespace wordsBuf {
	static const size_t input_words_buffer_size = 10240000;
	static char* input_words_buffer = new char[input_words_buffer_size];
	
}
namespace sounds 
{
	static Music main_theme_track;
	
	
	static std::vector <std::string> playlist;

	static ImVec4 player_color{ 1.f, 0.f, 0.f, 1.f };

	static float volume_of_sound = 20.f;
	static float volume_of_music = 20.f;
	static bool sound_off = false;
}
namespace res 
{
	namespace texture 
	{
		static sf::Texture warning_box;
		static sf::Texture question_box;


		static sf::Texture window_example;


		static std::vector  <sf::Texture> all_backgrounds;
		static size_t array_allocate_size = 1;
		static size_t array_current_size = 0;

	}
	namespace sprite 
	{
		static sf::Sprite background1;
		static sf::Sprite background2;
		static sf::Sprite static_background;

		static sf::Sprite warning_box;
		static sf::Sprite question_box;

		static std::vector <sf::Sprite> all_backgrounds;
	}
}
namespace windowSize 
{
	static sf::Vector2u const_current;
	static sf::Vector2u current{ 800, 600 };
	static sf::Vector2i New;
	static sf::Vector2u min{ 800, 600 };
	static sf::Vector2u max{
		static_cast<unsigned>(GetSystemMetrics(SM_CXSCREEN)),
		static_cast<unsigned>(GetSystemMetrics(SM_CYSCREEN))
	};
}
namespace pathes 
{
	static std::vector <std::string> music_path;
	static std::vector <std::string> image_path;
	static std::vector <std::string> addicted_image_pathes;
}
namespace bmpScreen 
{
	static BYTE *bmp_pixels = nullptr;
	static BITMAPINFO bmp_info;
	static BITMAPFILEHEADER bmfHeader;
	static DWORD dwSizeofDIB;
	static size_t size_of_bmp;
	static BYTE *file = nullptr;
	static BYTE *h    = nullptr;
	static BYTE *i    = nullptr;
	static BYTE *p    = nullptr;
}
namespace threadLockers 
{
	static std::mutex lock_play_sound;
	static std::condition_variable play_sound;
	static bool sound_thread_notified = false;

	static std::mutex lock_color_change;
	static std::condition_variable color_change;
	static bool color_thread_notified = false;
	static bool color_thread_start_work = false;

	static std::mutex lock_background_change;
	static std::condition_variable background_change;
	static bool backround_change_thread_notified = false;

}
namespace threadEndFlags 
{
	static std::promise <bool> animation_p;
	static std::promise <bool> dinamic_color_p;
	static std::promise <bool> sound_play_p;
	static auto animation_f = animation_p.get_future();
	static auto dinamic_color_f = dinamic_color_p.get_future();
	static auto sound_play_f = sound_play_p.get_future();
}
namespace showParams 
{
	//enabling - disabling
	static bool show_exit_game_enabled       = false;
	static bool category_enable_deleting     = false;
	static bool category_enable_adding       = false;
	static bool category_enable_save         = false;
	static bool add_player_enabled           = true;
	static bool show_player_list_enabled     = true;
	static bool close_program_button_enabled = true;
	static bool category_editor_enabled      = true;
	static bool category_enable_search       = true;

	//showing
	static bool show_settings        = false;
	static bool show_users_list      = false;
	static bool show_music_manager   = false;
	static bool show_add_player_menu = false;
	static bool show_category_editor = false;
	static bool show_category_redo   = false;
	static bool show_only_clear      = false;
	static bool show_category_choose = false;
	static bool del_category         = false;
	static bool show_get_name_dialog = false;
	static bool show_start_game_dlg  = false;
	static bool show_save_game_dlg   = false;
	static bool show_load_game_dlg   = false;
	static bool show_help_search     = false;
}




void DoScreenshot          (HWND handle);
		
void WakeDinamicColorThread();
void WakeMsgBoxSoundThread ();
void WakeAnimationThread   ();

void msgBoxSoundPlayThread ();
void AnimationThread       ();
void ThreadDinamicColor    ();

void ShowMenu              (sf::Vector2i wnd_pos);
void ShowSettings          (sf::RenderWindow &window);
void ShowMusicManager      (Music &track);
void ShowUsersList         ();
void ShowAddPlayerMenu     ();
void ShowPlayerEntry       (size_t id);
void ShowCategorySettings  (sf::Vector2i wnd_pos);

void ShowRenameDialog      (sf::Vector2i wnd_pos);
void ShowStartGameDialog   (sf::Vector2i wnd_pos);
void ShowSaveMenu          (sf::Vector2i wnd_pos);
void ShowLoadGameMenu      ();
void ShowHelpWindow        ();
int main()
{

	setlocale(LC_ALL, "");
	srand(static_cast<unsigned int>(time(nullptr)));
	//=======================================================
	//loading system params and resources
	sounds::main_theme_track.setPlayingFlag(true);
	{
		bool flag;
		LoadSystemParams(sounds::volume_of_sound,
			sounds::volume_of_music,
			windowSize::current,
			sounds::sound_off,
			flag,
			fullscreen,
			static_background,
			current_background);
		sounds::main_theme_track.setPlayingFlag(flag);
	}
	sf::Uint32 style;
	if (fullscreen)
	{
		style = sf::Style::Fullscreen;
		windowSize::current = windowSize::max;
	}
	else style = sf::Style::Default;
	

	//pre init window
	sf::RenderWindow window(sf::VideoMode(windowSize::current.x, windowSize::current.y),
		"words", style);

	start_loading_window = true;
	
	auto f = std::async(std::launch::async, LoadingWindow, 
											window.getPosition(), 
											windowSize::current,
											std::ref(start_loading_window));
	
	
	//search of files
	LoadAddictedImagePaths(pathes::addicted_image_pathes);

	GetImagesPathes(pathes::image_path);
	GetMusicPathes(pathes::music_path, sounds::playlist);
	
	

	

	

	//=======================================================
	//init window
	
	window.setFramerateLimit(60);
	ImGui::SFML::Init(window, false);
	
	LoadAllBackgroundTextures(res::texture::all_backgrounds,
                              pathes::image_path,
                              pathes::addicted_image_pathes);

	
	
	//=======================================================
	//init some variables
	
	

	windowSize::const_current = window.getSize();

	windowSize::New = { static_cast<int>(windowSize::current.x),
						static_cast<int>(windowSize::current.y) };

	ZeroMemory(user_name_arr.data(), user_name_arr.size());
	ZeroMemory(categEditor::category_name_arr.data(), categEditor::category_name_arr.size());
	ZeroMemory(wordsBuf::input_words_buffer, wordsBuf::input_words_buffer_size);
	ZeroMemory(categEditor::rename_buf_arr.data(), categEditor::rename_buf_arr.size());
	ZeroMemory(save_name_arr.data(), save_name_arr.size());

	//=======================================================
	//setting sound
	if (sounds::sound_off) sounds::button_click.sound.setVolume(0.0f);
	else sounds::button_click.sound.setVolume(sounds::volume_of_sound);
	
	if (sounds::sound_off) sounds::show_msg_box.sound.setVolume(0.0f);
	else sounds::show_msg_box.sound.setVolume(sounds::volume_of_sound);

	

	sounds::main_theme_track.init(&pathes::music_path);
	sounds::main_theme_track.setVolume(sounds::volume_of_music);
	

	if (sounds::volume_of_music == 0.0f && sounds::main_theme_track.isPlaying()) 
		sounds::main_theme_track.pause();
	else if (sounds::volume_of_music != 0.0f && sounds::main_theme_track.isPlaying()) 
		sounds::main_theme_track.play();
	

	//=======================================================
	//init threads
	std::thread animation(AnimationThread);
	std::thread play_msg_box_sound(msgBoxSoundPlayThread);
	std::thread color_change_thread(ThreadDinamicColor);

	

	
	

	//=======================================================
	//detach threads
	animation.detach();
	play_msg_box_sound.detach();
	color_change_thread.detach();

	//=======================================================
	//resource loading
	ImGui::GetIO().Fonts->AddFontFromFileTTF("data\\12144.ttf", 20.f, nullptr, ImGui::GetIO().Fonts->GetGlyphRangesCyrillic());
	ImGui::GetIO().Fonts->AddFontFromFileTTF("data\\12144.ttf", 28.f, nullptr, ImGui::GetIO().Fonts->GetGlyphRangesCyrillic());
	ImGui::GetIO().Fonts->AddFontFromFileTTF("data\\12144.ttf", 36.f, nullptr, ImGui::GetIO().Fonts->GetGlyphRangesCyrillic());
	ImGui::GetIO().Fonts->AddFontFromFileTTF("data\\12144.ttf", 16.f, nullptr, ImGui::GetIO().Fonts->GetGlyphRangesCyrillic());
	ImGui::SFML::UpdateFontTexture();
	
	res::texture::warning_box.loadFromFile(".\\data\\textures\\!.png");
	res::sprite::warning_box.setTexture(res::texture::warning_box);
	res::sprite::warning_box.scale(0.25f, 0.25f);

	res::texture::question_box.loadFromFile(".\\data\\textures\\question.png");
	res::sprite::question_box.setTexture(res::texture::question_box);
	res::sprite::question_box.scale(0.35f, 0.35f);

	

	//=======================================================
	//init background sprites
	LoadAllBckgrndExampleSp(res::texture::all_backgrounds,
		res::sprite::all_backgrounds);

	if (res::sprite::all_backgrounds.size() != 0)
	{
		if (res::sprite::all_backgrounds.size() == 1)
		{
			static_background = true;
			res::sprite::static_background = SetBackgrounSprite(res::sprite::all_backgrounds[0],
				windowSize::const_current);
		}
		else
		{
			res::sprite::background1 = SetBackgrounSprite(res::sprite::all_backgrounds[0],
				windowSize::const_current);
			res::sprite::background2 = SetBackgrounSprite(res::sprite::all_backgrounds[1],
				windowSize::const_current);

		}
		if (static_background)
		{

			res::sprite::static_background = SetBackgrounSprite(res::sprite::all_backgrounds[current_background],
				windowSize::const_current);
		}
	}
	start_loading_window = false;
	//=======================================================
	//main loop
	sf::Clock deltaClock;
	while (window.isOpen()) 
	{
		// threads control section
		if (showParams::show_music_manager && !threadLockers::color_thread_notified)
			WakeDinamicColorThread();


		sf::Event event;
		while (window.pollEvent(event)) 
		{
			ImGui::SFML::ProcessEvent(event);

			if (event.type == sf::Event::Closed) 
			{
				if (showParams::close_program_button_enabled && !showParams::show_category_redo) 
				{
					WakeMsgBoxSoundThread();
					msgBoxFlags::msg_box_opend = true;
					msgBoxFlags::show_close_window_question = true;
				}
				else if (!game.game_ended)
				{
					msgBoxFlags::msg_box_opend = true;
					msgBoxFlags::show_end_game_dialog = true;
					WakeMsgBoxSoundThread();
				}
				else if (showParams::show_category_redo)
				{
					msgBoxFlags::msg_box_opend = true;
					msgBoxFlags::exit_categoty_redo = true;
					WakeMsgBoxSoundThread();
				}
			}
			if (event.type == sf::Event::Resized) 
			{
				window.setSize(windowSize::current);
			}

		}

		ImGui::SFML::Update(window, deltaClock.restart());
		sounds::main_theme_track.switchTrack();
		
		//====================================================================
		//ImGui render block...
		//====================================================================
		
		
		if (!msgBoxFlags::msg_box_opend) 
		{
			if (game.game_ended) 
			{
				if (showParams::show_users_list)		ShowUsersList();
				if (showParams::show_add_player_menu)	ShowAddPlayerMenu();
				if (game.show_result_of_game)			game.ShowResults();
				if (showParams::show_category_editor)   ShowCategorySettings(window.getPosition());
				if (showParams::show_start_game_dlg)	ShowStartGameDialog(window.getPosition());
				if (showParams::show_load_game_dlg)     ShowLoadGameMenu();
				if (showParams::show_help_search)		ShowHelpWindow();
			}
			else 
			{
				
				
					
				if (!msgBoxFlags::wrong_word_box)
				{
					game.ShowBot();
					game.ShowPlayers();

				} 
				else
				{
					WarningBox(res::sprite::warning_box, u8"Хммм..",
						u8"Кажется такого слова нет... \n(возможно вы его уже использовали)");
				
				}


				game.CheckPrintedWord(is_right_word);


				if (!is_right_word)
				{
					WakeMsgBoxSoundThread();
					msgBoxFlags::wrong_word_box = true;

					is_right_word = true;
				}


				if (game.all_players_disabled
					|| game.can_t_find_word
					|| (game.bot_losed && game.all_players.size() - game.disabled_players_counter < 2))
				{
					game.game_ended = true;
					start_loading_window = true;

					auto f = std::async(std::launch::async, LoadingWindow,
						window.getPosition(),
						windowSize::current,
						std::ref(start_loading_window));

					showParams::add_player_enabled = true;
					showParams::show_player_list_enabled = true;
					showParams::close_program_button_enabled = true;
					showParams::category_editor_enabled = true;

					if (game.bot_losed && game.all_players.size() - game.disabled_players_counter == 1)
					{
						WakeMsgBoxSoundThread();
						msgBoxFlags::msg_box_opend = true;
						msgBoxFlags::show_bot_losed_msg = true;
					}
					else if (game.can_t_find_word)
					{
						WakeMsgBoxSoundThread();
						msgBoxFlags::msg_box_opend = true;
						letter_utf8 = game.getUtf8Letter();
						msgBoxFlags::show_can_t_fint_word_msg = true;
					}
					else
					{
						WakeMsgBoxSoundThread();
						msgBoxFlags::all_players_lost = true;
						msgBoxFlags::msg_box_opend = true;
					}

					game.EndGame();


					start_loading_window = false;
					
				}
			}


			if (showParams::show_music_manager)		ShowMusicManager(sounds::main_theme_track);
			if (showParams::show_settings)			ShowSettings(window);
			if (showParams::show_save_game_dlg)	    ShowSaveMenu(window.getPosition());
			ShowMenu(window.getPosition());
		}
		else 
		{
			if      (msgBoxFlags::show_add_player_error)
			{
				WarningBox(res::sprite::warning_box,
					"Oooops...", u8"Игрок с таким ником уже существует!");
			}
			else if (msgBoxFlags::you_are_in_game)
			{
				WarningBox(res::sprite::warning_box, "Hey!", u8"Игра уже начата.");
			}
			else if (msgBoxFlags::you_are_not_in_game)
			{

				WarningBox(res::sprite::warning_box, "Hey!", u8"Нет начатой игры.");
			}
			else if (msgBoxFlags::you_should_add_players)
			{
				WarningBox(res::sprite::warning_box, "Sorry, but...",
					u8"Необходимо добавить хотя бы одного игрока!");
			}
			else if (msgBoxFlags::all_players_lost)
			{
				WarningBox(res::sprite::warning_box, u8"...",
					u8"Игра завершена. Все игроки проиграли.");
			}
			else if (msgBoxFlags::load_game_error)
			{
				WarningBox(res::sprite::warning_box, u8"Error",
					u8"Ошибка загрузки, сохранение было удалено.");
			}
			else if (msgBoxFlags::show_can_t_fint_word_msg)
			{
				WarningBox(res::sprite::warning_box, u8"...",
					(u8"Игра завершена. Слова на букву \"" + letter_utf8 + u8"\" закончились...").c_str());
			}
			else if (msgBoxFlags::show_bot_losed_msg)
			{
				WarningBox(res::sprite::warning_box, u8"...",
					u8"Игра завершена. Бот проиграл.");
			}
			else if (msgBoxFlags::show_miss_words_error)
			{
				WarningBox(res::sprite::warning_box, u8"Error",
					(std::string(u8"Слов в категории должно быть не меньше 100.\nСейчас слов в категории") + words_counter).c_str());
			}
			else if (msgBoxFlags::show_add_category_error)
			{
				WarningBox(res::sprite::warning_box, "Oooops...",
					u8"Категория с таким названием уже есть!");
			}
			else if (msgBoxFlags::show_close_window_question)
			{
				QuestionBox(res::sprite::question_box, "You sure?", u8"Выйти из игры?", window_closed);
			}
			else if (msgBoxFlags::show_end_game_dialog) 
			{
				QuestionBox(res::sprite::question_box, "Hey!",
					u8"Вы действительно хотите завершить игру?", game.game_ended);
				if (game.game_ended)
				{
					start_loading_window = true;

					auto f = std::async(std::launch::async, LoadingWindow,
										window.getPosition(),
										windowSize::current,
										std::ref(start_loading_window));

					showParams::add_player_enabled = true;
					showParams::show_player_list_enabled = true;
					showParams::close_program_button_enabled = true;
					showParams::category_editor_enabled = true;

					game.EndGame();
					

					start_loading_window = false;
					msgBoxFlags::show_end_game_dialog = false;
				}
			}
			else if (msgBoxFlags::really_delete) {
				bool del = false;
				QuestionBox(res::sprite::question_box, "?", u8"Удалить?", del);

				if (del) 
				{
					DeleteBackground(choosed_background,
                                     res::texture::all_backgrounds,
                                     res::sprite::all_backgrounds,
                                     pathes::image_path,
                                     pathes::addicted_image_pathes, true);
					if (!res::sprite::all_backgrounds.empty() && choosed_background == current_background)
					{
						if (current_background >= res::sprite::all_backgrounds.size())
							current_background = res::sprite::all_backgrounds.size() - 1;


						res::sprite::static_background = SetBackgrounSprite(res::sprite::all_backgrounds[current_background],
							windowSize::const_current);
					}
					else if (!res::sprite::all_backgrounds.empty() && choosed_background < current_background)
					{
						current_background = (current_background == 0) ? 0 : current_background - 1;
						res::sprite::static_background = SetBackgrounSprite(res::sprite::all_backgrounds[current_background],
							windowSize::const_current);


					}
				}
			}
			else if (msgBoxFlags::really_exclude) 
			{
				bool exclude = false;
				QuestionBox(res::sprite::question_box, "?", u8"Исключить?", exclude);

				if (exclude) 
				{
					DeleteBackground(choosed_background,
						res::texture::all_backgrounds,
						res::sprite::all_backgrounds,
						pathes::image_path,
						pathes::addicted_image_pathes);
					if (!res::sprite::all_backgrounds.empty() && choosed_background == current_background) 
					{
						if (current_background >= res::sprite::all_backgrounds.size()) 
							current_background = res::sprite::all_backgrounds.size() - 1;
						
							
						res::sprite::static_background = SetBackgrounSprite(res::sprite::all_backgrounds[current_background],
							windowSize::const_current);
					}
					else if (!res::sprite::all_backgrounds.empty() && choosed_background < current_background)
					{
						current_background = (current_background == 0) ? 0 : current_background - 1;
						res::sprite::static_background = SetBackgrounSprite(res::sprite::all_backgrounds[current_background],
							windowSize::const_current);
					}
				}
			}
			else if (msgBoxFlags::delete_word)
			{
				QuestionBox(res::sprite::question_box, "You sure?", u8"Удалить слова?",
					categEditor::word_deleted);

				if (categEditor::word_deleted)
				{
					showParams::show_only_clear = true;
					showParams::category_enable_save = true;
					categEditor::changeable_category.setFullSaveFlag(true);
					start_loading_window = true;
					categEditor::changeable_category.delWords();

					auto f = std::async(std::launch::async, LoadingWindow,
										window.getPosition(),
										windowSize::current,
										std::ref(start_loading_window));


					std::string input = categEditor::changeable_category.createTextOnlyExist();

					ZeroMemory(wordsBuf::input_words_buffer, wordsBuf::input_words_buffer_size);
					if (wordsBuf::input_words_buffer_size > input.size())
						std::memcpy(wordsBuf::input_words_buffer, input.c_str(),
							input.size());
					else
						std::memcpy(wordsBuf::input_words_buffer, input.c_str(),
							wordsBuf::input_words_buffer_size - 1);

					start_loading_window = false;
					categEditor::word_deleted = false;
				}

			}
			else if (msgBoxFlags::add_word)
			{
			
				QuestionBox(res::sprite::question_box, "You sure?", u8"Добавить слова?",
					categEditor::word_added);

				if (categEditor::word_added)
				{
					showParams::show_only_clear = true;
					showParams::category_enable_save = true;
					start_loading_window = true;


					auto f = std::async(std::launch::async, LoadingWindow,
						window.getPosition(),
						windowSize::current,
						std::ref(start_loading_window));

					std::string input = categEditor::changeable_category.createTextOnlyAdded();
					categEditor::changeable_category.addWords();

					ZeroMemory(wordsBuf::input_words_buffer, wordsBuf::input_words_buffer_size);
					if (wordsBuf::input_words_buffer_size > input.size())
						std::memcpy(wordsBuf::input_words_buffer, input.c_str(),
							input.size());
					else
						std::memcpy(wordsBuf::input_words_buffer, input.c_str(),
							wordsBuf::input_words_buffer_size - 1);


					start_loading_window = false;

					categEditor::word_added = false;
				}
			}
			else if (msgBoxFlags::exit_categoty_redo)
			{
				QuestionBox(res::sprite::question_box, u8"Выйти из режима редактирования?",
					u8"Если вы выйдите, изменения не будут сохранены.", categEditor::exit_redo);

				if (categEditor::exit_redo)
				{
					showParams::show_category_redo = false;
					showParams::show_category_choose = true;
					showParams::close_program_button_enabled = true;
					showParams::category_editor_enabled = true;
					showParams::category_enable_adding = false;
					showParams::category_enable_deleting = false;
					showParams::category_enable_save = false;
					showParams::category_enable_search = true;

					categEditor::changeable_category.setFullSaveFlag(false);
					categEditor::changeable_category.unloadData();


				}

			}
			else if (msgBoxFlags::show_del_category_q)
			{
				QuestionBox(res::sprite::question_box, "You sure?", 
					u8"Удалить выбранную категорию?",
					showParams::del_category);

				if (showParams::del_category)
				{
					remove(game.categories_paths[categEditor::choosed_category].c_str());
					game.categories_paths.erase(game.categories_paths.begin() + categEditor::choosed_category);
					game.categories_names.erase(game.categories_names.begin() + categEditor::choosed_category);

					showParams::del_category = false;
				}

			}
			else if (msgBoxFlags::really_load_save)
			{
				QuestionBox(res::sprite::question_box, "You sure?",
					u8"Загрузить выбранное сохраненние?", load_save);
				if (load_save)
				{
					start_loading_window = true;

					auto f = std::async(std::launch::async, LoadingWindow,
						window.getPosition(),
						windowSize::current,
						std::ref(start_loading_window));

					

					if (!game.LoadGame(static_cast<int>(idx_of_save)))
					{
						msgBoxFlags::load_game_error = true;
						msgBoxFlags::msg_box_opend = true;
						WakeMsgBoxSoundThread();
					}
					else
					{
						showParams::add_player_enabled = false;
						showParams::show_player_list_enabled = false;
						showParams::close_program_button_enabled = false;
						showParams::category_editor_enabled = false;
						showParams::show_add_player_menu = false;
						showParams::show_category_editor = false;
						showParams::show_category_choose = false;
						showParams::show_users_list = false;
						showParams::show_start_game_dlg = false;
						showParams::show_load_game_dlg = false;

					}

					load_save = false;
					start_loading_window = false;
				}
			}
			else if (msgBoxFlags::really_delete_save)
			{
				QuestionBox(res::sprite::question_box, "You sure?",
					u8"Удалить выбранное сохранение?", del_save);

				if (del_save)
				{
					remove(game.saves_paths[idx_of_save].c_str());
					game.saves_names.erase(game.saves_names.begin() + idx_of_save);
					game.saves_paths.erase(game.saves_paths.begin() + idx_of_save);
					del_save = false;
				}
			}
			else if (showParams::show_get_name_dialog)
			{
				ShowCategorySettings(window.getPosition());
				ShowRenameDialog(window.getPosition());
			}
		}
		

		
		//====================================================================
		//keyboard events...
		//====================================================================
		
		
		//if all windows closed and pressed esc -> quitting the game 
		if (ImGui::IsKeyPressed(ImGui::GetKeyIndex(ImGuiKey_::ImGuiKey_Escape))
			&& game.game_ended
			&& !showParams::show_add_player_menu
			&& !showParams::show_settings
			&& !showParams::show_users_list
			&& !showParams::show_category_redo
			&& !showParams::show_category_editor) 
		{
			msgBoxFlags::msg_box_opend = true;
			msgBoxFlags::show_close_window_question = true;
			WakeMsgBoxSoundThread();
		}
		
		if (ImGui::IsKeyPressed(ImGui::GetKeyIndex(ImGuiKey_::ImGuiKey_Escape))
			&& !game.game_ended)
		{
			msgBoxFlags::msg_box_opend = true;
			msgBoxFlags::show_end_game_dialog = true;
			WakeMsgBoxSoundThread();
		}

		if (ImGui::IsKeyPressed(ImGui::GetKeyIndex(ImGuiKey_::ImGuiKey_Escape))
			&& showParams::show_category_redo)
		{
			msgBoxFlags::msg_box_opend = true;
			msgBoxFlags::exit_categoty_redo = true;
			WakeMsgBoxSoundThread();


		}

		//if escape pressed -> close all windows
		if (ImGui::IsKeyPressed(ImGui::GetKeyIndex(ImGuiKey_::ImGuiKey_Escape)))
		{
			showParams::show_add_player_menu = false;
			showParams::show_settings = false;
			showParams::show_users_list = false;
			showParams::show_music_manager = false;
			if (!showParams::show_category_redo)
			{
				showParams::show_category_editor = false;
				showParams::show_category_choose = false;
			}
		}
		if (ImGui::GetIO().KeyAlt)
		{
			
			if (ImGui::IsKeyPressed(ImGui::GetKeyIndex(ImGuiKey_::ImGuiKey_A)))
			{

				showParams::show_add_player_menu = true;
			}
			if (ImGui::IsKeyPressed(ImGui::GetKeyIndex(ImGuiKey_::ImGuiKey_Z)))
			{

				showParams::show_settings = true;
			}
			if (ImGui::IsKeyPressed(ImGui::GetKeyIndex(ImGuiKey_::ImGuiKey_V)))
			{
				showParams::show_music_manager = true;
			}
		}
		
		



		//====================================================================
		//SFML render block...
		//====================================================================
		window.clear();

		if (static_background) window.draw(res::sprite::static_background);
		else 
		{
			window.draw(res::sprite::background1);
			window.draw(res::sprite::background2);
		}
		
		ImGui::SFML::Render(window);
		window.display();


		//====================================================================
		//processing window_closed event...
		//====================================================================
		if (window_closed) 
		{
			window.close();
			showParams::show_music_manager = false;

			
			bool music_flag = sounds::main_theme_track.isPlaying();
			SaveSystemParams(sounds::volume_of_sound,
                             sounds::volume_of_music,
                             windowSize::current,
                             sounds::sound_off,
				             music_flag,
                             fullscreen,
                             static_background,
                             current_background);

			UnLoadAddictedImagePaths(pathes::addicted_image_pathes);
			
			sounds::main_theme_track.stop();
			sounds::show_msg_box.sound.setVolume(0.f);
			sounds::button_click.sound.setVolume(0.f);

			sounds::sound_off = true;
			WakeDinamicColorThread();
			WakeMsgBoxSoundThread();
			WakeAnimationThread();

			ImGui::SFML::Shutdown();
			


			auto animation_status =     threadEndFlags::animation_f.wait_for(std::chrono::milliseconds(0));
			auto dinamic_color_status = threadEndFlags::dinamic_color_f.wait_for(std::chrono::milliseconds(0));
			auto sound_play_status =    threadEndFlags::sound_play_f.wait_for(std::chrono::milliseconds(0));


			while (animation_status !=     std::future_status::ready
				|| dinamic_color_status != std::future_status::ready
				|| sound_play_status !=    std::future_status::ready) 
			{

				animation_status =     threadEndFlags::animation_f.wait_for(std::chrono::milliseconds(0));
				dinamic_color_status = threadEndFlags::dinamic_color_f.wait_for(std::chrono::milliseconds(0));
				sound_play_status =    threadEndFlags::sound_play_f.wait_for(std::chrono::milliseconds(0));
			}
		}
	}
	delete[] bmpScreen::file;
	
	return 0;
}

void DoScreenshot(HWND handle) {
	using namespace bmpScreen;

	RECT client_rect = { 0 };
	GetClientRect(handle, &client_rect);
	int width = client_rect.right - client_rect.left;
	int height = client_rect.bottom - client_rect.top; HDC hdcScreen = GetDC(handle);
	HDC hdc = CreateCompatibleDC(hdcScreen);
	HBITMAP hbmp = CreateCompatibleBitmap(hdcScreen, width, height);
	SelectObject(hdc, hbmp);

	BitBlt(hdc, 0, 0, width, height, hdcScreen, 0, 0, SRCCOPY);

	bmp_info = { 0 };
	bmp_info.bmiHeader.biSize = sizeof(bmp_info.bmiHeader);
	bmp_info.bmiHeader.biWidth = width;
	bmp_info.bmiHeader.biHeight = height;
	bmp_info.bmiHeader.biPlanes = 1;
	bmp_info.bmiHeader.biBitCount = 24;
	bmp_info.bmiHeader.biCompression = BI_RGB;

	int bmp_padding = (width * 3) % 4;
	if (bmp_padding != 0) bmp_padding = 4 - bmp_padding;

	size_of_bmp = (width * 3 + bmp_padding) * height;
	bmp_pixels = new BYTE[size_of_bmp];
	GetDIBits(hdc, hbmp, 0, height, bmp_pixels, &bmp_info, DIB_RGB_COLORS);

	// Add the size of the headers to the size of the bitmap to get the total file size
	dwSizeofDIB = (width * 3 + bmp_padding) * height + sizeof(BITMAPFILEHEADER) + sizeof(BITMAPINFOHEADER);

	//Offset to where the actual bitmap bits start.
	bmfHeader.bfOffBits = (DWORD)sizeof(BITMAPFILEHEADER) + (DWORD)sizeof(BITMAPINFOHEADER);

	//Size of the file
	bmfHeader.bfSize = dwSizeofDIB;

	//bfType must always be BM for Bitmaps
	bmfHeader.bfType = 0x4D42; //BM

	//file = new BYTE [dwSizeofDIB];
	if (file != nullptr) delete[] file;
	file = new BYTE[dwSizeofDIB];


	h = &(file[0]);
	i = &(file[sizeof(BITMAPFILEHEADER)]);
	p = &(file[sizeof(BITMAPINFOHEADER) + sizeof(BITMAPFILEHEADER)]);
	std::memcpy(h, &bmfHeader, sizeof(BITMAPFILEHEADER));
	std::memcpy(i, &bmp_info.bmiHeader, sizeof(BITMAPINFOHEADER));
	std::memcpy(p, bmp_pixels, size_of_bmp);


	DeleteDC(hdc);
	DeleteObject(hbmp);
	ReleaseDC(NULL, hdcScreen);
	delete[] bmp_pixels;
}

//threads
void AnimationThread()
{
	size_t picture_idx = 2;
	sf::Color mask(255, 255, 255, 255);
	
	
	using namespace std::chrono_literals;
	std::this_thread::sleep_for(2s);


	while (!window_closed) 
	{
		
		//delay
		std::this_thread::sleep_for(2s);
		if (window_closed) break;
		std::this_thread::sleep_for(2s);
		if (window_closed) break;
		std::this_thread::sleep_for(2s);
		if (window_closed) break;
		std::this_thread::sleep_for(2s);
		if (window_closed) break;

		if (!static_background) 
		{
			if (res::sprite::all_backgrounds.size() >= 2)
			{
				do 
				{

					mask.a--;
					std::this_thread::sleep_for(10ms);

					res::sprite::background2.setColor(mask);

				} while (mask.a > 0);
				if (window_closed) break;

				//delay
				std::this_thread::sleep_for(2s);
				if (window_closed ) break;
				std::this_thread::sleep_for(2s);
				if (window_closed) break;
				std::this_thread::sleep_for(2s);
				if (window_closed) break;
				std::this_thread::sleep_for(2s);
				if (window_closed) break;
				
			
				if (picture_idx >= res::sprite::all_backgrounds.size()) picture_idx = 0;
				res::sprite::background2 = SetBackgrounSprite(res::sprite::all_backgrounds[picture_idx],
					windowSize::const_current);
				res::sprite::background2.setColor(mask);
				picture_idx++;
			


				if (window_closed) break;
				do 
				{
					mask.a++;
					std::this_thread::sleep_for(10ms);

					res::sprite::background2.setColor(mask);

				} while (mask.a < 255);
				if (window_closed) break;

			
				if (picture_idx >= res::sprite::all_backgrounds.size()) picture_idx = 0;
				res::sprite::background1 = SetBackgrounSprite(res::sprite::all_backgrounds[picture_idx],
					windowSize::const_current);
				picture_idx++;
			}
		}
		else 
		{
			
			std::unique_lock <std::mutex> locker(threadLockers::lock_background_change);
			
			while (!threadLockers::backround_change_thread_notified) 
			{
				threadLockers::background_change.wait(locker);
			}
			
			threadLockers::backround_change_thread_notified = false;
		}
	}
	threadEndFlags::animation_p.set_value(true);
}

#define TIME_DELAY 30ms
void ThreadDinamicColor() {
	using namespace std::chrono_literals;
	while (!window_closed) 
	{
		{
			std::unique_lock <std::mutex> locker(threadLockers::lock_color_change);

			while (!threadLockers::color_thread_notified)
			{
				threadLockers::color_change.wait(locker);
			}
		}
		
		threadLockers::color_thread_start_work = true;
		while (showParams::show_music_manager) 
		{

			if (window_closed && !showParams::show_music_manager) break;

			while (sounds::player_color.y < 1.0f) 
			{
				sounds::player_color.y += 0.01f;
				std::this_thread::sleep_for(TIME_DELAY);
				if (!showParams::show_music_manager) break;
			}
			sounds::player_color.y = 1.0f;

			if (window_closed && !showParams::show_music_manager) break;

			while (sounds::player_color.x > 0.0f) 
			{
				sounds::player_color.x -= 0.01f;
				std::this_thread::sleep_for(TIME_DELAY);
				if (!showParams::show_music_manager) break;
			}
			sounds::player_color.x = 0.0f;

			
			
			
			
			
			if (window_closed && !showParams::show_music_manager) break;
			
			while (sounds::player_color.z < 1.0f) 
			{
				sounds::player_color.z += 0.01f;
				std::this_thread::sleep_for(TIME_DELAY);
				if (!showParams::show_music_manager) break;
			}
			sounds::player_color.z = 1.0f;
			
			if (window_closed && !showParams::show_music_manager) break;

			while (sounds::player_color.y > 0.0f) 
			{
				sounds::player_color.y -= 0.01f;
				std::this_thread::sleep_for(TIME_DELAY);
				if (!showParams::show_music_manager) break;
			}
			sounds::player_color.y = 0.0f;
			
			
			

			if (window_closed && !showParams::show_music_manager) break;
			
			while (sounds::player_color.x < 1.0f) 
			{
				sounds::player_color.x += 0.01f;
				std::this_thread::sleep_for(TIME_DELAY);
				if (!showParams::show_music_manager) break;
			}
			sounds::player_color.x = 1.0f;

			if (window_closed && !showParams::show_music_manager) break;

			while (sounds::player_color.z > 0.0f) 
			{
				sounds::player_color.z -= 0.01f;
				std::this_thread::sleep_for(TIME_DELAY);
				if (!showParams::show_music_manager) break;
			}
			sounds::player_color.z = 0.0f;

		}
		
		threadLockers::color_thread_notified = false;
		threadLockers::color_thread_start_work = false;

	}
	threadEndFlags::dinamic_color_p.set_value(true);
}
void msgBoxSoundPlayThread() 
{
	using namespace std::chrono_literals;
	while (!window_closed) 
	{
		{
			std::unique_lock <std::mutex> locker(threadLockers::lock_play_sound);

			while (!threadLockers::sound_thread_notified)
			{
				threadLockers::play_sound.wait(locker);
			}
		}
		if (!sounds::sound_off) 
		{
			float old_volume = sounds::main_theme_track.getVolume();
			sounds::main_theme_track.setVolume(0.2f * old_volume);
			sounds::show_msg_box.sound.play();
			std::this_thread::sleep_for(1100ms);
			sounds::main_theme_track.setVolume(old_volume);
		}
		threadLockers::sound_thread_notified = false;
	}
	threadEndFlags::sound_play_p.set_value(true);
}

//threads defrost
void WakeDinamicColorThread() 
{
	std::unique_lock <std::mutex> locker(threadLockers::lock_color_change);
	threadLockers::color_thread_notified = true;
	threadLockers::color_change.notify_one();
}
void WakeMsgBoxSoundThread() 
{
	std::unique_lock <std::mutex> locker(threadLockers::lock_play_sound);
	threadLockers::sound_thread_notified = true;
	threadLockers::play_sound.notify_one();
}
void WakeAnimationThread() 
{
	std::unique_lock <std::mutex> locker(threadLockers::lock_background_change);
	threadLockers::backround_change_thread_notified = true;
	threadLockers::background_change.notify_one();
}


void ShowMenu(sf::Vector2i wnd_pos)
{
	if (ImGui::BeginMainMenuBar()) 
	{

		if (ImGui::BeginMenu(u8"Игровое Меню")) 
		{
			if (ImGui::MenuItem(u8"Редактор категорий", nullptr, nullptr,
				showParams::category_editor_enabled))
			{
				showParams::show_category_editor = true;
				showParams::show_category_choose = true;
				sounds::button_click.sound.play();
			
			}
			if (ImGui::MenuItem(u8"Добавить игрока", "Alt+A",
				nullptr, showParams::add_player_enabled)) 
			{
				showParams::show_add_player_menu = true;
				sounds::button_click.sound.play();

			}
			if (ImGui::MenuItem(u8"Показать список игроков",
				(const char*)0, nullptr, showParams::show_player_list_enabled)) 
			{
				showParams::show_users_list = true;
				sounds::button_click.sound.play();

			}

			if (ImGui::MenuItem(u8"Громкость", "Alt+V")) 
			{
				WakeDinamicColorThread();
				showParams::show_music_manager = true;
				sounds::button_click.sound.play();

			}


			if (ImGui::MenuItem(u8"Настройки", "Alt+Z", nullptr)) 
			{
				showParams::show_settings = true;
				sounds::button_click.sound.play();

			}

			if (ImGui::MenuItem(u8"Выйти из игры", nullptr, nullptr,
				showParams::close_program_button_enabled)) 
			{

				WakeMsgBoxSoundThread();
				msgBoxFlags::msg_box_opend = true;
				msgBoxFlags::show_close_window_question = true;
				sounds::button_click.sound.play();
			}




			ImGui::EndMenu();
		}
		if (!showParams::show_category_redo)
		{
			if (game.game_ended)
			{
				if (ImGui::BeginMenu(u8"Начать игру"))
				{
					if (ImGui::MenuItem(u8"Новая игра"))
					{
						sounds::button_click.sound.play();
						if (game.game_ended)
						{
							if (game.all_players.size() > 0)
							{
								showParams::show_start_game_dlg = true;

							}
							else
							{
								WakeMsgBoxSoundThread();
								msgBoxFlags::msg_box_opend = true;
								msgBoxFlags::you_should_add_players = true;
							}
						}
						else
						{
							WakeMsgBoxSoundThread();
							msgBoxFlags::msg_box_opend = true;
							msgBoxFlags::you_are_in_game = true;

						}
					}
					if (ImGui::MenuItem(u8"Сохранения"))
					{
						sounds::button_click.sound.play();
						showParams::show_load_game_dlg = true;
					}
					ImGui::EndMenu();
				}
			}
			else
			{
				if (ImGui::BeginMenu(u8"Завершить игру"))
				{
					if (ImGui::MenuItem(u8"Выйти без сохранения"))
					{
						sounds::button_click.sound.play();
						if (game.game_ended)
						{
							WakeMsgBoxSoundThread();
							msgBoxFlags::msg_box_opend = true;
							msgBoxFlags::you_are_not_in_game = true;
						}
						else
						{
							WakeMsgBoxSoundThread();
							msgBoxFlags::msg_box_opend = true;
							msgBoxFlags::show_end_game_dialog = true;
						}
					}
					if (ImGui::MenuItem(u8"Сохранить и выйти"))
					{
						sounds::button_click.sound.play();
						showParams::show_save_game_dlg = true;

					}
					ImGui::EndMenu();
				}
			}
		}
		ImGui::EndMainMenuBar();
	}
}

void ShowSettings(sf::RenderWindow &window)
{
	
	ImGui::SetNextWindowPosCenter(ImGuiCond_::ImGuiSetCond_FirstUseEver);

	ImGui::PushFont(ImGui::GetIO().Fonts->Fonts[1]);
	ImGui::Begin(u8"Настройки", nullptr, { 700, 580 }, 1.0f,
		ImGuiWindowFlags_::ImGuiWindowFlags_HorizontalScrollbar |
		ImGuiWindowFlags_::ImGuiWindowFlags_NoSavedSettings |
		ImGuiWindowFlags_::ImGuiWindowFlags_NoCollapse |
		ImGuiWindowFlags_::ImGuiWindowFlags_NoResize);
	
	ImGui::PopFont();
	ImGui::PushFont(ImGui::GetIO().Fonts->Fonts[0]);
	
	ImGui::BeginChild("settings##main", {0, 480}, true);
	if (ImGui::CollapsingHeader(u8"Размер окна")) 
	{
		if (frame_count == 0) 
		{
			DoScreenshot(window.getSystemHandle());
			res::texture::window_example.loadFromMemory(bmpScreen::file, bmpScreen::dwSizeofDIB);
		}

		if (++frame_count > 50) frame_count = 0;
		

		sf::Sprite example_sp_current;
		sf::Sprite examplt_sp_new;

		
		example_sp_current.setTexture(res::texture::window_example);
		examplt_sp_new.setTexture(res::texture::window_example);
		sf::Vector2u tex_size = res::texture::window_example.getSize();

		ImVec2 wnd_size = ImGui::GetWindowSize();


		float k_current;
		if (tex_size.x > tex_size.y) k_current = wnd_size.x / 3.5f / static_cast<float>(tex_size.x);
		else k_current = 140.f / static_cast<float>(tex_size.y);

		example_sp_current.scale(k_current, k_current);
		examplt_sp_new.scale(k_current, k_current);

		float ky_new, kx_new;
		

		kx_new = static_cast<float>(windowSize::New.x) / static_cast<float>(tex_size.x);
		ky_new = static_cast<float>(windowSize::New.y) / static_cast<float>(tex_size.y);

		examplt_sp_new.scale(kx_new, ky_new);

		if (fullscreen) 
		{
			float k = 0.4f;
			ImVec4 c = ImGui::GetStyleColorVec4(ImGuiCol_::ImGuiCol_SliderGrab);
			ImGui::PushStyleColor(ImGuiCol_::ImGuiCol_SliderGrab, { c.x * k, c.y * k, c.z * k, c.w });
			ImGui::PushStyleColor(ImGuiCol_::ImGuiCol_SliderGrabActive, { c.x * k, c.y * k, c.z * k, c.w });
			ImGui::PushStyleColor(ImGuiCol_::ImGuiCol_TabHovered, { c.x * k, c.y * k, c.z * k, c.w });
			c = ImGui::GetStyleColorVec4(ImGuiCol_::ImGuiCol_Button);
			ImGui::PushStyleColor(ImGuiCol_::ImGuiCol_Button, { c.x * k, c.y * k, c.z * k, c.w });
			ImGui::PushStyleColor(ImGuiCol_::ImGuiCol_ButtonActive, { c.x * k, c.y * k, c.z * k, c.w });
			ImGui::PushStyleColor(ImGuiCol_::ImGuiCol_ButtonHovered, { c.x * k, c.y * k, c.z * k, c.w });

		}




		ImGui::Columns(3, nullptr, false);
		ImGui::SetColumnWidth(0, wnd_size.x * 0.4f);
		ImGui::SetColumnWidth(1, wnd_size.x * 0.2f);
		ImGui::SetColumnWidth(2, wnd_size.x * 0.2f);
		ImGui::PushButtonRepeat(true);

		ImGui::NewLine();
		ImGui::SliderInt("X", &windowSize::New.x, (int)windowSize::min.x, (int)windowSize::max.x);
		ImGui::SliderInt("Y", &windowSize::New.y, (int)windowSize::min.y, (int)windowSize::max.y);
		


		ImGui::NextColumn();



		ImGui::Text("1x");
		if (ImGui::ArrowButton("x--", ImGuiDir_::ImGuiDir_Left)) 
		{
			windowSize::New.x--;
			if (windowSize::New.x < (int)windowSize::min.x) 
				windowSize::New.x = (int)windowSize::min.x;
			sounds::button_click.sound.play();
		}
		ImGui::SameLine();
		if (ImGui::ArrowButton("x++", ImGuiDir_::ImGuiDir_Right)) 
		{
			windowSize::New.x++;
			if (windowSize::New.x > (int)windowSize::max.x) 
				windowSize::New.x = (int)windowSize::max.x;
			sounds::button_click.sound.play();
		}




		if (ImGui::ArrowButton("y--", ImGuiDir_::ImGuiDir_Left)) 
		{
			windowSize::New.y--;
			if (windowSize::New.y < (int)windowSize::min.y) 
				windowSize::New.y = (int)windowSize::min.y;
			sounds::button_click.sound.play();
		}
		ImGui::SameLine();
		if (ImGui::ArrowButton("y++", ImGuiDir_::ImGuiDir_Right)) 
		{
			windowSize::New.y++;
			if (windowSize::New.y > (int)windowSize::max.y) 
				windowSize::New.y = (int)windowSize::max.y;
			sounds::button_click.sound.play();
		}



		ImGui::NextColumn();



		ImGui::Text("10x");
		if (ImGui::ArrowButton("x--10", ImGuiDir_::ImGuiDir_Left)) 
		{
			windowSize::New.x-=10;
			if (windowSize::New.x < (int)windowSize::min.x)
				windowSize::New.x = (int)windowSize::min.x;
			sounds::button_click.sound.play();
		}
		ImGui::SameLine();
		if (ImGui::ArrowButton("x++10", ImGuiDir_::ImGuiDir_Right))
		{
			windowSize::New.x+=10;
			if (windowSize::New.x > (int)windowSize::max.x)
				windowSize::New.x = (int)windowSize::max.x;
			sounds::button_click.sound.play();
		}
		
		
		
		
		
		if (ImGui::ArrowButton("y--10", ImGuiDir_::ImGuiDir_Left)) 
		{
			windowSize::New.y-=10;
			if (windowSize::New.y < (int)windowSize::min.y)
				windowSize::New.y = (int)windowSize::min.y;
			sounds::button_click.sound.play();
		}
		ImGui::SameLine();
		if (ImGui::ArrowButton("y++10", ImGuiDir_::ImGuiDir_Right))
		{
			windowSize::New.y+=10;
			if (windowSize::New.y > (int)windowSize::max.y)
				windowSize::New.y = (int)windowSize::max.y;
			sounds::button_click.sound.play();
		}
		
		
		
		
		
		if (fullscreen) 
		{
			ImGui::PopStyleColor(6);
			windowSize::New = { static_cast<int>(windowSize::const_current.x),
								static_cast<int>(windowSize::const_current.y) };
		}

		ImGui::NextColumn();
		if(ImGui::Checkbox("Fullscreen", &fullscreen)) sounds::button_click.sound.play();
		ImGui::Separator();
		ImGui::PopButtonRepeat();
		ImGui::Columns(1);
		ImGui::NewLine();


		ImGui::Columns(2, nullptr);
		ImGui::SetColumnWidth(0, wnd_size.x * 0.5f);
		ImGui::SetColumnWidth(1, wnd_size.x * 0.5f);


		ImGui::Text(u8"Новый размер");
		ImGui::Image(examplt_sp_new);
		ImGui::NextColumn();
		ImGui::Text(u8"Текущий размер");
		ImGui::Image(example_sp_current);
		ImGui::Columns(1);


		ImGui::TextColored({ 1.f, 0.f, 0.f, 1.f },
			u8"Окно изменит размер после перезапуска игры");
		ImGui::Separator();
		ImGui::NewLine();




		
	}
	if (ImGui::CollapsingHeader(u8"Фон")) 
	{
		
		if (ImGui::Checkbox(u8"Отключить автоматическую смену фона", &static_background)) 
		{
			if (!static_background)
			{
				WakeAnimationThread();
				if (res::sprite::all_backgrounds.size() > 0)
				{
					if (res::sprite::all_backgrounds.size() == 1)
					{
						static_background = true;
						res::sprite::static_background = SetBackgrounSprite(res::sprite::all_backgrounds[0],
							windowSize::const_current);
					}
					else
					{
						res::sprite::background1 = SetBackgrounSprite(res::sprite::all_backgrounds[0],
							windowSize::const_current);
						res::sprite::background2 = SetBackgrounSprite(res::sprite::all_backgrounds[1],
							windowSize::const_current);

					}
				}

			}
			sounds::button_click.sound.play();
		}




		if (static_background) 
		{
			
			
			if (ImGui::Button(u8"Загрузить изображение")) 
			{
				sounds::button_click.sound.play();
				auto file = pfd::open_file(u8"Открыть изображение",
					std::filesystem::current_path().string(),
					{ "Image files (.bmp .png .tga .jpg .psd .hdr .pic)" 
					,"*.bmp *.png *.tga *.jpg *.psd *.hdr *.pic" });
				
				
				if (!file.result().empty())
				{
					pathes::addicted_image_pathes.push_back(file.result()[0]);

					AddBackground(file.result()[0],
						res::texture::all_backgrounds,
						res::sprite::all_backgrounds);
					if (res::sprite::all_backgrounds.size() == 1) {
						res::sprite::static_background = SetBackgrounSprite(res::sprite::all_backgrounds[0],
							windowSize::const_current);
						current_background = 0;
					}
				}
			}





			sf::Vector2f img_size = { 240.f, 120.f };
			ImGui::BeginChild("##columns", ImVec2(0, img_size.y * 3 + ImGui::GetStyle().ItemSpacing.y * 4), false,
				ImGuiWindowFlags_::ImGuiWindowFlags_HorizontalScrollbar);
			{
				ImGui::Columns(4, "##images", false);
				ImGui::SetColumnWidth(0, img_size.x);
				ImGui::SetColumnWidth(1, img_size.x / 2);
				ImGui::SetColumnWidth(2, img_size.x / 2);
				ImGui::SetColumnWidth(3, img_size.x / 2);

				size_t counter = res::sprite::all_backgrounds.size();
				for (size_t i = 0; i < counter; i++) 
				{
					ImGui::PushID(i);
					ImGui::Image(res::sprite::all_backgrounds[i], img_size);
					ImGui::NextColumn();




					if (ImGui::Button(u8"Выбрать##images", ImVec2(img_size.x / 2, img_size.y))) 
					{
						res::sprite::static_background = SetBackgrounSprite(res::sprite::all_backgrounds[i],
							windowSize::const_current);
						current_background = i;
						sounds::button_click.sound.play();
					}
					ImGui::NextColumn();





					if (ImGui::Button(u8"Исключить##images", ImVec2(img_size.x / 2, img_size.y))) 
						{
							msgBoxFlags::msg_box_opend = true;
							msgBoxFlags::really_exclude = true;
							WakeMsgBoxSoundThread();
							choosed_background = i;
							sounds::button_click.sound.play();
						}
					if (ImGui::IsItemHovered()) 
					{
						ImGui::BeginTooltip();
						ImGui::PushTextWrapPos(ImGui::GetFontSize() * 35.0f);
						ImGui::TextUnformatted(u8"Исключит добавленное изображение из списка\n(изображения в папке с игрой добавятся автоматически\nпри следующем запуске)");
						ImGui::PopTextWrapPos();
						ImGui::EndTooltip();
					}
					ImGui::NextColumn();





					if (ImGui::Button(u8"Удалить##images", ImVec2(img_size.x / 2, img_size.y))) {
							msgBoxFlags::msg_box_opend = true;
							msgBoxFlags::really_delete = true;
							
							WakeMsgBoxSoundThread();
							choosed_background = i;

							sounds::button_click.sound.play();
						}
					if (ImGui::IsItemHovered()) 
					{
						ImGui::BeginTooltip();
						ImGui::PushTextWrapPos(ImGui::GetFontSize() * 35.0f);
						ImGui::TextUnformatted(u8"Удалит изображение с диска");
						ImGui::PopTextWrapPos();
						ImGui::EndTooltip();
					}
					ImGui::NextColumn();



					ImGui::PopID();
				}
				ImGui::Columns(1);
			}
			ImGui::EndChild();
			


		}


	}
	ImGui::EndChild();


	
	ImGui::SetCursorPosX((ImGui::GetItemRectSize().x - 400));
	if (ImGui::Button(u8"Выйти", { 200, 35 }))
	{
		windowSize::New = { static_cast<int>(windowSize::current.x),
								static_cast<int>(windowSize::current.y) };
		showParams::show_settings = false;
		sounds::button_click.sound.play();
	}
	ImGui::SameLine();
	if (ImGui::Button(u8"Сохранить и выйти", {200, 35}))
	{
		showParams::show_settings = false;
		windowSize::current = { static_cast<unsigned>(windowSize::New.x),
								static_cast<unsigned>(windowSize::New.y) };
		sounds::button_click.sound.play();
	}

	ImGui::PopFont();
	ImGui::End();
}

void ShowUsersList()
{
	ImGui::SetNextWindowPosCenter(ImGuiCond_::ImGuiSetCond_FirstUseEver);
	ImGui::StyleColorsDark();
	ImGui::PushFont(ImGui::GetIO().Fonts->Fonts[1]);
	ImGui::Begin(u8"Список игроков", nullptr, { 400.f, 500.f }, 0.8f,
		ImGuiWindowFlags_::ImGuiWindowFlags_NoResize |
		ImGuiWindowFlags_::ImGuiWindowFlags_NoTitleBar |
		ImGuiWindowFlags_::ImGuiWindowFlags_HorizontalScrollbar |
		ImGuiWindowFlags_::ImGuiWindowFlags_NoSavedSettings);
	ImGui::Separator();
	ImGui::TextColored(ImVec4(0.f, 255.f, 43.f, 255.f), u8"Список игроков");
	ImGui::Separator();
	
	ImGui::PopFont();
	ImGui::PushFont(ImGui::GetIO().Fonts->Fonts[0]);
	ImGui::BeginChild(u8"##show_users", { 0, 400.f }, true,
		ImGuiWindowFlags_::ImGuiWindowFlags_HorizontalScrollbar);
	{
		
		for (size_t i = 0; i < game.all_players.size(); i++) 
		{
			ShowPlayerEntry(i);
		}
	}
	ImGui::EndChild();

	if (ImGui::Button(u8"Закрыть"))
	{
		showParams::show_users_list = false;
		sounds::button_click.sound.play();
	}

	ImGui::End();
	ImGui::PopFont();

}
void ShowMusicManager(Music &track)
{
	ImGui::SetNextWindowPosCenter(ImGuiCond_::ImGuiSetCond_FirstUseEver);
	using namespace sounds;
	ImGui::StyleColorsDark();
	ImGui::PushStyleColor(ImGuiCol_::ImGuiCol_WindowBg, player_color);
	ImGui::PushFont(ImGui::GetIO().Fonts->Fonts[1]);

	ImGui::Begin(u8"Громкость", nullptr, { 600.f, 290.f }, 0.8f,
		ImGuiWindowFlags_::ImGuiWindowFlags_NoSavedSettings |
		ImGuiWindowFlags_::ImGuiWindowFlags_NoResize);



	ImGui::PopFont();
	ImGui::PushFont(ImGui::GetIO().Fonts->Fonts[0]);
	ImGui::BeginChild("change_volume", { 0, 200 });
	{
		ImGui::Columns(3, nullptr, false);
		
		
		
		ImGui::Text(u8"Звук");
		ImGui::NextColumn();
		if (sound_off) 
		{
			if (ImGui::Button(u8"Включить", { 120, 26 })) 
			{
				button_click.sound.play();
				sound_off = false;
				button_click.sound.setVolume(volume_of_sound);
				show_msg_box.sound.setVolume(volume_of_sound);
			}
		}
		else 
		{
			if (ImGui::Button(u8"Выключить", { 120, 26 })) 
			{
				button_click.sound.play();
				sound_off = true;
				button_click.sound.setVolume(0.f);
				show_msg_box.sound.setVolume(0.f);
			}
		}
		ImGui::NextColumn();






		if (ImGui::SliderFloat("  ", &volume_of_sound,
			0.0f, 100.0f, "%.0f %%")) 
		{
			button_click.sound.setVolume(volume_of_sound);
			show_msg_box.sound.setVolume(volume_of_sound);
		} 
		ImGui::Separator();
		ImGui::NextColumn();





		ImGui::Text(u8"Музыка");
		ImGui::NextColumn();
		if (ImGui::ArrowButton("prev_track", ImGuiDir_::ImGuiDir_Left)) 
		{
			if (track.isPlaying())
			{
				button_click.sound.play();
				track.prevTrack();
			}
		}
		ImGui::SameLine();
		if (track.isPlaying() && sounds::volume_of_music > 0.0f) 
		{
			if (ImGui::Button("stop", { 60, 26 })) 
			{
				button_click.sound.play();
				track.pause();
			}
		}
		else 
		{
			if (ImGui::Button("play", { 60, 26 })) 
			{
				button_click.sound.play();

				if(sounds::volume_of_music > 0.0f) track.play();
			}
		}
		ImGui::SameLine();
		if (ImGui::ArrowButton("next_track", ImGuiDir_::ImGuiDir_Right)) 
		{
			if (track.isPlaying())
			{
				button_click.sound.play();
				track.nextTrack();
			}
		}
		





		ImGui::NextColumn();
		if (ImGui::SliderFloat(" ", &volume_of_music,
			0.0f, 100.0f, "%.0f %%")) 
		{
			if (volume_of_music == 0.0f && main_theme_track.isPlaying()) main_theme_track.pause();
			else if (volume_of_music != 0.0f && main_theme_track.isPlaying()) main_theme_track.play();
			track.setVolume(volume_of_music);
		}
		int current_idx = static_cast<int>(track.getCurrentTrkIdx());
		ImGui::Columns(1);
		ImGui::Separator();
		if (ImGui::TreeNode("PlayList")) 
		{
			ImGui::PopFont();
			ImGui::PushItemWidth(560.f);
			ImGui::PushFont(ImGui::GetIO().Fonts->Fonts[3]);
			if (ListBox("##playlist", &current_idx, playlist, 4)) 
			{
				track.setCurrentTrkIdx(static_cast<size_t>(current_idx));
				track.setTrack(track.getCurrentTrkIdx());
			}
			ImGui::PopItemWidth();
			ImGui::TreePop();
		}
		



	}
	ImGui::EndChild();
	ImGui::PopStyleColor();
	ImGui::PopFont();
	ImGui::PushFont(ImGui::GetIO().Fonts->Fonts[0]);
	ImGui::SetCursorPosX(ImGui::GetItemRectSize().x - 80);
	if (ImGui::Button(u8"Закрыть", { 80, 35 }))
	{
		button_click.sound.play();
		showParams::show_music_manager = false;
	}

	ImGui::PopFont();
	ImGui::End();
}
void ShowPlayerEntry(size_t id) {
	

	ImGui::PushID(id);
	if (ImGui::Button(u8"Удалить")) 
	{
		game.all_players.erase(game.all_players.begin() + id);
		game.player_enabled.erase(game.player_enabled.begin() + id);
		sounds::button_click.sound.play();
	}
	ImGui::SameLine();
	if (id < game.all_players.size())
	ImGui::Text(game.all_players[id].getName().c_str());
	
	ImGui::Separator();
	ImGui::PopID();


}


static sf::Clock clock_add_player;
static bool added = false;
static bool success = true;
void ShowAddPlayerMenu() {

	ImGui::SetNextWindowPosCenter(ImGuiCond_::ImGuiSetCond_FirstUseEver);
	ImGui::StyleColorsDark();
	ImGui::PushFont(ImGui::GetIO().Fonts->Fonts[1]);
	ImGui::Begin(u8"Добавить игрока", nullptr, { 320.f, 200.f }, 0.8f,
		ImGuiWindowFlags_::ImGuiWindowFlags_NoResize |
		ImGuiWindowFlags_::ImGuiWindowFlags_NoSavedSettings |
		ImGuiWindowFlags_::ImGuiWindowFlags_NoTitleBar);

	{
		ImGui::Separator();
		ImGui::TextColored(ImVec4(0.f, 1.f, 1.f, 1.f), u8"Добавить игрока");
		ImGui::Separator();
		ImGui::PopFont();
		ImGui::PushFont(ImGui::GetIO().Fonts->Fonts[0]);
		
		
		if (keyboard_focus && !showParams::show_category_redo)
		{
			ImGui::SetKeyboardFocusHere();
			keyboard_focus = false;
		}
		
		ImGui::InputText(u8"имя", user_name_arr.data(), user_name_arr.size(),
			ImGuiInputTextFlags_::ImGuiInputTextFlags_CallbackCharFilter,
			FilterLetters);


		if (ImGui::Button(u8"Добавить нового игрока", ImVec2(320.f - 2.f * ImGui::GetStyle().ItemSpacing.x, 30.f))
			|| ImGui::IsKeyPressed(ImGui::GetKeyIndex(ImGuiKey_::ImGuiKey_Enter), false))
		{
			user_name_str = user_name_arr.data();
			sounds::button_click.sound.play();
			if (!user_name_str.empty()) 
			{
				added = true;
				success = true;
				clock_add_player.restart();
				if (!game.AddNewPlayer(user_name_str)) 
				{
					msgBoxFlags::show_add_player_error = true;
					msgBoxFlags::msg_box_opend = true;
					WakeMsgBoxSoundThread();
					success = false;
				}
				user_name_str.clear();
			}
			ZeroMemory(user_name_arr.data(), user_name_arr.size());
			keyboard_focus = true;
		}
		ImGui::Separator();


		if (ImGui::Button(u8"Закрыть меню", ImVec2(320.f - 2.f * ImGui::GetStyle().ItemSpacing.x, 30.f)))
		{
			showParams::show_add_player_menu = false;
			sounds::button_click.sound.play();
		}
		ImGui::SetCursorPosY(150.f);
		

		if (added)
		{
			if (success) ImGui::TextColored({ 0.f, 1.f, 0.f, 1.f }, u8"успешно добавлен");
			else ImGui::TextColored({ 1.f, 0.f, 0.f, 1.f }, u8"ошибка добавления");

			if (clock_add_player.getElapsedTime().asSeconds() > 10)
				added = false;
		}
		
		
		
	}
	ImGui::End();
	ImGui::PopFont();
	
}

static bool keyboard_focus_categ_name = false;
void ShowCategorySettings(sf::Vector2i wnd_pos)
{
	
	
	ImGui::PushFont(ImGui::GetIO().Fonts->Fonts[1]);
	ImGui::SetNextWindowPosCenter(ImGuiCond_::ImGuiCond_FirstUseEver);
	ImGui::Begin(u8"Редактор категорий", nullptr, { 750.f, 570.f }, 0.9f,
		ImGuiWindowFlags_::ImGuiWindowFlags_NoResize |
		ImGuiWindowFlags_::ImGuiWindowFlags_NoSavedSettings |
		ImGuiWindowFlags_::ImGuiWindowFlags_NoCollapse);
	
	
	ImGui::PopFont();
	ImGui::PushFont(ImGui::GetIO().Fonts->Fonts[0]);
	//=============================================================================================
	
	if (showParams::show_category_choose)
	{
		ImGui::BeginChild("categories_list", { 0, 460 }, true, 
			ImGuiWindowFlags_::ImGuiWindowFlags_HorizontalScrollbar);
		
		for (size_t i = 0; i < game.categories_names.size(); ++i)
		{
			ImGui::PushID(i);
			
		
			ImGui::PushStyleColor(ImGuiCol_::ImGuiCol_ButtonHovered, { 0.68f, 0.67f, 0.65f, 0.4f });
			ImGui::PushStyleColor(ImGuiCol_::ImGuiCol_ButtonActive, { 0.68f, 0.67f, 0.65f, 0.4f });
			ImGui::PushStyleColor(ImGuiCol_::ImGuiCol_Button, { 0.68f, 0.67f, 0.65f, 0.0f });

			

			ImGui::Button("##back", 
				{ ImGui::GetItemRectSize().x - ImGui::GetStyle().ItemSpacing.x * 2.f, 0.f });
			if (ImGui::IsItemHovered())
			{
				ImGui::BeginTooltip();
				ImGui::TextUnformatted(u8"ПКМ для редактирования.");
				ImGui::EndTooltip();

			}
			ImGui::PopStyleColor(3);
			if (ImGui::BeginPopupContextItem("context_menu"))
			{
				
				if (ImGui::Button(u8"Удалить", { 200.f, 30.f }))
				{
					sounds::button_click.sound.play();
					if (!showParams::show_get_name_dialog)
					{
						WakeMsgBoxSoundThread();
						msgBoxFlags::msg_box_opend = true;
						msgBoxFlags::show_del_category_q = true;
						categEditor::choosed_category = i;
					}
					
				}
				if (ImGui::Button(u8"Редактировать", { 200.f, 30.f }))
				{
					sounds::button_click.sound.play();
					if (!showParams::show_get_name_dialog)
					{
						start_loading_window = true;
						auto f = std::async(std::launch::async, LoadingWindow,
							wnd_pos,
							windowSize::current,
							std::ref(start_loading_window));

						ZeroMemory(wordsBuf::input_words_buffer, wordsBuf::input_words_buffer_size);

						showParams::show_category_redo = true;
						showParams::show_category_choose = false;
						showParams::close_program_button_enabled = false;
						showParams::category_editor_enabled = false;


						categEditor::changeable_category.setPath(game.categories_paths[i]);
						categEditor::changeable_category.loadData();
						categEditor::choosed_category = i;
						start_loading_window = false;
					}

				}
				if (ImGui::Button(u8"Переименовать", { 200.f, 30.f }))
				{
					
					sounds::button_click.sound.play();
					if (!showParams::show_get_name_dialog)
					{
						msgBoxFlags::msg_box_opend = true;

						showParams::show_get_name_dialog = true;
						categEditor::choosed_category = i;
					}
				}
				if (ImGui::Button(u8"Дублировать", { 200.f, 30.f }))
				{
					sounds::button_click.sound.play();
					msgBoxFlags::msg_box_opend = true;
					showParams::show_get_name_dialog = true;
					categEditor::do_duplicate = true;
					categEditor::choosed_category = i;
				}


				ImGui::EndPopup();
			}
			
			ImGui::SameLine();
			ImGui::SetCursorPosX(ImGui::GetStyle().ItemInnerSpacing.x*3);
			ImGui::Text(game.categories_names[i].c_str());
			ImGui::Separator();
			

			ImGui::PopID();
		}

		if (keyboard_focus_categ_name)
		{
			ImGui::SetKeyboardFocusHere();
			keyboard_focus_categ_name = false;
		}

		ImGui::PushItemWidth(450.f);
		ImGui::InputText(u8"Название новой категории", 
			categEditor::category_name_arr.data(), categEditor::category_name_arr.size(),
			ImGuiInputTextFlags_::ImGuiInputTextFlags_CallbackCharFilter,
			FilterLetters);
		ImGui::PopItemWidth();

		if (ImGui::Button(u8"Добавить новую категорию", {0.f, 0.f})
			|| ImGui::IsKeyPressed(ImGui::GetKeyIndex(ImGuiKey_::ImGuiKey_Enter), false))
		{
			sounds::button_click.sound.play();
			categEditor::category_name_str = categEditor::category_name_arr.data();
			if (!categEditor::category_name_str.empty())
			{
				bool exist = false;
				for (size_t i = 0; i < game.categories_names.size(); ++i)
					if (game.categories_names[i] == categEditor::category_name_str) exist = true;
				if (!exist)
				{
					game.categories_names.push_back(categEditor::category_name_str);
					std::string path = createWayToNewCategory(categEditor::category_name_str);
					game.categories_paths.push_back(path);
					std::ofstream file(path);
					file << '\0';
					file.close();
					file.open(path, std::ios::trunc);
					file.close();
				}
				else
				{
					WakeMsgBoxSoundThread();
					msgBoxFlags::msg_box_opend = true;
					msgBoxFlags::show_add_category_error = true;

				}
				ZeroMemory(categEditor::category_name_arr.data(), categEditor::category_name_arr.size());
			}
			keyboard_focus_categ_name = true;
		}

		ImGui::EndChild();
		


		ImGui::SetCursorPosX(ImGui::GetItemRectSize().x - 100 + ImGui::GetStyle().ItemSpacing.x);
		if (ImGui::Button(u8"Закрыть", { 100, 35 }))
		{
			sounds::button_click.sound.play();
			showParams::show_category_editor = false;
			showParams::show_category_choose = false;
			showParams::show_category_redo = false;
		}


	}
	


	if(showParams::show_category_redo)
	{
		float spacing = ImGui::GetStyle().ItemSpacing.x;

		ImGui::TextColored({ 1.f, 0.f, 0.f, 1.f }, u8"Каждое слово отделяйте пробелами!");
		ImGui::SameLine();

		ImGui::SetCursorPosX(550.f - ImGui::GetStyle().ItemSpacing.x);
		if (ImGui::Button(u8"Справка по поиску", {200, 23}))
		{
			sounds::button_click.sound.play();
			showParams::show_help_search = true;
		}

		ImGui::InputTextMultiline("##editor", wordsBuf::input_words_buffer,
			wordsBuf::input_words_buffer_size, { -1.f, ImGui::GetTextLineHeight() * 18 },
			ImGuiInputTextFlags_::ImGuiInputTextFlags_AllowTabInput);


		if (strlen(wordsBuf::input_words_buffer) == 0
			|| showParams::category_enable_adding
			|| showParams::category_enable_deleting) 
			showParams::category_enable_search = false;
		else showParams::category_enable_search = true;
		
		if (ImGui::Button(u8"Очистить"))
		{
			sounds::button_click.sound.play();
			ZeroMemory(wordsBuf::input_words_buffer, wordsBuf::input_words_buffer_size);
			showParams::show_only_clear = false;
			showParams::category_enable_adding = false;
			showParams::category_enable_deleting = false;
			showParams::category_enable_search = true;
		}
		if (ImGui::IsItemHovered())
		{
			ImGui::BeginTooltip();
			ImGui::TextUnformatted(u8"Очистит поле ввода.");
			ImGui::EndTooltip();
		}
		ImGui::SameLine();
		ImGui::SetCursorPosX(150);
		ImGui::Text("UTF8 %d/%d", wordsBuf::input_words_buffer_size,
			strlen(wordsBuf::input_words_buffer));
		ImGui::SameLine();
		ImGui::SetCursorPosX(450);
		ImGui::Text(u8"Всего слов: %u", categEditor::changeable_category.getWordsCount());
		if (!showParams::show_only_clear)
		{
			if (!showParams::category_enable_adding)
			{
				ImGui::PushStyleColor(ImGuiCol_::ImGuiCol_ButtonHovered, { 0.14f, 0.85f, 0.22f, 0.3f });
				ImGui::PushStyleColor(ImGuiCol_::ImGuiCol_ButtonActive, { 0.14f, 0.85f, 0.22f, 0.3f });
				ImGui::PushStyleColor(ImGuiCol_::ImGuiCol_Button, { 0.14f, 0.85f, 0.22f, 0.3f });
			}
			else
			{
				ImGui::PushStyleColor(ImGuiCol_::ImGuiCol_Button, { 0.14f, 0.85f, 0.22f,  0.7f });
				ImGui::PushStyleColor(ImGuiCol_::ImGuiCol_ButtonActive, { 0.14f, 0.85f, 0.22f, 0.3f });
				ImGui::PushStyleColor(ImGuiCol_::ImGuiCol_ButtonHovered, { 0.14f, 0.85f, 0.22f, 1.0f });
			}

			
			
			if (ImGui::Button(u8"Добавить", { (750.f - 4.f * spacing) / 3.f, 30.f })
				&& showParams::category_enable_adding)
			{
				sounds::button_click.sound.play();
				msgBoxFlags::add_word = true;
				msgBoxFlags::msg_box_opend = true;
				WakeMsgBoxSoundThread();


				
			}
			if (ImGui::IsItemHovered() && !showParams::category_enable_adding)
			{
				ImGui::BeginTooltip();
				ImGui::TextUnformatted(u8"Сначала нажмите \"Проверить на наличие в категори\".");
				ImGui::EndTooltip();

			}
			ImGui::PopStyleColor(3);
			ImGui::SameLine();


			if (!showParams::category_enable_deleting)
			{
				ImGui::PushStyleColor(ImGuiCol_::ImGuiCol_ButtonHovered, { 0.85f, 0.14f, 0.22f, 0.3f });
				ImGui::PushStyleColor(ImGuiCol_::ImGuiCol_ButtonActive, { 0.85f, 0.14f, 0.22f, 0.3f });
				ImGui::PushStyleColor(ImGuiCol_::ImGuiCol_Button, { 0.85f, 0.14f, 0.22f, 0.3f });
			}
			else
			{
				ImGui::PushStyleColor(ImGuiCol_::ImGuiCol_Button, { 0.85f, 0.14f, 0.22f,  0.7f });
				ImGui::PushStyleColor(ImGuiCol_::ImGuiCol_ButtonActive, { 0.85f, 0.14f, 0.22f, 0.3f });
				ImGui::PushStyleColor(ImGuiCol_::ImGuiCol_ButtonHovered, { 0.85f, 0.14f, 0.22f, 1.0f });
			}
			if (ImGui::Button(u8"Удалить", { (750.f - 4.f * spacing) / 3.f, 30.f })
				&& showParams::category_enable_deleting)
			{
				sounds::button_click.sound.play();
				msgBoxFlags::msg_box_opend = true;
				msgBoxFlags::delete_word = true;
				WakeMsgBoxSoundThread();
				
			}
			if (ImGui::IsItemHovered() && showParams::category_enable_deleting)
			{
				ImGui::BeginTooltip();
				ImGui::TextUnformatted(u8"Удалит из категории слова,\nнаходящиеся в группе \"Уже есть\".");
				ImGui::EndTooltip();
			}
			else if (ImGui::IsItemHovered() && !showParams::category_enable_deleting)
			{
				ImGui::BeginTooltip();
				ImGui::TextUnformatted(u8"Сначала нажмите \"Проверить на наличие в категори\".");
				ImGui::EndTooltip();

			}
			ImGui::PopStyleColor(3);
			ImGui::SameLine();



			if (ImGui::Button(u8"Отмена", { (750.f - 4.f * spacing) / 3.f, 30.f }))
			{
				sounds::button_click.sound.play();
				showParams::show_category_redo           = false;
				showParams::show_category_choose         = true;
				showParams::close_program_button_enabled = true;
				showParams::category_editor_enabled      = true;
				showParams::category_enable_adding       = false;
				showParams::category_enable_deleting     = false;
				showParams::category_enable_save         = false;
				showParams::category_enable_search       = true;

				categEditor::changeable_category.setFullSaveFlag(false);
				categEditor::changeable_category.unloadData();

			}
			if (ImGui::IsItemHovered())
			{
				ImGui::BeginTooltip();
				ImGui::TextUnformatted(u8"Выход.\nИзменения не сохранятся.");
				ImGui::EndTooltip();
			}


			if (!showParams::category_enable_search)
			{
				ImVec4 button_col = ImGui::GetStyle().Colors[ImGuiCol_::ImGuiCol_Button];
				button_col.w *= 0.8f;

				ImGui::PushStyleColor(ImGuiCol_::ImGuiCol_Button, button_col);
				ImGui::PushStyleColor(ImGuiCol_::ImGuiCol_ButtonActive, button_col);
				ImGui::PushStyleColor(ImGuiCol_::ImGuiCol_ButtonHovered, button_col);

			}
			if (ImGui::Button(u8"Проверить на наличие в категории", { 750.f - 2.f * spacing, 30.f })
				&& showParams::category_enable_search)
			{
				sounds::button_click.sound.play();
				start_loading_window = true;
				showParams::category_enable_search = false;
				showParams::category_enable_adding = true;
				showParams::category_enable_deleting = true;
				auto f = std::async(std::launch::async, LoadingWindow,
					wnd_pos,
					windowSize::current,
					std::ref(start_loading_window));


				std::string input = wordsBuf::input_words_buffer;
				categEditor::changeable_category.inputStrParser(input);
				categEditor::changeable_category.splitWordsList();
				input = categEditor::changeable_category.createResultText();

				ZeroMemory(wordsBuf::input_words_buffer, wordsBuf::input_words_buffer_size);
				if (wordsBuf::input_words_buffer_size > input.size())
					std::memcpy(wordsBuf::input_words_buffer, input.c_str(),
						input.size());
				else
					std::memcpy(wordsBuf::input_words_buffer, input.c_str(),
						wordsBuf::input_words_buffer_size - 1);


				start_loading_window = false;

			}
			else if (!showParams::category_enable_search) ImGui::PopStyleColor(3);
			if (!showParams::category_enable_search
				&& ImGui::IsItemHovered()
				&& strlen(wordsBuf::input_words_buffer) != 0)
			{
				ImGui::BeginTooltip();
				ImGui::TextUnformatted(u8"Вы уже выполнили поиск.\nВыберите один из вариантов:\nУдалить; Добавить; Очистить поле ввода.");
				ImGui::EndTooltip();


			}
			if (!showParams::category_enable_search
				&& ImGui::IsItemHovered()
				&& strlen(wordsBuf::input_words_buffer) == 0)
			{
				ImGui::BeginTooltip();
				ImGui::TextUnformatted(u8"Необходимо ввести слова.");
				ImGui::EndTooltip();

			}

			if (!showParams::category_enable_save)
			{
				ImVec4 button_col = ImGui::GetStyle().Colors[ImGuiCol_::ImGuiCol_Button];
				button_col.w *= 0.8f;

				ImGui::PushStyleColor(ImGuiCol_::ImGuiCol_Button, button_col);
				ImGui::PushStyleColor(ImGuiCol_::ImGuiCol_ButtonActive, button_col);
				ImGui::PushStyleColor(ImGuiCol_::ImGuiCol_ButtonHovered, button_col);

			}
			if (ImGui::Button(u8"Сохранить и выйти", { 750.f - 2.f * spacing, 30.f })
				&& showParams::category_enable_save)
			{
				sounds::button_click.sound.play();
				showParams::show_category_redo           = false;
				showParams::show_category_choose         = true;
				showParams::close_program_button_enabled = true;
				showParams::category_editor_enabled      = true;
				showParams::category_enable_adding       = false;
				showParams::category_enable_deleting     = false;
				showParams::category_enable_save         = false;
				showParams::category_enable_search       = true;
				if (categEditor::changeable_category.isFullSave())
				{
					categEditor::changeable_category.saveData();
					categEditor::changeable_category.unloadData();

					categEditor::changeable_category.setFullSaveFlag(false);

				}
				else
				{
					categEditor::changeable_category.updateDataInFile();
					categEditor::changeable_category.unloadData();

				}
			}
			else if (!showParams::category_enable_save) ImGui::PopStyleColor(3);
			
			if (ImGui::IsItemHovered() && showParams::category_enable_save)
			{
				ImGui::BeginTooltip();
				ImGui::TextUnformatted(u8"Данные запишутся в файл.");
				ImGui::EndTooltip();
			}
			else if (ImGui::IsItemHovered() && !showParams::category_enable_save)
			{
				ImGui::BeginTooltip();
				ImGui::TextUnformatted(u8"Вы ничего не меняли.\nМожете нажать \"Отмена\".");
				ImGui::EndTooltip();
			}
		}
	}

	//=============================================================================================
	ImGui::End();
	ImGui::PopFont();
}

static sf::Clock clock_rename_categ;
static bool changed = false;
static bool change_success = true;
void ShowRenameDialog(sf::Vector2i wnd_pos)
{

	ImGui::SetNextWindowPosCenter(ImGuiCond_::ImGuiSetCond_FirstUseEver);
	ImGui::StyleColorsDark();
	ImGui::PushFont(ImGui::GetIO().Fonts->Fonts[1]);
	ImGui::Begin(u8"Введите название категории", nullptr, { 600.f, 220.f }, 1.0f,
		ImGuiWindowFlags_::ImGuiWindowFlags_NoResize |
		ImGuiWindowFlags_::ImGuiWindowFlags_NoSavedSettings |
		ImGuiWindowFlags_::ImGuiWindowFlags_NoCollapse);

	{
		ImGui::PopFont();
		ImGui::PushFont(ImGui::GetIO().Fonts->Fonts[0]);
		
		if (keyboard_focus)
		{
			ImGui::SetKeyboardFocusHere();
			keyboard_focus = false;
		}

		ImGui::InputText(u8"название", 
			categEditor::rename_buf_arr.data(), categEditor::rename_buf_arr.size(),
			ImGuiInputTextFlags_::ImGuiInputTextFlags_CallbackCharFilter,
			FilterLetters);
		
		ImGui::NewLine();
		ImGui::NewLine();
		if (ImGui::Button(u8"Применить", ImVec2((600.f - 3.f * ImGui::GetStyle().ItemSpacing.x) / 2.f, 30.f))
			|| ImGui::IsKeyPressed(ImGui::GetKeyIndex(ImGuiKey_::ImGuiKey_Enter), false))
		{
			categEditor::rename_buf_str = categEditor::rename_buf_arr.data();
			sounds::button_click.sound.play();
			
			if (!categEditor::rename_buf_str.empty())
			{
				bool exist = false;
				changed = true;
				change_success = false;
				clock_rename_categ.restart();
				for (size_t i = 0; i < game.categories_names.size(); ++i)
					if (game.categories_names[i] == categEditor::rename_buf_str) exist = true;
				if (!exist)
				{
					std::string new_path = createWayToNewCategory(categEditor::rename_buf_str);
					if (categEditor::do_duplicate)
					{
						start_loading_window = true;
						auto f = std::async(std::launch::async, LoadingWindow,
							wnd_pos,
							windowSize::current,
							std::ref(start_loading_window));


						std::ifstream infile(game.categories_paths[categEditor::choosed_category]);
						std::ofstream outfile(new_path, std::ios::trunc);
						outfile << infile.rdbuf();
						outfile.close();
						infile.close();
						game.categories_paths.push_back(new_path);
						game.categories_names.push_back(categEditor::rename_buf_str);
						start_loading_window = false;
					}
					else
					{
						

						rename(game.categories_paths[categEditor::choosed_category].c_str(),
							new_path.c_str());
						game.categories_paths[categEditor::choosed_category] = new_path;
						game.categories_names[categEditor::choosed_category] = categEditor::rename_buf_str;
					}

					change_success = true;
				}


			}
			ZeroMemory(categEditor::rename_buf_arr.data(), categEditor::category_name_arr.size());

			keyboard_focus = true;
		}
		ImGui::SameLine();
		if (ImGui::Button(u8"Отмена", ImVec2((600.f - 3.f * ImGui::GetStyle().ItemSpacing.x) / 2.f, 30.f)))
		{
			showParams::show_get_name_dialog = false;
			msgBoxFlags::msg_box_opend = false;
			sounds::button_click.sound.play();

		}
		ImGui::NewLine();

		std::string s1;
		std::string s2;
		if (categEditor::do_duplicate)
		{
			s1 = u8"успешно скопировано";
			s2 = u8"ошибка копирования, такое имя уже есть";

		}
		else
		{
			s1 = u8"успешно переименовано";
			s2 = u8"ошибка переименования, такое имя уже есть";
		}

		if (changed)
		{
			if (change_success)
			{

				ImGui::TextColored({ 0.f, 1.f, 0.f, 1.f }, s1.c_str());
				showParams::show_get_name_dialog = false;
				msgBoxFlags::msg_box_opend = false;
				categEditor::do_duplicate = false;
				changed = false;
			}
			else ImGui::TextColored({ 1.f, 0.f, 0.f, 1.f }, s2.c_str());

			if (clock_rename_categ.getElapsedTime().asSeconds() > 10)
				changed = false;

		}
		
	}
	ImGui::End();
	ImGui::PopFont();
	
}


bool category_choosed = false;
int bot_lvl = 2;
void ShowStartGameDialog(sf::Vector2i wnd_pos)
{
	ImGui::PushFont(ImGui::GetIO().Fonts->Fonts[1]);
	ImGui::SetNextWindowPosCenter(ImGuiCond_::ImGuiCond_FirstUseEver);
	ImGui::Begin(u8"Выбрать категорию для игры", nullptr, { 750.f, 570.f }, 0.9f,
		ImGuiWindowFlags_::ImGuiWindowFlags_NoResize |
		ImGuiWindowFlags_::ImGuiWindowFlags_NoSavedSettings |
		ImGuiWindowFlags_::ImGuiWindowFlags_NoCollapse);


	ImGui::PopFont();
	ImGui::PushFont(ImGui::GetIO().Fonts->Fonts[0]);
	//=============================================================================================

	
	{
		ImGui::BeginChild("categories_list_2", { 0, 335 }, true,
			ImGuiWindowFlags_::ImGuiWindowFlags_HorizontalScrollbar);

		for (size_t i = 0; i < game.categories_names.size(); ++i)
		{
			ImGui::PushID(i);

			
			if (categEditor::choosed_category != i)
			{
				ImGui::PushStyleColor(ImGuiCol_::ImGuiCol_ButtonHovered, { 0.68f, 0.67f, 0.65f, 0.4f });
				ImGui::PushStyleColor(ImGuiCol_::ImGuiCol_ButtonActive, { 0.68f, 0.67f, 0.65f, 0.4f });
				ImGui::PushStyleColor(ImGuiCol_::ImGuiCol_Button, { 0.68f, 0.67f, 0.65f, 0.0f });
			}
			else
			{
				ImGui::PushStyleColor(ImGuiCol_::ImGuiCol_ButtonHovered, { 0.68f, 0.67f, 0.65f, 0.4f });
				ImGui::PushStyleColor(ImGuiCol_::ImGuiCol_ButtonActive, { 0.68f, 0.67f, 0.65f, 0.4f });
				ImGui::PushStyleColor(ImGuiCol_::ImGuiCol_Button, { 0.68f, 0.67f, 0.65f, 0.4f });
				category_choosed = true;

			}
			

			if (ImGui::Button("##back_2",
				{ ImGui::GetItemRectSize().x - ImGui::GetStyle().ItemSpacing.x * 2.f, 0.f }))
			{
				sounds::button_click.sound.play();
				categEditor::choosed_category = i;
				category_choosed = true;

			}
			if (ImGui::IsItemHovered())
			{
				ImGui::BeginTooltip();
				ImGui::TextUnformatted(u8"Нажмите для выбора");
				ImGui::EndTooltip();
			}

			ImGui::PopStyleColor(3);
			

			ImGui::SameLine();
			ImGui::SetCursorPosX(ImGui::GetStyle().ItemSpacing.x * 2);
			ImGui::Text(game.categories_names[i].c_str());
			ImGui::Separator();


			ImGui::PopID();
		}
		

		ImGui::EndChild();
		float sz = ImGui::GetItemRectSize().x;
		ImGui::SetCursorPosX((750.f - ImGui::CalcTextSize(u8"Ранг бота:").x) / 2.f);
		ImGui::Text(u8"Ранг бота:");
		ImGui::SetCursorPosX(ImGui::GetStyle().ItemSpacing.x);
		ImGui::Text(u8"сложнее");
		ImGui::SameLine();

		ImGui::PushItemWidth(750.f - ImGui::GetStyle().ItemSpacing.x * 4.f - ImGui::CalcTextSize(u8"сложнее").x - ImGui::CalcTextSize(u8"проще").x);
		ImGui::SliderInt("##bot_lvl", &bot_lvl, 1, 4, "%d rank");
		ImGui::PopItemWidth();

		ImGui::SameLine();
		ImGui::Text(u8"проще");

		ImGui::NewLine();
		ImGui::NewLine();
		ImGui::NewLine();
		ImGui::SetCursorPosX(sz - 200);
		int counter = 3;
		if (game.categories_names.size() == 0
			|| categEditor::choosed_category >= game.categories_names.size()
			|| game.all_players.size() == 0) category_choosed = false;
		if (!category_choosed)
		{
			ImVec4 button_col = ImGui::GetStyle().Colors[ImGuiCol_::ImGuiCol_Button];
			button_col.w *= 0.8f;

			ImGui::PushStyleColor(ImGuiCol_::ImGuiCol_Button, button_col);
			ImGui::PushStyleColor(ImGuiCol_::ImGuiCol_ButtonActive, button_col);
			ImGui::PushStyleColor(ImGuiCol_::ImGuiCol_ButtonHovered, button_col);

		}
		if (ImGui::Button(u8"Начать", { 100, 35 }) && category_choosed)
		{
			sounds::button_click.sound.play();
			if (game.StartGame(wnd_pos, windowSize::const_current,
				categEditor::choosed_category, static_cast<size_t>(bot_lvl)))
			{
				bot_lvl = 2;
				showParams::add_player_enabled = false;
				showParams::show_player_list_enabled = false;
				showParams::close_program_button_enabled = false;
				showParams::category_editor_enabled = false;
				showParams::show_add_player_menu = false;
				showParams::show_category_editor = false;
				showParams::show_category_choose = false;
				showParams::show_users_list = false;
				showParams::show_start_game_dlg = false;
				showParams::show_load_game_dlg = false;

				category_choosed = false;
				counter = 0;
			}
			else
			{
				words_counter = " " + std::to_string(game.words_counter) + " ";
				words_counter = cp1251_to_utf8(words_counter);

				WakeMsgBoxSoundThread();
				showParams::show_start_game_dlg = false;
				msgBoxFlags::msg_box_opend = true;
				msgBoxFlags::show_miss_words_error = true;
			}
		}
		if (!category_choosed) ImGui::PopStyleColor(counter);

		if (!category_choosed && ImGui::IsItemHovered())
		{
			ImGui::BeginTooltip();
			ImGui::TextUnformatted(u8"Не выбрана категория\nили\nне добавлены игроки.");
			ImGui::EndTooltip();

		}
		ImGui::SameLine();
		if (ImGui::Button(u8"Отмена", { 100, 35 }))
		{
			sounds::button_click.sound.play();
			showParams::show_start_game_dlg = false;
		}


	}
	ImGui::PopFont();
	ImGui::End();
}


static sf::Clock save_game_clock;
static bool saved = false;
static bool save_success = true;
void ShowSaveMenu(sf::Vector2i wnd_pos)
{
	
	ImGui::SetNextWindowPosCenter(ImGuiCond_::ImGuiSetCond_FirstUseEver);
	ImGui::StyleColorsDark();
	ImGui::PushFont(ImGui::GetIO().Fonts->Fonts[1]);
	ImGui::Begin(u8"Ведите имя сохранения", nullptr, { 600.f, 220.f }, 1.0f,
		ImGuiWindowFlags_::ImGuiWindowFlags_NoResize |
		ImGuiWindowFlags_::ImGuiWindowFlags_NoSavedSettings |
		ImGuiWindowFlags_::ImGuiWindowFlags_NoCollapse);

	{
		ImGui::PopFont();
		ImGui::PushFont(ImGui::GetIO().Fonts->Fonts[0]);
		

		if (keyboard_focus)
		{
			ImGui::SetKeyboardFocusHere();
			keyboard_focus = false;
		}

		ImGui::InputText(u8"имя",
			save_name_arr.data(), save_name_arr.size(),
			ImGuiInputTextFlags_::ImGuiInputTextFlags_CallbackCharFilter,
			FilterLetters);
		ImGui::NewLine();
		ImGui::NewLine();
		if (ImGui::Button(u8"Сохранить и выйти", ImVec2((600.f - 3.f * ImGui::GetStyle().ItemSpacing.x) / 2.f, 30.f))
			|| ImGui::IsKeyPressed(ImGui::GetKeyIndex(ImGuiKey_::ImGuiKey_Enter), false))
		{
			save_name_str = save_name_arr.data();
			sounds::button_click.sound.play();

			if (!save_name_str.empty())
			{
				bool exist = false;
				saved = true;
				save_success = false;
				save_game_clock.restart();
				for (size_t i = 0; i < game.saves_names.size(); ++i)
					if (game.saves_names[i] == save_name_str) exist = true;
				if (!exist)
				{
					
					start_loading_window = true;
					auto f = std::async(std::launch::async, LoadingWindow,
						wnd_pos,
						windowSize::current,
						std::ref(start_loading_window));
					
					game.SaveGame(save_name_str);


					start_loading_window = false;
					showParams::show_save_game_dlg = false;

					showParams::add_player_enabled = true;
					showParams::show_player_list_enabled = true;
					showParams::close_program_button_enabled = true;
					showParams::category_editor_enabled = true;

					save_success = true;
				}


			}
			ZeroMemory(save_name_arr.data(), save_name_arr.size());

			keyboard_focus = true;
		}
		ImGui::SameLine();
		if (ImGui::Button(u8"Отмена", ImVec2((600.f - 3.f * ImGui::GetStyle().ItemSpacing.x) / 2.f, 30.f)))
		{
			
			showParams::show_save_game_dlg = false;
			sounds::button_click.sound.play();

		}
		ImGui::NewLine();

		

		if (saved)
		{
			if (save_success)
			{

				ImGui::TextColored({ 0.f, 1.f, 0.f, 1.f }, u8"успешно сохранено");
				
				saved = false;
			}
			else ImGui::TextColored({ 1.f, 0.f, 0.f, 1.f }, u8"ошибка сохранения (такое сохранение уже есть)");

			if (save_game_clock.getElapsedTime().asSeconds() > 10)
				saved = false;

		}

	}
	ImGui::End();
	ImGui::PopFont();



}

void ShowLoadGameMenu()
{
	ImGui::PushFont(ImGui::GetIO().Fonts->Fonts[1]);
	ImGui::SetNextWindowPosCenter(ImGuiCond_::ImGuiCond_FirstUseEver);
	ImGui::Begin(u8"Выбрать сохранение", nullptr, { 750.f, 570.f }, 0.9f,
		ImGuiWindowFlags_::ImGuiWindowFlags_NoResize |
		ImGuiWindowFlags_::ImGuiWindowFlags_NoSavedSettings |
		ImGuiWindowFlags_::ImGuiWindowFlags_NoCollapse);


	ImGui::PopFont();
	ImGui::PushFont(ImGui::GetIO().Fonts->Fonts[0]);
	//=============================================================================================


	{
		ImGui::BeginChild("categories_list_2", { 0, 470 }, true,
			ImGuiWindowFlags_::ImGuiWindowFlags_HorizontalScrollbar);

		for (size_t i = 0; i < game.saves_names.size(); ++i)
		{
			ImGui::PushID(i);
			if (ImGui::Button(u8"Загрузить", { 100.f, 30.f }))
			{
				sounds::button_click.sound.play();
				WakeMsgBoxSoundThread();
				msgBoxFlags::msg_box_opend = true;
				msgBoxFlags::really_load_save = true;
				idx_of_save = i;
			}
			ImGui::SameLine();
			if (ImGui::Button(u8"Удалить", { 100.f, 30.f }))
			{
				sounds::button_click.sound.play();
				WakeMsgBoxSoundThread();
				msgBoxFlags::msg_box_opend = true;
				msgBoxFlags::really_delete_save = true;
				idx_of_save = i;
			}
			
			ImGui::SameLine();
			ImGui::SetCursorPosX(ImGui::GetStyle().ItemSpacing.x * 4.f + 200.f);
			ImGui::Text(game.saves_names[i].c_str());
			

			ImGui::Separator();
			ImGui::PopID();
		}


		ImGui::EndChild();
		
		
		ImGui::SetCursorPosX(ImGui::GetItemRectSize().x - 100.f + ImGui::GetStyle().ItemSpacing.x);
		if (ImGui::Button(u8"Закрыть", { 100, 35 }))
		{
			sounds::button_click.sound.play();
			showParams::show_load_game_dlg = false;
		}


	}
	ImGui::PopFont();
	ImGui::End();
}

void ShowHelpWindow()
{
	ImGui::PushFont(ImGui::GetIO().Fonts->Fonts[1]);
	ImGui::SetNextWindowPosCenter(ImGuiCond_::ImGuiCond_FirstUseEver);
	ImGui::Begin(u8"Справка по расширенному поиску", nullptr, { 550.f, 570.f }, 0.9f,
		ImGuiWindowFlags_::ImGuiWindowFlags_NoResize |
		ImGuiWindowFlags_::ImGuiWindowFlags_NoSavedSettings |
		ImGuiWindowFlags_::ImGuiWindowFlags_NoCollapse);


	ImGui::PopFont();
	ImGui::PushFont(ImGui::GetIO().Fonts->Fonts[0]);
	//=============================================================================================


	{
		ImGui::BeginChild("categories_list_2", { 0, 470 }, true,
			ImGuiWindowFlags_::ImGuiWindowFlags_HorizontalScrollbar);
		
		ImGui::PushTextWrapPos(ImGui::GetCursorPos().x + 510.f);
		if (ImGui::TreeNode(u8"Метасимволы"))
		{
			ImGui::TextColored({ 0.f, 1.f, 0.f, 1.f }, "\\"); ImGui::SameLine(); ImGui::SetCursorPosX(60.f);
			ImGui::Text(u8"- считать следующий метасимвол как обычный символ");
			ImGui::Separator();
			ImGui::TextColored({ 0.f, 1.f, 0.f, 1.f }, "^"); ImGui::SameLine(); ImGui::SetCursorPosX(60.f);
			ImGui::Text(u8"- начало строки");
			ImGui::Separator();
			ImGui::TextColored({ 0.f, 1.f, 0.f, 1.f }, "."); ImGui::SameLine(); ImGui::SetCursorPosX(60.f);
			ImGui::Text(u8"- один произвольный символ. Кроме '\\n', обозначающего конец строки");
			ImGui::Separator();
			ImGui::TextColored({ 0.f, 1.f, 0.f, 1.f }, "$"); ImGui::SameLine(); ImGui::SetCursorPosX(60.f);
			ImGui::Text(u8"- конец строки");
			ImGui::Separator();
			ImGui::TextColored({ 0.f, 1.f, 0.f, 1.f }, "|"); ImGui::SameLine(); ImGui::SetCursorPosX(60.f);
			ImGui::Text(u8"- альтернатива (или)");
			ImGui::Separator();
			ImGui::TextColored({ 0.f, 1.f, 0.f, 1.f }, "( )"); ImGui::SameLine(); ImGui::SetCursorPosX(60.f);
			ImGui::Text(u8"- группировка");
			ImGui::Separator();
			ImGui::TextColored({ 0.f, 1.f, 0.f, 1.f }, "[ ]"); ImGui::SameLine(); ImGui::SetCursorPosX(60.f);
			ImGui::Text(u8"- класс символов");
			
			ImGui::TreePop();
		}

		if (ImGui::TreeNode(u8"Модификаторы метасимволов"))
		{
			ImGui::TextColored({ 1.f, 0.58f, 0.f, 1.f }, u8"Модификаторы пишутся сразу после символа (метасимвола), к которому они применяются");
			ImGui::Separator();
			ImGui::TextColored({ 0.f, 1.f, 0.f, 1.f }, "*"); ImGui::SameLine(); ImGui::SetCursorPosX(80.f);
			ImGui::Text(u8"- повторяется 0 или большее число раз");
			ImGui::Separator();
			ImGui::TextColored({ 0.f, 1.f, 0.f, 1.f }, "+"); ImGui::SameLine(); ImGui::SetCursorPosX(80.f);
			ImGui::Text(u8"- повторяется 1 или большее число раз");
			ImGui::Separator();
			ImGui::TextColored({ 0.f, 1.f, 0.f, 1.f }, "?"); ImGui::SameLine(); ImGui::SetCursorPosX(80.f);
			ImGui::Text(u8"- 1 или 0 раз");
			ImGui::Separator();
			ImGui::TextColored({ 0.f, 1.f, 0.f, 1.f }, "{n}"); ImGui::SameLine(); ImGui::SetCursorPosX(80.f);
			ImGui::Text(u8"- точно n раз");
			ImGui::Separator();
			ImGui::TextColored({ 0.f, 1.f, 0.f, 1.f }, "{n,}"); ImGui::SameLine(); ImGui::SetCursorPosX(80.f);
			ImGui::Text(u8"- по меньшей мере раз");
			ImGui::Separator();
			ImGui::TextColored({ 0.f, 1.f, 0.f, 1.f }, "{n,m}"); ImGui::SameLine(); ImGui::SetCursorPosX(80.f);
			ImGui::Text(u8"- не менше n, но и не больше m");
			ImGui::Separator();
			

			ImGui::TreePop();
		}
		/*if (ImGui::TreeNode(u8"Модификаторы метасимволов"))
		{
			ImGui::TextColored({ 1.f, 0.58f, 0.f, 1.f }, u8"Модификаторы пишутся сразу после символа (метасимвола), к которому они применяются");
			ImGui::Separator();
			ImGui::TextColored({ 0.f, 1.f, 0.f, 1.f }, "*"); ImGui::SameLine(); ImGui::SetCursorPosX(80.f);
			ImGui::Text(u8"- повторяется 0 или большее число раз");
			ImGui::Separator();
			ImGui::TextColored({ 0.f, 1.f, 0.f, 1.f }, "+"); ImGui::SameLine(); ImGui::SetCursorPosX(80.f);
			ImGui::Text(u8"- повторяется 1 или большее число раз");
			ImGui::Separator();
			ImGui::TextColored({ 0.f, 1.f, 0.f, 1.f }, "?"); ImGui::SameLine(); ImGui::SetCursorPosX(80.f);
			ImGui::Text(u8"- 1 или 0 раз");
			ImGui::Separator();
			ImGui::TextColored({ 0.f, 1.f, 0.f, 1.f }, "{n}"); ImGui::SameLine(); ImGui::SetCursorPosX(80.f);
			ImGui::Text(u8"- точно n раз");
			ImGui::Separator();
			ImGui::TextColored({ 0.f, 1.f, 0.f, 1.f }, "{n,}"); ImGui::SameLine(); ImGui::SetCursorPosX(80.f);
			ImGui::Text(u8"- по меньшей мере раз");
			ImGui::Separator();
			ImGui::TextColored({ 0.f, 1.f, 0.f, 1.f }, "{n,m}"); ImGui::SameLine(); ImGui::SetCursorPosX(80.f);
			ImGui::Text(u8"- не менше n, но и не больше m");
			ImGui::Separator();


			ImGui::TreePop();
		}*/

		ImGui::PopTextWrapPos();

		
		ImGui::EndChild();


		ImGui::SetCursorPosX(ImGui::GetItemRectSize().x - 100.f + ImGui::GetStyle().ItemSpacing.x);
		if (ImGui::Button(u8"Закрыть", { 100, 35 }))
		{
			sounds::button_click.sound.play();
			showParams::show_help_search = false;
		}


	}
	ImGui::PopFont();
	ImGui::End();
}
