#ifndef __CORE_MODEL_H__
#define __CORE_MODEL_H__

#include <vector>
#include "geometry.h"

class Model {
private:
	std::vector<Vec3f> verts_;
	std::vector<std::vector<Vec3f> > surfaces_;
public:
	Model(const char* filename);
	~Model();
	int num_verts();
	int num_faces();
	Vec3f vert(int i);
	std::vector<int> face(int i);
};


#endif // 
