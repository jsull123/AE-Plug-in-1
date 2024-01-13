#ifndef SETTINGS_H
#define SETTINGS_H

#include <fstream>
#include <iostream>
#include "json.hpp"

using namespace nlohmann;
using std::string;
using std::ifstream;
using std::ofstream;
using std::ostream;

extern bool B_LOG;

namespace DataConstants {
	const static string COMMAND_CALL = "commandCall";
	const static string LAYER_TYPE = "layerType";
	const static string B_ADD_LAYERS = "bAddLayers";
	const static string B_CUSTOM_DURATION = "bCustomDuration";
	const static string CUSTOM_DURATION = "customDuration";
	const static string B_MARKER_SNAP = "bMarkerSnap";
	const static string B_BSL_MARKERS = "bBSLMarkers";
	const static string B_BSL_COMMENTS = "bBSLComments";
};

class PanelSettings {
private:
	const string settingsFilePath = "C:\\Users\\DeMarcus Cousins\\Documents\\ae plugins (dev)\\newest plugin\\settings\\settings.json";

	json settings;

	json ReadSettings();

	void WriteSettings();
public:

	PanelSettings();

	int GetValueInt(string key) {
		settings = ReadSettings();
		if (!settings.contains(key))
			return 0;
		return settings[key].get<int>();
	}

	bool GetValueBool(string key) {
		settings = ReadSettings();
		if (!settings.contains(key))
			return false;
		return settings[key].get<bool>();
	}

	string GetValueString(string key) {
		settings = ReadSettings();
		if (!settings.contains(key))
			return "";
		return settings[key].get<string>();
	}

	template <typename T>
	void SetValue(string key, T value) {
		settings = ReadSettings();
		if (!settings.contains(key))
			return;
		settings[key] = value;
		WriteSettings();
	}
};

class Log {
private:
	const string logFilePath = "C:\\Users\\DeMarcus Cousins\\Desktop\\log.txt";
public:

	/* Clear log file */
	Log();

	/* Append data to log */
	Log& operator <<(const string data);

	Log& operator <<(const int data);
};

#endif