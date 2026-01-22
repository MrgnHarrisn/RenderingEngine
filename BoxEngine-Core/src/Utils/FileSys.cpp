#include "Utils/FileSys.h"

using namespace std;

FileSys::FileSys()
{
	printf("Do something?");
}

std::string FileSys::readContentsH(const char* filename)
{
	std::ifstream file;
	std::stringstream fileStream;
	file.exceptions(std::ifstream::failbit | std::ifstream::badbit);

	try
	{
		file.open(filename);
		
		fileStream << file.rdbuf();
		file.close();
	}
	catch (ifstream::failure e)
	{
		cout << "FAILED TO READ FILE: " << filename << endl;
	}
	return fileStream.str();
}

//std::string FileSys::readContentsB(const char* filename)
//{
//	return std::string();
//}
