#include "Commands.h"
#include "Scripts.h"

static PanelSettings SETTINGS;
static Log LOG;
static vector<int>* RELEASE_WAIT = nullptr;

//---Command Functions---//

void EffectLinkFunc() {
	AEGP_SuiteHandler suites(sP);
	suites.UtilitySuite6()->AEGP_ExecuteScript(S_my_id, effectLinkScript, true, 0, 0);
}

void FrameNumMarkerFunc() {
	if (!SETTINGS.GetValueBool(DataConstants::B_BSL_MARKERS)) return;
	AEGP_SuiteHandler suites(sP);
	if (SETTINGS.GetValueBool(DataConstants::B_BSL_COMMENTS))
		suites.UtilitySuite6()->AEGP_ExecuteScript(S_my_id, frameNumMarkerScript, true, 0, 0);
	else
		suites.UtilitySuite6()->AEGP_ExecuteScript(S_my_id, bslMarkerScript, true, 0, 0);
}

void CrunchKFFunc() {
	AEGP_SuiteHandler suites(sP);

	Project project;
	Comp comp(project.GetActiveItem());

	if (!comp)
		return;

	LOG << "Adjusting Keyframes\n";

	suites.UtilitySuite6()->AEGP_StartUndoGroup("crunch kf");

	vector<Layer> selectedLayers = comp.GetSelection().GetAll<Layer>();
	for (int i = 0; i < selectedLayers.size(); i++) {
		Layer layer = selectedLayers.at(i);
		Stream trStream = layer.GetStream(AEGP_LayerStream_TIME_REMAP);

		KeyframeList keyframes = trStream.GetKeyframes();

		Time inPoint = layer.GetInPoint();
		const Time duration = layer.GetDuration();
		const Time frameDuration = comp.GetFrameDuration();

		LOG << "Read Keyframes:\n" << keyframes.ToString(frameDuration.value) << "\n";
		keyframes.Trim(inPoint, inPoint + duration);
		LOG << "Trimmed Keyframes:\n" << keyframes.ToString(frameDuration.value) << "\n";
		keyframes.AdjustTimes(frameDuration.value);
		LOG << "Adjusted Keyframes:\n" << keyframes.ToString(frameDuration.value) << "\n";
		trStream.SetKeyframes(keyframes);
		LOG << "New Keyframes on Stream:\n" << trStream.GetKeyframes().ToString(frameDuration.value) << "\n";
	}

	suites.UtilitySuite6()->AEGP_EndUndoGroup();
}

void AddLayersFunc() {
	const bool bAddLayers = SETTINGS.GetValueBool(DataConstants::B_ADD_LAYERS);
	const bool bCustomDuration = SETTINGS.GetValueBool(DataConstants::B_CUSTOM_DURATION);
	const int customDuration = SETTINGS.GetValueInt(DataConstants::CUSTOM_DURATION);
	const LayerType layerType = (LayerType)SETTINGS.GetValueInt(DataConstants::LAYER_TYPE);

	if (!bAddLayers) 
		return;

	AEGP_SuiteHandler suites(sP);

	Project project;
	Comp comp(project.GetActiveItem());

	if (!comp)
		return;

	LOG << "Adding Layers\n";

	suites.UtilitySuite6()->AEGP_StartUndoGroup("add layers");
	
	vector<Layer> selectedLayers = comp.GetSelection().GetAll<Layer>();
	
	Collection switchSelect;
	for (int i = 0; i < selectedLayers.size(); i++) {
		LOG << "Iteration " << i << ":\n";
		Layer selectedLayer = selectedLayers.at(i);
		LOG << "Selected Layer:\n" << selectedLayer.ToString() << "\n";
		Layer newLayer = comp.CreateLayer(layerType);
		LOG << "New Layer (Not adjusted):\n" << newLayer.ToString() << "\n";

		if (!selectedLayer || !newLayer) continue;
		
		A_Time duration = selectedLayer.GetDuration();
		if (bCustomDuration) {
			duration = { comp.GetFrameDuration().value * customDuration,
							comp.GetFrameDuration().scale };
		}

		LOG << "Setting Duration to:\n" << Time(duration).ToString(comp.GetFrameDuration().value) << "\n";
		
		newLayer.SetInPointAndDuration(selectedLayer.GetInPoint(), duration);
		newLayer.Reorder(selectedLayer.GetIndex()-1);

		LOG << "New Layer (Adjusted):\n" << newLayer.ToString() << "\n";

		switchSelect.PushBack(CollectionItem(newLayer));
	}

	if (switchSelect.Size() > 0)
		comp.SetSelection(switchSelect);

	suites.UtilitySuite6()->AEGP_EndUndoGroup();
	
}

void MarkerSnap(bool b) {
	const bool bMarkerSnap = SETTINGS.GetValueBool(DataConstants::B_MARKER_SNAP);

	if (!bMarkerSnap) return;

	Project project;
	Comp comp(project.GetActiveItem());

	if (!comp)
		return;

	vector<Layer> layers = comp.GetLayers();
	KeyframeList snapMarkers;
	for (int i = 0; i < layers.size(); i++) {
		Layer layer = layers.at(i);
		Stream stream = layer.GetStream(AEGP_LayerStream_MARKER);
		KeyframeList markers = stream.GetKeyframes();
		if (markers.Size() == 0) continue;
		A_long label = ((Marker)(markers.Get(0))).GetLabel();
		if (label == AEGP_Label_1) {
			snapMarkers = markers;
			break;
		}
	}

	if (snapMarkers.Size() == 0) return;

	if (b) {
		for (int i = snapMarkers.Size() - 1; i >= 0; i--) {
			A_Time markerTime = snapMarkers.Get(i).GetTime();
			if (markerTime.value < Item(comp).GetCurrTime().value) {
				Item(comp).SetCurrTime(markerTime);
				return;
			}
		}
	}
	else {
		for (int i = 0; i < snapMarkers.Size(); i++) {
			A_Time markerTime = snapMarkers.Get(i).GetTime();
			if (markerTime.value > Item(comp).GetCurrTime().value) {
				Item(comp).SetCurrTime(markerTime);
				return;
			}
		}
	}
}

void MarkerSnapFFunc() {
	MarkerSnap(false);
}

void MarkerSnapBFunc() {
	MarkerSnap(true);
}

static vector<KeyCommand> commands = {
	KeyCommand((CommandFunc)AddLayersFunc, KeyBinds::addLayers),
	KeyCommand((CommandFunc)MarkerSnapFFunc, KeyBinds::markerSnapF),
	KeyCommand((CommandFunc)MarkerSnapBFunc, KeyBinds::markerSnapB),
	KeyCommand((CommandFunc)FrameNumMarkerFunc, KeyBinds::frameNumMarker)
};

static vector<FileCommand> fileCommands = {
	FileCommand((CommandFunc)CrunchKFFunc, CommandStrings::crunchKF),
	FileCommand((CommandFunc)EffectLinkFunc, CommandStrings::effectLink)
};

Command::Command(CommandFunc func) noexcept {
	this->func = func;
}

void Command::Do() noexcept {
	func();
}

KeyCommand::KeyCommand(CommandFunc func, vector<int> keys) : Command(func) {
	this->keys = keys;
}

bool KeyCommand::Check() {
	if (!IsPressing(keys) && RELEASE_WAIT == &keys) {
		RELEASE_WAIT = nullptr;
	}
	if (IsPressing(keys) && !RELEASE_WAIT) {
		RELEASE_WAIT = &keys;
		return true;
	}
	return false;
}

FileCommand::FileCommand(CommandFunc func, string commandStr) : Command(func) {
	this->commandStr = commandStr;
}

bool FileCommand::Check() {
	if (SETTINGS.GetValueString(DataConstants::COMMAND_CALL) == this->commandStr) {
		SETTINGS.SetValue(DataConstants::COMMAND_CALL, "");
		return true;
	}
	return false;
}

bool IsPressing(vector<int> keys) {
	for (int i = 0; i < keys.size(); i++) {
		if (!GetAsyncKeyState(keys.at(i)))
			return false;
	}
	return true;
}

void DoCommands() {
	for (int i = 0; i < commands.size(); i++) {
		if (commands.at(i).Check())
			commands.at(i).Do();
	}
	for (int i = 0; i < fileCommands.size(); i++) {
		if (fileCommands.at(i).Check())
			fileCommands.at(i).Do();
	}
}
