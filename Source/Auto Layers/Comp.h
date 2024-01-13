#ifndef COMP_H
#define COMP_H

#include "AutoLayers.h"
#include "Time.h"
#include <vector>
#include <codecvt>
using std::vector;
using std::string;
using std::u16string;

class Layer;
class Item;
class Comp;
class Collection;
class Stream;
struct Time;

enum LayerType {
	ADJUSTMENT,
	SOLID,
	NULL_LAYER
};

struct Al_Vec2 {
	A_long w;
	A_long h;

	Al_Vec2(A_long w = 0, A_long h = 0) noexcept {
		this->w = w; this->h = h;
	}
};

u16string U8ToU16(string u8);

class Project {
private:
public:
	Item GetActiveItem();
};

class Item {
private:
	AEGP_ItemH itemH;
public:
	Item(AEGP_ItemH itemH) noexcept;

	Item(const Comp& comp);

	AEGP_ItemType GetItemType() const;

	AEGP_ItemH GetItemH() const noexcept;

	Al_Vec2 GetDimensions() const;

	Time GetCurrTime() const;

	void SetCurrTime(Time time);

	operator bool() const noexcept;
};

class Comp {
private:
	AEGP_CompH compH;
public:

	Comp(const Item& item);

	Comp(AEGP_CompH compH) noexcept;
	
	Layer CreateLayer(LayerType layerType);

	Time GetFrameDuration() const;

	Collection GetSelection();

	WorkArea GetWorkArea() const;

	void SetSelection(const Collection& collection);

	AEGP_CompH GetCompH() const noexcept;

	operator bool() const noexcept;

	vector<Layer> GetLayers();
};

class Layer {
private:
	AEGP_LayerH layerH;
public:

	Layer(AEGP_LayerH layerH) noexcept;
	
	Time GetInPoint();

	Time GetDuration();

	A_long GetIndex();

	void SetInPointAndDuration(Time inPoint, Time duration);

	Time GetOffset() const;

	void SetOffset(const Time offset);

	void Reorder(A_long index);

	AEGP_LayerH GetLayerH() const noexcept;

	Comp GetParentComp() const;
	
	Stream GetStream(AEGP_LayerStream stream);

	operator bool() const noexcept;

	string ToString();
};

class LayerList {
private:
	vector<Layer> layers;
public:

	LayerList(vector<Layer>* layers = nullptr) noexcept;

	int Size() noexcept;

	Layer Get(int index);
	
	void Add(const Layer& layer);

	string ToString();
};

#endif // !COMP_H