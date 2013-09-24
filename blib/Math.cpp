#include "Math.h"
#include <stdlib.h>
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