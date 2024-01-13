#ifndef COMMANDS_H
#define COMMANDS_H

#include "AutoLayers.h"
#include "Settings.h"
#include "Collection.h"
#include "Comp.h"
#include "Stream.h"
#include <vector>
using std::vector;

typedef void (*CommandFunc)();

extern vector<int>* RELEASE_WAIT;

extern PanelSettings SETTINGS;

extern Log LOG;

class Command {
protected:
	CommandFunc func;
public:

	Command(CommandFunc func) noexcept;

	virtual bool Check() = 0;

	void Do() noexcept;
};

class KeyCommand : public Command {
private:
	vector<int> keys;

public:

	KeyCommand(CommandFunc func, vector<int> keys);

	bool Check() override;
};

class FileCommand : public Command {
private:
	string commandStr;

public:

	FileCommand(CommandFunc func, string commandStr);

	bool Check() override;
};

namespace KeyBinds {
	static vector<int> addLayers = { VK_SHIFT, 0x56 };
	static vector<int> markerSnapF = { VK_XBUTTON2 };
	static vector<int> markerSnapB = { VK_XBUTTON1 };
	static vector<int> frameNumMarker = { 0xDC };
}

namespace CommandStrings {
	static string crunchKF = "crunchKF";
	static string effectLink = "effectLink";
}

bool IsPressing(vector<int> keys);

extern vector<KeyCommand> commands;

extern vector<FileCommand> fileCommands;

void DoCommands();

#endif // !COMMANDS_H