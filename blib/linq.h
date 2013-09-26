#pragma once


namespace blib
{
	namespace linq
	{
		template<class Return, class Iterator, class Operator>
		Return max(Iterator begin, Iterator end, Operator func)
		{
			Return ret = func(*begin);
			for(Iterator it = begin; it != end; it++)
			{
				Return v = func(*it);
				if(v > ret)
					ret = v;
			}
			return ret;
		}

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
		template<class Return, class Storage, class Operator>
		Return min(const Storage& data, Operator func)
		{
			Return ret = func(*std::begin(data));
			for(Storage::const_iterator it = std::begin(data); it != std::end(data); it++)
			{
				Return v = func(*it);
				if(v < ret)
					ret = v;
			}
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
			for(Storage::const_iterator it = std::begin(data); it != std::end(data); it++)
			{
				ret.insert(ret.end(), func(*it));
			}
			return ret;
		}

	}




}