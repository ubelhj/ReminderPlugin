#include "pch.h"
#include "Reminders.h"


BAKKESMOD_PLUGIN(Reminders, "Reminders", plugin_version, PLUGINTYPE_FREEPLAY)

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
			currentGame = numGamesExtra;
			});
	
	gameWrapper->HookEventPost("Function TAGame.GameEvent_Soccar_TA.EventMatchEnded", [this](std::string eventName) {
		matchEnded();
	});

	
	
}

void Reminders::onUnload() {
}

void Reminders::matchEnded() {
	bool prevValue = cvarManager->getCvar("cl_notifications_enabled_beta").getBoolValue();
	cvarManager->executeCommand("cl_notifications_enabled_beta 1");

	gameWrapper->Toast("Reminder", everyGame, "default", 5.0F);

	currentGame--;

	if (currentGame <= 0) {
		currentGame = numGamesExtra;
		gameWrapper->Toast("Reminder", extraReminder, "default", 5.0F);
	}

	cvarManager->executeCommand("cl_notifications_enabled_beta " + std::to_string(prevValue));
}