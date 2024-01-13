#include "Settings.h"

using namespace nlohmann;

static bool B_LOG = false;

json PanelSettings::ReadSettings() {
	ifstream settingsFile(settingsFilePath, ifstream::binary);
	json settings;
	settingsFile >> settings;
	settingsFile.close();
	return settings;
}

PanelSettings::PanelSettings() {
	settings = ReadSettings();
}

void PanelSettings::WriteSettings() {
	ofstream settingsFile(settingsFilePath);
	settingsFile << settings;
	settingsFile.close();
}

Log::Log() {
	if (!B_LOG) return;
	ofstream logFile(logFilePath);
	logFile.close();
}

Log& Log::operator <<(const string data) {
	if (!B_LOG) return *this;
	ifstream iLogFile(logFilePath);
	string contents;
	string line;
	while (std::getline(iLogFile, line)) {
		contents += line + "\n";
	}
	iLogFile.close();

	contents += data;
	ofstream oLogFile(logFilePath);
	oLogFile << contents;
	oLogFile.close();

	return *this;
}

Log& Log::operator <<(const int data) {
	return Log::operator<<(std::to_string(data));
}