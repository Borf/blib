#include "ResourceManager.h"
#include <blib/Resource.h>

#include <assert.h>

namespace blib
{


	ResourceManager::ResourceManager()
	{

	}

	ResourceManager::~ResourceManager()
	{
		for (auto it : resources)
			delete it.first;
	}

	Resource* ResourceManager::regResource(Resource* resource)
	{
		if (resources.find(resource) == resources.end())
			resources[resource] = 0;
		resources[resource]++;
		return resource;
	}


	void ResourceManager::dispose(Resource* resource)
	{
		assert(resources.find(resource) != resources.end());
		resources[resource]--;
		if (resources[resource] == 0)
		{
			delete resource;
			resources.erase(resources.find(resource));
		}
	}

}