#include <fstream>
#include <string>
#include "model.h"

Model::Model(const char* filename)
{
	std::fstream fou;
	fou.open(filename);
	if (!fou.is_open())
	{
		return;
	}
	std::string line;
	while (!fou.eof()) {
		getline(fou, line);
	}
}
