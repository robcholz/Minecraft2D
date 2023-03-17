//
// Created by robcholz on 3/9/23.
//

#ifndef RUNCRAFT_AUDIO_HPP
#define RUNCRAFT_AUDIO_HPP

#include <string>
#include <iostream>
#include <SFML/Graphics/Shape.hpp>
#include <SFML/Audio/Sound.hpp>
#include <SFML/Audio.hpp>
#include <cstdlib>
#include "util/Math_Helper.hpp"
#include "Log.h"

class Audio {
protected:
    // sound player
    sf::Sound sound;
    // file path
    std::string audioPath;
    const std::string audioFileType = ".ogg";

public:
    bool isPlaying() {
        return (sound.getStatus() == sf::SoundSource::Status::Playing);
    }

    void pause() { sound.pause(); }

    void stop() {
        if (isPlaying())
            sound.stop();
    }

    void play() {
        sound.play();
    }
};

class AudioList : public Audio {
private:
    // Sound struct
    class SoundInfo {
    public:
        std::string filename;
        sf::SoundBuffer *soundBuffer = nullptr;

        SoundInfo(const std::string &filename, sf::SoundBuffer *soundBuffer) {
            this->filename = filename;
            this->soundBuffer = soundBuffer;
        }
    };

    // where all the sounds stored here
    std::vector<SoundInfo *> playlist;
public:
    /**
     * @param folder_path the path of the audio stored in the resources/sound/ directory.
     * \n Default is ../resources/sound
     */
    explicit AudioList(const std::string &folder_path = "../resources/sound/") {
        audioPath = folder_path;
    };

    ~AudioList() {
        std::destroy(playlist.begin(), playlist.end());
    }

    /**
     * @param filename the name of the .ogg file(doesn't contain file format)
     * @return AudioList&
     */
    AudioList &addAudio(const std::string &filename) {
        auto *soundBuffer = new sf::SoundBuffer;
        if (!soundBuffer->loadFromFile(audioPath + filename + audioFileType)) {

            PLOG_DEBUG << "CANNOT load .ogg file from the directory " + audioPath + filename + audioFileType
                      << " check the path again!";
        }

        playlist.push_back(new SoundInfo(filename, soundBuffer));
        sound.setBuffer(*playlist[0]->soundBuffer);
        return *this;
    }

    /**
     * @brief randomly play a sound from the soundList
     */
    void playRandomly() {
        stop();
        sound.setBuffer(*(playlist[Random(0, playlist.size())]->soundBuffer));
        Audio::play();
    }

    /**
     * @brief play the nth sound in the list
     * @param n index of the sound in the list
     */
    void play(int n) {
        stop();
        if (n < playlist.size()) {
            sound.setBuffer(*playlist[n]->soundBuffer);
        } else {
            std::cout << "IndexOutOfBoundException" << std::endl;
        }

        Audio::play();
    }

    /**
     * @brief play a sound by its name
     * @param filename sound name
     */
    void play(const std::string &filename) {
        for (auto &obj: playlist) {
            if (filename == obj->filename) {
                stop();
                sound.setBuffer(*(obj->soundBuffer));
                Audio::play();
            }
        }
    }

};

class AudioPlayer : public Audio {
private:
    std::string filename;
    sf::SoundBuffer *soundBuffer = nullptr;

public:
    explicit AudioPlayer(const std::string &filename) {
        audioPath = "../resources/sound/" + filename + audioFileType;
    };

    ~AudioPlayer(){
        delete soundBuffer;
    }

    /**
     * @brief add a single sound
     * @return
     */
    [[maybe_unused]] AudioPlayer &addAudio() {
        soundBuffer = new sf::SoundBuffer;
        if (!soundBuffer->loadFromFile(audioPath)) {
            std::cout << "CANNOT load .ogg file from the directory " + audioPath
                      << " check the path again!"
                      << std::endl;
        }
        sound.setBuffer(*soundBuffer);
        return *this;
    }
};

#endif //RUNCRAFT_AUDIO_HPP
