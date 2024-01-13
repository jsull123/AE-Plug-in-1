#ifndef COLLECTION_H
#define COLLECTION_H

#include "AutoLayers.h"
#include "Comp.h"
#include <vector>
using std::vector;

class Collection;
class CollectionItem;

class CollectionItem {
private:
	AEGP_CollectionItemV2 collectionItem;
public:
	CollectionItem(const AEGP_CollectionItemV2& collectionItem) noexcept;

	explicit CollectionItem(const Layer& layer);

	AEGP_CollectionItemV2 GetCollectionItem() const noexcept;

	AEGP_CollectionItemType GetType() const noexcept;
};

class Collection {
private:
	AEGP_Collection2H collection2H;
public:

	Collection(AEGP_Collection2H collection2H = nullptr);

	A_u_long Size();

	CollectionItem Get(A_u_long index) const;

	AEGP_Collection2H GetCollection2H() const;

	void PushBack(const CollectionItem& collectionItem);

	void Remove(A_u_long index);

	template <typename T>
	vector<T> GetAll() {
		vector<T> vector;
		return vector;
	}

	template <>
	vector<Layer> GetAll() {
		vector<Layer> vector;

		for (A_u_long i = 0; i < this->Size(); i++) {
			CollectionItem item = this->Get(i);
			if (item.GetType() == AEGP_CollectionItemType_LAYER) {
				vector.push_back(Layer(item.GetCollectionItem().u.layer.layerH));
			}
		}

		return vector;
	}

	operator bool() const noexcept;

	~Collection();
};

#endif