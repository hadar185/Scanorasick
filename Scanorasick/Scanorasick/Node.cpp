#include "Node.h"

int Node::count = 0;

Node::Node(uint8_t value) :
	m_value(value),
	m_fail(NULL),
	m_index(count)
{
	count++;
}

uint8_t Node::get_value()
{
	return m_value;
}

std::map<uint8_t, Node*> Node::get_nexts()
{
	return m_nexts;
}

Node *Node::get_next(uint8_t value)
{
	std::map<uint8_t, Node*>::iterator it = m_nexts.find(value);
	if (it == m_nexts.end())
	{
		return NULL;
	}
	return it->second;
}

Node *Node::add_next(uint8_t value) {
	Node* next = new Node(value);
	std::vector<uint8_t> full_value = m_full_value;
	full_value.push_back(value);
	next->set_full_value(full_value);
	m_nexts.insert({ value, next });

	return m_nexts.at(value);
}

std::vector<uint8_t> Node::get_full_value()
{
	return m_full_value;
}

void Node::set_full_value(std::vector<uint8_t> value)
{
	m_full_value = value;
}

void Node::set_fail(Node *fail)
{
	m_fail = fail;
}

void Node::print(int i)
{
	if (m_fail != NULL)
	{
		std::cout << "value: " << m_value << ", index: " << m_index << ", fail_index: " << m_fail->m_index << ", full: ";
		for (std::vector<uint8_t>::const_iterator i = m_full_value.begin(); i != m_full_value.end(); ++i)
			std::cout << *i;
		std::cout << std::endl;
	}

	std::map<uint8_t, Node*>::iterator it;
	for (it = m_nexts.begin(); it != m_nexts.end(); it++)
	{
		it->second->print(i + 1);
	}
}
