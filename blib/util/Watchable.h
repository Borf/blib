#pragma once
#include <functional>

namespace blib
{
	namespace util
	{

		template<class T>
		class Watchable
		{
		public:
			T value;
			std::function<void(const T& old, const T& newValue)> callback;

			Watchable()
			{
				
			}

			Watchable(const T& value)
			{
				this->value = value;
			}

			const Watchable<T> & operator = (const Watchable<T> &other)
			{
				if (this->value != other.value)
				{
					const T oldValue = this->value;
					this->value = other.value;
					if(callback)
						callback(oldValue, value);
				}
				return other;
			}

			operator T&()
			{
				/*if(callback)
					callback(value, value); //TODO, proper values*/
				return value;
			}
			operator const T&() const
			{
				return value;
			}

			const T* operator ->() const
			{
				return &value;
			}

			T* operator ->()
			{
				callback(value, value);
				return &value;
			}
		};

	}
}
