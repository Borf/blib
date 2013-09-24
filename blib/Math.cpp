#include "Math.h"

namespace blib
{
	namespace math
	{


		double randomDouble()
		{
			return rand() / (double)RAND_MAX;
		}

	}
}