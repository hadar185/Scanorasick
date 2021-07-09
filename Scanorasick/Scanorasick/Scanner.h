#pragma once

#include <iostream>

#include "Trie.h"

class Scanner
{
private:
	void show_output(std::map<Buffer, int> patterns_found, std::string output_path);
public:
	int scan(Buffer buffer, Trie& trie, std::string = "");
};
