#include <fstream>
#include <string>
#include <sstream>
#include <iostream>
#include "model.h"

Model::Model(const char* filename)
{
	std::fstream fou;
	fou.open(filename);
	if (!fou.is_open())
	{
		return;
	}
	verts_.clear();
	surfaces_.clear();
	std::string line;
	std::stringstream ss;
	Vec3f v;
	while (!fou.eof()) {
		getline(fou, line);
		ss.clear();
		ss.str(line);
		char letter;
		std::vector<int> f;
		int itrash, idx;
		//v xx xx xx vertex data
		if (!line.compare(0, 2, "v "))
		{
			ss >> letter;
			for (int i = 0; i < 3; ++i) ss >> v.raw[i];
			verts_.push_back(v);
		}
		else if (!line.compare(0, 2, "f "))
		{
			ss >> letter;
			f.clear();
			while (ss >> idx >> letter >> itrash >> letter >> itrash)
			{
				idx--;
				f.push_back(idx);
			}
			surfaces_.push_back(f);
		}
	}
	fou.close();
	std::cout << "verts:" << verts_.size() << " face num:" << surfaces_.size() << std::endl;
}

int Model::num_verts()
{
	return verts_.size();
}

int Model::num_faces()
{
	return surfaces_.size();
}

Vec3f Model::vert(int i)
{
	return verts_[i];
}

std::vector<int> Model::face(int i)
{
	return surfaces_[i];
}
