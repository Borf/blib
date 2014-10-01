#pragma once

#include <map>
#include <vector>
#include <typeinfo>


namespace blib
{
	namespace util
	{
		class ListNodeBase
		{
		public:
			ListNodeBase* next;
		};

		template<class T>
		class ListNode : public ListNodeBase
		{
		public:
			T* data;
		};


		class ListAllocator
		{
			static int typeCount;
		public:

			std::vector<ListNodeBase*> data;
			std::vector<ListNodeBase*> first;


			template<class T> 
			T* get()
			{
				static int typeId = -1;
				if (typeId == -1)
					typeId = typeCount++;

				if (typeId >= (int)data.size())
				{
					data.resize(typeId + 1, NULL);
					first.resize(typeId + 1, NULL);
				}

				if (data[typeId] == NULL) // if first
				{
					ListNode<T>* node = new ListNode<T>();
					first[typeId] = data[typeId] = node;
					node->next = NULL;
					node->data = new T();
					return node->data;
				}
				
				if (data[typeId]->next) //if next data is allocated
				{
					data[typeId] = data[typeId]->next;
					return ((ListNode<T>*)data[typeId])->data;
				}

				ListNode<T>* newNode = new ListNode<T>();
				newNode->data = new T();
				newNode->next = NULL;
				data[typeId]->next = newNode;
				data[typeId] = newNode;
				return newNode->data;
			}

			void clear()
			{
				for (size_t i = 0; i < data.size(); i++)
				{
					data[i] = first[i];
				}
			}
		};
	}
}