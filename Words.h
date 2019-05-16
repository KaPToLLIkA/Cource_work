#pragma once
#include "pch.h"
#include "wstring.h"
#include <boost/regex.hpp>


void encode(std::wstring& in);
void decode(std::wstring& in);
void encode(wstring& in);
void decode(wstring& in);
void encode(std::string& in);
void decode(std::string& in);

class Words final 
{
	

private:
	

	std::wstring             path;
	std::set <wstring>		 used_words;
	parsed_list				 list;
	existing_words_list		 exist;
	added_words_list		 added;
	
	std::vector<boost::wregex> rx_list;
	std::vector<wchar_t>	 rx_letters_list;
			
	void		recBinarySearch         (wstring &word, size_t left, size_t right);


	
	
	std::vector <wstring> words_set;
	std::vector <wstring>::iterator founded_element;

public:

	
	const std::vector <wstring>& WordsSet() const;
	

	
	
	std::wstring GetPath() const;
	const std::set <wstring>& GetUsedWords() const;

	uint64_t getWordsCount() const;

	uint64_t getUsedWordsCount() const;

	uint64_t getSizeOfTable() const;

	


	void inputStrParser(std::wstring &str);
	void splitWordsList();
	void regexProcessing();
	std::wstring& createResultText   ();
	std::wstring& createTextOnlyExist();
	std::wstring& createTextOnlyAdded();
	

	//construct
	Words();

	//data
	
	void setPathAndLoadData(std::wstring new_path);
	void setPath           (std::wstring new_path);
	void loadData          ();
	void unloadData        ();
	void saveData          ();
	
	

	//words
	bool findWord          (const std::wstring &word);
	void addWords          ();
	void delWords          ();
	void useThisWord       ();
	void useThisWord       (const std::wstring &word);
	void clearUsedWordsList();
	void getLastChar       (const std::wstring &word,
							wchar_t       &next_letter);
	
	bool findLetter(wchar_t letter, unsigned *idx);
	bool findLetter(wchar_t letter);
	~Words();
};

