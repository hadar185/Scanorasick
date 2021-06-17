#include "Scanner.h"
#include "File.h"

int Scanner::scan(Buffer buffer, Trie trie, std::string output_path)
{
	Node* root = trie.get_root();
	Node* current = root;
	Node* next = NULL;
	int found_count = 0;

	File output_file(output_path);

	// Loop through every byte in the given buffer
	for (int byte_index = 0; byte_index < buffer.size(); ++byte_index)
	{
		next = current->get_next(buffer[byte_index]);

		if (next == NULL)
		{
			if (current == root)
			{
				continue;
			}

			// Go through the back link with the current byte
			current = current->get_fail_link();

			--byte_index;
		}
		else if (next == root)
		{
			break;
		}
		else
		{
			current = next;
		}

		// Check if a match found
		if (current->is_match())
		{
			current->print_full_value();
			if (output_path != "")
			{
				output_file.write_to_file(current->get_full_value());
			}
			++found_count;
		}
	}

	return found_count;
}
