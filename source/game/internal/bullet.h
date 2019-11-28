//---------------------------------------------------------------------------
// PROJECT      : Asteroid Style Game
// COPYRIGHT    : Andy Thomas (C) 2019
// WEB URL      : https://kuiper.zone
// LICENSE      : GPLv3
//---------------------------------------------------------------------------

#ifndef GAME_BULLET_H
#define GAME_BULLET_H

#include "game_entity.h"

namespace Game { namespace Internal {

//! A bullet fired from the Ship class. The constructor should be used to
//! set the velocity.
class Bullet : public GameEntity
{
public:

    Bullet(Universe *owner, const PairXy& vel = PairXy());

    // Overrides
    EntityKind kind() const override { return EntityKind::Bullet; }
    double mass() const override { return 1.0; }
    virtual bool crunch(GameEntity *other) override;
    virtual bool advance() override;
    void draw() override;

private:

    bool _hit {false};
};

}} // namespace
#endif

