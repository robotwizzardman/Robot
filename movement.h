#pragma once
#include <math.h>
#include <errno.h>
#include <iostream>

void move_slowly(int zmienna, int cel,int step) {
	if (step == 0)

		throw std::runtime_error("debilu ! nie mo¿na dzieliæ przez zero");
	if (zmienna < cel)
		int delta = abs(zmienna - cel) / step;
	else

}