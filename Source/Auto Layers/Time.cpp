#include "Time.h"

Time::Time(A_long value, A_u_long scale) noexcept {
	this->value = value;
	this->scale = scale;
}

Time::Time(A_Time time) noexcept {
	this->value = time.value;
	this->scale = time.scale;
}
// 0:00:00:00
string Time::ToString(const A_long frameDuration) {
	if (frameDuration == 0) return "";
	const int h = this->value / (this->scale * 60 * 60);
	const int m = (this->value % (this->scale * 60 * 60)) / (this->scale * 60);
	const int s = ((this->value % (this->scale * 60 * 60) % (this->scale * 60))) / this->scale;
	const int f = (((this->value % (this->scale * 60 * 60) % (this->scale * 60))) % this->scale) / frameDuration;

	const string hs = std::to_string(h);
	const string ms = ((m / 10) ? "" : "0") + std::to_string(m);
	const string ss = ((s / 10) ? "" : "0") + std::to_string(s);
	const string fs = ((f / 10) ? "" : "0") + std::to_string(f);

	return hs + ":" + ms + ":" + ss + ":" + fs;
}

bool Time::operator <(const Time& time) noexcept {
	if (this->value < time.value) return true;
	return false;
}

bool Time::operator >(const Time& time) noexcept {
	if (this->value > time.value) return true;
	return false;
}

bool Time::operator <=(const Time& time) noexcept {
	if (this->value <= time.value) return true;
	return false;
}

bool Time::operator >=(const Time& time) noexcept {
	if (this->value >= time.value) return true;
	return false;
}

Time Time::operator +(const Time& time) noexcept {
	Time copy(*this);
	copy.value += time.value;
	return copy;
}

Time Time::operator +(const A_long value) noexcept {
	Time copy(*this);
	copy.value += value;
	return copy;
}

void Time::operator +=(const Time& time) noexcept {
	this->value += time.value;
}

void Time::operator +=(const A_long value) noexcept {
	this->value += value;
}

bool Time::operator ==(const Time& time) noexcept {
	if (this->value == time.value && this->scale == time.scale)
		return true;
	return false;
}

Time Time::operator /(const A_long value) noexcept {
	Time copy(*this);
	copy.value /= value;
	return copy;
}

Time Time::operator /(const Time& time) noexcept {
	Time copy(*this);
	copy.value /= time.value;
	return copy;
}

Time Time::operator -(const Time& time) noexcept {
	Time copy(*this);
	copy.value -= time.value;
	return copy;
}