//
// Created by robcholz on 3/11/23.
//

#ifndef RUNCRAFT_UTILS_HPP
#define RUNCRAFT_UTILS_HPP

class Once {
private:
    bool _once = true;
public:
    bool once() {
        if (_once) {
            _once = false;
            return true;
        } else {
            return false;
        }
    }
};

#endif //RUNCRAFT_UTILS_HPP
