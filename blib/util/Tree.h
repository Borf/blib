#pragma once


namespace blib
{
	namespace util
	{
		template<int childCount, class child>
		class Tree
		{
		public:
			child* children[childCount];


			Tree()
			{
				for (int i = 0; i < childCount; i++)
					children[i] = NULL;
			}

			template<class T>
			void foreach(T op)
			{
				op((child*)this);
				for (int i = 0; i < childCount; i++)
					if (children[i] != NULL)
						children[i]->foreach(op);
			}
			template<class T>
			void foreachLevel(T op, int level = 0)
			{
				op((child*)this, level);
				for (int i = 0; i < childCount; i++)
					if (children[i] != NULL)
						children[i]->foreachLevel(op, level+1);
			}
			std::vector<Tree*> flatten()
			{
				std::vector<Tree*> ret;
				ret.push_back(this);
				for (int i = 0; i < childCount; i++)
					if (children[i] != NULL)
                        ret.insert(ret.end(), children[i]->flatten());
				return ret;
			}
		};
	}
}
