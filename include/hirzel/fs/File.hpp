#ifndef HIRZEL_FS_FILE_HPP
#define HIRZEL_FS_FILE_HPP

#include "hirzel/fs/Path.hpp"

#include <vector>

namespace hirzel::fs
{
	class File
	{
		std::vector<char> _content;
		Path _path;

	private:

		File(std::vector<char>&& content, const Path& path);

	public:

		File(File&&) = default;
		File(const File&) = delete;

		static File read(const Path& filepath);

		void write();
		std::string getRelativePath() const;
		
		const char& operator[](size_t index) const;

		const auto& content() const { return _content; }
		const auto& path() const { return _path; }
		auto length() const { return _content.size(); }
	};
}

#endif
