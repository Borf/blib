#include "ResourceManager.h"
#include <blib/Resource.h>
#include <blib/util/Log.h>

#include <assert.h>
using blib::util::Log;

namespace blib
{
	ResourceManager* ResourceManager::manager = nullptr;
	std::map<std::string, Texture*> textureCache;

	void ResourceManager::cleanup()
	{
		for (auto &r : toDelete)
		{
			r.first--;
			if (r.first < 0)
			{
#ifdef _DEBUG
				Log::out << "Deleting " << r.second->name << Log::newline;
#endif
				blib::Texture* texture = dynamic_cast<blib::Texture*>(r.second);
				if (texture)
				{
					for (auto it = textureCache.begin(); it != textureCache.end(); ++it)
						if (it->second == texture)
						{
							textureCache.erase(it);
							break;
						}
				}
				delete r.second;
			}
		}
		toDelete.erase(std::remove_if(toDelete.begin(),	toDelete.end(),
			[](std::pair<int, Resource*> r) {return r.first < 0; }),
			toDelete.end());

	}

	ResourceManager::ResourceManager()
	{
		manager = this;
	}


	ResourceManager::~ResourceManager()
	{
		for (auto it : resources)
			delete it.first;
	}

	ResourceManager& ResourceManager::getInstance()
	{
		return *manager;
	}

	Resource* ResourceManager::regResource(Resource* resource)
	{
		if (resources.find(resource) == resources.end())
			resources[resource] = 0;

		if (std::find_if(toDelete.begin(), toDelete.end(), [resource](std::pair<int, Resource*> &r) { return r.second == resource; }) != toDelete.end())
		{
			toDelete.erase(std::remove_if(toDelete.begin(), toDelete.end(),
				[resource](std::pair<int, Resource*> r) {return r.second == resource; }),
				toDelete.end());
		}

		resources[resource]++;
		return resource;
	}


	void ResourceManager::dispose(Resource* resource)
	{
		//assert(resources.find(resource) != resources.end());
		resources[resource]--;
		if (resources[resource] == 0)
		{
#if defined(_DEBUG) && !defined(BLIB_NX)
			Log::out<<"Marking "<<(int)resource<<"\t"<<resource->name<<Log::newline;
#endif
			assert(resources[resource] == 0);
			toDelete.push_back(std::pair<int, Resource*>(10, resource));

			resources.erase(resources.find(resource));
		}
	}


	void ResourceManager::printDebug()
	{
		for (auto r : resources)
		{
			if (r.second > 0)
			{
				Log::out << r.first->name << " still loaded " << r.second << " times" << Log::newline;
#ifdef RESOURCECALLSTACK
				Log::out << r.first->callstack << Log::newline;
#endif
			}
		}
	}

}
