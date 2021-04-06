#include "pch.h"
#include "Reminders.h"


BAKKESMOD_PLUGIN(Reminders, "write a plugin description here", plugin_version, PLUGINTYPE_FREEPLAY)

std::shared_ptr<CVarManagerWrapper> _globalCvarManager;
std::string everyGame = "Take a sip of water";
std::string extraReminder = "Take an eye break, look outside for 20 seconds";

int numGamesExtra = 5;
int currentGame = numGamesExtra;


void Reminders::onLoad() {
	_globalCvarManager = cvarManager;

	cvarManager->registerCvar("reminder_every_game", everyGame, "Sets reminder for every game")
		.addOnValueChanged([this](std::string, CVarWrapper cvar) {
			everyGame = cvar.getStringValue();
			});

	cvarManager->registerCvar("reminder_extra_reminder", extraReminder, "Sets reminder for every x games")
		.addOnValueChanged([this](std::string, CVarWrapper cvar) {
			extraReminder = cvar.getStringValue();
			});

	cvarManager->registerCvar("reminder_extra_reminder_games", std::to_string(numGamesExtra), 
		"Number of games between extra reminders")
		.addOnValueChanged([this](std::string, CVarWrapper cvar) {
			numGamesExtra = cvar.getIntValue();
			if (currentGame > numGamesExtra) {
				currentGame = numGamesExtra;
			}
			});
	
	gameWrapper->HookEvent("Function TAGame.GameEvent_Soccar_TA.EventMatchEnded", [this](std::string eventName) {
		matchEnded();
	});

	cvarManager->executeCommand("cl_notifications_enabled_beta 1");
	
}

void Reminders::onUnload() {
}

void Reminders::matchEnded() {
	gameWrapper->Toast("Reminder", everyGame, "default", 5.0F);

	currentGame--;

	if (currentGame == 0) {
		currentGame = numGamesExtra;
		gameWrapper->Toast("Reminder", extraReminder, "default", 5.0F);
	}
}