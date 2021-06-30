#include "Scanner.h"
#include "File.h"

int Scanner::scan(Buffer buffer, Trie& trie, std::string output_path)
{
	Node* root = trie.get_root();
	Node* current = root;
	Node* next = NULL;
	int found_count = 0;

	std::map<Buffer, int> patterns_found;

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
			patterns_found.count(current->get_full_value()) > 0 ? patterns_found[current->get_full_value()]++ : patterns_found[current->get_full_value()] = 1;
			//current->print_full_value();
			//if (output_path != "")
			//{
			//	output_file.write_to_file(current->get_full_value());
			//}
			++found_count;
		}
	}
	
	show_output(patterns_found, output_path);

	return found_count;
}

void Scanner::show_output(std::map<Buffer, int> patterns_found, std::string output_path)
{
	File output_file(output_path);
	for (const auto& [key, value] : patterns_found)
	{
		for (const auto& e : key) std::cout << e;
		std::cout << " " + std::to_string(value) + " times." << std::endl;
		if (output_path != "")
		{
			output_file.write_to_file(key);
			output_file.write_to_file(" " + std::to_string(value) + " times.");
		}
	}
}
