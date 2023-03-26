//
// Created by robcholz on 3/11/23.
//

#ifndef RUNCRAFT_UTILS_HPP
#define RUNCRAFT_UTILS_HPP

#pragma once

class Once {
private:
    bool once = true;
public:
    bool runOnce() {
        if (once) {
            once = false;
            return true;
        } else {
            return false;
        }
    }
};

#endif //RUNCRAFT_UTILS_HPP
