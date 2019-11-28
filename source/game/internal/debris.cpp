//---------------------------------------------------------------------------
// PROJECT      : Asteroid Style Game
// COPYRIGHT    : Andy Thomas (C) 2019
// WEB URL      : https://kuiper.zone
// LICENSE      : GPLv3
//---------------------------------------------------------------------------

#include "debris.h"
#include "universe.h"

#include <cmath>

using namespace Game::Internal;

//---------------------------------------------------------------------------
// CLASS Debris : PUBLIC MEMBERS
//---------------------------------------------------------------------------
Debris::Debris(Universe *owner)
    : Rotator(owner)
{
    std::vector<PairXy> poly(4);
    poly[0] = PairXy(0, 3);
    poly[1] = PairXy(3, 0);
    poly[2] = PairXy(-2, -3);
    poly[3] = PairXy(0, 3);

    setPolygon(poly);
    setFragility(0);
    setRotation(owner->random(-M_PI / 15.0, +M_PI / 15.0));
    setMaxSeconds(owner->random(1, 3));
}

