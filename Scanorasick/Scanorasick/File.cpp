#include "File.h"

#include <iterator>

File::File(std::string path) :
	m_path(path)
{}

// CR: Remove parameter if not used
Buffer File::read_file(bool read_spaces = true)
{
	std::ifstream file(m_path, std::ios::binary);

	if (!file.good())
	{
		std::cerr << "File does not exist" << std::endl;
	}

	// Leave new lines in binary mode
	if (read_spaces)
	{
		file.unsetf(std::ios::skipws);
	}

	Buffer bytes((std::istream_iterator<uint8_t>(file)),
		std::istream_iterator<uint8_t>());

	std::cout << "Read " << bytes.size() << " bytes from file" << std::endl;

	return bytes;
}

void File::write_to_file(Buffer content)
{
	std::ofstream file(m_path, std::ios_base::app | std::ios::binary);

	// CR: Write all at once or in chunks, content.data()

	for (const auto& e : content) file << e;
	//file << std::endl;
	file.close();
}

void File::write_to_file(std::string content)
{
	// CR: Merge the two write functions

	std::ofstream file(m_path, std::ios_base::app | std::ios::binary);
	file << content;
	file << std::endl;
	file.close();
}

std::vector<Buffer> File::split_signatures(Buffer& bytes)
{
	std::vector<Buffer> signatures = { {} };
	int index = 0;

	for (Buffer::const_iterator i = bytes.begin(); i != bytes.end(); ++i)
	{
		if (*i == '\n') {
			++index;
			signatures.push_back({});
		}
		else if (*i != '\r')
		{
			signatures[index].push_back(*i);
		}
	}
	return signatures;
}

Buffer File::get_content() {
	if (m_content.size() == 0)
	{
		m_content = read_file();
	}

	return m_content;
}
