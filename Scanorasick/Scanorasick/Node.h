#pragma once

#include <iostream>
#include <map>

class Node
{
private:
	uint8_t m_value;
	std::map<uint8_t, Node*> m_nexts;
	Node *m_fail;
public:
	Node(uint8_t);
	uint8_t get_value();
	Node* get_next(uint8_t);
	Node *add_next(uint8_t);
	void print(int);
};

