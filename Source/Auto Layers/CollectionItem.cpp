/*
#include "CollectionItem.h"

CollectionItem::CollectionItem(AEGP_CollectionItemV2 collectionItem) noexcept {
	this->collectionItem = collectionItem;
}


CollectionItem::CollectionItem(Layer& layer) {
	this->collectionItem = {};
	if (!layer.GetLayerH())
		return;
	this->collectionItem.type = AEGP_CollectionItemType_LAYER;
	this->collectionItem.u.layer.layerH = layer.GetLayerH();
}

AEGP_CollectionItemV2 CollectionItem::GetCollectionItem() noexcept {
	return this->collectionItem;
}

AEGP_CollectionItemType CollectionItem::GetType() noexcept {
	return this->collectionItem.type;
}*/