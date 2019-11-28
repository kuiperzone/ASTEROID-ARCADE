//---------------------------------------------------------------------------
// PROJECT      : Asteroid Style Game
// COPYRIGHT    : Andy Thomas (C) 2019
// WEB URL      : https://kuiper.zone
// LICENSE      : GPLv3
//---------------------------------------------------------------------------

#ifndef GAME_SMALL_ROCK_H
#define GAME_SMALL_ROCK_H

#include "rotator.h"

namespace Game { namespace Internal {

//! A concrete small rock class.  Derived from Rotator and fragments into
//! debris when it explodes.
class SmallRock : public Rotator
{
public:

    SmallRock(Universe *owner);

    // Overrides
    EntityKind kind() const override { return EntityKind::SmallRock; }
    int score() const override { return 25; };
    double mass() const override { return 75; }
};

}} // namespace
#endif

