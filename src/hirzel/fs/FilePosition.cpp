#include "hirzel/fs/FilePosition.hpp"

namespace hirzel::fs
{
	FilePosition::FilePosition(const File& file):
		_file(&file),
		_index(0),
		_line(1),
		_column(1)
	{}

	FilePosition::FilePosition(const File& file, uint32_t index, uint16_t line, uint16_t column):
		_file(&file),
		_index(index),
		_line(line),
		_column(column)
	{}

	FilePosition FilePosition::seekEndOfLine() const
	{
		auto result = *this;
		const auto& file = *_file;
		auto shouldBreak = false;
		
		while (_index < file.length() && !shouldBreak)
		{
			auto c = file[_index];

			if (c == '\n')
			{
				shouldBreak = true;
				result._line += 1;
				result._column = 0;
			}

			result._index += 1;
			result._column += 1;
		}

		return result;
	}

	FilePosition FilePosition::seekEndOfBlockComment() const
	{
		auto result = *this;
		const auto& file = *_file;
		auto shouldBreak = false;

		while (_index < file.length() && !shouldBreak)
		{
			auto c = file[_index];

			switch (c)
			{
				case '/':
					if (file[_index - 1] == '*')
						shouldBreak = true;
					break;

				case '\n':
					result._line += 1;
					result._column = 0;
					break;
			}
			
			result._index += 1;
			result._column += 1;
		}

		return result;
	}

	FilePosition FilePosition::seekNext() const
	{
		auto result = *this;
		const auto& file = *result._file;

		while (result._index < file.length())
		{
			auto c = file[result._index];

			if (c == '/')
			{
				switch (file[result._index + 1])
				{
					case '/': // Line comment
						result._index += 2;
						result = result.seekEndOfLine();
						continue;

					case '*': // Block comment
						result._index += 2;
						result.seekEndOfBlockComment();
						continue;

					default:
						break;
				}
			}

			if (c > ' ')
				break;

			if (c == '\n')
			{
				result._line += 1;
				result._column = 0;
			}

			result._index += 1;
			result._column += 1;
		}

		return result;
	}

	char FilePosition::operator*() const
	{
		return (*_file)[_index];
	}

	char FilePosition::operator[](size_t index) const
	{
		return (*_file)[_index + index];
	}

	FilePosition FilePosition::operator+(uint32_t offset) const
	{
		auto index = std::min(_index + offset, (uint32_t)_file->length());

		return FilePosition(*_file, index, _line, _column + (index - _index));
	}

	bool FilePosition::operator==(const FilePosition& other) const
	{
		return _file == other._file && _index == other._index;
	}

	bool FilePosition::operator!=(const FilePosition& other) const
	{
		return !(*this == other);
	}

	bool FilePosition::operator<(const FilePosition& other) const
	{
		return _index < other._index;
	}

	bool FilePosition::operator<=(const FilePosition& other) const
	{
		return _index <= other._index;
	}

	bool FilePosition::operator>(const FilePosition& other) const
	{
		return _index > other._index;
	}

	bool FilePosition::operator>=(const FilePosition& other) const
	{
		return _index >= other._index;
	}
}
