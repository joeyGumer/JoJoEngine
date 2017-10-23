#ifndef  _IMPORTERMATERIAL_H_
#define _IMPORTERMATERIAL_H_

#include "Importer.h"

#include <string>

class Importer::ImporterMaterial
{
public:
	bool Import(const char* file_name);
	bool Load(const char* file_name, int * texture);

private:


};

#endif // !
