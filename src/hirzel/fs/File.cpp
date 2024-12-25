#include "hirzel/fs/File.hpp"
#include "hirzel/fs/Error.hpp"
#include "hirzel/fs/ErrorType.hpp"
#include "hirzel/fs/Path.hpp"

#include <cassert>
#include <cstdio>
#include <cstring>

// TODO: Cross platform
#include <fcntl.h>
#include <stdexcept>
#include <sys/mman.h>
#include <sys/stat.h>
#include <unistd.h>

namespace hirzel::fs
{
	File::File(std::vector<char>&& content, const Path& path):
		_content(std::move(content)),
		_path(path)
	{}

	static std::vector<char> readFileContent(const Path& filePath)
	{
		// TODO: close on error
		const auto* path = filePath.text().c_str();
		auto fd = open(path, O_RDONLY);

		if (fd == -1)
		{
			pushError(ErrorType::FileOpen, path);
			return {};
		}

		struct stat sb;

		if (fstat(fd, &sb) == -1)
		{
			pushError(ErrorType::FileStat, path);
			close(fd);
			return {};
		}

		auto length = sb.st_size;
		auto* ptr = mmap(nullptr, length, PROT_READ, MAP_SHARED, fd, 0);

		if (ptr == (void*)-1)
		{
			pushError(ErrorType::FileRead, path);
			close(fd);
			return {};
		}

		auto content = std::vector<char>((char*)ptr, (char*)ptr + length);

		munmap(ptr, length);
		close(fd);

		return content;
	}

	static void writeFileContent(const char *filePath, const std::vector<char>& content)
	{
		// TODO: Memory map
		FILE *file = fopen(filePath, "w");

		if (!file)
		{
			pushError(ErrorType::FileWrite, filePath);
			return;
		}

		size_t bytesWritten = fwrite(content.data(), sizeof(char), content.size(), file);

		fclose(file);

		if (bytesWritten != content.size())
		{
			pushError(ErrorType::FileWrite, filePath);
			return;
		}
	}

	File File::read(const Path& path)
	{
		auto content = readFileContent(path);
		auto file = File(std::move(content), path);

		return file;
	}

	void File::write()
	{
		return writeFileContent(_path.text().c_str(), _content);
	}

	std::string File::getRelativePath() const
	{
		throw std::runtime_error("getRelativePath not implemented.");
		// return _path.substr(_relativePathOffset);
	}

	const char& File::operator[](size_t index) const
	{
		assert(index <= _content.size());
		
		return _content[index];
	}
}
