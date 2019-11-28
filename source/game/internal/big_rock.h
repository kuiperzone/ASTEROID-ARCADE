//---------------------------------------------------------------------------
// PROJECT      : Asteroid Style Game
// COPYRIGHT    : Andy Thomas (C) 2019
// WEB URL      : https://kuiper.zone
// LICENSE      : GPLv3
//---------------------------------------------------------------------------

#ifndef GAME_BIG_ROCK_H
#define GAME_BIG_ROCK_H

#include "rotator.h"

namespace Game { namespace Internal {

//! A concrete big rock class. Derived from Rotator and fragments into
//! medium rocks when it explodes.
class BigRock : public Rotator
{
public:

    BigRock(Universe *owner);

    // Overrides
    EntityKind kind() const override { return EntityKind::BigRock; }
    int score() const override { return 100; }
    double mass() const override { return 300; }
};

}} // namespace
#endif
