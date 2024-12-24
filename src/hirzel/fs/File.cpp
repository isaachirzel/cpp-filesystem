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
	File::File(std::string&& text, const Path& path, uint16_t projectOffset, FileType fileType):
		_text(std::move(text)),
		_path(path),
		_projectOffset(projectOffset),
		_type(FileType::Regular)
	{}

	static std::string readFileText(const Path& filePath)
	{
		// TODO: close on error
		const auto* path = filePath.text().c_str();
		auto fd = open(path, O_RDONLY);

		if (fd == -1)
		{
			pushError(ErrorType::FileOpen, path);
			return "";
		}

		struct stat sb;

		if (fstat(fd, &sb) == -1)
		{
			pushError(ErrorType::FileStat, path);
			close(fd);
			return "";
		}

		auto length = sb.st_size;
		auto* ptr = mmap(nullptr, length, PROT_READ, MAP_SHARED, fd, 0);

		if (ptr == (void*)-1)
		{
			pushError(ErrorType::FileRead, path);
			close(fd);
			return "";
		}

		auto text = std::string((char*)ptr, length);

		munmap(ptr, length);
		close(fd);

		return text;
	}

	static void writeFileText(const char *filePath, const std::string& content)
	{
		FILE *file = fopen(filePath, "w");

		if (!file)
		{
			pushError(ErrorType::FileWrite, filePath);
			return;
		}

		size_t bytesWritten = fwrite(content.c_str(), sizeof(char), content.length(), file);

		fclose(file);

		if (bytesWritten != content.length())
		{
			pushError(ErrorType::FileWrite, filePath);
			return;
		}
	}

	File File::read(const Path& path, uint16_t projectOffset)
	{
		auto text = readFileText(path);
		auto file = File(std::move(text), path, projectOffset, FileType::Regular);

		return file;
	}

	File File::createTestMainSrcFile(const Path& directoryPath, uint16_t projectOffset, std::string&& src)
	{
		auto file = File(std::move(src), directoryPath / "main.pk", projectOffset, FileType::Source);

		return file;
	}

	void File::write()
	{
		return writeFileText(_path.text().c_str(), _text);
	}

	std::string File::getRelativePath() const
	{
		throw std::runtime_error("getRelativePath not implemented.");
		// return _path.substr(_relativePathOffset);
	}

	const char& File::operator[](size_t index) const
	{
		assert(index <= _text.length());
		
		return _text[index];
	}
}
