#pragma once

#include <iterator>
#undef max
#undef min


namespace blib
{
	namespace linq
	{
		template<class Return, class Storage, class Operator>
		Return max(const Storage& data, Operator func)
		{
			Return ret = func(*std::begin(data));
			for(typename Storage::const_iterator it = std::begin(data); it != std::end(data); it++)
			{
				Return v = func(*it);
				if(v > ret)
					ret = v;
			}
			return ret;
		}
		template<class CompareType, class Return, class Storage, class Operator, class Operator2>
		Return max(const Storage& data, Operator comperator, Operator2 toReturn)
		{
			Return ret = toReturn(*std::begin(data));
			CompareType minValue = comperator(*std::begin(data));
			for(typename Storage::const_iterator it = std::begin(data); it != std::end(data); it++)
			{
				CompareType v = comperator(*it);
				if(v > minValue)
				{
					minValue = v;
					ret = toReturn(*it);
				}
			}
			return ret;
		}


		template<class Return, class Storage, class Operator>
		Return min(const Storage& data, Operator func, Return ifEmpty = 0)
		{
			if(data.empty())
				return ifEmpty;

			Return ret = func(*std::begin(data));
			for(typename Storage::const_iterator it = std::begin(data); it != std::end(data); it++)
			{
				Return v = func(*it);
				if(v < ret)
					ret = v;
			}
			return ret;
		}	
		
		template<class CompareType, class Return, class Storage, class Operator, class Operator2>
			Return min(const Storage& data, Operator comperator, Operator2 toReturn)
		{
			Return ret = toReturn(*std::begin(data));
			CompareType minValue = comperator(*std::begin(data));
			for(typename Storage::const_iterator it = std::begin(data); it != std::end(data); it++)
			{
				CompareType v = comperator(*it);
				if(v < minValue)
				{
					minValue = v;
					ret = toReturn(*it);
				}
			}
			return ret;
		}

		template<class Return, class Storage, class Operator>
		Return sum(const Storage& data, Operator func)
		{
			Return ret = 0;
			for(auto it = std::begin(data); it != std::end(data); it++)
				ret += func(*it);
			return ret;
		}


		template<class Storage, class Operator>
		int count(const Storage& data, Operator func)
		{
			int c = 0;
			for(typename Storage::const_iterator it = std::begin(data); it != std::end(data); it++)
			{
				if(func(*it))
					c++;
			}
			return c;
		}

		template<class Storage, class Operator>
		bool contains(const Storage& data, Operator func)
		{
			/*for(typename Storage::const_iterator it = std::begin(data); it != std::end(data); it++)
			{
				if(func(*it))
					return true;
			}*/
			for (auto const& i : data)
				if (func(i))
					return true;

			return false;
		}

		template<class Storage, class ValueType>
		bool containsValue(const Storage& data, const ValueType &value)
		{
			for(typename Storage::const_iterator it = std::begin(data); it != std::end(data); it++)
			{
				if(*it == value)
					return true;
			}
			return false;
		}

		template<class Storage, class Operator>
		bool any(const Storage& data, Operator func)
		{
			for(typename Storage::const_iterator it = std::begin(data); it != std::end(data); it++)
			{
				if(func(*it))
					return true;
			}
			return false;
		}

		template<class Storage, class Operator>
		Storage where(const Storage& data, Operator func)
		{
			Storage ret;
			for(typename Storage::const_iterator it = std::begin(data); it != std::end(data); it++)
			{
				if(func(*it))
					ret.insert(ret.end(), *it);
			}
			return ret;
		}

		template<class Return, class Storage, class Operator>
		Return where(const Storage& data, Operator func)
		{
			Return ret;
			for(typename Storage::const_iterator it = std::begin(data); it != std::end(data); it++)
			{
				if(func(*it))
					ret.insert(ret.end(), *it);
			}
			return ret;
		}

		template<class Return, class Storage, class Operator>
		Return select(const Storage& data, Operator func)
		{
			Return ret;
			for(typename Storage::const_iterator it = data.cbegin(); it != data.cend(); it++)
			{
				ret.insert(ret.end(), func(*it));
			}
			return ret;
		}
		template<class Storage, class Operator>
		void foreach(const Storage& data, Operator func)
		{
			for(typename Storage::const_iterator it = data.cbegin(); it != data.cend(); it++)
			{
				func(*it);
			}
		}
		template<class Storage, class Operator>
		void foreach(Storage& data, Operator func)
		{
			for(typename Storage::iterator it = data.begin(); it != data.end(); it++)
			{
				func(*it);
			}
		}

		template<class Storage>
		void deleteall(Storage& data)
		{
			for(typename Storage::iterator it = data.begin(); it != data.end(); it++)
			{
				delete *it;
			}
			data.clear();
		}

		template<class Storage, class Element>
		int indexOf(const Storage& data, const Element& el)
		{
			int i = 0;
			for (typename Storage::const_iterator it = data.begin(); it != data.end(); it++)
			{
				if (*it == el)
					return i;
				i++;
			}

			return -1;
		}


		template<class Storage, class Func>
		void deletewhere(Storage& data, Func func)
		{
			for (typename Storage::iterator it = data.begin(); it != data.end(); it++)
			{
				if (func(*it))
				{
					delete *it;
					it = data.erase(it);
					if (it == data.end())
						break;
				}
			}
		}


		template<class Storage, class Func>
		void removewhere(Storage& data, Func func)
		{
			for (typename Storage::iterator it = data.begin(); it != data.end(); it++)
			{
				if (func(*it))
				{
					it = data.erase(it);
					if (it == data.end())
						break;
				}
			}
		}
		template<class Return, class A, class B>
		Return merge(const A &a, const B &b)
		{
			Return ret;
			ret.insert(ret.end(), a.begin(), a.end());
			ret.insert(ret.end(), b.begin(), b.end());
			return ret;
		}
		template<class Return, class Storage, class Func>
		Return firstOrDefault(const Storage& data, Func func, Return default_ = NULL)
		{
			for(typename Storage::const_iterator it = std::begin(data); it != std::end(data); it++)
			{
				if(func(*it))
					return *it;
			}
			return default_;
		}


		template<class Storage>
		Storage unique(const Storage &data)
		{
			Storage ret;
			for(typename Storage::const_iterator it = std::begin(data); it != std::end(data); it++)
			{
				if(!containsValue(ret, *it))
					ret.push_back(*it);
			}
			return ret;
		}

		template<class Storage>
		Storage reverse(const Storage &data)
		{
			Storage ret;
			for (typename Storage::const_reverse_iterator it = data.rbegin(); it != data.rend(); it++)
				ret.push_back(*it);
			return ret;
		}

		template<class R, class Q>
		R flatten(const Q &data)
		{
			R ret;
			for (auto row : data)
				for (auto el : row)
					ret.push_back(el);
			return ret;
		}

	}




}