#pragma once

#include <map>
#include <vector>
#include <typeinfo>
#include <blib/Util.h>


namespace blib
{
	namespace util
	{
		class ListNodeBase
		{
		public:
			bool isMain;
			ListNodeBase* next;
			virtual ~ListNodeBase() {};
		};

		template<class T>
		class ListNode : public ListNodeBase
		{
		public:
			T* data;
			virtual ~ListNode() { if (isMain) { delete data; } }
		};


		class ListAllocator
		{
			static int typeCount;
		public:

			std::vector<ListNodeBase*> data;
			std::vector<ListNodeBase*> first;

			~ListAllocator()
			{
				for (auto a : first)
				{
					ListNodeBase* b = a;
					while (b)
					{
						ListNodeBase* c = b->next;
						delete b;
						b = c;
					}
				}
				first.clear();
				data.clear();
			}


			template<class T> 
			T* get()
			{
				static bool isMain = blib::util::isMainModule();
				static int typeId = -1;
				if (typeId == -1)
					typeId = typeCount++;

				if (typeId >= (int)data.size())
				{
					data.resize(typeId + 1, nullptr);
					first.resize(typeId + 1, nullptr);
				}

				if (data[typeId] == nullptr) // if first
				{
					ListNode<T>* node = new ListNode<T>();
					first[typeId] = data[typeId] = node;
					node->next = nullptr;
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
				newNode->isMain = isMain;
				newNode->next = nullptr;
				data[typeId]->next = newNode;
				data[typeId] = newNode;
				return newNode->data;
			}

			void clear()
			{
				for (std::size_t i = 0; i < data.size(); i++)
				{
					data[i] = first[i];
				}
			}
		};
	}
}