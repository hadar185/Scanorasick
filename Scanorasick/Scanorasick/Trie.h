#pragma once

#include <iostream>
#include <vector>
#include <map>

#include "Node.h"

class Trie
{
private:
	Node *m_root;

public:
	Trie();
	void add_pattern(std::vector<uint8_t>);
	void print();
};

