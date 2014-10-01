#pragma once

#include <map>
#include <vector>
#include <typeinfo>


namespace blib
{
	namespace util
	{
		template<class T>
		class ListNode
		{
		public:
			ListNode<T>* next;
			T* data;
		};

		class ListAllocator
		{
			std::vector<std::function<void()>> resetFuncs;
		public:
			template<class T> 
			T* get()
			{
				static ListNode<T>* firstNode = NULL;
				static ListNode<T>* currentNode = firstNode;

				if (currentNode == NULL)
				{
					currentNode = new ListNode<T>();
					currentNode->data = new T();
					currentNode->next = NULL;
					if (firstNode == NULL)
					{
						firstNode = currentNode;
						resetFuncs.push_back([]()
						{
							currentNode = firstNode;
						});
					}
					return currentNode->data;
				}
				else
				{
					if (currentNode->next)
					{
						currentNode = currentNode->next;
						return currentNode->data;
					}
					else
					{
						currentNode = currentNode->next = new ListNode<T>();
						currentNode->data = new T();
						currentNode->next = NULL;
						return currentNode->data;
					}
				}
			}

			void clear()
			{
				for (size_t i = 0; i < resetFuncs.size(); i++)
					resetFuncs[i]();
			}
		};
	}
}