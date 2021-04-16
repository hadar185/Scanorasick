#pragma once

#include <iostream>
#include <map>
#include <vector>

class Node
{
private:
	uint8_t m_value;
	std::map<uint8_t, Node*> m_nexts;
	std::vector<uint8_t> m_full_value = {};
	Node* m_fail;
	int m_index;


public:
	static int count;

	Node(uint8_t);
	uint8_t get_value();
	std::map<uint8_t, Node*> get_nexts();
	Node* get_next(uint8_t);
	Node* add_next(uint8_t);

	std::vector<uint8_t> get_full_value();
	void set_full_value(std::vector<uint8_t>);
	void set_fail(Node *);

	void print(int);
};

