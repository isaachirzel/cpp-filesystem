#include <filesystem>
#include <iostream>

int main()
{
	auto path = std::filesystem::path("../.././.");
	auto abs = std::filesystem::absolute(path);

	std::cout << path << "\n" << abs << "\n";

	return 0;
}
