#include <memory>

#include "Trie.h"

Trie::Trie() : m_patterns_amount(0), m_size(0), 
			   m_root(std::make_unique<Node>())
{
	// Increase amount of nodes in trie
	++m_size;
	m_root->set_fail_link(m_root.get());
}

Node* Trie::get_root()
{
	return m_root.get();
}

int Trie::get_size()
{
	return m_size;
}

int Trie::get_patterns_amount()
{
	return m_patterns_amount;
}

void Trie::add_pattern(Buffer pattern)
{
	Node* current = m_root.get();
	for (int pattern_index = 0; pattern_index < pattern.size(); ++pattern_index)
	{
		Node* next = current->get_next(pattern[pattern_index]);
		if (next != nullptr)
		{
			current = next;
		}
		else
		{
			// CR: Private function for adding node to trie
			// CR: Create the new node here and pass it to add_next
			current = current->add_next(pattern[pattern_index], m_size);
			++m_size;
			current->set_fail_link(m_root.get());
		}

		if ((pattern_index == (pattern.size() - 1)) &&
			(!current->is_match()))
		{
			current->set_match(true);
			++m_patterns_amount;
		}
	}
}

void Trie::add_patterns(std::vector<Buffer> patterns_array)
{
	std::vector<Buffer>::iterator it;

	for (it = patterns_array.begin(); it != patterns_array.end(); it++)
	{
		add_pattern(*it);
	}
}

Node* Trie::search(Buffer pattern)
{
	Node* current = m_root.get();
	for (int i = 0; i < pattern.size(); ++i)
	{
		current = current->get_next(pattern[i]);
		if (current == nullptr)
		{
			break;
		}
	}
	return current;
}

void Trie::add_back_links()
{
	add_back_links(m_root.get());
}

void Trie::add_back_links(Node* node)
{
	Buffer suffix = node->get_full_value();
	node->set_fail_link(m_root.get());

	while (suffix.size() > 0)
	{
		// TO DO: Don't erase
		suffix.erase(suffix.begin());
		Node* target_fail = search(suffix);
		if (target_fail != nullptr)
		{
			node->set_fail_link(target_fail);
			break;
		}
	}

	//std::map<uint8_t, std::unique_ptr<Node>> nexts = std::move(node->get_nexts());
	auto next_values = node->get_next_values();
	for (const auto value : next_values)
	{
		add_back_links(value);
	}
}

void Trie::print()
{
	m_root->print(0);
}

std::unique_ptr<NodeStruct[]> Trie::serialize()
{
	// TO DO: Future features, serialize in chunks
	auto nodes_array = std::make_unique<NodeStruct[]>(m_size);

	Node* current_node;

	// Create a queue for BFS
	std::queue<Node*> queue;
	queue.push(m_root.get());

	while (!queue.empty())
	{
		current_node = queue.front();
		queue.pop();

		nodes_array[current_node->get_index()] = current_node->serialize();

		auto next_values = current_node->get_next_values();
		for (const auto value : next_values)
		{
			queue.push(value);
		}
	}

	return nodes_array;
}

void Trie::deserialize(std::vector<NodeStruct> node_structs)
{
	auto nodes = std::make_unique<Node*[]>(node_structs.size());

	nodes[0] = m_root.get();
	for (auto i = 0; i < node_structs.size(); i++)
	{
		for (auto j = 0; j < node_structs[i].next_amount; j++)
		{
			NodeStruct next = node_structs[node_structs[i].next_indexes[j]];
			nodes[next.index] = nodes[i]->add_next(next, nodes[next.fail_index]);
			++m_size;
			if (next.end)
			{
				++m_patterns_amount;
			}
		}
	}
	for (auto i = 0; i < node_structs.size(); i++)
	{
		nodes[node_structs[i].index]->set_fail_link(nodes[node_structs[node_structs[i].index].fail_index]);
	}
}
