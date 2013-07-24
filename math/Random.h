#pragma once

namespace blib
{
	namespace math
	{

		class Random
		{
		public:
			double nextDouble();
			float nextFloat();
			int nextInt(int max);
		};


	}
}
