#pragma once
#include <string.h>
#include <string>
#include <iostream>
#include <fstream>
class wstring
{
private:
	wchar_t *m_str = nullptr;
	size_t m_size = 0;
	
public:
	static std::wstring m_input_buffer;
	wstring();
	wstring(const wchar_t* str);
	wstring(const std::wstring &str);
	wstring(const wstring &str);

	const wchar_t* c_str() const;
	const wchar_t* data() const;
	size_t size() const;
	bool empty() const;
	wchar_t& at(size_t idx);
	wchar_t& at(int idx);
	wstring& operator=(const wstring& str);
	wstring& operator=(const std::wstring& str);

	friend bool operator >  (const wstring& l, const wstring& r);
	friend bool operator <  (const wstring& l, const wstring& r);
	friend bool operator >= (const wstring& l, const wstring& r);
	friend bool operator <= (const wstring& l, const wstring& r);
	friend bool operator == (const wstring& l, const wstring& r);
	friend bool operator != (const wstring& l, const wstring& r);

	wchar_t& operator[](size_t idx);
	wchar_t& operator[](int idx);
	wchar_t operator[](size_t idx) const;
	wchar_t operator[](int idx) const;

	friend std::wifstream& operator >> (std::wifstream& in, wstring& str);
	friend std::wofstream& operator << (std::wofstream& out, wstring& str);

	friend std::wistream& operator >> (std::wistream& in, wstring& str);
	friend std::wostream& operator << (std::wostream& out, wstring& str);

	~wstring();
};

