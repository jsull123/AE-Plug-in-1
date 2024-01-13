#include "Collection.h"

Collection::Collection(AEGP_Collection2H collection2H) {
	this->collection2H = nullptr;
	if (collection2H) {
		this->collection2H = collection2H;
		return;
	}
	AEGP_SuiteHandler suites(sP);
	suites.CollectionSuite2()->AEGP_NewCollection(S_my_id, &this->collection2H);
}

A_u_long Collection::Size() {
	AEGP_SuiteHandler suites(sP);
	
	A_u_long size = 0;
	if (!this->collection2H)
		return size;

	suites.CollectionSuite2()->AEGP_GetCollectionNumItems(this->collection2H, &size);
	return size;
}

AEGP_Collection2H Collection::GetCollection2H() const {
	return this->collection2H;
}

CollectionItem Collection::Get(A_u_long index) const {
	AEGP_SuiteHandler suites(sP);

	AEGP_CollectionItemV2 item = {};
	if (!this->collection2H)
		return CollectionItem(item);

	suites.CollectionSuite2()->AEGP_GetCollectionItemByIndex(this->collection2H, index, &item);
	return CollectionItem(item);
}

void Collection::PushBack(const CollectionItem& collectionItem) {
	AEGP_SuiteHandler suites(sP);

	if (!this->collection2H)
		return;

	suites.CollectionSuite2()->AEGP_CollectionPushBack(this->collection2H, &(collectionItem.GetCollectionItem()));
}

void Collection::Remove(A_u_long index) {
	AEGP_SuiteHandler suites(sP);
	
	if (!this->collection2H || index >= this->Size())
		return;

	suites.CollectionSuite2()->AEGP_CollectionErase(this->collection2H, index, index+1);
}

Collection::operator bool() const noexcept {
	return this->collection2H ? true : false;
}

Collection::~Collection() {
	AEGP_SuiteHandler suites(sP);

	if (this->collection2H) {
		suites.CollectionSuite2()->AEGP_DisposeCollection(this->collection2H);
	}
}

CollectionItem::CollectionItem(const AEGP_CollectionItemV2& collectionItem) noexcept {
	this->collectionItem = collectionItem;
}

CollectionItem::CollectionItem(const Layer& layer) {
	this->collectionItem = {};
	if (!layer)
		return;

	this->collectionItem.type = AEGP_CollectionItemType_LAYER;
	this->collectionItem.u.layer.layerH = layer.GetLayerH();
}

AEGP_CollectionItemV2 CollectionItem::GetCollectionItem() const noexcept {
	return this->collectionItem;
}

AEGP_CollectionItemType CollectionItem::GetType() const noexcept {
	return this->collectionItem.type;
}