#pragma once
#include "pch.h"

class SystemSound {
private:
	sf::SoundBuffer buffer;
public:
	sf::Sound sound;


	SystemSound(const char* way_to_file) {
		buffer.loadFromFile(way_to_file);
		sound.setBuffer(buffer);
	}

};


namespace sounds 
{
	static SystemSound button_click(".\\data\\sound\\button_click.wav");
	static SystemSound show_msg_box(".\\data\\sound\\notification.wav");
}
namespace msgBoxFlags 
{
	static bool msg_box_opend              = false;
	static bool show_add_player_error      = false;
	static bool show_close_window_question = false;
	static bool you_should_add_players     = false;
	static bool show_end_game_dialog       = false;
	static bool you_are_in_game            = false;
	static bool you_are_not_in_game        = false;
	static bool really_delete              = false;
	static bool really_exclude             = false;
	static bool wrong_word_box             = false;
	static bool all_players_lost           = false;
	static bool delete_word                = false;
	static bool add_word                   = false;
	static bool exit_categoty_redo         = false;
	static bool show_add_category_error    = false;
	static bool show_del_category_q        = false;
	static bool show_miss_words_error      = false;
	static bool show_bot_losed_msg         = false;
	static bool show_can_t_fint_word_msg   = false;
	static bool really_load_save           = false;
	static bool really_delete_save         = false;
	static bool load_game_error            = false;
}


void WarningBox(sf::Sprite  &warning_sp, 
	            const char*  label, 
	            const char*  text) 
{

	static ImVec2 txt_size = ImGui::CalcTextSize(u8"ctrl >> закрыть окно");
	ImGui::SetNextWindowPosCenter();
	ImGui::PushStyleColor(ImGuiCol_::ImGuiCol_WindowBg,      (ImVec4)ImColor::HSV(0.683f, 1.f, 1.f));
	ImGui::PushStyleColor(ImGuiCol_::ImGuiCol_Button,        (ImVec4)ImColor::HSV(0.163f, 0.6f, 0.6f));
	ImGui::PushStyleColor(ImGuiCol_::ImGuiCol_ButtonHovered, (ImVec4)ImColor::HSV(0.163f, 0.8f, 0.8f));
	ImGui::PushStyleColor(ImGuiCol_::ImGuiCol_ButtonActive,  (ImVec4)ImColor::HSV(0.163f, 1.f, 1.f));
	ImGui::PushFont(ImGui::GetIO().Fonts->Fonts[1]);
	ImGui::Begin(label, nullptr, { 500, 250 }, 0.65f,
		ImGuiWindowFlags_::ImGuiWindowFlags_NoResize |
		ImGuiWindowFlags_::ImGuiWindowFlags_HorizontalScrollbar |
		ImGuiWindowFlags_::ImGuiWindowFlags_NoSavedSettings |
		ImGuiWindowFlags_::ImGuiWindowFlags_NoCollapse);
	ImGui::PopFont();
	ImGui::PushFont(ImGui::GetIO().Fonts->Fonts[0]);
	ImGui::Columns(2, nullptr, false);
	ImGui::Image(warning_sp);
	ImGui::NextColumn();
	ImGui::TextWrapped(text);
	ImGui::NewLine();

	if (ImGui::Button("OK", {100, 50}) || ImGui::GetIO().KeyCtrl)
	{
		msgBoxFlags::show_add_player_error    = false;
		msgBoxFlags::you_should_add_players   = false;
		msgBoxFlags::msg_box_opend            = false;
		msgBoxFlags::you_are_in_game          = false;
		msgBoxFlags::you_are_not_in_game      = false;
		msgBoxFlags::wrong_word_box           = false;
		msgBoxFlags::all_players_lost         = false;
		msgBoxFlags::show_add_category_error  = false;
		msgBoxFlags::show_miss_words_error    = false;
		msgBoxFlags::show_bot_losed_msg       = false;
		msgBoxFlags::show_can_t_fint_word_msg = false;
		msgBoxFlags::load_game_error          = false;
		
		sounds::button_click.sound.play();
	}
	ImGui::Columns(1);
	
	ImGui::SetCursorPosX(480.f - txt_size.x);
	ImGui::SetCursorPosY(230.f - txt_size.y);
	ImGui::Text(u8"ctrl >> закрыть окно");
	ImGui::PopFont();
	ImGui::End();
	ImGui::PopStyleColor(4);


	/*in.read(reinterpret_cast<char*>(&sound_vol), sizeof(float));
	in.read(reinterpret_cast<char*>(&music_vol), sizeof(float));
	in.read(reinterpret_cast<char*>(&wnd_size.x), sizeof(unsigned));
	in.read(reinterpret_cast<char*>(&wnd_size.y), sizeof(unsigned));
	in.read(reinterpret_cast<char*>(&sound_off_flag), sizeof(bool));
	in.read(reinterpret_cast<char*>(&music_off_flag), sizeof(bool));
	in.read(reinterpret_cast<char*>(&fullscreen), sizeof(bool));
	in.read(reinterpret_cast<char*>(&static_background), sizeof(bool));
	in.read(reinterpret_cast<char*>(&idx_of_background), sizeof(size_t));*/
}


void QuestionBox(sf::Sprite   &question_sp, 
	             const char*   label,
	             const char*   text,
	             bool         &answer) 
{

	
	ImGui::SetNextWindowPosCenter();
	ImGui::PushStyleColor(ImGuiCol_::ImGuiCol_WindowBg,      (ImVec4)ImColor::HSV(0.683f, 1.f, 1.f));
	ImGui::PushStyleColor(ImGuiCol_::ImGuiCol_Button,        (ImVec4)ImColor::HSV(0.736f, 0.6f, 0.6f));
	ImGui::PushStyleColor(ImGuiCol_::ImGuiCol_ButtonHovered, (ImVec4)ImColor::HSV(0.736f, 0.8f, 0.8f));
	ImGui::PushStyleColor(ImGuiCol_::ImGuiCol_ButtonActive,  (ImVec4)ImColor::HSV(0.736f, 1.f, 1.f));
	ImGui::PushFont(ImGui::GetIO().Fonts->Fonts[1]);
	ImGui::Begin(label, nullptr, { 500, 250 }, 0.65f,
		ImGuiWindowFlags_::ImGuiWindowFlags_NoResize |
		ImGuiWindowFlags_::ImGuiWindowFlags_HorizontalScrollbar |
		ImGuiWindowFlags_::ImGuiWindowFlags_NoSavedSettings |
		ImGuiWindowFlags_::ImGuiWindowFlags_NoCollapse);
	ImGui::PopFont();
	ImGui::PushFont(ImGui::GetIO().Fonts->Fonts[0]);
	ImGui::Columns(2, nullptr, false);
	ImGui::Image(question_sp);
	ImGui::NextColumn();
	ImGui::TextWrapped(text);
	ImGui::NewLine();
	bool answered = false;
	if (ImGui::Button("Yes", { 100, 50 }))
	{
		answered = true;
		answer = true;
		sounds::button_click.sound.play();
	}
	ImGui::SameLine();
	if (ImGui::Button("No", {100, 50}))
	{
		answered = true;
		answer = false;
		
		sounds::button_click.sound.play();
	}
	if (answered)
	{
		msgBoxFlags::msg_box_opend              = false;
		msgBoxFlags::show_close_window_question = false;
		msgBoxFlags::really_delete              = false;
		msgBoxFlags::really_exclude             = false;
		msgBoxFlags::delete_word                = false;
		msgBoxFlags::add_word                   = false;
		msgBoxFlags::exit_categoty_redo         = false;
		msgBoxFlags::show_del_category_q        = false;
		msgBoxFlags::really_delete_save         = false;
		msgBoxFlags::really_load_save           = false;

	}
	ImGui::PopFont();
	ImGui::End();
	ImGui::PopStyleColor(4);



}