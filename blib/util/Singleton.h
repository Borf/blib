#pragma once

namespace blib
{
	namespace util
	{

		template <class T>
		class Singleton
		{
		public:
			static T* getInstance()
			{
				static T* t = new T();
				return t;
			}
		};


	}
}
