#include "Random.h"
#include "Math.h"


float RandomGenerator::RandFloat()
{
	LCG rand;
	return rand.Float();
}

int RandomGenerator::RandInt(int min, int max)
{
	LCG rand;
	return rand.Int(min, max);
}