#pragma once


namespace blib
{
	namespace linq
	{
		template<class Return, class Storage, class Operator>
		Return max(const Storage& data, Operator func)
		{
			Return ret = func(*std::begin(data));
			for(Storage::const_iterator it = std::begin(data); it != std::end(data); it++)
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
			for(Storage::const_iterator it = std::begin(data); it != std::end(data); it++)
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
			for(Storage::const_iterator it = std::begin(data); it != std::end(data); it++)
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
			for(Storage::const_iterator it = std::begin(data); it != std::end(data); it++)
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
			for(Storage::const_iterator it = std::begin(data); it != std::end(data); it++)
				ret += func(*it);
			return ret;
		}


		template<class Storage, class Operator>
		int count(const Storage& data, Operator func)
		{
			int c = 0;
			for(Storage::const_iterator it = std::begin(data); it != std::end(data); it++)
			{
				if(func(*it))
					c++;
			}
			return c;
		}

		template<class Storage, class Operator>
		bool contains(const Storage& data, Operator func)
		{
			for(Storage::const_iterator it = std::begin(data); it != std::end(data); it++)
			{
				if(func(*it))
					return true;
			}
			return false;
		}

		template<class Storage, class Operator>
		bool any(const Storage& data, Operator func)
		{
			for(Storage::const_iterator it = std::begin(data); it != std::end(data); it++)
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
			for(Storage::const_iterator it = std::begin(data); it != std::end(data); it++)
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
			for(Storage::const_iterator it = std::begin(data); it != std::end(data); it++)
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
			for(Storage::const_iterator it = data.cbegin(); it != data.cend(); it++)
			{
				ret.insert(ret.end(), func(*it));
			}
			return ret;
		}
		template<class Storage, class Operator>
		void foreach(const Storage& data, Operator func)
		{
			for(Storage::const_iterator it = data.cbegin(); it != data.cend(); it++)
			{
				func(*it);
			}
		}
		template<class Storage, class Operator>
		void foreach(Storage& data, Operator func)
		{
			for(Storage::iterator it = data.begin(); it != data.end(); it++)
			{
				func(*it);
			}
		}

		template<class Storage>
		void deleteall(Storage& data)
		{
			for(Storage::iterator it = data.begin(); it != data.end(); it++)
			{
				delete *it;
			}
			data.clear();
		}

		template<class Storage, class Func>
		void deletewhere(Storage& data, Func func)
		{
			for(Storage::iterator it = data.begin(); it != data.end(); it++)
			{
				if(func(*it))
				{
					delete *it;
					it = data.erase(it);
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


	}




}