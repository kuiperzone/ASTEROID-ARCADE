//---------------------------------------------------------------------------
// PROJECT      : Asteroid Style Game
// COPYRIGHT    : Andy Thomas (C) 2019
// WEB URL      : https://kuiper.zone
// LICENSE      : GPLv3
//---------------------------------------------------------------------------

#ifndef GAME_MEDIUM_ROCK_H
#define GAME_MEDIUM_ROCK_H

#include "rotator.h"

namespace Game { namespace Internal {

//! A concrete medium sized rock class. Derived from Rotator and fragments
//! into small rocks when it explodes.
class MediumRock : public Rotator
{
public:

    MediumRock(Universe *owner);

    // Overrides
    EntityKind kind() const override { return EntityKind::MediumRock; }
    int score() const override { return 50; };
    double mass() const override { return 150; }
};

}} // namespace
#endif
