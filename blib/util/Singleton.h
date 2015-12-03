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


		class DynamicSingleton
		{
		public:
			template<class T>
			static T* getInstance()
			{
				static T* t = new T();
				return t;
			}
		};

	}
}
