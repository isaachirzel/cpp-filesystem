#include "hirzel/fs/FilePosition.hpp"

namespace hirzel::fs
{
	FilePosition::FilePosition(const File& file):
		_file(&file),
		_index(0),
		_line(0),
		_column(0)
	{}

	FilePosition::FilePosition(const File& file, uint32_t index, uint16_t line, uint16_t column):
		_file(&file),
		_index(index),
		_line(line),
		_column(column)
	{}

	void FilePosition::seekEndOfLine()
	{
		const auto& file = *_file;
		auto shouldBreak = false;
		
		while (_index < file.length() && !shouldBreak)
		{
			auto c = file[_index];

			if (c == '\n')
			{
				shouldBreak = true;
				_line += 1;
				_column = 0;
			}

			_index += 1;
			_column += 1;
		}
	}

	void FilePosition::seekEndOfBlockComment()
	{
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
					_line += 1;
					_column = 0;
					break;
			}
			
			_index += 1;
			_column += 1;
		}
	}

	void FilePosition::seekNext()
	{
		const auto& file = *_file;

		while (_index < file.length())
		{
			auto c = file[_index];

			if (c == '/')
			{
				switch (file[_index + 1])
				{
					case '/': // Line comment
						_index += 2;
						seekEndOfLine();
						continue;

					case '*': // Block comment
						_index += 2;
						seekEndOfBlockComment();
						continue;

					default:
						break;
				}
			}

			if (c > ' ')
				break;

			if (c == '\n')
			{
				_line += 1;
				_column = 0;
			}

			_index += 1;
			_column += 1;
		}
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
