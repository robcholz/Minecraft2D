//
// Created by robcholz on 3/31/23.
//
#pragma once

#ifndef MINECRAFT_MENU_HPP
#define MINECRAFT_MENU_HPP


#include "util/Utils.hpp"
#include "Initializers/ConsoleInitializer.h"
#include "client/gui/screen/Background.hpp"
#include "client/gui/widget/ButtonWidget.hpp"
#include "client/gui/widget/TexturedButtonWidget.hpp"
#include "client/gui/widget/SoundSliderWidget.hpp"
#include "client/gui/widget/TextFieldWidget.hpp"
#include "client/gui/widget/TextureWidget.hpp"
#include "client/gui/widget/SplashingTextFieldWidget.hpp"
#include "client/gui/screen/ScreenManager.hpp"
#include "client/scene/SceneAccess.hpp"
#include "sound/SoundEvents.hpp"
#include "MinecraftClientAccess.hpp"


class Menu : public SceneAccess {
public:
	explicit Menu(MinecraftClientAccess* minecraftClientAccess) {
		this->minecraftClientAccess = minecraftClientAccess;
		backgroundMenuScreen = std::make_unique<Screen>(minecraftClientAccess, &backgroundTexture);
		backgroundBiomeSettingScreen = std::make_unique<Screen>(minecraftClientAccess, &backgroundTexture);
		settingVolumeScreen = std::make_unique<Screen>(minecraftClientAccess, &settingBackground);
		settingSingleplayerScreen = std::make_unique<Screen>(minecraftClientAccess, &settingBackground);

		PLOG_DEBUG << "Menu started!";
		onInitialize();
	}

	~Menu() override {
		PLOG_DEBUG << "Menu stopped!";
	}

	void onUpdate() override {
		screenManager.update();
	}

	void onRender() override {
		screenManager.render();
	}

private:
	int screenWidth = (int) RenderSystem::getScreenWidth();
	int screenHeight = (int) RenderSystem::getScreenHeight();

	MinecraftClientAccess* minecraftClientAccess;

	Background backgroundTexture{"background"};
	Background settingBackground{"options_background"};

	/*main menu*/
	Layout::Stack backgroundMenuStack;
	ButtonWidget backgroundMenuSinglePlayer{"singleplayer", 800, 80, true, screenWidth / 2 - 800 / 2, 432};
	ButtonWidget backgroundMenuLanguage{"language", 800, 80, true, screenWidth / 2 - 800 / 2, 528};
	ButtonWidget backgroundMenuWhat{"multiplayer", 800, 80, true, screenWidth / 2 - 800 / 2, 624};
	ButtonWidget backgroundMenuOptions{"options", 380, 80, true, screenWidth / 2 - 800 / 2, 768};//
	ButtonWidget backgroundMenuQuitGame{"quit_game", 380, 80, true, screenWidth / 2 + 800 / 2 - 380, 768};
	TexturedButtonWidget backgroundMenuLanguageTexturedButton{32, 32, true, screenWidth / 2 - 500, 768};
	TextureWidget backgroundMinecraftTitle{"title.minecraft", true, screenWidth / 2 - 982 / 2, 128};
	SplashingTextFieldWidget backgroundSplashingText{"splashing_text", 70, 5, -30};
	std::unique_ptr<Screen> backgroundMenuScreen;

	/*settings*/
	Layout::Stack backgroundSettingBiomeStack;
	ButtonWidget backgroundSettingVolume{"music", 1200, 80, true, screenWidth / 2 - 1200 / 2, 200};
	ButtonWidget backgroundSettingBack{"done", 790, 80, true, screenWidth / 2 - 780 / 2, screenHeight - 80 - 50};
	std::unique_ptr<Screen> backgroundBiomeSettingScreen;

	/*music settings*/
	Layout::Stack settingVolumeStack;
	TextFieldWidget settingVolumeSliderTitle{"music_sound_options", 70, true, screenWidth / 2 - 440 / 2, 20};
	SoundSliderWidget settingVolumeSliderMasterVolume{"master_volume", 1240, 80, true, screenWidth / 2 - 620, 115};
	SoundSliderWidget settingVolumeSliderMusic{"music_volume", 590, 80, true, screenWidth / 2 - 620, 225};
	SoundSliderWidget settingVolumeSliderJukeboxNoteblocks{"jukebox_noteblocks_volume", 590, 80, true, screenWidth / 2 + 30, 225};
	SoundSliderWidget settingVolumeSliderWeather{"weather_volume", 590, 80, true, screenWidth / 2 - 620, 335};
	SoundSliderWidget settingVolumeSliderBlocks{"blocks_volume", 36, 590, 80, true, screenWidth / 2 + 30, 335};
	SoundSliderWidget settingVolumeSliderHostileCreatures{"hostile_creatures_volume", 590, 80, true, screenWidth / 2 - 620, 445};
	SoundSliderWidget settingVolumeSliderFriendlyCreatures{"friendly_creatures_volume", 590, 80, true, screenWidth / 2 + 30, 445};
	SoundSliderWidget settingVolumeSliderPlayers{"players_volume", 590, 80, true, screenWidth / 2 - 620, 555};
	SoundSliderWidget settingVolumeSliderAmbientEnvironment{"ambient_environment_volume", 590, 80, true, screenWidth / 2 + 30, 555};
	ButtonWidget settingVolumeBack{"done", 790, 80, true, screenWidth / 2 - 390, 555 + 260};
	std::unique_ptr<Screen> settingVolumeScreen;

	/*singleplayer page*/
	Layout::Stack singleplayerStack;
	ButtonWidget singleplayerGenerateWorld{"generate_experimental_world", 800, 80, true, screenWidth / 2 - 800 / 2, screenHeight / 2 + 80};
	ButtonWidget singleplayerSettingBack{"done", 790, 80, true, screenWidth / 2 - 790 / 2, screenHeight - 80 - 50};
	std::unique_ptr<Screen> settingSingleplayerScreen;

	ScreenManager screenManager;

	void onInitialize() {
		backgroundTexture.fitToScreen();
		settingBackground.composeToScreen();
		initWidget();
		playRandomMusic();

		PLOG_DEBUG << "Initialize assets.";
	}

	void initWidget() {
		screenManager.addScreen(backgroundMenuScreen.get())
		             .addScreen(backgroundBiomeSettingScreen.get())
		             .addScreen(settingSingleplayerScreen.get())
		             .addScreen(settingVolumeScreen.get())
		             .setEntry(backgroundMenuScreen.get());

		backgroundMenuScreen->addCallbackScreen(backgroundBiomeSettingScreen.get(), &backgroundMenuOptions)
		                    .addCallbackScreen(settingSingleplayerScreen.get(), &backgroundMenuSinglePlayer)
		                    .addWidget(&backgroundMenuOptions)
		                    .addWidget(&backgroundMenuSinglePlayer)
		                    .addWidget(&backgroundMenuLanguage)
		                    .addWidget(&backgroundMenuWhat)
		                    .addWidget(&backgroundMenuLanguageTexturedButton)
		                    .addWidget(&backgroundMenuQuitGame)
		                    .addWidget(&backgroundMinecraftTitle)
		                    .addWidget(&backgroundSplashingText);
		backgroundMenuQuitGame.executeFuncWhenActivated([] {
			PLOG_DEBUG << "Cancel minecraft!";
			RenderSystem::getWindow()->close();
		});

		settingVolumeScreen->addCallbackScreen(backgroundBiomeSettingScreen.get(), &settingVolumeBack)
		                   .addWidget(&settingVolumeSliderTitle)
		                   .addWidget(&settingVolumeSliderAmbientEnvironment)
		                   .addWidget(&settingVolumeSliderBlocks)
		                   .addWidget(&settingVolumeSliderFriendlyCreatures)
		                   .addWidget(&settingVolumeSliderHostileCreatures)
		                   .addWidget(&settingVolumeSliderJukeboxNoteblocks)
		                   .addWidget(&settingVolumeSliderMasterVolume)
		                   .addWidget(&settingVolumeSliderMusic)
		                   .addWidget(&settingVolumeSliderPlayers)
		                   .addWidget(&settingVolumeSliderWeather)
		                   .addWidget(&settingVolumeBack);
		settingVolumeSliderAmbientEnvironment.executeFuncWhenActivated([this]() {
			this->minecraftClientAccess->getSoundManager()->setVolume(SoundManager::Sound::ENVIRONMENT, settingVolumeSliderAmbientEnvironment.getValue());
		});
		settingVolumeSliderBlocks.executeFuncWhenActivated([this]() {
			this->minecraftClientAccess->getSoundManager()->setVolume(SoundManager::Sound::BLOCKS, settingVolumeSliderBlocks.getValue());
		});
		settingVolumeSliderFriendlyCreatures.executeFuncWhenActivated([this]() {
			this->minecraftClientAccess->getSoundManager()->setVolume(SoundManager::Sound::FRIENDLY_CREATURES, settingVolumeSliderFriendlyCreatures.getValue());
		});
		settingVolumeSliderHostileCreatures.executeFuncWhenActivated([this]() {
			this->minecraftClientAccess->getSoundManager()->setVolume(SoundManager::Sound::HOSTILE_CREATURES, settingVolumeSliderHostileCreatures.getValue());
		});
		settingVolumeSliderJukeboxNoteblocks.executeFuncWhenActivated([this]() {
			this->minecraftClientAccess->getSoundManager()->setVolume(SoundManager::Sound::JUKEBOX, settingVolumeSliderJukeboxNoteblocks.getValue());
		});
		settingVolumeSliderMasterVolume.executeFuncWhenActivated([this]() {
			this->minecraftClientAccess->getSoundManager()->setVolume(SoundManager::Sound::MASTER, settingVolumeSliderMasterVolume.getValue());
		});
		settingVolumeSliderMusic.executeFuncWhenActivated([this]() {
			this->minecraftClientAccess->getSoundManager()->setVolume(SoundManager::Sound::MUSIC, settingVolumeSliderMusic.getValue());
		});
		settingVolumeSliderPlayers.executeFuncWhenActivated([this]() {
			this->minecraftClientAccess->getSoundManager()->setVolume(SoundManager::Sound::PLAYERS, settingVolumeSliderPlayers.getValue());
		});
		settingVolumeSliderWeather.executeFuncWhenActivated([this]() {
			this->minecraftClientAccess->getSoundManager()->setVolume(SoundManager::Sound::WEATHER, settingVolumeSliderWeather.getValue());
		});

		settingSingleplayerScreen->addCallbackScreen(backgroundMenuScreen.get(), &singleplayerSettingBack)
		                         .addWidget(&singleplayerGenerateWorld)
		                         .addWidget(&singleplayerSettingBack);
		singleplayerGenerateWorld.executeFuncWhenActivated([this] { terminateScene(); });

		backgroundBiomeSettingScreen->addCallbackScreen(backgroundMenuScreen.get(), &backgroundSettingBack)
		                            .addCallbackScreen(settingVolumeScreen.get(), &backgroundSettingVolume)
		                            .addWidget(&backgroundSettingBack)
		                            .addWidget(&backgroundSettingVolume);

		PLOG_DEBUG << "Initialize widget components";
	}

	void playRandomMusic() {
		this->minecraftClientAccess->getSoundManager()->addSound(SoundEvents::getInstance().MUSIC_MENU);
	}
};

#endif //MINECRAFT_MENU_HPP
