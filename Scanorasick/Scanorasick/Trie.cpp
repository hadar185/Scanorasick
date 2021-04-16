#include "Trie.h"

Trie::Trie()
{
	m_root = new Node(5);
}

void Trie::add_pattern(std::vector<uint8_t> pattern)
{
    Node* current = m_root;
    while (pattern.size() > 0)
    {
        Node* next = current->get_next(pattern[0]);
        if (next != NULL)
        {
            current = next;
            pattern.erase(pattern.begin());
            continue;
        }

        current = current->add_next(pattern[0]);
        pattern.erase(pattern.begin());
    }
    
}

Node* Trie::search(std::vector<uint8_t> pattern)
{
	Node* current = m_root;
	while(current != NULL && pattern.size() > 0)
	{
		current = current->get_next(pattern[0]);
		pattern.erase(pattern.begin());
	}
	return current;
}

void Trie::add_backs()
{
	add_backs(m_root);
}

void Trie::add_backs(Node *node)
{
	std::vector<uint8_t> suffix = node->get_full_value();
	while (suffix.size() > 0)
	{
		suffix.erase(suffix.begin());
		Node* target_fail = search(suffix);
		if (target_fail != NULL)
		{
			node->set_fail(target_fail);
			break;
		}
		node->set_fail(m_root);
	}

	std::map<uint8_t, Node*> nexts = node->get_nexts();
	std::map<uint8_t, Node*>::iterator it;
	for (it = nexts.begin(); it != nexts.end(); it++)
	{
		add_backs(it->second);
	}
}

void Trie::print()
{
	m_root->print(0);
}
