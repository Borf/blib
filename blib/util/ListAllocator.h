#pragma once

#include <map>
#include <vector>
#include <typeinfo>


namespace blib
{
	namespace util
	{
		class ListAllocator
		{
			std::map<int, std::pair<size_t, std::vector<void*> > > objects;

		public:

			template<class T> 
			T* get()
			{
				static int size = typeid(T).hash_code();
				static bool loaded = false;
				static std::map<int, std::pair<size_t, std::vector<void*> > >::iterator it;
				if (!loaded)
				{
					it = objects.find(size);
					if (it == objects.end())
					{
						objects[size] = std::pair<size_t, std::vector<void*> >(0, std::vector<void*>());
						it = objects.find(size);
					}
					loaded = true;
				}
				static std::pair < size_t, std::vector<void*> > &cache = it->second;
				if (cache.first < cache.second.size())
					return (T*)cache.second[cache.first++];

				T* newValue = new T();

				cache.second.push_back((void*)newValue);
				cache.first++;

				return newValue;
			}

			void clear()
			{
				for (std::map<int, std::pair<size_t, std::vector<void*> > >::iterator it = objects.begin(); it != objects.end(); it++)
					it->second.first = 0;
			}
		};
	}
}