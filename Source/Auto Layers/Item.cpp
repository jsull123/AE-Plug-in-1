/*
#include "Item.h"

Item::Item(AEGP_ItemH itemH) noexcept {
	this->itemH = itemH;
}

Item::Item(Comp& comp){
	this->itemH = NULL;

	AEGP_CompH compH = comp.GetCompH();
	if (!compH)
		return;

	AEGP_SuiteHandler suites(sP);
	suites.CompSuite11()->AEGP_GetItemFromComp(compH, &this->itemH);
}

AEGP_ItemH Item::GetItemH() {
	return this->itemH;
}

AEGP_ItemType Item::GetItemType() {
	AEGP_SuiteHandler suites(sP);
	AEGP_ItemType itemType = AEGP_ItemType_NONE;

	if (!this->itemH)
		return itemType;

	suites.ItemSuite9()->AEGP_GetItemType(itemH, &itemType);
	
	return itemType;
}

Al_Vec2 Item::GetDimensions() {
	AEGP_SuiteHandler suites(sP);

	Al_Vec2 dimensions;
	suites.ItemSuite9()->AEGP_GetItemDimensions(this->itemH, &dimensions.w, &dimensions.h);
	return dimensions;
}

A_Time Item::GetCurrTime() {
	AEGP_SuiteHandler suites(sP);

	A_Time time = { 0,1 };
	if (!this->itemH)
		return time;

	suites.ItemSuite9()->AEGP_GetItemCurrentTime(this->itemH, &time);
	return time;
}

void Item::SetCurrTime(A_Time time) {
	AEGP_SuiteHandler suites(sP);

	if (!this->itemH)
		return;

	suiteds.ItemSuite9()->AEGP_SetItemCurrentTime(this->itemH, &time);
}
*/