#include "Scanner.h"
#include "File.h"

int Scanner::scan(Buffer buffer, Trie& trie, std::string output_path)
{
	Node* root = trie.get_root();
	Node* current = root;
	Node* next = nullptr;
	int found_count = 0;

	std::map<Buffer, int> patterns_found;

	// Loop through every byte in the given buffer
	for (int byte_index = 0; byte_index < buffer.size(); ++byte_index)
	{
		next = current->get_next(buffer[byte_index]);

		if (next == nullptr)
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
			Buffer current_full_value = current->get_full_value();
			patterns_found.count(current_full_value) > 0 ? patterns_found[current_full_value]++ : patterns_found[current_full_value] = 1;
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
		//for (const auto& e : key) std::cout << e;
		std::cout << key.data() << " " + std::to_string(value) + " times." << std::endl;
		if (output_path != "")
		{
			output_file.write_to_file(key.data());
			std::string text = " " + std::to_string(value) + " times.\n";
			output_file.write_to_file(reinterpret_cast<const uint8_t*>(&text[0]));
		}
	}
}
