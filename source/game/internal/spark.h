//---------------------------------------------------------------------------
// PROJECT      : Asteroid Style Game
// COPYRIGHT    : Andy Thomas (C) 2019
// WEB URL      : https://kuiper.zone
// LICENSE      : GPLv3
//---------------------------------------------------------------------------

#ifndef GAME_SPARK_H
#define GAME_SPARK_H

#include "game_entity.h"

namespace Game { namespace Internal {

//! A short-lived sparkle associated with ship exhaust.
class Spark : public GameEntity
{
public:

    Spark(Universe *owner);

    // Overrides
    EntityKind kind() const override { return EntityKind::Spark; }

private:

};

}} // namespace
#endif

