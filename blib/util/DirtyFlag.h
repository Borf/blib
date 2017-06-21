#pragma once

namespace blib
{
	namespace util
	{

		template<class T>
		class DirtyFlag
		{
		public:
			T value;
			bool dirty = false;

			DirtyFlag()
			{
				dirty = false;
			}

			DirtyFlag(const T& value)
			{
				this->value = value;
				dirty = false;
			}

			const DirtyFlag<T> & operator = (const DirtyFlag<T> &other)
			{
				this->value = other.value;
				this->dirty = true;
				return other;
			}

			bool isDirty()
			{
				return dirty;
			}

			void clean()
			{
				dirty = false;
			}

			operator T&()
			{
				dirty = true;
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
				dirty = true;
				return &value;
			}
		};

	}
}
d