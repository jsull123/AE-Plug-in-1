/*
#ifndef ITEM_H
#define ITEM_H

#include "AutoLayers.h"
#include "Comp.h"

struct Al_Vec2 {
	A_long w;
	A_long h;

	Al_Vec2(A_long w = 0, A_long h = 0) noexcept {
		this->w = w; this->h = h;
	}
};

class Item {
private:
	AEGP_ItemH itemH;
public:
	Item(AEGP_ItemH itemH) noexcept;

	Item(Comp& comp);

	AEGP_ItemType GetItemType();

	AEGP_ItemH GetItemH();

	Al_Vec2 GetDimensions();

	A_Time GetCurrTime();

	void SetCurrTime(A_Time time);
};

#endif // !ITEM_H
*/