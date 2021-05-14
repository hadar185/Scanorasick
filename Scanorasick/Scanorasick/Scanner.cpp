#include "Scanner.h"

bool Scanner::scan(
	std::vector<uint8_t> buffer,
	Trie trie)
{
	int i = 0;
	Node* root = trie.get_root();
	Node* current = root;
	Node* next = NULL;

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
		}
	}

	return true;
}

/*while (i < buffer.size() && next != root)
	{
		next = current->get_next(buffer[i]);

		if (next == NULL)
		{
			if (current != root)
			{
				current = current->get_fail();
				next = current->get_next(buffer[i]);

				if (current == root && next == root)
				{
					++i;
				}
				else
				{
					if (current->is_end())
					{
						current->print_full_value();
					}
				}
			}
			else
			{
				++i;
			}
			continue;
		}

		current = next;
		if (current->is_end())
		{
			current->print_full_value();
		}

		++i;
	}*/