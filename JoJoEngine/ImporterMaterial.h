#ifndef  _IMPORTERMATERIAL_H_
#define _IMPORTERMATERIAL_H_

#include "Importer.h"

#include <string>

namespace ImporterMaterial
{
	bool Import(const char* file_name, std::string& output);
	bool Load(const char* file_name, int * texture);
};

#endif // !
