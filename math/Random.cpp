#include <blib/math/Random.h>

#include <stdlib.h>


float blib::math::Random::nextFloat()
{
	return rand() / (float)RAND_MAX;
}
