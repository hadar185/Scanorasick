#include "Node.h"

// TO DO: Replace 'new' everywhere!

Node::Node() :
	m_value(0),
	m_fail_link(NULL),
	m_match(false),
	m_index(0),
	m_full_value()
{}

Node::Node(uint8_t value, int index) :
	m_value(value),
	m_fail_link(NULL),
	m_match(false),
	m_index(index),
	m_full_value()
{}

Node::Node(NodeStruct node_struct, Node *fail) :
	m_value(node_struct.value),
	m_fail_link(fail),
	m_match(node_struct.end),
	m_index(node_struct.index),
	m_full_value()
{}

uint8_t Node::get_value()
{
	return m_value;
}

std::vector<Node*> Node::get_next_values()
{
	std::vector<Node*> next_values(m_nexts.size());
	for (const auto& [key, value] : m_nexts)
	{
		next_values.push_back(value.get());
	}
	return next_values;
}

Node* Node::get_next(uint8_t value)
{
	NextMap::iterator it = m_nexts.find(value);
	if (it == m_nexts.end())
	{
		return NULL;
	}
	return it->second.get();
}

Node* Node::add_next(uint8_t value, int index)
{
	// Create a node with the given value and index.
	auto next = std::make_unique<Node>(value, index);

	// Set the full value of the node starting from the root.
	Buffer full_value = m_full_value;
	full_value.push_back(value);
	next->set_full_value(full_value);

	// Set the node as one of the next nodes.
	m_nexts.insert({ value, std::move(next) });

	// Return a pointer to the new node.
	return m_nexts.at(value).get();
}

Node* Node::add_next(NodeStruct node_struct, Node *fail)
{
	//Node* next = new Node(node_struct, fail); //std::make_unique<Node>(node_struct, fail);
	auto next = std::make_unique<Node>(node_struct, fail);
	Buffer full_value = m_full_value;
	full_value.push_back(node_struct.value);
	next->set_full_value(full_value);
	m_nexts.insert({ node_struct.value, std::move(next) });

	return m_nexts.at(node_struct.value).get();
}

Buffer Node::get_full_value()
{
	return m_full_value;
}

void Node::set_full_value(Buffer value)
{
	m_full_value = value;
}

Node* Node::get_fail_link()
{
	return m_fail_link;
}

void Node::set_fail_link(Node* fail)
{
	m_fail_link = fail;
}

bool Node::is_match()
{
	return m_match;
}

void Node::set_match(bool match)
{
	m_match = match;
}

int Node::get_index()
{
	return m_index;
}

void Node::print(int i)
{
	if (m_fail_link != NULL)
	{
		std::cout << "value: " << m_value << ", index: " << m_index << ", fail_index: " << m_fail_link->m_index << ", match: " << m_match << ", full: ";
		print_full_value();
	}

	for (const auto& [key, value] : m_nexts)
	{
		value->print(i + 1);
	}
}

void Node::print_full_value()
{
	for (Buffer::const_iterator it = m_full_value.begin(); it != m_full_value.end(); ++it)
	{
		std::cout << *it;
	}
	std::cout << std::endl;
}

NodeStruct Node::serialize()
{
	int length = (int)m_nexts.size();
	int* nexts = (int*)malloc(length * sizeof(int));
	if (!nexts)
	{
		std::cout << "Memory Allocation Failed";
		exit(1);
	}

	int i = 0;
	for (const auto& [key, value] : m_nexts)
	{
		nexts[i] = value->m_index;
		++i;
	}

	return { m_index, m_fail_link->m_index, m_value, m_match, length, nexts };

}
