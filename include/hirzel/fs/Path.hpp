#ifndef HIRZEL_FS_PATH_HPP
#define HIRZEL_FS_PATH_HPP

#include <string>
#include <cstdint>

namespace hirzel::fs
{
	class Path
	{
		std::string _text;
		uint16_t _projectOffset;
		uint16_t _nameOffset;
		uint16_t _extensionOffset;

	public:

		Path();
		Path(const char* text);
		Path(const std::string& text);
		Path(Path&&) = default;
		Path(const Path&) = default;
		Path& operator=(Path&&) = default;
		Path& operator=(const Path&) = default;

		bool hasExtension(const char *extension) const;

		const auto& text() const { return _text; }
		bool empty() const { return _text.empty(); }
		const auto* extension() const { return &_text[_extensionOffset]; }

		Path operator/(const char *other) const;
		Path& operator/=(const char *other);

		friend std::ostream& operator<<(std::ostream& out, const Path& path);
	};
}

#endif
