#include "pch.h"
#include "Words.h"

std::wstring wstring::m_input_buffer = L"";




//encoding and decoding
void encode(std::wstring& in)
{
	wstring w(in);
	encode(w);
	in = w.c_str();
}
void decode(std::wstring& in)
{
	wstring w(in);
	decode(w);
	in = w.c_str();
}
void encode(wstring &in)
{
	for (size_t i = 0; i < in.size(); ++i)
	{
		in[i] = ~in[i];
		int16_t x = in[i] & 0xAAAA;
		int16_t y = in[i] & 0x5555;
		x >>= 1;
		y <<= 1;
		x &= 0x7FFF;
		in[i] = x | y;

		x = in[i] & 0xCCCC;
		y = in[i] & 0X3333;
		x >>= 2;
		y <<= 2;
		x &= 0x3FFF;
		in[i] = x | y;


		if (in[i] == 0x0000) in[i] = 0x0F00;
		else if (in[i] == 0x000F) in[i] = 0xFF00;
		else if (in[i] == 0x00F0) in[i] = 0xF00F;
		else if (in[i] == 0x00FF) in[i] = 0xF0FF;
		else if (in[i] == 0x0F00) in[i] = 0x0000;
		else if (in[i] == 0x0F0F) in[i] = 0xFF0F;
		else if (in[i] == 0x0FF0) in[i] = 0xFFF0;
		else if (in[i] == 0x0FFF) in[i] = 0xF0F0;
		else if (in[i] == 0xF000) in[i] = 0xFFFF;
		else if (in[i] == 0xF00F) in[i] = 0x00F0;
		else if (in[i] == 0xF0F0) in[i] = 0x0FFF;
		else if (in[i] == 0xF0FF) in[i] = 0x00FF;
		else if (in[i] == 0xFF00) in[i] = 0x000F;
		else if (in[i] == 0xFF0F) in[i] = 0x0F0F;
		else if (in[i] == 0xFFF0) in[i] = 0x0FF0;
		else if (in[i] == 0xFFFF) in[i] = 0xF000;


		if (in[i] == 0xAAAA) in[i] = 0xA5AA;
		else if (in[i] == 0xAAA5) in[i] = 0x55AA;
		else if (in[i] == 0xAA5A) in[i] = 0x5AA5;
		else if (in[i] == 0xAA55) in[i] = 0x5A55;
		else if (in[i] == 0xA5AA) in[i] = 0xAAAA;
		else if (in[i] == 0xA5A5) in[i] = 0x55A5;
		else if (in[i] == 0xA55A) in[i] = 0x555A;
		else if (in[i] == 0xA555) in[i] = 0x5A5A;
		else if (in[i] == 0x5AAA) in[i] = 0x5555;
		else if (in[i] == 0x5AA5) in[i] = 0xAA5A;
		else if (in[i] == 0x5A5A) in[i] = 0xA555;
		else if (in[i] == 0x5A55) in[i] = 0xAA55;
		else if (in[i] == 0x55AA) in[i] = 0xAAA5;
		else if (in[i] == 0x55A5) in[i] = 0xA5A5;
		else if (in[i] == 0x555A) in[i] = 0xA55A;
		else if (in[i] == 0x5555) in[i] = 0x5AAA;

		in[i] = ~in[i];
	}

}
void decode(wstring &in)
{
	for (size_t i = 0; i < in.size(); ++i)
	{
		in[i] = ~in[i];



		if (in[i] == 0x0000) in[i] = 0x0F00;
		else if (in[i] == 0x000F) in[i] = 0xFF00;
		else if (in[i] == 0x00F0) in[i] = 0xF00F;
		else if (in[i] == 0x00FF) in[i] = 0xF0FF;
		else if (in[i] == 0x0F00) in[i] = 0x0000;
		else if (in[i] == 0x0F0F) in[i] = 0xFF0F;
		else if (in[i] == 0x0FF0) in[i] = 0xFFF0;
		else if (in[i] == 0x0FFF) in[i] = 0xF0F0;
		else if (in[i] == 0xF000) in[i] = 0xFFFF;
		else if (in[i] == 0xF00F) in[i] = 0x00F0;
		else if (in[i] == 0xF0F0) in[i] = 0x0FFF;
		else if (in[i] == 0xF0FF) in[i] = 0x00FF;
		else if (in[i] == 0xFF00) in[i] = 0x000F;
		else if (in[i] == 0xFF0F) in[i] = 0x0F0F;
		else if (in[i] == 0xFFF0) in[i] = 0x0FF0;
		else if (in[i] == 0xFFFF) in[i] = 0xF000;

		if (in[i] == 0xAAAA) in[i] = 0xA5AA;
		else if (in[i] == 0xAAA5) in[i] = 0x55AA;
		else if (in[i] == 0xAA5A) in[i] = 0x5AA5;
		else if (in[i] == 0xAA55) in[i] = 0x5A55;
		else if (in[i] == 0xA5AA) in[i] = 0xAAAA;
		else if (in[i] == 0xA5A5) in[i] = 0x55A5;
		else if (in[i] == 0xA55A) in[i] = 0x555A;
		else if (in[i] == 0xA555) in[i] = 0x5A5A;
		else if (in[i] == 0x5AAA) in[i] = 0x5555;
		else if (in[i] == 0x5AA5) in[i] = 0xAA5A;
		else if (in[i] == 0x5A5A) in[i] = 0xA555;
		else if (in[i] == 0x5A55) in[i] = 0xAA55;
		else if (in[i] == 0x55AA) in[i] = 0xAAA5;
		else if (in[i] == 0x55A5) in[i] = 0xA5A5;
		else if (in[i] == 0x555A) in[i] = 0xA55A;
		else if (in[i] == 0x5555) in[i] = 0x5AAA;


		int16_t x = in[i] & 0xCCCC;
		int16_t y = in[i] & 0X3333;
		x >>= 2;
		y <<= 2;
		x &= 0x3FFF;
		in[i] = x | y;

		x = in[i] & 0xAAAA;
		y = in[i] & 0x5555;
		x >>= 1;
		y <<= 1;
		x &= 0x7FFF;
		in[i] = x | y;

		in[i] = ~in[i];
	}

}
void encode(std::string &in)
{
	size_t size = in.size();
	for (size_t i = 0; i < size; ++i)
	{
		in[i] = ~in[i];
		unsigned char a = in[i] & 0b10101010;
		unsigned char b = in[i] & 0b01010101;
		a >>= 1;
		b <<= 1;
		a &= 0b01111111;
		in[i] = a | b;


		b = in[i] & 0b00001111;
		a = in[i] & 0b11110000;
		b <<= 4;
		a >>= 4;
		a &= 0b00001111;
		in[i] = a | b;

		a = in[i] & 0b11001100;
		b = in[i] & 0b00110011;
		a >>= 2;
		b <<= 2;
		a &= 0b00111111;
		in[i] = a | b;

		a = in[i] & 0b11110000;
		b = in[i] & 0b00001111;

		if (a == 0x00) a = 0x40;
		else if (a == 0x10) a = 0xC0;
		else if (a == 0x20) a = 0x90;
		else if (a == 0x30) a = 0xB0;
		else if (a == 0x40) a = 0x00;
		else if (a == 0x50) a = 0xD0;
		else if (a == 0x60) a = 0xE0;
		else if (a == 0x70) a = 0xA0;
		else if (a == 0x80) a = 0xF0;
		else if (a == 0x90) a = 0x20;
		else if (a == 0xA0) a = 0x70;
		else if (a == 0xB0) a = 0x30;
		else if (a == 0xC0) a = 0x10;
		else if (a == 0xD0) a = 0x50;
		else if (a == 0xE0) a = 0x60;
		else if (a == 0xF0) a = 0x80;

		if (b == 0x00) b = 0x04;
		else if (b == 0x01) b = 0x0C;
		else if (b == 0x02) b = 0x09;
		else if (b == 0x03) b = 0x0B;
		else if (b == 0x04) b = 0x00;
		else if (b == 0x05) b = 0x0D;
		else if (b == 0x06) b = 0x0E;
		else if (b == 0x07) b = 0x0A;
		else if (b == 0x08) b = 0x0F;
		else if (b == 0x09) b = 0x02;
		else if (b == 0x0A) b = 0x07;
		else if (b == 0x0B) b = 0x03;
		else if (b == 0x0C) b = 0x01;
		else if (b == 0x0D) b = 0x05;
		else if (b == 0x0E) b = 0x06;
		else if (b == 0x0F) b = 0x08;

		in[i] = a | b;
		in[i] = ~in[i];
	}

}
void decode(std::string &in)
{
	size_t size = in.size();
	for (size_t i = 0; i < size; ++i)
	{
		in[i] = ~in[i];
		unsigned char a = in[i] & 0b11110000;
		unsigned char b = in[i] & 0b00001111;

		if (a == 0x00) a = 0x40;
		else if (a == 0x10) a = 0xC0;
		else if (a == 0x20) a = 0x90;
		else if (a == 0x30) a = 0xB0;
		else if (a == 0x40) a = 0x00;
		else if (a == 0x50) a = 0xD0;
		else if (a == 0x60) a = 0xE0;
		else if (a == 0x70) a = 0xA0;
		else if (a == 0x80) a = 0xF0;
		else if (a == 0x90) a = 0x20;
		else if (a == 0xA0) a = 0x70;
		else if (a == 0xB0) a = 0x30;
		else if (a == 0xC0) a = 0x10;
		else if (a == 0xD0) a = 0x50;
		else if (a == 0xE0) a = 0x60;
		else if (a == 0xF0) a = 0x80;

		if (b == 0x00) b = 0x04;
		else if (b == 0x01) b = 0x0C;
		else if (b == 0x02) b = 0x09;
		else if (b == 0x03) b = 0x0B;
		else if (b == 0x04) b = 0x00;
		else if (b == 0x05) b = 0x0D;
		else if (b == 0x06) b = 0x0E;
		else if (b == 0x07) b = 0x0A;
		else if (b == 0x08) b = 0x0F;
		else if (b == 0x09) b = 0x02;
		else if (b == 0x0A) b = 0x07;
		else if (b == 0x0B) b = 0x03;
		else if (b == 0x0C) b = 0x01;
		else if (b == 0x0D) b = 0x05;
		else if (b == 0x0E) b = 0x06;
		else if (b == 0x0F) b = 0x08;


		in[i] = a | b;

		b = in[i] & 0b00110011;
		a = in[i] & 0b11001100;
		b <<= 2;
		a >>= 2;
		a &= 0b00111111;
		in[i] = a | b;

		b = in[i] & 0b00001111;
		a = in[i] & 0b11110000;
		b <<= 4;
		a >>= 4;
		a &= 0b00001111;
		in[i] = a | b;

		b = in[i] & 0b01010101;
		a = in[i] & 0b10101010;
		b <<= 1;
		a >>= 1;
		a &= 0b01111111;
		in[i] = a | b;
		in[i] = ~in[i];

	}

}
bool isRegex(const std::wstring &in, boost::wregex &result_rx)
{
	if (in.size() < 4) return false;

	std::wstring regex;

	if (*in.begin() == L'{' && *(--in.end()) == L'}')
	{
		if (in[1] != L'[') return false;
		if (in[3] == L']') 
		{
			if (wcschr(L"\\/^.$|()[]*+?{},&'`~#:=", in[2])) return false;
		}
		else if (in[2] != L']') return false;

		
		if(in[3] == L']') regex += in.substr(4, in.size() - 5);
		else regex += in.substr(3, in.size() - 4);

		
		for (size_t i = 0; i < regex.size(); ++i)
		{
			if (regex[i] == L'\\') ++i;
			else regex[i] = towlower(regex[i]);
		}

		try
		{
			
			result_rx = boost::wregex(regex.c_str());
		}
		catch (...)
		{
			return false;
		}
		
		return true;
	}

	return false;
}


//===================


void Words::recBinarySearch(wstring & word, size_t left, size_t right)
{
	if (left >= right || words_set.size() == 0) return;


	size_t mid = left + ((right - left) >> 1);
	if (words_set[mid] == word)
	{
		founded_element = words_set.begin() + mid;
		return;
	}
	if (words_set[mid] > word)
	{
		recBinarySearch(word, left, mid);
		return;
	}
	else
	{
		recBinarySearch(word, mid + 1, right);
		return;
	}

}







Words::Words()
{
	
}


Words::~Words()
{
}


const std::vector <wstring> & Words::WordsSet() const
{
	return words_set;
}

std::wstring Words::GetPath() const
{
	return path;
}

const std::set<wstring>& Words::GetUsedWords() const
{
	return used_words;
}

uint64_t Words::getWordsCount() const
{
	return uint64_t(words_set.size());
}

uint64_t Words::getUsedWordsCount() const
{
	return uint64_t(used_words.size());
}

uint64_t Words::getSizeOfTable() const
{
	return uint64_t(words_set.size());
}



void Words::inputStrParser(std::wstring &str)
{
	std::wstring buffer;
	for (size_t i = 0; i < str.size(); ++i)
	{
		if (str[i] != L' ' 
			&& str[i] != L'\n'
			&& str[i] != L'\t'  
			&& str[i] != L'\0')
			buffer.push_back(str[i]);

		if ((str[i] == L' ' 
			|| i == str.size() - 1 
			|| str[i] == L'\n' 
			|| str[i] == L'\t' 
			|| str[i] == L'\0')
			&& !buffer.empty())
		{
			boost::wregex rx;
			if (isRegex(buffer, rx))
			{
				rx_list.push_back(rx);
				std::transform(buffer.begin(), buffer.end(), buffer.data(), towlower);
				rx_letters_list.push_back(buffer[2]);
			}
			else
			{
				std::transform(buffer.begin(), buffer.end(), buffer.data(), towlower);
				list.insert(buffer);
			}
			buffer.clear();
		}
	}

}
void Words::splitWordsList()
{
	added.clear();
	exist.clear();
	
	for (auto & word : list)
	{	
		if (findWord(word)) exist.push_back(word);
		else added.insert(word);
		
	}
	regexProcessing();
	list.clear();
}
void Words::regexProcessing()
{
	setlocale(LC_ALL, "");
	std::wstring word;
	for (size_t x = 0; x < rx_list.size(); ++x)
	{
		if (rx_letters_list[x] != L']')
		{
			
			unsigned idx = 0;
			findLetter(rx_letters_list[x], &idx);
			
				unsigned tmp_idx = idx;


				while (1)
				{
					word = words_set[idx].c_str();
					if (word[0] != rx_letters_list[x] && word[0] != L'\0') break;
					if (boost::regex_match(word, rx_list[x]))
						exist.push_back(words_set[idx].c_str());
					++idx;
					if (idx == words_set.size() - 1) break;
				}
				
				if (tmp_idx == 0) continue;
				else idx = tmp_idx - 1;

				while (1)
				{
					
					word = words_set[idx].c_str();
					if (word[0] != rx_letters_list[x] && word[0] != L'\0') break;
					if (boost::regex_match(word, rx_list[x]))
						exist.push_back(words_set[idx].c_str());
					if (idx == 0) break;
					--idx;
					
				}
			
		}
		else
		{
			for (size_t i = 0; i < words_set.size(); ++i)
			{
				word = words_set[i].c_str();
				if (boost::regex_match(word, rx_list[x]))
					exist.push_back(words_set[i].c_str());

			}

		}
	}

	rx_list.clear();
	rx_letters_list.clear();
}

std::wstring& Words::createResultText()
{
	static std::wstring result;
	result.clear();
	result += L"Будут добавлены:\n";

	for (auto & element : added)
	{
		result += L"   " + element + L"\n";
	}

	result += L"\n\nУже есть:\n";
	for (auto & element : exist)
	{
		result += L"   " + element + L"\n";
	}

	return result;
}

std::wstring& Words::createTextOnlyExist()
{
	static std::wstring result;
	result.clear();
	result += L"Успешно удалены:\n";
	for (auto & element : exist)
	{
		result += L"   " + element + L"\n";
	}

	return result;
}

std::wstring& Words::createTextOnlyAdded()
{
	static std::wstring result;
	result.clear();
	result += L"Успешно добавлены:\n";
	for (auto & element : added)
	{
		result += L"   " + element + L"\n";
	}
	return result;
}






//data operations

void Words::setPathAndLoadData(std::wstring new_path)
{
	unloadData();
	path = new_path;
	loadData();
}

void Words::setPath(std::wstring new_path)
{
	path = new_path;
}

void Words::loadData() 
{
	std::wifstream input;
	
	input.imbue(std::locale("ru_RU.utf8"));
	
	input.open(path);
	if (input.is_open()) 
	{
		while(true)
		{
			wstring buffer;
			
			input >> buffer;
			if (input.eof()) break;
			decode(buffer);
			
			words_set.push_back(buffer);
			
			
		} 	
	}
	input.close();
}

void Words::unloadData()
{
	if (!words_set.empty())
	{
		words_set.clear();

		
	}
}

void Words::saveData()
{

	if (!words_set.empty())
	{
		std::wofstream out;
		out.imbue(std::locale("ru_RU.utf8"));
		out.open(path, std::ios::trunc);
		
		
		for (auto element : words_set)
		{
			if (!element.empty())
			{
				encode(element);
				out << element << std::endl;
			}
		}
			
		



		out.close();
	}
}


//words
void Words::clearUsedWordsList()
{
	used_words.clear();

}



void Words::getLastChar(const std::wstring &word, wchar_t &next_letter)
{
	size_t length = word.size();

	for (size_t i = length - 1; i >= 0; i--) {
		if (word[i] != L'ы'
			&& word[i] != L'ь'
			&& word[i] != L'ъ') {
			next_letter = word[i];
			break;
		}

	}
}

bool Words::findLetter(wchar_t letter, unsigned * idx)
{
	size_t mid, 
		right = words_set.size(),
		left = 0;
	mid = left + ((right - left) >> 1);
	

	while (1)
	{
		
		
		if (left >= right || words_set.size() == 0) break;
		if (words_set[mid][0] == letter)
		{
			*idx = mid;
			return true;
		}
		if (words_set[mid][0] > letter)
		{
			right = mid;
			mid = left + ((right - left) >> 1);
		}
		if (words_set[mid][0] < letter)
		{
			left = mid + 1;
			mid = left + ((right - left) >> 1);
		}
	}
	return false;
}

bool Words::findLetter(wchar_t letter)
{
	size_t mid,
		right = words_set.size(),
		left = 0;
	mid = left + ((right - left) >> 1);


	while (1)
	{


		if (left >= right || words_set.size() == 0) break;
		if (words_set[mid][0] == letter)
		{
			return true;
		}
		if (words_set[mid][0] > letter)
		{
			right = mid;
			mid = left + ((right - left) >> 1);
		}
		if (words_set[mid][0] < letter)
		{
			left = mid + 1;
			mid = left + ((right - left) >> 1);
		}
	}
	return false;
}

bool Words::findWord(const std::wstring &word) 
{
	
	wstring w(word);
	founded_element = words_set.end();
	recBinarySearch(w, 0, words_set.size());
	if(founded_element == words_set.end()) return false;
	return true;
}


void Words::addWords()
{
	
	
	unsigned idx = 0;
	if (added.size() > 0)
	{
		for (auto it = added.begin(); it != added.end(); ++it)
		{
			wstring s(it->c_str());
			
			
			
			if (words_set.size() - 1 == idx || words_set.size() == 0)
			{
				words_set.push_back(it->c_str());
				continue;
			}

			while (s[0] < words_set[idx][0] 
				&& idx < words_set.size() - 1 
				&& s[1] < words_set[idx][1]) ++idx;

			
			
			
			if (s <= words_set[0])
			{
				words_set.insert(words_set.begin(), it->c_str());
				continue;
			}
			while (1)
			{
				
				if (idx == words_set.size() - 1)
				{
					words_set.push_back(s);
					break;
				}
				
				if ((s <= words_set[idx + 1] && s >= words_set[idx]))
				{
					words_set.insert(words_set.begin() + idx, s);
					break;
				}
				++idx;
				

			}
			
			
		}
		
	}
}

void Words::delWords()
{
	
	if (exist.size() > 0)
	{
		for (auto it = exist.begin(); it != exist.end(); ++it)
		{
			std::wstring target = *it;

			if (findWord(target))
			{
				words_set.erase(founded_element);
				
			}
		}
	}
}

void Words::useThisWord()
{
	
	used_words.insert(*founded_element);
	words_set.erase(founded_element);
	
}

void Words::useThisWord(const std::wstring & word)
{
	if (findWord(word)) useThisWord();
}


