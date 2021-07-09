#pragma once

#include <iostream>
#include <fstream>
#include <string>
#include <iterator>

#include "Common.h"

class File
{
private:
	std::string m_path;
	Buffer m_content;

public:
	File(std::string path);

	// CR: Use references when possible
	Buffer read_file();
	//void write_to_file(Buffer);
	void write_to_file(const uint8_t* content);
	std::vector<Buffer> split_signatures(Buffer& bytes);
	Buffer get_content();
};
