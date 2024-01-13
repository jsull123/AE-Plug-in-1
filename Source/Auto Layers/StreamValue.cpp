/*
#include "StreamValue.h"


StreamValue::StreamValue(AEGP_StreamValue2 streamValue2, A_Time time) noexcept {
	this->streamValue2 = streamValue2;
	this->time = time;
}

A_Time StreamValue::GetTime() noexcept {
	return this->time;
}

StreamValue::~StreamValue() {
	AEGP_SuiteHandler suites(sP);
	suites.StreamSuite5()->AEGP_DisposeStreamValue(&this->streamValue2);
}

MarkerValue::MarkerValue(AEGP_StreamValue2 streamValue2, A_Time time) : StreamValue(streamValue2, time) {
	this->markerP = streamValue2.val.markerP;
}

A_long MarkerValue::GetLabel() {
	AEGP_SuiteHandler suites(sP);

	A_long label = 0;
	if (!this->markerP)
		return label;

	suites.MarkerSuite3()->AEGP_GetMarkerLabel(this->markerP, &label);
	return label;
}

MarkerValue::operator bool() noexcept {
	if (this->markerP)
		return true;
	return false;
}
*/