#pragma once
#include "pch.h"
#include "Words.h"




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
void GetImagesPathes(std::vector <std::string> &image_path);
void GetMusicPathes(std::vector <std::string> &music_path,
					std::vector <std::string> &playlist);
void GetCategoriesPathes(std::vector <std::string> &categories_path);
void GetSavesPathes(std::vector <std::string> &saves_path);

void LoadAddictedImagePaths(std::vector <std::string> &paths);
void UnLoadAddictedImagePaths(std::vector <std::string> &paths);

//sprites and textures
void LoadAllBackgroundTextures(std::vector <sf::Texture> &all_textures,
	                           std::vector <std::string> &pathes,
	                           std::vector <std::string> &added_pathes);

void LoadAllBckgrndExampleSp(std::vector <sf::Texture> &all_textures,
	                         std::vector <sf::Sprite>  &all_sprites);

sf::Sprite SetBackgrounSprite(sf::Sprite   sprite,
	                          sf::Vector2u transform_size);

void AddBackground(std::string               &path,
	               std::vector <sf::Texture> &all_textures,
	               std::vector <sf::Sprite>  &all_sprites);

void DeleteBackground(size_t                     idx,
	                  std::vector <sf::Texture> &all_textures,
	                  std::vector <sf::Sprite>  &all_sprites,
	                  std::vector <std::string> &paths,
	                  std::vector <std::string> &added_paths,
	                  bool                       remove_from_device = false);


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


//utf8 encoding and decoding
std::string utf8_to_string          (const char *utf8str, const std::locale& loc);
std::string utf8_to_string          (const wchar_t *utf8str, const std::locale& loc);
void        cp1251_to_utf8          (char *out, const char *in);
std::string cp1251_to_utf8          (std::string &input);
std::string convert_1251char_to_utf8(char c);

//other
std::string        getFilename(std::string path);
int                FilterLetters(ImGuiInputTextCallbackData* data);


std::string createWayToNewCategory(std::string &name);
std::string createWayToNewSave(std::string &name);