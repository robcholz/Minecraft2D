//
// Created by robcholz on 3/4/23.
//

#ifndef RUNCRAFT_FILE_HPP
#define RUNCRAFT_FILE_HPP

#include <string>
#include <utility>
#include <fstream>
#include <vector>
#include "filereadstream.h"
#include "document.h"

class File {
private:
    const char *pathJSON = nullptr;

    char readBuffer[65536];
    rapidjson::FileReadStream *jsonDocStream = nullptr;
    FILE *file = nullptr;
    rapidjson::Document *jsonDoc = new rapidjson::Document;

public:
    explicit File(std::string path) {
        pathJSON = path.c_str();
    }

    ~File() {
        free(file);
    };

    bool openJSON() {
        file = fopen(pathJSON, "r");
        jsonDocStream = new rapidjson::FileReadStream(file, readBuffer, sizeof(readBuffer));
        jsonDoc->ParseStream(*jsonDocStream);
    }

    rapidjson::Document *getJSON() {
        return jsonDoc;
    }
};

#endif //RUNCRAFT_FILE_HPP
