#pragma once
#include "imgui.h"
#include "imgui-SFML.h"

//SFML
#include <SFML/System/Clock.hpp>
#include <SFML/Window/Event.hpp>
#include <SFML/Graphics.hpp>
#include <SFML/Audio/Music.hpp>
#include <SFML/Audio/Sound.hpp>
#include <SFML/Audio/SoundBuffer.hpp>


#include <Windows.h>
#include <array>
#include <filesystem>
#include <sstream>
#include <vector>
#include <iterator>
#include <locale>
#include <string>
#include <set>
#include <fstream>
#include <codecvt>


#include <iostream>

typedef std::vector <std::pair<std::string, uint64_t>> result_list;
typedef std::set    <std::wstring>                      parsed_list;
typedef std::set    <std::wstring>                      added_words_list;
typedef std::vector <std::wstring>                      existing_words_list;





