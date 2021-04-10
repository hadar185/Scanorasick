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

void Trie::print()
{
	m_root->print(0);
}
