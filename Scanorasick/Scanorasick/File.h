#pragma once

#include <iostream>
#include <fstream>
#include <string>
#include <vector>

using Buffer = std::vector<uint8_t>;

class File
{
private:
	std::string m_path;
	Buffer m_content;

public:
	File(std::string);

	Buffer read_file(bool);
	void write_to_file(Buffer);
	std::vector<Buffer> split_signatures(Buffer);
	Buffer get_content();
};
