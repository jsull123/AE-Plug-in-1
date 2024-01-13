#ifndef TIME_H
#define TIME_H

#include "AutoLayers.h"
#include <iostream>
#include <string>
using std::string;

struct Time : public A_Time {

	Time(A_long	value = 0, A_u_long	scale = 1) noexcept;

	Time(A_Time time) noexcept;

	string ToString(const A_long frameDuration);

	bool operator <(const Time& time) noexcept;

	bool operator >(const Time& time) noexcept;

	bool operator >=(const Time& time) noexcept;

	bool operator <=(const Time& time) noexcept;

	Time operator +(const Time& time) noexcept;

	Time operator +(const A_long value) noexcept;

	void operator +=(const Time& time) noexcept;

	void operator +=(const A_long value) noexcept;

	bool operator ==(const Time& time) noexcept;

	Time operator /(const A_long value) noexcept;

	Time operator /(const Time& time) noexcept;

	Time operator -(const Time& time) noexcept;
};

struct WorkArea {
	Time start;
	Time duration;

	WorkArea(Time start, Time duration) noexcept {
		this->start = start;
		this->duration = duration;
	}
};

#endif // !TIME_H
