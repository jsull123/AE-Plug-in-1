#ifndef STREAM_H
#define STREAM_H

#include "AutoLayers.h"
#include "Time.h"
#include <iostream>
#include <string>
#include <vector>
using std::vector;
using std::string;

class Layer;
class Marker;

/*
class Marker {
private:
	AEGP_MarkerValP markerP;
	Time time;
public:

	Marker(AEGP_MarkerValP markerP, Time time) noexcept;
	
	A_long GetLabel();

	Time GetTime() noexcept;
};
*/
class Keyframe {
private:
	AEGP_StreamValue2 value;
	Time time;
public:

	Keyframe(const AEGP_StreamValue2& value, Time time) noexcept;

	Keyframe(Time time = {0,1}) noexcept;

	Time GetTime() const noexcept;

	void SetTime(Time time) noexcept;

	AEGP_StreamValue2 GetValue() const noexcept;

	string ToString(A_long frameDuration);

	bool operator ==(const Keyframe& keyframe) const noexcept;

	operator Marker() noexcept;
};

class Marker : public Keyframe {
private:
public:
	Marker(const AEGP_StreamValue2& value, Time time) noexcept;
	
	A_long GetLabel();
};

class KeyframeList {
private:
	vector<Keyframe> keyframes;
public:

	KeyframeList(vector<Keyframe>* keyframes = nullptr) noexcept;

	int Size() const noexcept;

	Keyframe Get(int index) const;

	void Trim(Time start, Time end);
	
	void AdjustTimes(A_long frameDuration);

	void Add(const Keyframe* keyframe);
	
	bool Contains(const Keyframe& keyframe) const;

	string ToString(const A_long frameDuration);
};

class Stream {
private:
	Layer* layer;
	AEGP_StreamRefH streamRefH;
	AEGP_LayerStream stream;
public:

	explicit Stream(AEGP_StreamRefH streamRefH, AEGP_LayerStream stream, Layer* layer) noexcept;

	/* Temp */
	//vector<Marker> GetMarkers();

	KeyframeList GetKeyframes();

	A_long NumKeyframes();

	/*
	* Delete all from start to end(exclusive)
	*/
	void DeleteKeyframes(int start, int end);

	/*
	* Delete all at and after
	*/
	void DeleteKeyframes(int start);

	void InsertKeyframe(const AEGP_StreamValue2& streamValue2, Time time);

	void InsertKeyframe(const Keyframe& keyframe);

	void SetKeyframes(const KeyframeList& keyframeList);

	~Stream();
};

#endif