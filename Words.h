#pragma once
#include "pch.h"
#include <regex>

void encode(std::string& in);
void decode(std::string& out);


class Words final 
{
	

private:
	

	std::string            path;
	std::set <std::string> used_words;
	parsed_list_cp1251		 list;
	existing_words_list_utf8 exist;
	added_words_list_utf8    added;
	added_words_list_utf8    total_added;
	std::vector<std::regex>  rx_list;
	std::vector<char>		 rx_letters_list;
			
	//codepages
	void		cp1251_to_utf8			(char *out, const char *in);
	std::string cp1251_to_utf8			(std::string &input);
	std::string convert_1251char_to_utf8(char c);
	std::string utf8_to_string			(const char *utf8str, const std::locale& loc);
	//===================
	void        sortedInsert			(unsigned idx, std::string &word);
	void		recBinarySearch         (unsigned idx, std::string &word, size_t left, size_t right);

	uint64_t words_counter = 0;
	uint64_t counter_of_used_words = 0;
	size_t size = 32 * 32 * 32;
	bool full_save = false;
	
	set words_set;
	set_iterator founded_element;

public:

	
	const set& WordsSet() const;
	

	
	
	std::string GetPath() const;
	const std::set <std::string>& GetUsedWords() const;

	uint64_t getWordsCount() const;

	uint64_t getUsedWordsCount() const;

	size_t getSizeOfTable() const;

	bool isFullSave() const;
	void setFullSaveFlag(bool flag);


	void inputStrParser(std::string &str);
	void splitWordsList();
	void regexProcessing();
	std::string createResultText   ();
	std::string createTextOnlyExist();
	std::string createTextOnlyAdded();
	

	//construct
	Words();


	//hash f(x)
	unsigned hashFunction(char a, char b, char c);
	unsigned hashFunction(char a, char b);
	unsigned hashFunction(char a);
	unsigned safeHashF(std::string &str);
	//data
	
	void setPathAndLoadData(std::string new_path);
	void setPath           (std::string new_path);
	void loadData          ();
	void unloadData        ();
	void saveData          ();
	void updateDataInFile  ();

	//words
	bool findWord          (std::string &word);
	void addWords          ();
	void delWords          ();
	void useThisWord       ();
	void useThisWord       (std::string &word);
	void clearUsedWordsList();
	void getLastChar       (std::string &word,
		                    char        &next_letter);
	
	~Words();
};

