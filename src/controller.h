#ifndef CONTROLLER_H
#define CONTROLLER_H

#include "ship.h"

class Controller {
    public:
        void HandleInput(bool &running, Ship &player) const;
};

#endif