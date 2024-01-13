#include "Comp.h"
#include "Collection.h"
#include "Stream.h"

Item Project::GetActiveItem() {
	AEGP_SuiteHandler suites(sP);

	AEGP_ItemH itemH = NULL;
	suites.ItemSuite9()->AEGP_GetActiveItem(&itemH);

	return Item(itemH);
}

Comp::Comp(AEGP_CompH compH) noexcept {
	this->compH = compH;
}

Comp::Comp(const Item& item) {
	AEGP_SuiteHandler suites(sP);

	this->compH = NULL;
	if (!item)
		return;
	if (item.GetItemType() != AEGP_ItemType_COMP)
		return;

	suites.CompSuite11()->AEGP_GetCompFromItem(item.GetItemH(), &this->compH);
}

Time Comp::GetFrameDuration() const {
	AEGP_SuiteHandler suites(sP);
	
	A_Time frameDuration = { 0,1 };
	if (!this->compH)
		return frameDuration;

	suites.CompSuite11()->AEGP_GetCompFrameDuration(this->compH, &frameDuration);
	return frameDuration;
}

WorkArea Comp::GetWorkArea() const {
	AEGP_SuiteHandler suites(sP);

	WorkArea workarea({0,1}, {0,1});
	if (!this->compH)
		return workarea;

	suites.CompSuite11()->AEGP_GetCompWorkAreaStart(this->compH, &workarea.start);
	suites.CompSuite11()->AEGP_GetCompWorkAreaDuration(this->compH, &workarea.duration);
	return workarea;
}

Item::operator bool() const noexcept{
	return this->itemH ? true : false;
}

u16string U8ToU16(string u8) {
	std::wstring_convert<std::codecvt<char16_t, char, std::mbstate_t>, char16_t> convert;
	std::u16string u16 = convert.from_bytes(u8);
	return u16;
}

Layer Comp::CreateLayer(LayerType layerType) {
	AEGP_SuiteHandler suites(sP);

	AEGP_LayerH layerH = NULL;

	if (!this->compH)
		return nullptr;

	u16string name = U8ToU16(u8"Layer");

	switch (layerType)
	{
	case ADJUSTMENT:
		name = U8ToU16(u8"Adjustment Layer");
		break;
	case SOLID:
		name = U8ToU16(u8"Solid Layer");
		break;
	case NULL_LAYER:
		name = U8ToU16(u8"Null Layer");
	default:
		break;
	}

	//--Name thing--//
	/*
	string u8;
	switch (layerType)
	{
	case ADJUSTMENT:
		u8 = u8"Adjustment Layer";
		break;
	case SOLID:
		u8 = u8"Solid Layer";
		break;
	case NULL_LAYER:
		u8 = u8"Null Layer";
	default:
		break;
	}
	std::wstring_convert<std::codecvt<char16_t, char, std::mbstate_t>, char16_t> convert;
	std::u16string u16 = convert.from_bytes(u8);
	*/
	//--------------//

	AEGP_ColorVal color = { 255,255,255,255 };

	if (layerType == NULL_LAYER) {
		suites.CompSuite11()->AEGP_CreateNullInComp((A_UTF16Char*)name.c_str(),
			this->compH, &this->GetFrameDuration(), &layerH);
	}
	else {
		suites.CompSuite11()->AEGP_CreateSolidInComp((A_UTF16Char*)name.c_str(),
			Item(*this).GetDimensions().w, Item(*this).GetDimensions().h, 
			&color, this->compH, &this->GetFrameDuration(), &layerH);
	}

	if (layerType == ADJUSTMENT) {
		suites.LayerSuite8()->AEGP_SetLayerFlag(layerH, AEGP_LayerFlag_ADJUSTMENT_LAYER, 1);
		suites.LayerSuite8()->AEGP_SetLayerLabel(layerH, AEGP_Label_5);
	}

	return Layer(layerH);
}

Collection Comp::GetSelection() {
	AEGP_SuiteHandler suites(sP);
	
	AEGP_Collection2H collection2H = {};
	if (!this->compH)
		return Collection(collection2H);

	suites.CompSuite11()->AEGP_GetNewCollectionFromCompSelection(S_my_id, this->compH, &collection2H);
	return Collection(collection2H);
}


void Comp::SetSelection(const Collection& collection) {
	AEGP_SuiteHandler suites(sP);

	if (!this->compH || !collection)
		return;

	suites.CompSuite11()->AEGP_SetSelection(this->compH, collection.GetCollection2H());
}

AEGP_CompH Comp::GetCompH() const noexcept {
	return this->compH;
}

Comp::operator bool() const noexcept {
	return this->compH ? true : false;
}

vector<Layer> Comp::GetLayers() {
	AEGP_SuiteHandler suites(sP);

	vector<Layer> layers;
	A_long numLayers = 0;

	if (!this->compH)
		return layers;

	suites.LayerSuite8()->AEGP_GetCompNumLayers(this->compH, &numLayers);

	for (A_long i = 0; i < numLayers; i++) {
		AEGP_LayerH layerH = NULL;
		suites.LayerSuite8()->AEGP_GetCompLayerByIndex(this->compH, i, &layerH);

		if (!layerH) continue;

		layers.push_back(Layer(layerH));
	}

	return layers;
}

Layer::Layer(AEGP_LayerH layerH) noexcept {
	this->layerH = layerH;
}

Time Layer::GetInPoint() {
	AEGP_SuiteHandler suites(sP);

	A_Time inPoint = { 0,1 };
	if (!this->layerH)
		return inPoint;

	Comp comp = GetParentComp();
	if (!comp)
		return inPoint;

	suites.LayerSuite8()->AEGP_GetLayerInPoint(this->layerH, AEGP_LTimeMode_CompTime, &inPoint);
	if (inPoint.scale != comp.GetFrameDuration().scale) {
		inPoint.value = (A_long)((float)((float)inPoint.value / (float)inPoint.scale) * (float)(comp.GetFrameDuration().scale));
		inPoint.scale = comp.GetFrameDuration().scale;
	}
	return inPoint;
}

Time Layer::GetDuration() {
	AEGP_SuiteHandler suites(sP);

	A_Time duration = { 0,1 };
	if (!this->layerH)
		return duration;

	Comp comp = GetParentComp();
	if (!comp)
		return duration;

	suites.LayerSuite8()->AEGP_GetLayerDuration(this->layerH, AEGP_LTimeMode_CompTime, &duration);
	if (duration.scale != comp.GetFrameDuration().scale) {
		duration.value = (A_long)((float)((float)duration.value / (float)duration.scale) * (float)(comp.GetFrameDuration().scale));
		duration.scale = comp.GetFrameDuration().scale;
	}
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

void Layer::SetInPointAndDuration(Time inPoint, Time duration) {
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

AEGP_LayerH Layer::GetLayerH() const noexcept {
	return this->layerH;
}

Comp Layer::GetParentComp() const {
	AEGP_SuiteHandler suites(sP);

	AEGP_CompH compH = NULL;
	
	if (!this->layerH)
		return Comp(compH);

	suites.LayerSuite8()->AEGP_GetLayerParentComp(this->layerH, &compH);
	
	return Comp(compH);
}

Stream Layer::GetStream(AEGP_LayerStream stream) {
	AEGP_SuiteHandler suites(sP);

	AEGP_StreamRefH streamRefH = NULL;
	if (!this->layerH)
		return Stream(streamRefH, stream, this);

	suites.StreamSuite5()->AEGP_GetNewLayerStream(S_my_id, this->layerH, stream, &streamRefH);
	return Stream(streamRefH, stream, this);
}

Layer::operator bool() const noexcept {
	return this->layerH ? true : false;
}

/* Item */
Item::Item(AEGP_ItemH itemH) noexcept {
	this->itemH = itemH;
}

Item::Item(const Comp& comp) {
	this->itemH = NULL;
	if (!comp)
		return;

	AEGP_CompH compH = comp.GetCompH();

	AEGP_SuiteHandler suites(sP);
	suites.CompSuite11()->AEGP_GetItemFromComp(compH, &this->itemH);
}

AEGP_ItemH Item::GetItemH() const noexcept {
	return this->itemH;
}

AEGP_ItemType Item::GetItemType() const {
	AEGP_SuiteHandler suites(sP);
	AEGP_ItemType itemType = AEGP_ItemType_NONE;

	if (!this->itemH)
		return itemType;

	suites.ItemSuite9()->AEGP_GetItemType(itemH, &itemType);

	return itemType;
}

Al_Vec2 Item::GetDimensions() const {
	AEGP_SuiteHandler suites(sP);

	Al_Vec2 dimensions;
	suites.ItemSuite9()->AEGP_GetItemDimensions(this->itemH, &dimensions.w, &dimensions.h);
	return dimensions;
}

Time Item::GetCurrTime() const {
	AEGP_SuiteHandler suites(sP);

	Time time = { 0,1 };
	if (!this->itemH)
		return time;

	suites.ItemSuite9()->AEGP_GetItemCurrentTime(this->itemH, &time);
	return time;
}

void Item::SetCurrTime(Time time) {
	AEGP_SuiteHandler suites(sP);

	if (!this->itemH)
		return;

	suites.ItemSuite9()->AEGP_SetItemCurrentTime(this->itemH, &time);
}

string Layer::ToString() {
	Comp comp = GetParentComp();
	if (!comp) return "";
	const Time fd = comp.GetFrameDuration();

	const string inPointS = GetInPoint().ToString(fd.value);
	const string durationS = GetDuration().ToString(fd.value);
	const string indexS = std::to_string(GetIndex());
	
	string s = "(Layer){ " + (string)"(In Point) " + inPointS + ", (Duration) " + durationS + ", (Index) " + indexS + " }";
	return s;
}

LayerList::LayerList(vector<Layer>* layers) noexcept {
	if (!layers) return;
	this->layers = *layers;
}

int LayerList::Size() noexcept {
	return (int)(this->layers.size());
}

Layer LayerList::Get(int index) {
	return this->layers.at(index);
}

void LayerList::Add(const Layer& layer) {
	if (!layer) return;
	this->layers.push_back(layer);
}

string LayerList::ToString() {
	string str = "Layer List (" + std::to_string(this->layers.size()) + " elements):\n";

	for (int i = 0; i < this->layers.size(); i++) {
		str += "\t" + this->layers.at(i).ToString() + "\n";
	}
	return str;
}

Time Layer::GetOffset() const {
	AEGP_SuiteHandler suites(sP);

	Time offset;
	if (!this->layerH)
		return offset;

	suites.LayerSuite8()->AEGP_GetLayerOffset(this->layerH, &offset);
	return offset;
}

void Layer::SetOffset(const Time offset) {
	AEGP_SuiteHandler suites(sP);

	if (!this->layerH)
		return;
	
	suites.LayerSuite8()->AEGP_SetLayerOffset(this->layerH, &offset);
}