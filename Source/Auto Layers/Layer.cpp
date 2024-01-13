/*#include "Layer.h"

Layer::Layer(AEGP_LayerH layerH) noexcept {
	this->layerH = layerH;
}

A_Time Layer::GetInPoint() {
	AEGP_SuiteHandler suites(sP);
	
	A_Time inPoint = { 0,1 };
	if (!this->layerH)
		return inPoint;

	suites.LayerSuite8()->AEGP_GetLayerInPoint(this->layerH, AEGP_LTimeMode_CompTime, &inPoint);
	return inPoint;
}

A_Time Layer::GetDuration() {
	AEGP_SuiteHandler suites(sP);

	A_Time duration = { 0,1 };
	if (!this->layerH)
		return duration;

	suites.LayerSuite8()->AEGP_GetLayerDuration(this->layerH, AEGP_LTimeMode_CompTime, &duration);
	return duration;
}

A_long Layer::GetIndex() {
	AEGP_SuiteHandler suites(sP);

	A_long index = 0;
	if (!this->layerH)
		return index;

	suites.LayerSuite8()->AEGP_GetLayerIndex(this->layerH, &index);
	return index;
}

void Layer::SetInPointAndDuration(A_Time inPoint, A_Time duration) {
	AEGP_SuiteHandler suites(sP);

	if (!this->layerH)
		return;

	suites.LayerSuite8()->AEGP_SetLayerInPointAndDuration(this->layerH, AEGP_LTimeMode_CompTime, &inPoint, &duration);
}

void Layer::Reorder(A_long index) {
	AEGP_SuiteHandler suites(sP);

	if (!this->layerH)
		return;

	suites.LayerSuite8()->AEGP_ReorderLayer(this->layerH, index);
}

AEGP_LayerH Layer::GetLayerH() {
	return this->layerH;
}

Comp& Layer::GetParentComp() {
	AEGP_SuiteHandler suites(sP);

	AEGP_CompH compH = NULL;

	if (!this->layerH)
		return Comp(compH);

	suites.LayerSuite8()->AEGP_GetLayerParentComp(this->layerH, &compH);

	return Comp(compH);
}
/*
Stream& Layer::GetStream(AEGP_LayerStream stream) {
	AEGP_SuiteHandler suites(sP);

	AEGP_StreamRefH streamRefH = NULL;
	if (!this->layerH)
		return Stream(streamRefH, *this);
	
	suites.StreamSuite5()->AEGP_GetNewLayerStream(S_my_id, this->layerH, stream, &streamRefH);
	return Stream(streamRefH, *this);
}

Layer::operator bool() noexcept {
	return this->layerH ? true : false;
}
*/
