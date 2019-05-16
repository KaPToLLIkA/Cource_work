#include "pch.h"
#include "utils.h"



//utf8 encoding and decoding
std::string utf8_to_string(const char *utf8str, const std::locale& loc)
{
	// UTF-8 to wwstring
	std::wstring_convert<std::codecvt_utf8<wchar_t>> wconv;
	std::wstring wstr;
	try
	{
		wstr = wconv.from_bytes(utf8str);
	}
	catch (...)
	{

	}
	// wwstring to wstring
	std::vector<char> buf(wstr.size());
	std::use_facet<std::ctype<wchar_t>>(loc).narrow(wstr.data(), wstr.data() + wstr.size(), '?', buf.data());
	return std::string(buf.data(), buf.size());
}

std::string utf8_to_string(const wchar_t * utf8str, const std::locale & loc)
{
	std::wstring wstr = utf8str;
	// wwstring to wstring
	std::vector<char> buf(wstr.size());
	std::use_facet<std::ctype<wchar_t>>(loc).narrow(wstr.data(), wstr.data() + wstr.size(), '?', buf.data());
	return std::string(buf.data(), buf.size());
}
std::wstring utf8_to_wstring(const char* utf8str)
{
	std::wstring_convert<std::codecvt_utf8<wchar_t>> wconv;
	std::wstring wstr = wconv.from_bytes(utf8str);
	return wstr;
}
std::string wstring_to_utf8(const wchar_t* wstr)
{
	std::wstring_convert<std::codecvt_utf8<wchar_t>> wconv;
	std::string str = wconv.to_bytes(wstr);
	return str;
}
std::string wchar_to_utf8(wchar_t wstr)
{
	std::wstring_convert<std::codecvt_utf8<wchar_t>> wconv;
	std::string str = wconv.to_bytes(wstr);
	return str;
}
std::string cp1251_to_utf8(std::string &input)
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
std::string utf8_to_string(const std::wstring& wstr, const std::locale& loc)
{
	// wstring to string
	std::vector<char> buf(wstr.size());
	std::use_facet<std::ctype<wchar_t>>(loc).narrow(wstr.data(), wstr.data() + wstr.size(), '?', buf.data());
	std::string res(buf.data(), buf.size());
	return res;
}



//imgui addons
void ShowHelpMarker(const char* desc)
{
	ImGui::TextDisabled("(?)");
	if (ImGui::IsItemHovered())
	{
		ImGui::BeginTooltip();
		ImGui::PushTextWrapPos(ImGui::GetFontSize() * 35.0f);
		ImGui::TextUnformatted(desc);
		ImGui::PopTextWrapPos();
		ImGui::EndTooltip();
	}
}

bool ListBox(const char*                label,
	         int*                       currIndex,
	         std::vector<std::string>  &values,
	         int                        height_in_items)
{
	if (values.empty()) { return false; }
	return	ImGui::ListBox(label, currIndex, vector_to_const_char,
		static_cast<void*>(&values), values.size(), height_in_items);
}


//paths
void GetImagesPathes(std::vector <std::wstring> &image_path) 
{

	const std::filesystem::path work_path = std::filesystem::current_path();
	const std::filesystem::path path = work_path / "data" / "backgrounds";

	try
	{
		for (auto const &entry : std::filesystem::directory_iterator(path))
		{
			try
			{
				if (".bmp" == entry.path().extension()
					|| ".png" == entry.path().extension()
					|| ".tga" == entry.path().extension()
					|| ".jpg" == entry.path().extension()
					|| ".psd" == entry.path().extension()
					|| ".hdr" == entry.path().extension()
					|| ".pic" == entry.path().extension())
					image_path.push_back(entry.path().wstring());
			}
			catch (...) {
				//maybe rename file
				std::cerr << "Image loading error, only:\n.bmp \n.png \n.tga \n.jpg \n.psd \n.hdr \n.pic \n can be loaded" << std::endl;
			}


		}
	}
	catch (...)
	{
		std::cerr << "Failed to open directory " + path.string() << std::endl;
	}
}

void GetMusicPathes(std::vector <std::wstring> &music_path,
                    std::vector <std::string> &playlist) 
{

	const std::filesystem::path work_path = std::filesystem::current_path();
	const std::filesystem::path path = work_path / "data" / "music";
	try
	{
		for (auto const &entry : std::filesystem::directory_iterator(path))
		{
			try
			{
				if (".wav" == entry.path().extension() ||
					".ogg" == entry.path().extension())
				{
					music_path.push_back(entry.path().wstring());
					playlist.push_back(entry.path().filename().string());

				}
			}
			catch (...)
			{
				//maybe rename file
				std::cerr << "Music loading error only:\n.wav \n.ogg \n can be loaded" << std::endl;
			}
		}
	}
	catch (...)
	{
		std::cerr << "Failed to open directory " + path.string() << std::endl;
	}
	for (std::string &name: playlist) {
		for (auto it = name.end() - 1; it >= name.begin(); it--) 
		{
			if (*it == '.') 
			{
				name.erase(it);
				break;
			}
			name.erase(it);
		}
	}
}

void GetCategoriesPathes(std::vector <std::wstring> &categories_path) {

	const std::filesystem::path work_path = std::filesystem::current_path();
	const std::filesystem::path path = work_path / "data" / "categories";
	try
	{
		for (auto const &entry : std::filesystem::directory_iterator(path))
		{
			try
			{
				if (".zero" == entry.path().extension().string())
					categories_path.push_back(entry.path().wstring());
			}
			catch (...) {
				//maybe rename file
				std::cerr << "Category loading error, only program saved files" << std::endl;

			}

		}
	}
	catch (...)
	{
		std::cerr << "Failed to open directory " + path.string() << std::endl;
	}
}

void GetSavesPathes(std::vector <std::wstring> &saves_path) {

	const std::filesystem::path work_path = std::filesystem::current_path();
	const std::filesystem::path path = work_path / "data" / "saves";
	try
	{
		for (auto const &entry : std::filesystem::directory_iterator(path))
		{
			try
			{
				if (".zero" == entry.path().extension().string())
					saves_path.push_back(entry.path().wstring());
			}
			catch (...) {
				//maybe rename file
				std::cerr << "Saves loading error, only program saved files" << std::endl;

			}

		}
	}
	catch (...)
	{

		std::cerr << "Failed to open directory " + path.string() << std::endl;
	}
}





//sprites and textures
sf::Sprite SetBackgrounSprite(sf::Sprite   sprite,
                              sf::Vector2u transform_size) 
{

	


	sf::Vector2u img_size = sprite.getTexture()->getSize();
	sf::Vector2f wnd_size_f{ static_cast<float>(transform_size.x), static_cast<float>(transform_size.y) };
	sf::Vector2f img_size_f{ static_cast<float>(img_size.x), static_cast<float>(img_size.y) };
	float k;
	sprite.setOrigin(static_cast<float>(img_size.x >> 1), static_cast<float>(img_size.y >> 1));
	sprite.setPosition(static_cast<float>(transform_size.x >> 1), static_cast<float>(transform_size.y >> 1));


	k = wnd_size_f.y / img_size_f.y;
	if (img_size_f.x * k < wnd_size_f.x)	k = wnd_size_f.x / img_size_f.x;

	k += 0.03f;
	sprite.scale(k, k);

	return sprite;
}

void LoadAllBackgroundTextures(std::vector <sf::Texture> &all_textures,
                               std::vector <std::wstring> &pathes) 
{
	

	for (auto &path : pathes) 
	{
		
		sf::Texture tmp;
		if (tmp.loadFromFile(utf8_to_string(path, std::locale(".1251"))))
		{
			
			all_textures.push_back(tmp);
		}
		
	}
}

void LoadAllBckgrndExampleSp(std::vector <sf::Texture> &all_textures,
                             std::vector <sf::Sprite>  &all_sprites)
{
	for (auto &texture : all_textures) 
	{
		sf::Sprite tmp;
		tmp.setTexture(texture);


		all_sprites.push_back(tmp);
	}
	
}

void AddBackground(std::wstring              &path,
                   std::vector <sf::Texture> &all_textures,
                   std::vector <sf::Sprite>  &all_sprites) 
{
	std::time_t cur_time = std::time(nullptr);
	char buf[128];
	ZeroMemory(buf, 128);
	ctime_s(buf, 128, &cur_time);
	std::string time_str = buf;
	for (size_t i = 0; i < time_str.size(); ++i)
		if (time_str[i] == ':') time_str[i] = '.';
	time_str.erase(--time_str.end());
	
	
	
	sf::Texture tmp_tex;
	sf::Sprite tmp_sp;




	std::filesystem::path sourse(path);
	std::filesystem::path target_dir = std::filesystem::current_path() / "data" / "backgrounds" / (time_str + sourse.extension().string());
	std::filesystem::copy_file(sourse, target_dir);
	tmp_tex.loadFromFile(utf8_to_string(path.c_str(), std::locale(".1251")));

	all_textures.push_back(tmp_tex);
	all_sprites.push_back(tmp_sp);

	for (size_t i = 0; i < all_textures.size(); i++)
	{
		all_sprites[i].setTexture(all_textures[i]);
	}

}

void DeleteBackground(size_t                     idx,
                      std::vector <sf::Texture>  &all_textures,
                      std::vector <sf::Sprite>   &all_sprites,
                      std::vector <std::wstring> &paths) 
{
	all_textures.erase(all_textures.begin() + idx);
	all_sprites.erase(all_sprites.begin() + idx);
	
	std::filesystem::remove(paths[idx]);
	
	for (size_t i = 0; i < all_textures.size(); i++)
	{
		all_sprites[i].setTexture(all_textures[i]);
	}

	
	paths.erase(paths.begin() + idx);
}


//load/unload information
void LoadSystemParams(float        &sound_vol,
	                  float        &music_vol,
	                  sf::Vector2u &wnd_size,
	                  bool         &sound_off_flag,
                      bool         &music_off_flag,
	                  bool         &fullscreen,
	                  bool         &static_background,
	                  size_t       &idx_of_background) 
{
	std::ifstream in;
	in.open(".\\data\\settings.bin", std::ios::binary);
	if (in.is_open()) 
	{
		in.read(reinterpret_cast<char*>(&sound_vol),         sizeof(float));
		in.read(reinterpret_cast<char*>(&music_vol),         sizeof(float));
		in.read(reinterpret_cast<char*>(&wnd_size.x),        sizeof(unsigned));
		in.read(reinterpret_cast<char*>(&wnd_size.y),        sizeof(unsigned));
		in.read(reinterpret_cast<char*>(&sound_off_flag),    sizeof(bool));
		in.read(reinterpret_cast<char*>(&music_off_flag),    sizeof(bool));
		in.read(reinterpret_cast<char*>(&fullscreen),        sizeof(bool));
		in.read(reinterpret_cast<char*>(&static_background), sizeof(bool));
		in.read(reinterpret_cast<char*>(&idx_of_background), sizeof(size_t));
	}
	in.close();
}

void SaveSystemParams(float        &sound_vol,
	                  float        &music_vol,
	                  sf::Vector2u &wnd_size,
	                  bool         &sound_off_flag,
	                  bool         &music_off_flag,
	                  bool         &fullscreen,
	                  bool         &static_background,
	                  size_t       &idx_of_background) 
{
	std::ofstream out;
	out.open(".\\data\\settings.bin", std::ios::binary | std::ios::trunc);
	if (out.is_open()) 
	{
		out.write(reinterpret_cast<char*>(&sound_vol),         sizeof(float));
		out.write(reinterpret_cast<char*>(&music_vol),         sizeof(float));
		out.write(reinterpret_cast<char*>(&wnd_size.x),        sizeof(unsigned));
		out.write(reinterpret_cast<char*>(&wnd_size.y),        sizeof(unsigned));
		out.write(reinterpret_cast<char*>(&sound_off_flag),    sizeof(bool));
		out.write(reinterpret_cast<char*>(&music_off_flag),    sizeof(bool));
		out.write(reinterpret_cast<char*>(&fullscreen),        sizeof(bool));
		out.write(reinterpret_cast<char*>(&static_background), sizeof(bool));
		out.write(reinterpret_cast<char*>(&idx_of_background), sizeof(size_t));
	}
	out.close();
}






void LoadingWindow(sf::Vector2i window_pos, 
                   sf::Vector2u window_size, 
	               bool        &start_loading_window) {
	sf::Vector2i size = { 512, 512 };

	sf::Texture load_tex;
	load_tex.loadFromFile(".\\data\\textures\\loading status.png");
	sf::Sprite load_sp;
	load_sp.setOrigin(static_cast<float>(size.x >> 1), static_cast<float>(size.y >> 1));
	load_sp.setPosition(static_cast<float>(size.x >> 1), static_cast<float>(size.y >> 1));
	load_sp.setTexture(load_tex);
	load_sp.scale(0.5f, 0.5f);
	sf::Image img;
	img.create(size.x, size.y, sf::Color(127, 127, 127, 255));
	sf::Texture background_tex;
	background_tex.loadFromImage(img);
	sf::Sprite background_sp;
	background_sp.setTexture(background_tex);


	sf::RenderWindow window(sf::VideoMode(size.x, size.y), " ", sf::Style::None);
	window.setFramerateLimit(35);
	window.setPosition({ (static_cast<int>(window_size.x) >> 1) - (size.x >> 1) + window_pos.x,
						(static_cast<int>(window_size.y) >> 1) - (size.y >> 1) + window_pos.y });

	sf::Clock clock;
	sf::Int32 delta_time = 0;
	while (start_loading_window) {
		window.clear();


		load_sp.rotate(static_cast<float>(delta_time % 10));
		window.draw(background_sp);
		window.draw(load_sp);
		delta_time = clock.getElapsedTime().asMilliseconds();
		clock.restart();

		window.display();
	}
	window.close();
}





//other
std::wstring getFilename(std::wstring path)
{
	std::wstring name;
	while (*(--path.end()) != L'.') path.erase(--path.end());
	path.erase(--path.end());

	while (*(--path.end()) != L'\\')
	{
		name.insert(name.begin(), *(--path.end()));
		path.erase(--path.end());
	}
	return name;
}

int FilterLetters(ImGuiInputTextCallbackData* data)
{
	wchar_t ch = data->EventChar;
	wchar_t str[2];
	ZeroMemory(str, 2);
	str[0] = ch;
	if (strchr("qwertyuiopasdfghjklzxcvbnmÈˆÛÍÂÌ„¯˘Áı˙Ù˚‚‡ÔÓÎ‰Ê˝ˇ˜ÒÏËÚ¸·˛∏_1234567890+-@…÷” ≈Õ√ÿŸ«’⁄‘€¬¿œ–ŒÀƒ∆›®ﬂ◊—Ã»“‹¡ﬁQWERTYUIOPASDFGHJKLZXCVBNM",
			(utf8_to_string(str, std::locale(".1251"))).c_str()[0]))
		return 0;
	return 1;
}

std::wstring createWayToNewCategory(std::wstring &name)
{
	
	const std::filesystem::path work_path = std::filesystem::current_path();
	const std::filesystem::path path = work_path / "data" / "categories";
	return(path.wstring() + L"\\" + name + L".zero");

}
std::wstring createWayToNewSave(std::wstring &name)
{

	const std::filesystem::path work_path = std::filesystem::current_path();
	const std::filesystem::path path = work_path / "data" / "saves";
	return(path.wstring() + L"\\" + name + L".zero");

}


