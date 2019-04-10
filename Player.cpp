#include "pch.h"
#include "Player.h"



const std::string & Player::getName() const
{
	return name;
}

uint64_t Player::getScore() const
{
	return score;
}

void Player::setScore(uint64_t score)
{
	this->score = score;
}

void Player::incScore()
{
	++score;
}

void Player::decScore()
{
	--score;
}



Player::Player()
{
}

Player::Player(std::string  name,
               std::string* printed_word):	name(name),
											printed_word(printed_word)					
{
	ZeroMemory(buffer.data(), buffer.size());
}

Player::Player(const Player &obj) :	name(obj.name),
										printed_word(obj.printed_word)

{
	ZeroMemory(buffer.data(), buffer.size());
}

Player &Player::operator=(const Player &right)
{
	if (this == &right) return *this;
	
	this->name = right.name;
	this->printed_word = right.printed_word;
	ZeroMemory(buffer.data(), buffer.size());

	return *this;
}

Player::~Player()
{
	
}

void Player::ResetStyle()
{
	background_col[0] = 0.f;
	background_col[1] = 0.f;
	background_col[2] = 0.f;
	background_col[3] = 0.7f;

	text_col[0] = 1.f;
	text_col[1] = 1.f;
	text_col[2] = 1.f;

	header_col[0] = 0.f;
	header_col[1] = 0.f;
	header_col[2] = 0.f;
	header_col[3] = 0.85f;
}

void Player::ShowLight(bool &word_printed, std::string letter)
{
	ImGui::StyleColorsLight();
	ImGui::PushFont(ImGui::GetIO().Fonts->Fonts[1]);
	
	
	ImGui::Begin(name.c_str(), nullptr, { 420, 0.f }, 0.6f,
		ImGuiWindowFlags_::ImGuiWindowFlags_NoResize |
		ImGuiWindowFlags_::ImGuiWindowFlags_NoSavedSettings |
		ImGuiWindowFlags_::ImGuiWindowFlags_NoCollapse |
		ImGuiWindowFlags_::ImGuiWindowFlags_AlwaysAutoResize);
	{
		ImGui::PopFont();
		ImGui::PushFont(ImGui::GetIO().Fonts->Fonts[0]);
		ImGui::Text(u8"Вам нужно сказать слово на букву:");
		


		float txt_size = ImGui::CalcTextSize(u8"Вам нужно сказать слово на букву:").x;
		txt_size += ImGui::CalcTextSize(letter.c_str()).x;
		txt_size -= ImGui::CalcTextSize(u8"Ваши очки:").x;
		txt_size -= ImGui::GetStyle().ItemSpacing.x * 2.f;
		txt_size -= ImGui::CalcTextSize(u8"(?)").x;
		
		
		
		
		ImGui::SameLine();
		ImGui::Text(letter.c_str());
		
		
		
		if (keyboard_focus || first_show_light)
		{

			ImGui::SetKeyboardFocusHere();
			keyboard_focus = false;
			first_show_light = false;
		}
		
		
		
		
		ImGui::PushItemWidth(txt_size);
		ImGui::InputText(u8"                                ", 
			buffer.data(), buffer.size());
		ImGui::PopItemWidth();




		if (ImGui::Button(u8"сказать", 
			{ txt_size, 0.f })
			|| ImGui::IsKeyPressed(ImGui::GetKeyIndex(ImGuiKey_::ImGuiKey_Enter)))
		{
			*printed_word = buffer.data();
			if(!(printed_word->empty())) word_printed = true;
			ZeroMemory(buffer.data(), buffer.size());
			keyboard_focus = true;
		}
		ImGui::SameLine();
		
		
		
		
		
		
		ImGui::TextDisabled("(?)");
		if (ImGui::IsItemHovered())
		{
			ImGui::BeginTooltip();


			ImGui::Text(u8"Очки начисляются в процессе игры:");
			ImGui::BulletText(u8"+1 если сказанное слово есть в списке");
			ImGui::BulletText(u8"-1 если сказанного слова нет в списке");
			ImGui::BulletText(u8"если счётчик обнулится, игрок проиграет \nи будет заморожен");


			ImGui::EndTooltip();
		}







		ImGui::SameLine();
		ImGui::Text(u8"Ваши очки:"); ImGui::SameLine(); ImGui::Text(std::to_string(score).c_str());
		
		
		

	}
	ImGui::End();
	ImGui::PopFont();
	
	ImGui::StyleColorsDark();
}

void Player::ShowDark()
{
	first_show_light = true;
	ImGui::PushStyleColor(ImGuiCol_::ImGuiCol_WindowBg,
		{ background_col[0], background_col[1], background_col[2], background_col[3] });
	
	ImGui::PushStyleColor(ImGuiCol_::ImGuiCol_TitleBgActive,
		{ header_col[0], header_col[1], header_col[2], header_col[3] });
	ImGui::PushStyleColor(ImGuiCol_::ImGuiCol_TitleBg,
		{ header_col[0] * 0.5f, header_col[1] * 0.5f, header_col[2] * 0.5f, header_col[3] * 0.8f});
	ImGui::PushStyleColor(ImGuiCol_::ImGuiCol_TitleBgCollapsed,
		{ header_col[0], header_col[1], header_col[2], header_col[3] * 0.5f});

	ImGui::PushStyleColor(ImGuiCol_::ImGuiCol_Text,
		{ text_col[0], text_col[1], text_col[2], 1.f});

	ImGui::PushFont(ImGui::GetIO().Fonts->Fonts[1]);
	
	ImGui::Begin(name.c_str(), nullptr, { 0.f, 0.f }, 1.0f,
		ImGuiWindowFlags_::ImGuiWindowFlags_NoResize |
		ImGuiWindowFlags_::ImGuiWindowFlags_NoSavedSettings |
		ImGuiWindowFlags_::ImGuiWindowFlags_AlwaysAutoResize |
		ImGuiWindowFlags_::ImGuiWindowFlags_NoCollapse);
	{
		ImGui::PopFont();
		ImGui::PushFont(ImGui::GetIO().Fonts->Fonts[0]);
		ImGui::Text(u8"Ждите своего хода");
		ImGui::Text(u8"Ваши очки: "); ImGui::SameLine(); ImGui::Text(std::to_string(score).c_str());
		
		
		
		
		
		ImGui::SameLine(); ImGui::TextDisabled("(?)");
		if (ImGui::IsItemHovered()) 
		{
			ImGui::BeginTooltip();
			ImGui::PopStyleColor();
			
			ImGui::Text(u8"Очки начисляются в процессе игры:");
			ImGui::BulletText(u8"+1 если сказанное слово есть в списке");
			ImGui::BulletText(u8"-1 если сказанного слова нет в списке");
			ImGui::BulletText(u8"если счётчик обнулится, игрок проиграет \nи будет заморожен");
			ImGui::PushStyleColor(ImGuiCol_::ImGuiCol_Text,
					{ text_col[0], text_col[1], text_col[2], 1.f });
			
			ImGui::EndTooltip();
		}





		if (ImGui::CollapsingHeader(u8"Изменить цвет")) 
		{
			ImGui::PopFont();
			ImGui::PushFont(ImGui::GetIO().Fonts->Fonts[3]);
			ImGui::PushItemWidth(180.f);

			ImGui::ColorEdit4(u8"Фон",            background_col);
			ImGui::ColorEdit4(u8"Заголовок окна", header_col);
			ImGui::ColorEdit3(u8"Текст",          text_col);
			ImGui::PopItemWidth();
		}
	}
	ImGui::PopFont();
	ImGui::PopStyleColor(5);
	ImGui::End();
	
	
}



float* Player::getColors()
{
	static float colors[11];
	std::memcpy(&colors[0], header_col,     sizeof(header_col));
	std::memcpy(&colors[4], background_col, sizeof(background_col));
	std::memcpy(&colors[8], text_col,       sizeof(text_col));
	return colors;
}

void Player::setColors(float * header, float * background, float * text)
{
	std::memcpy(header_col,     header,     sizeof(header_col));
	std::memcpy(background_col, background, sizeof(background_col));
	std::memcpy(text_col,       text,       sizeof(text_col));
}




