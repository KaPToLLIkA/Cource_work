#include "wstring.h"
#include "pch.h"


wstring::wstring()
{
}

wstring::wstring(const wchar_t * str)
{
	m_size = wcslen(str);
	m_str = new wchar_t[m_size + 1];
	std::memcpy(m_str, str, sizeof(wchar_t) * m_size);
	m_str[m_size] = L'\0';
}

wstring::wstring(const std::wstring &str)
{
	m_size = str.size();
	m_str = new wchar_t[m_size + 1];
	std::memcpy(m_str, str.c_str(), sizeof(wchar_t) * (m_size + 1));
	
}

wstring::wstring(const wstring &str)
{
	m_size = str.m_size;
	m_str = new wchar_t[m_size + 1];
	std::memcpy(m_str, str.m_str, sizeof(wchar_t) * m_size);
	m_str[m_size] = L'\0';
}

const wchar_t * wstring::c_str() const
{
	return const_cast<const wchar_t*>(m_str);
}

const wchar_t * wstring::data() const
{
	return const_cast<const wchar_t*>(m_str);
}

size_t wstring::size() const
{
	return m_size;
}

bool wstring::empty() const
{
	return (m_size == 0) ? true : false;
}

wchar_t & wstring::at(size_t idx)
{
	if (idx > m_size - 1) return m_str[0];
	else return m_str[idx];
}

wchar_t & wstring::at(int idx)
{
	if (idx > m_size - 1 && idx < 0) return m_str[0];
	else return m_str[idx];
}

wstring & wstring::operator=(const wstring & str)
{
	if (this == &str) return *this;

	delete[] m_str;
	m_size = str.m_size;

	m_str = new wchar_t[m_size + 1];
	std::memcpy(m_str, str.m_str, sizeof(wchar_t) * (m_size + 1));
	return *this;
}


wstring & wstring::operator=(const std::wstring& str)
{
	
	delete[] m_str;
	m_size = str.size();
	m_str = new wchar_t[m_size + 1];
	std::memcpy(m_str, str.c_str(), sizeof(wchar_t) * (m_size + 1));
	return *this;
}

wchar_t & wstring::operator[](size_t idx)
{
	return m_str[idx];
}

wchar_t & wstring::operator[](int idx)
{
	return m_str[idx];
}

wchar_t wstring::operator[](size_t idx) const
{
	return m_str[idx];
}

wchar_t wstring::operator[](int idx) const
{
	return m_str[idx];
}

wstring::~wstring()
{
	delete[] m_str;
}

bool operator>(const wstring & l, const wstring & r)
{
	
	size_t max_iter = (l.m_size > r.m_size) ? r.m_size : l.m_size;
	for (size_t i = 0; i < max_iter; ++i)
		if (l.m_str[i] > r.m_str[i]) return true;
		else if(l.m_str[i] == r.m_str[i]) continue;
		else return false;

	if (l.m_size > r.m_size) return true;
	else return false;

}

bool operator<(const wstring & l, const wstring & r)
{
	size_t max_iter = (l.m_size > r.m_size) ? r.m_size : l.m_size;
	for (size_t i = 0; i < max_iter; ++i)
		if (l.m_str[i] < r.m_str[i]) return true;
		else if (l.m_str[i] == r.m_str[i]) continue;
		else return false;

	if (l.m_size < r.m_size) return true;
	else return false;
}

bool operator>=(const wstring & l, const wstring & r)
{
	size_t max_iter = (l.m_size > r.m_size) ? r.m_size : l.m_size;
	for (size_t i = 0; i < max_iter; ++i)
		if (l.m_str[i] > r.m_str[i]) return true;
		else if (l.m_str[i] == r.m_str[i]) continue;
		else return false;

	if (l.m_size >= r.m_size) return true;
	else return false;
}

bool operator<=(const wstring & l, const wstring & r)
{
	size_t max_iter = (l.m_size > r.m_size) ? r.m_size : l.m_size;
	for (size_t i = 0; i < max_iter; ++i)
		if (l.m_str[i] < r.m_str[i]) return true;
		else if (l.m_str[i] == r.m_str[i]) continue;
		else return false;

	if (l.m_size <= r.m_size) return true;
	else return false;
}

bool operator==(const wstring & l, const wstring & r)
{
	if (l.m_size != r.m_size) return false;
	
	
	for (size_t i = 0; i < l.m_size; ++i)
		if (l.m_str[i] != r.m_str[i]) return false;

	return true;
}

bool operator!=(const wstring & l, const wstring & r)
{
	if (l.m_size != r.m_size) return true;


	for (size_t i = 0; i < l.m_size; ++i)
		if (l.m_str[i] != r.m_str[i]) return true;

	return false;
}

std::wifstream & operator>>(std::wifstream & in, wstring & str)
{
	wstring::m_input_buffer.clear();
	in >> str.m_input_buffer;


	str.m_size = wstring::m_input_buffer.size();

	delete[] str.m_str;
	str.m_str = new wchar_t[str.m_size + 1];

	std::memcpy(str.m_str, wstring::m_input_buffer.c_str(), sizeof(wchar_t) * (str.m_size + 1));
	return in;
}

std::wofstream & operator<<(std::wofstream & out, wstring & str)
{
	out << str.m_str;
	return out;
}

std::wistream & operator>>(std::wistream & in, wstring & str)
{
	wstring::m_input_buffer.clear();
	in >> str.m_input_buffer;


	str.m_size = wstring::m_input_buffer.size();

	delete[] str.m_str;
	str.m_str = new wchar_t[str.m_size + 1];

	std::memcpy(str.m_str, wstring::m_input_buffer.c_str(), sizeof(wchar_t) * (str.m_size + 1));
	return in;
}

std::wostream & operator<<(std::wostream & out, wstring & str)
{
	out << str.m_str;
	return out;
}

