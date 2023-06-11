#include "AstriaException.h"
#include <sstream>


AstriaException::AstriaException(int line, const char* file) noexcept
	:
	line(line),
	file(file)
{}

const char* AstriaException::what() const noexcept
{
	std::ostringstream oss;
	oss << GetType() << std::endl
		<< GetOriginString();
	whatBuffer = oss.str();
	return whatBuffer.c_str();
}

const char* AstriaException::GetType() const noexcept
{
	return "Astria Exception";
}

int AstriaException::GetLine() const noexcept
{
	return line;
}

const std::string& AstriaException::GetFile() const noexcept
{
	return file;
}

std::string AstriaException::GetOriginString() const noexcept
{
	std::ostringstream oss;
	oss << "[File] " << file << std::endl
		<< "[Line] " << line;
	return oss.str();
}