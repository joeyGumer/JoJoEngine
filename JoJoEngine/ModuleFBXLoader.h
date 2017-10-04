#ifndef _MODULEFBXLOADER_H_
#define _MODULEFBXLOADER_H_

#include "Module.h"

class ModuleFBXLoader : public Module
{
public:
	ModuleFBXLoader(bool start_enabled = true);
	virtual ~ModuleFBXLoader();

};
#endif // !_MODULEFBXLOADER_H_
