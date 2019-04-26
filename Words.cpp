#include "pch.h"
#include "Words.h"

#include <regex>




//encoding and decoding
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
bool isRegex(const std::string &in, std::regex &result_rx)
{
	if (in.size() < 4) return false;

	std::string regex;

	if (*in.begin() == '{' && *(--in.end()) == '}')
	{
		if (in[1] != '[') return false;
		if (in[3] == ']') 
		{
			if (strchr("\\/^.$|()[]*+?{},&'`~#:=", in[2])) return false;
		}
		else if (in[2] != ']') return false;

		//regex += in[2];
		if(in[3] == ']') regex += in.substr(4, in.size() - 5);
		else regex += in.substr(3, in.size() - 4);

		for (auto & ch : regex)
		{
			switch (ch)
			{
			case 'А': ch = 'а';
				break;
			case 'Б': ch = 'б';
				break;
			case 'В': ch = 'в';
				break;
			case 'Г': ch = 'г';
				break;
			case 'Д': ch = 'д';
				break;
			case 'Е': ch = 'е';
				break;
			case 'Ё': ch = 'ё';
				break;
			case 'Ж': ch = 'ж';
				break;
			case 'З': ch = 'з';
				break;
			case 'И': ch = 'и';
				break;
			case 'Й': ch = 'й';
				break;
			case 'К': ch = 'к';
				break;
			case 'Л': ch = 'л';
				break;
			case 'М': ch = 'м';
				break;
			case 'Н': ch = 'н';
				break;
			case 'О': ch = 'о';
				break;
			case 'П': ch = 'п';
				break;
			case 'Р': ch = 'р';
				break;
			case 'С': ch = 'с';
				break;
			case 'Т': ch = 'т';
				break;
			case 'У': ch = 'у';
				break;
			case 'Ф': ch = 'ф';
				break;
			case 'Х': ch = 'х';
				break;
			case 'Ц': ch = 'ц';
				break;
			case 'Ч': ch = 'ч';
				break;
			case 'Ш': ch = 'ш';
				break;
			case 'Щ': ch = 'щ';
				break;
			case 'Ь': ch = 'ь';
				break;
			case 'Ъ': ch = 'ъ';
				break;
			case 'Э': ch = 'э';
				break;
			case 'Ю': ch = 'ю';
				break;
			case 'Я': ch = 'я';
				break;
			case 'Ы': ch = 'ы';
				break;
			}

		}

		try
		{
			
			result_rx = std::regex(regex.c_str(), std::regex_constants::icase);
		}
		catch (...)
		{
			return false;
		}
		
		return true;
	}

	return false;
}

//codepages
void		Words::cp1251_to_utf8		   (char *out, const char *in) {
	static const int table[128] = {
		0x82D0,0x83D0,0x9A80E2,0x93D1,0x9E80E2,0xA680E2,0xA080E2,0xA180E2,
		0xAC82E2,0xB080E2,0x89D0,0xB980E2,0x8AD0,0x8CD0,0x8BD0,0x8FD0,
		0x92D1,0x9880E2,0x9980E2,0x9C80E2,0x9D80E2,0xA280E2,0x9380E2,0x9480E2,
		0,0xA284E2,0x99D1,0xBA80E2,0x9AD1,0x9CD1,0x9BD1,0x9FD1,
		0xA0C2,0x8ED0,0x9ED1,0x88D0,0xA4C2,0x90D2,0xA6C2,0xA7C2,
		0x81D0,0xA9C2,0x84D0,0xABC2,0xACC2,0xADC2,0xAEC2,0x87D0,
		0xB0C2,0xB1C2,0x86D0,0x96D1,0x91D2,0xB5C2,0xB6C2,0xB7C2,
		0x91D1,0x9684E2,0x94D1,0xBBC2,0x98D1,0x85D0,0x95D1,0x97D1,
		0x90D0,0x91D0,0x92D0,0x93D0,0x94D0,0x95D0,0x96D0,0x97D0,
		0x98D0,0x99D0,0x9AD0,0x9BD0,0x9CD0,0x9DD0,0x9ED0,0x9FD0,
		0xA0D0,0xA1D0,0xA2D0,0xA3D0,0xA4D0,0xA5D0,0xA6D0,0xA7D0,
		0xA8D0,0xA9D0,0xAAD0,0xABD0,0xACD0,0xADD0,0xAED0,0xAFD0,
		0xB0D0,0xB1D0,0xB2D0,0xB3D0,0xB4D0,0xB5D0,0xB6D0,0xB7D0,
		0xB8D0,0xB9D0,0xBAD0,0xBBD0,0xBCD0,0xBDD0,0xBED0,0xBFD0,
		0x80D1,0x81D1,0x82D1,0x83D1,0x84D1,0x85D1,0x86D1,0x87D1,
		0x88D1,0x89D1,0x8AD1,0x8BD1,0x8CD1,0x8DD1,0x8ED1,0x8FD1
	};
	while (*in)
		if (*in & 0x80) {
			int v = table[(int)(0x7f & *in++)];
			if (!v)
				continue;
			*out++ = (char)v;
			*out++ = (char)(v >> 8);
			if (v >>= 16)
				*out++ = (char)v;
		}
		else
			*out++ = *in++;
	*out = 0;
}
std::string Words::cp1251_to_utf8		   (std::string &input)
{
	static const int table[128] = {
	   0x82D0,0x83D0,0x9A80E2,0x93D1,0x9E80E2,0xA680E2,0xA080E2,0xA180E2,
	   0xAC82E2,0xB080E2,0x89D0,0xB980E2,0x8AD0,0x8CD0,0x8BD0,0x8FD0,
	   0x92D1,0x9880E2,0x9980E2,0x9C80E2,0x9D80E2,0xA280E2,0x9380E2,0x9480E2,
	   0,0xA284E2,0x99D1,0xBA80E2,0x9AD1,0x9CD1,0x9BD1,0x9FD1,
	   0xA0C2,0x8ED0,0x9ED1,0x88D0,0xA4C2,0x90D2,0xA6C2,0xA7C2,
	   0x81D0,0xA9C2,0x84D0,0xABC2,0xACC2,0xADC2,0xAEC2,0x87D0,
	   0xB0C2,0xB1C2,0x86D0,0x96D1,0x91D2,0xB5C2,0xB6C2,0xB7C2,
	   0x91D1,0x9684E2,0x94D1,0xBBC2,0x98D1,0x85D0,0x95D1,0x97D1,
	   0x90D0,0x91D0,0x92D0,0x93D0,0x94D0,0x95D0,0x96D0,0x97D0,
	   0x98D0,0x99D0,0x9AD0,0x9BD0,0x9CD0,0x9DD0,0x9ED0,0x9FD0,
	   0xA0D0,0xA1D0,0xA2D0,0xA3D0,0xA4D0,0xA5D0,0xA6D0,0xA7D0,
	   0xA8D0,0xA9D0,0xAAD0,0xABD0,0xACD0,0xADD0,0xAED0,0xAFD0,
	   0xB0D0,0xB1D0,0xB2D0,0xB3D0,0xB4D0,0xB5D0,0xB6D0,0xB7D0,
	   0xB8D0,0xB9D0,0xBAD0,0xBBD0,0xBCD0,0xBDD0,0xBED0,0xBFD0,
	   0x80D1,0x81D1,0x82D1,0x83D1,0x84D1,0x85D1,0x86D1,0x87D1,
	   0x88D1,0x89D1,0x8AD1,0x8BD1,0x8CD1,0x8DD1,0x8ED1,0x8FD1
	};
	const char *in = input.c_str();
	char *out = new char[input.size() * 4];
	char *out_old = out;
	ZeroMemory(out, input.size() * 4);
	while (*in)
		if (*in & 0x80) {
			int v = table[(int)(0x7f & *in++)];
			if (!v)
				continue;
			*out++ = (char)v;
			*out++ = (char)(v >> 8);
			if (v >>= 16)
				*out++ = (char)v;
		}
		else
			*out++ = *in++;

	std::string result = out_old;
	delete[] out_old;

	return result;
}
std::string Words::convert_1251char_to_utf8(char c)
{
	char out[4] = { '\0' };
	char in[2] = { '\0' };
	in[0] = c;
	cp1251_to_utf8(out, in);
	std::string result = out;

	return result;
}
std::string Words::utf8_to_string		   (const char *utf8str, const std::locale& loc)
{
	// UTF-8 to wstring
	std::wstring_convert<std::codecvt_utf8<wchar_t>> wconv;
	
	std::wstring wstr = wconv.from_bytes(utf8str);
	// wstring to string
	std::vector<char> buf(wstr.size());
	std::use_facet<std::ctype<wchar_t>>(loc).narrow(wstr.data(), wstr.data() + wstr.size(), '?', buf.data());
	return std::string(buf.data(), buf.size());
}



//===================

void Words::sortedInsert(unsigned idx, std::string &word)
{
	if (words_set[idx].size() == 0)
	{
		words_set[idx].push_back(word);
		return;
	}
	if (words_set[idx].size() == 1)
	{
		if (*(words_set[idx].begin()) >= word)
		{
			words_set[idx].insert(words_set[idx].begin(), word);
			return;
		}
		else
		{
			words_set[idx].push_back(word);
			return;
		}
	}
	size_t size = words_set[idx].size();
	for (size_t i = 0; i < size; ++i)
	{
		if (i == 0 && words_set[idx][i] >= word)
		{
			words_set[idx].insert(words_set[idx].begin(), word);
			return;
		}
		if (i == size - 1)
		{
			words_set[idx].push_back(word);
			return;
		}
		if (words_set[idx][i] <= word && words_set[idx][i + 1] >= word)
		{
			words_set[idx].insert(words_set[idx].begin() + i + 1, word);
			return;
		}

	}

}

void Words::recBinarySearch(unsigned idx, std::string & word, size_t left, size_t right)
{
	if (left >= right || words_set[idx].size() == 0) return;


	size_t mid = left + ((right - left) >> 1);
	if (words_set[idx][mid] == word)
	{
		founded_element = words_set[idx].begin() + mid;
		return;
	}
	if (words_set[idx][mid] > word)
	{
		recBinarySearch(idx, word, left, mid);
		return;
	}
	else
	{
		recBinarySearch(idx, word, mid + 1, right);
		return;
	}

}







Words::Words()
{
}


Words::~Words()
{
}


const set & Words::WordsSet() const
{
	return words_set;
}

std::string Words::GetPath() const
{
	return path;
}

const std::set<std::string>& Words::GetUsedWords() const
{
	return used_words;
}

uint64_t Words::getWordsCount() const
{
	return words_counter;
}

uint64_t Words::getUsedWordsCount() const
{
	return counter_of_used_words;
}

size_t Words::getSizeOfTable() const
{
	return size;
}

bool Words::isFullSave() const
{
	return full_save;
}

void Words::setFullSaveFlag(bool flag)
{
	full_save = flag;
}

void Words::inputStrParser(std::string &str)
{
	std::string ansi_str = utf8_to_string(str.c_str(), std::locale(".1251"));
	//std::transform(ansi_str.begin(), ansi_str.end(), ansi_str.data(), tolower);
	

	std::string buffer;
	for (size_t i = 0; i < ansi_str.size(); ++i)
	{
		if (ansi_str[i] != ' ' && ansi_str[i] != '\n' && ansi_str[i] != '\t'  && ansi_str[i] != '\0')
			buffer.push_back(ansi_str[i]);

		if ((ansi_str[i] == ' ' || i == ansi_str.size() - 1 || ansi_str[i] == '\n' || ansi_str[i] == '\t' || ansi_str[i] == '\0')
			&& !buffer.empty())
		{
			std::regex rx;
			if (isRegex(buffer, rx))
			{
				rx_list.push_back(rx);
				std::transform(buffer.begin(), buffer.end(), buffer.data(), tolower);
				rx_letters_list.push_back(buffer[2]);
			}
			else
			{
				std::transform(buffer.begin(), buffer.end(), buffer.data(), tolower);
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
	std::string utf8_str;
	for (auto & word : list)
	{
		utf8_str = cp1251_to_utf8(const_cast<std::string&>(word));

		
		if (findWord(utf8_str)) exist.insert(utf8_str);
		else added.insert(utf8_str);
		
	}
	regexProcessing();
	list.clear();
}
void Words::regexProcessing()
{
	for (size_t x = 0; x < rx_list.size(); ++x)
	{
		if (rx_letters_list[x] != ']')
		{
			std::string utf8_letter = convert_1251char_to_utf8(rx_letters_list[x]);
			unsigned idx = safeHashF(utf8_letter);
			std::string word1251;


			while (words_set[idx].size() == 0 && idx < this->size) ++idx;


			while (1)
			{
				while (words_set[idx].size() == 0 && idx < this->size) ++idx;
				if (idx == this->size) break;

				word1251 = utf8_to_string(words_set[idx][0].c_str(), std::locale(".1251"));
				if (word1251[0] != rx_letters_list[x]) break;

				
				
				if (std::regex_match(word1251, rx_list[x]))
					exist.insert(words_set[idx][0]);

				size_t size = words_set[idx].size();
				for (size_t i = 1; i < size; ++i)
				{
					
					word1251 = utf8_to_string(words_set[idx][i].c_str(), std::locale(".1251"));
					
					if (std::regex_match(word1251, rx_list[x]))
						exist.insert(words_set[idx][i]);

				}

				++idx;
			}
		}
		else
		{
			for (size_t i = 0; i < this->size; ++i)
			{
				if (words_set[i].size() == 0) continue;

				size_t size = words_set[i].size();
				for (size_t k = 0; k < size; ++k)
				{
					std::string word1251 = utf8_to_string(words_set[i][k].c_str(), std::locale(".1251"));

					
					if (std::regex_match(word1251, rx_list[x]))
						exist.insert(words_set[i][k]);

				}
			}

		}
	}

	rx_list.clear();
	rx_letters_list.clear();
}

std::string Words::createResultText()
{
	std::string result;
	result += u8"Будут добавлены:\n";

	for (auto & element : added)
	{
		result += u8"   " + element + u8"\n";
	}

	result += u8"\n\nУже есть:\n";
	for (auto & element : exist)
	{
		result += u8"   " + element + u8"\n";
	}

	return result;
}

std::string Words::createTextOnlyExist()
{
	std::string result;
	result += u8"Успешно удалены:\n";
	for (auto & element : exist)
	{
		result += u8"   " + element + u8"\n";
	}

	return result;
}

std::string Words::createTextOnlyAdded()
{
	std::string result;
	result += u8"Успешно добавлены:\n";
	for (auto & element : added)
	{
		result += u8"   " + element + u8"\n";
	}
	return result;
}

//hash f(x)
unsigned Words::hashFunction(char a, char b, char c) 
{
	return(((static_cast<unsigned>(a) % 32) << 10) + ((static_cast<unsigned>(b) % 32) << 5) + (static_cast<unsigned>(c) % 32));
}
unsigned Words::hashFunction(char a, char b)
{
	return (((static_cast<unsigned>(a) % 32) << 10) + ((static_cast<unsigned>(b) % 32) << 5));
}
unsigned Words::hashFunction(char a)
{
	return ((static_cast<unsigned>(a) % 32) << 10);
}
unsigned Words::safeHashF(std::string &str)
{
	if (str.size() == 1) return hashFunction(str[0]);
	if (str.size() == 2) return hashFunction(str[0], str[1]);
	if (str.size() >= 3) return hashFunction(str[0], str[1], str[2]);
	return 0;
}





//data operations

void Words::setPathAndLoadData(std::string new_path)
{
	unloadData();
	path = new_path;
	loadData();
}

void Words::setPath(std::string new_path)
{
	path = new_path;
}

void Words::loadData() 
{
	words_set.resize(size);


	std::ifstream input;
	std::string buffer;
	size_t buf_size = buffer.size();
	unsigned idx;

	
	input.open(path);
	if (input.is_open()) 
	{
		while(true)
		{
			
			std::getline(input, buffer, '\n');
			if (input.eof()) break;
			decode(buffer);
			idx = safeHashF(buffer);
			sortedInsert(idx, buffer);
			words_counter++;
		} 	
	}
	
	input.close();



}

void Words::unloadData()
{
	if (!words_set.empty())
	{
		for (size_t i = 0; i < size; ++i)
			if (!words_set[i].empty()) 
				words_set[i].clear();

		words_set.clear();

		words_counter = 0;
	}
}

void Words::saveData()
{

	if (!words_set.empty())
	{
		std::ofstream out;
		out.open(path, std::ios::trunc);
		
		for (size_t i = 0; i < size; i++)
		{
			for (auto element : words_set[i])
			{
				if (element != "")
				{
					encode(element);
					out << element.c_str() << std::endl;
				}
			}
			
		}



		out.close();
	}
}

void Words::updateDataInFile()
{

	if (!total_added.empty())
	{
		std::ofstream out;
		out.open(path, std::ios::app);


		for (auto element : total_added)
		{
			encode(element);
			out << element.c_str() << std::endl;
		}
		out.close();
	}
	total_added.clear();

}

//words
void Words::clearUsedWordsList()
{
	words_counter += used_words.size();
	for (size_t i = 0; i < used_words.size(); i) 
	{
		std::string str = *used_words.begin();
		unsigned idx = safeHashF(str);
		words_set[idx].push_back(*used_words.begin());
		used_words.erase(used_words.begin());
	}
	counter_of_used_words = 0;

}



void Words::getLastChar(std::string &word, char &next_letter)
{
	size_t length = word.size();

	for (size_t i = length - 1; i >= 0; i--) {
		if (word[i] != 'ы'
			&& word[i] != 'ь'
			&& word[i] != 'ъ') {
			next_letter = word[i];
			break;
		}

	}
}

bool Words::findWord(std::string &word) 
{
	
	unsigned idx = safeHashF(word);
	founded_element = words_set[idx].end();
	/*for (auto it = words_set[idx].begin(); it != words_set[idx].end(); ++it)
	{
		if (*it == word)
		{
			founded_element = it;
			break;
		}
	}*/
	recBinarySearch(idx, word, 0, words_set[idx].size());
	if(founded_element == words_set[idx].end()) return false;
	return true;
}


void Words::addWords()
{
	unsigned idx;
	if (added.size() > 0)
	{
		for (auto it = added.begin(); it != added.end(); ++it)
		{
			total_added.insert(*it);
			std::string str = *it;
			idx = safeHashF(str);
			sortedInsert(idx, str);
			++words_counter;

		}
	}
}

void Words::delWords()
{
	unsigned idx;
	if (exist.size() > 0)
	{
		for (auto it = exist.begin(); it != exist.end(); ++it)
		{
			std::string str = *it;
			findWord(str);
			idx = safeHashF(str);
			words_set[idx].erase(founded_element);
			--words_counter;
		}
	}
}

void Words::useThisWord()
{
	--words_counter;
	used_words.insert(*founded_element);
	unsigned idx = safeHashF(*founded_element);
	words_set[idx].erase(founded_element);
	++counter_of_used_words;
}

void Words::useThisWord(std::string & word)
{
	if (findWord(word)) useThisWord();
}
