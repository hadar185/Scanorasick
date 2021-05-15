#include "Scanner.h"

int Scanner::scan(std::vector<uint8_t> buffer, Trie trie)
{
	Node* root = trie.get_root();
	Node* current = root;
	Node* next = NULL;
	int i = 0, found_count = 0;

	while (i < buffer.size() && next != root)
	{
		next = current->get_next(buffer[i]);

		if (next == NULL)
		{
			if (current == current->get_fail())
			{
				++i;
				continue;
			}

			current = current->get_fail();
			next = current->get_next(buffer[i]);
		}
		else
		{
			current = next;
			++i;
		}

		if (current->is_end())
		{
			current->print_full_value();
			++found_count;
		}
	}

	return found_count;
}
