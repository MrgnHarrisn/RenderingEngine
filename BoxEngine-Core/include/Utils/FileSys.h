#pragma once

#include <fstream>
#include <sstream>
#include <string>
#include <iostream>

class FileSys
{
public:
	FileSys();
	/// <summary>
	/// Read contents of a file as human readable
	/// </summary>
	/// <param name="filename">File to read</param>
	/// <returns>string output</returns>
	std::string readContentsH(const char* filename);	// only useing this for now

	/// <summary>
	/// Read contents of a file as raw binary
	/// </summary>
	/// <param name="filename">File to read</param>
	/// <returns></returns>
	// static std::string readContentsB(const char* filename); // not using for now
private:
};