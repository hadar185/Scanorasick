#pragma once

#include <iostream>
#include <map>
#include <list>

#include "Common.h"

#pragma pack(push, 1)
struct NodeStruct
{
	int index;
	int fail_index;
	uint8_t value;
	bool end;
	int next_amount;
	int* next_indexes;
};
#pragma pack(pop)

class Node
{
private:
	uint8_t m_value;
	std::map<uint8_t, std::unique_ptr<Node>> m_nexts;
	Buffer m_full_value;
	Node* m_fail_link;
	bool m_match;
	int m_index;


public:
	Node();
	Node(uint8_t, int index);
	Node(NodeStruct, Node *fail);

	uint8_t get_value();

	std::vector<Node*> get_next_values();
	Node* get_next(uint8_t);
	Node* add_next(uint8_t, int);
	Node* add_next(NodeStruct, Node*);

	Buffer get_full_value();
	void set_full_value(Buffer);

	Node* get_fail_link();
	void set_fail_link(Node*);

	bool is_match();
	void set_match(bool);

	int get_index();

	void print(int);
	void print_full_value();

	NodeStruct serialize();
};

using NodeUPtr = std::unique_ptr<Node>;
using NextMap = std::map<uint8_t, NodeUPtr>;