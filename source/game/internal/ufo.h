//---------------------------------------------------------------------------
// PROJECT      : Asteroid Style Game
// COPYRIGHT    : Andy Thomas (C) 2019
// WEB URL      : https://kuiper.zone
// LICENSE      : GPLv3
//---------------------------------------------------------------------------

#ifndef GAME_UFO_H
#define GAME_UFO_H

#include "exploder.h"

namespace Game { namespace Internal {

//! The UFO.
class Ufo : public Exploder
{
public:

    Ufo(Universe *owner);

    // Overrides
    EntityKind kind() const override { return EntityKind::Ufo; }
    int score() const override { return 1000; }
    double mass() const override { return 15; }
    bool crunch(GameEntity *other) override;
    bool advance() override;

private:

    static const int MaxSpeed = 3;

    PairXy _thrustAngle {PairXy(1, 1)};

    PairXy _avoidRockVector;
    double _avoidRockDelta {-1};

    // Here I was experimenting with flocking behaviour.
    // https://en.wikipedia.org/wiki/Flocking_(behavior)
    PairXy _avoidOtherVector;
    double _avoidOtherDelta {-1};

    int _flockCount {0};
    PairXy _flockVector;
    PairXy _flockVelocity;
};

}} // namespace
#endif
