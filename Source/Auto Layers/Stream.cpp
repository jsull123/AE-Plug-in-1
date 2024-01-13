#include "Stream.h"
#include "Comp.h"

Stream::Stream(AEGP_StreamRefH streamRefH, AEGP_LayerStream stream, Layer* layer) noexcept {
	this->layer = layer;
	this->streamRefH = streamRefH;
	this->stream = stream;
}

Stream::~Stream() {
	AEGP_SuiteHandler suites(sP);

	if (this->streamRefH)
		suites.StreamSuite5()->AEGP_DisposeStream(this->streamRefH);
}

/*
vector<Marker> Stream::GetMarkers() {
	AEGP_SuiteHandler suites(sP);
	
	vector<Marker> markers;
	if (!this->streamRefH)
		return markers;

	A_long numKeyframes = 0;
	suites.KeyframeSuite4()->AEGP_GetStreamNumKFs(this->streamRefH, &numKeyframes);

	for (AEGP_KeyframeIndex i = 0; i < numKeyframes; i++) {
		A_Time time = { 0 ,1 };
		AEGP_StreamValue2 value = {};
		suites.KeyframeSuite4()->AEGP_GetKeyframeTime(this->streamRefH, i, AEGP_LTimeMode_CompTime, &time);
		suites.KeyframeSuite4()->AEGP_GetNewKeyframeValue(S_my_id, this->streamRefH, i, &value);
		markers.push_back(Marker(value, time));
	}
	return markers;
}*/
/*
Marker::Marker(AEGP_MarkerValP markerP, Time time) noexcept {
	this->markerP = markerP;
	this->time = time;
}
*/
Marker::Marker(const AEGP_StreamValue2& value, Time time) noexcept : 
	Keyframe(value, time) {
}

A_long Marker::GetLabel() {
	AEGP_SuiteHandler suites(sP);

	A_long label = 0;
	if (!this->GetValue().val.markerP)
		return label;

	suites.MarkerSuite3()->AEGP_GetMarkerLabel(this->GetValue().val.markerP, &label);
	return label;
}

/*
Time Marker::GetTime() noexcept {
	return this->time;
}
*/

Keyframe::Keyframe(const AEGP_StreamValue2& value, Time time) noexcept {
	this->value = value;
	this->time = time;
}

Keyframe::Keyframe(Time time) noexcept {
	this->time = time;
}

Time Keyframe::GetTime() const noexcept {
	return this->time;
}

void Keyframe::SetTime(Time time) noexcept {
	this->time = time;
}

bool Keyframe::operator ==(const Keyframe& keyframe) const noexcept {
	if (Time(this->time) == Time(keyframe.time) && this->value.val.one_d == keyframe.value.val.one_d)
		return true;
	return false;
}

AEGP_StreamValue2 Keyframe::GetValue() const noexcept {
	return this->value;
}

Keyframe::operator Marker() noexcept {
	return Marker(this->value, this->time);
}

KeyframeList Stream::GetKeyframes() {
	AEGP_SuiteHandler suites(sP);

	vector<Keyframe> keyframes;
	if (!this->streamRefH)
		return KeyframeList(&keyframes);

	A_long numKeyframes = 0;
	suites.KeyframeSuite4()->AEGP_GetStreamNumKFs(this->streamRefH, &numKeyframes);

	for (AEGP_KeyframeIndex i = 0; i < numKeyframes; i++) {
		A_Time time = { 0, 1 };
		AEGP_StreamValue2 value;
		suites.KeyframeSuite4()->AEGP_GetKeyframeTime(this->streamRefH, i, AEGP_LTimeMode_CompTime, &time);
		suites.KeyframeSuite4()->AEGP_GetNewKeyframeValue(S_my_id, this->streamRefH, i, &value);
		keyframes.push_back(Keyframe(value, time));
	}
	return KeyframeList(&keyframes);
}

A_long Stream::NumKeyframes() {
	AEGP_SuiteHandler suites(sP);

	A_long numKeyframes = 0;
	if (!this->streamRefH)
		return numKeyframes;

	suites.KeyframeSuite4()->AEGP_GetStreamNumKFs(this->streamRefH, &numKeyframes);
	return numKeyframes;
}

void Stream::DeleteKeyframes(int start) {
	DeleteKeyframes(start, NumKeyframes());
}

void Stream::DeleteKeyframes(int start, int end) {
	AEGP_SuiteHandler suites(sP);

	if (!this->streamRefH)
		return;

	const A_long numKeyframes = NumKeyframes();

	if (start < 0 || numKeyframes == 0 
		|| end > numKeyframes|| end <= start)
		return;

	for (AEGP_KeyframeIndex i = 0; i < end-start; i++) {
		suites.KeyframeSuite4()->AEGP_DeleteKeyframe(this->streamRefH, start);
	}
}

void Stream::InsertKeyframe(const AEGP_StreamValue2& streamValue2, Time time) {
	AEGP_SuiteHandler suites(sP);
			
	if (!this->streamRefH)
		return;

	AEGP_KeyframeIndex index = 0;
	suites.KeyframeSuite4()->AEGP_InsertKeyframe(this->streamRefH, AEGP_LTimeMode_CompTime, &time, &index);
	suites.KeyframeSuite4()->AEGP_SetKeyframeValue(this->streamRefH, index, &streamValue2);
}

void Stream::InsertKeyframe(const Keyframe& keyframe) {
	InsertKeyframe(keyframe.GetValue(), keyframe.GetTime());
}

void Stream::SetKeyframes(const KeyframeList& keyframeList) {
	if (keyframeList.Size() == 0) return;
	this->DeleteKeyframes(0);
	for (int i = 0; i < keyframeList.Size(); i++) {
		this->InsertKeyframe(keyframeList.Get(i));
	}
	for (int i = 0; i < this->GetKeyframes().Size(); i++) {
		if (!keyframeList.Contains(this->GetKeyframes().Get(i))) {
			this->DeleteKeyframes(i, i + 1);
		}
	}
}

string Keyframe::ToString(const A_long frameDuration) {
	return "(Keyframe){ " + std::to_string(this->value.val.one_d) + ", " + Time(this->time).ToString(frameDuration) + " }";
}

KeyframeList::KeyframeList(vector<Keyframe>* keyframes) noexcept {
	if (!keyframes) return;
	this->keyframes = *keyframes;
}

int KeyframeList::Size() const noexcept {
	return (int)(this->keyframes.size());
}

Keyframe KeyframeList::Get(int index) const {
	return this->keyframes.at(index);
}

void KeyframeList::Add(const Keyframe* keyframe) {
	if (!keyframe) return;
	this->keyframes.push_back(*keyframe);
}

string KeyframeList::ToString(const A_long frameDuration) {
	string str = "Keyframe List ("+std::to_string(this->keyframes.size())+" elements):\n";

	for (int i = 0; i < this->keyframes.size(); i++) {
		str += "\t" + this->keyframes.at(i).ToString(frameDuration) + "\n";
	}
	return str;
}

bool KeyframeList::Contains(const Keyframe& keyframe) const {
	if (this->keyframes.empty()) return false;

	for (int i = 0; i < this->keyframes.size(); i++) {
		if (this->keyframes.at(i) == keyframe) return true;
	}
	return false;
}

void KeyframeList::Trim(Time start, Time end) {
	for (int i = 0; i < this->keyframes.size(); i++) {
		Keyframe keyframe = this->keyframes.at(i);
		if (Time(keyframe.GetTime()) < start || Time(keyframe.GetTime()) > end) {
			this->keyframes.erase(this->keyframes.begin() + i);
		}
	}
}

void KeyframeList::AdjustTimes(const A_long frameDuration) {
	if (this->keyframes.empty()) return;

	Time time = this->keyframes.at(0).GetTime();

	for (int i = 1; i < this->keyframes.size(); i++) {
		time += frameDuration;
		this->keyframes.at(i).SetTime(time);
	}
}