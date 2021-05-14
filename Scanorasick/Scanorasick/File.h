#pragma once

#include <iostream>
#include <fstream>
#include <string>
#include <vector>

class File
{
private:
	std::string m_path;
	std::vector<uint8_t> m_content;

public:
	File(std::string);

	std::vector<uint8_t> read_file(bool);
	std::vector<std::vector<uint8_t>> split_signatures(std::vector<uint8_t>);
	std::vector<uint8_t> get_content();
};

/*class TrieFile : public File
{
public:
	TrieFile(std::string);

	std::vector<uint8_t> read_file(bool);
};*/
