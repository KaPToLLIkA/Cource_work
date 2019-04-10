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
#include <thread>
#include <mutex>
#include <locale>
#include <string>
#include <set>

#include <fstream>
#include <codecvt>

#include <iostream>

typedef std::vector <std::pair<std::string, uint64_t>> result_list;
typedef std::set    <std::string>                      parsed_list_cp1251;
typedef std::set    <std::string>                      added_words_list_utf8;
typedef std::set    <std::string>                      existing_words_list_utf8;
typedef std::vector <std::vector <std::string>>        set;
typedef std::vector <std::string>::iterator            set_iterator;




