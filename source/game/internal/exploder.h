//---------------------------------------------------------------------------
// PROJECT      : Asteroid Style Game
// COPYRIGHT    : Andy Thomas (C) 2019
// WEB URL      : https://kuiper.zone
// LICENSE      : GPLv3
//---------------------------------------------------------------------------

#ifndef GAME_EXPLODER_H
#define GAME_EXPLODER_H

#include "../sound_id.h"
#include "game_entity.h"

namespace Game { namespace Internal {

//! An abstract base class which explodes into fragments when it dies.
//! Its behaviour can be set using protected methods.
class Exploder : public GameEntity
{
public:

    Exploder(Universe *owner)
        : GameEntity(owner)
    {
    }

    // Overrides
    bool crunch(GameEntity *other) override;
    bool advance() override;

protected:

    //! The fragility in range [0, 1.0]. Objects with a value 0 do not explode
    //! when hit by another. Objects with a value of 1.0 explode if hit by something
    //! heavier. The initial value is 1.0.
    void setFragility(double value);

    //! Fragmentation kind. This defines how the object "explodes" when it dies.
    //! The initial value is Debris and is typically set during construction.
    void setFragmentKind(EntityKind kind);

    //! Fragmentation count. A value of 0 disables fragmentation (explosion).
    //! The initial value is 0 and should be modifed during construction.
    void setFragmentCount(int count);

    //! Explosion sound. A value of SoundId::Count is silent.
    //! The initial value is Count and should be modifed during construction.
    void setExplosionSound(SoundId id);

private:

    bool _destruction {false};
    double _fragility {1.0};
    EntityKind _fragmentKind {EntityKind::Debris};
    int _fragmentCount {0};
    SoundId _explosionSound {SoundId::Count};

    void fragment(EntityKind kind, int count, double sf);
};

}} // namespace
#endif
