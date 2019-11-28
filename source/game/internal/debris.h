//---------------------------------------------------------------------------
// PROJECT      : Asteroid Style Game
// COPYRIGHT    : Andy Thomas (C) 2019
// WEB URL      : https://kuiper.zone
// LICENSE      : GPLv3
//---------------------------------------------------------------------------

#ifndef GAME_DEBRIS_H
#define GAME_DEBRIS_H

#include "rotator.h"

namespace Game { namespace Internal {

//! A short lived rotating rock-like particle with low mass.
class Debris : public Rotator
{
public:

    Debris(Universe *owner);

    // Overrides
    EntityKind kind() const override { return EntityKind::Debris; }
    double mass() const override { return 3; }

};

}} // namespace
#endif
