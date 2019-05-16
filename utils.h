#pragma once
#include "pch.h"
#include "Words.h"

//utf8 encoding and decoding
std::string  utf8_to_string(const char *utf8str, const std::locale& loc);
std::string  utf8_to_string(const wchar_t *utf8str, const std::locale& loc);
std::wstring utf8_to_wstring(const char* utf8str);
std::string  wstring_to_utf8(const wchar_t* wstr);
std::string  wchar_to_utf8(wchar_t wstr);
std::string  cp1251_to_utf8(std::string &input);
std::string  utf8_to_string(const std::wstring& wstr, const std::locale& loc);



//imgui addons
void ShowHelpMarker(const char* desc);

static auto vector_to_const_char = [](void* vec, int idx, const char** out_text)
{
	auto& vector = *static_cast<std::vector<std::string>*>(vec);
	if (idx < 0 || idx >= static_cast<int>(vector.size())) { return false; }
	*out_text = vector.at(idx).c_str();
	return true;
};

bool ListBox(const char* label, int* currIndex,
	std::vector<std::string>& values,
	int height_in_items);


//paths
void GetImagesPathes(std::vector <std::wstring> &image_path);
void GetMusicPathes(std::vector <std::wstring> &music_path,
					std::vector <std::string> &playlist);
void GetCategoriesPathes(std::vector <std::wstring> &categories_path);
void GetSavesPathes(std::vector <std::wstring> &saves_path);



//sprites and textures
void LoadAllBackgroundTextures(std::vector <sf::Texture> &all_textures,
	                           std::vector <std::wstring> &pathes);

void LoadAllBckgrndExampleSp(std::vector <sf::Texture> &all_textures,
	                         std::vector <sf::Sprite>  &all_sprites);

sf::Sprite SetBackgrounSprite(sf::Sprite   sprite,
	                          sf::Vector2u transform_size);

void AddBackground(std::wstring              &path,
	               std::vector <sf::Texture> &all_textures,
	               std::vector <sf::Sprite>  &all_sprites);

void DeleteBackground(size_t                     idx,
	                  std::vector <sf::Texture> &all_textures,
	                  std::vector <sf::Sprite>  &all_sprites,
	                  std::vector <std::wstring> &paths);


//load/unload information
void LoadSystemParams(float        &sound_vol, 
                      float        &music_vol, 
                      sf::Vector2u &wnd_size, 
                      bool         &sound_off_flag,
	                  bool         &music_off_flag,
                      bool         &fullscreen,
                      bool         &static_background,
                      size_t       &idx_of_background);


void SaveSystemParams(float         &sound_vol,
                      float         &music_vol,
                      sf::Vector2u  &wnd_size, 
                      bool          &sound_off_flag,
	                  bool          &music_off_flag,
                      bool          &fullscreen,
                      bool          &static_background,
                      size_t        &idx_of_background);



void LoadingWindow(sf::Vector2i window_pos, 
	               sf::Vector2u window_size, 
	               bool        &start_loading_window);




//other
std::wstring        getFilename(std::wstring path);
int                 FilterLetters(ImGuiInputTextCallbackData* data);


std::wstring createWayToNewCategory(std::wstring &name);
std::wstring createWayToNewSave(std::wstring &name);