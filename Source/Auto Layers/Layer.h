/*#ifndef LAYER_H
#define LAYER_H

#include "AutoLayers.h"
#include "Comp.h"

enum LayerType {
	ADJUSTMENT,
	SOLID,
	NULL_LAYER
};

class Layer {
private:
	AEGP_LayerH layerH;
public:

	Layer(AEGP_LayerH layerH) noexcept;
	
	A_Time GetInPoint(); 

	A_Time GetDuration();

	A_long GetIndex();

	void SetInPointAndDuration(A_Time inPoint, A_Time duration);

	void Reorder(A_long index);

	AEGP_LayerH GetLayerH();

	Comp& GetParentComp();

	//Stream& GetStream(AEGP_LayerStream stream);

	explicit operator bool() noexcept;
};

#endif*/