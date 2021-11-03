#pragma once

#include <map>
#include <string>
#include <vector>
#include <fstream>
#include <algorithm>
#include <exception>

class IniFile {
	std::map<std::string, std::map<std::string, std::string>> _data;
	const std::vector<std::string>  _boolValues = { "true", "on", "yes", "y", "1" };
public:
	IniFile(std::string path);

	void save(std::string path) const;

	template<typename T>
	T read(std::string section, std::string key, T defaultValue = T{}) const;

	template<typename T>
	void write(std::string section, std::string key, T value);

	bool sectionExists(std::string section) const;
	bool keyExists(std::string section, std::string key) const;

	std::vector<std::string> sections() const;
	std::vector<std::string> keys(std::string section) const;
};

IniFile::IniFile(std::string path) {
	std::string str;
	std::vector<std::string> buf;

	std::ifstream fin;
	fin.open(path);
	if (fin.is_open()) {
		while (!fin.eof()) {
			if (getline(fin, str) && !(str.length() == 1 && isspace(str[0]))) {
				buf.push_back(str);
			}
		}
	}
	fin.close();

	std::string section;
	std::string keys;
	std::string values;

	for (size_t i = 0; i < buf.size(); ++i) {
		size_t first = 0;
		size_t last = 0;

		if (buf[i][0] == '[') {
			first = 1;
			last = buf[i].find_last_of(']');
			section = buf[i].substr(first, last - first);
			_data.insert(std::pair<std::string, std::map<std::string, std::string>>(section, {}));
		}

		else {
			last = buf[i].find('=');
			keys = buf[i].substr(first, last);
			std::swap(first, last);
			++first;
			last = buf[i].size() - 1;
			values = buf[i].substr(first, last);
			if (!values.empty() && values[values.size() - 1] == '\r') {
				values.erase(values.size() - 1);
			}
			_data.at(section).insert(std::pair<std::string, std::string>(keys, values));
		}
	}
}

void IniFile::save(std::string path) const {
	std::ofstream fout;
	fout.open(path);
	if (!fout) {
		std::cout << "File open error!\n";
	}
	for (auto it = _data.begin(); it != _data.end(); ++it) {
		fout << '[' << it->first << "]\n";
		for (auto it2 = it->second.begin(); it2 != it->second.end(); ++it2) {
			fout << it2->first << " = " << it2->second << '\n';
		}
		fout << std::endl;
	}
}

bool IniFile::sectionExists(const std::string section) const {
	for (auto i : _data) {
		if (i.first == section) {
			return true;
		}
	}

	return false;
}

bool IniFile::keyExists(std::string section, std::string key) const {
	if (sectionExists(section)) {
		auto i = _data.at(section).find(key);
		return i != _data.at(section).end();
	}
	
	return false;
}

std::vector<std::string> IniFile::sections() const {
	std::vector<std::string> sections;
	for (auto i = _data.begin(); i != _data.end(); ++i) {
		sections.push_back(i->first);
	}

	return sections;
}

std::vector<std::string> IniFile::keys(std::string section) const {
	std::vector<std::string> keys;
	if (sectionExists(section)) {
		for (auto& i : _data.at(section)) {
			keys.push_back(i.first);
		}
	}

	return keys;
}

template<>
std::string IniFile::read(std::string section, 
						std::string key, 
						std::string defaultValue) const 
{
	if (!keyExists(section, key)) {
		return defaultValue;
	}

	return _data.at(section).at(key);
}

template<>
int IniFile::read(std::string section, 
				std::string key, 
				int defaultValue) const 
{
	try {
		return std::stoi(_data.at(section).at(key));
	}
	catch (std::exception err) {
		return defaultValue;
	}
}

template<>
float IniFile::read(std::string section, 
					std::string key, 
					float defaultValue) const 
{
	try {
		return std::stof(_data.at(section).at(key));
	}
	catch (std::exception err) {
		return defaultValue;
	}
}

template<>
bool IniFile::read(std::string section, 
				std::string key, 
				bool defaultValue) const 
{
	try {
		std::string keyLowerCase = _data.at(section).at(key);
		std::transform(keyLowerCase.begin(), keyLowerCase.end(),
			keyLowerCase.begin(), [](unsigned char c) { return std::tolower(c); });
		auto i = std::find(_boolValues.begin(), _boolValues.end(), keyLowerCase);
		return i != _boolValues.end();
	}
	catch (std::exception err) {
		return defaultValue;
	}
}

template<>
void IniFile::write(const std::string section,
	const std::string key,
	std::string value)
{
	_data[section][key] = value;
}

template<>
void IniFile::write(const std::string section,
	const std::string key,
	int value)
{
	_data[section][key] = std::to_string(value);
}

template<>
void IniFile::write(const std::string section,
	const std::string key,
	float value)
{
	_data[section][key] = std::to_string(value);
}

template<>
void IniFile::write(const std::string section,
	const std::string key,
	bool value)
{
	_data[section][key] = value ? "true" : "false";
}