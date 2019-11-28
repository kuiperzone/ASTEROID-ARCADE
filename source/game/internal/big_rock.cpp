//---------------------------------------------------------------------------
// PROJECT      : Asteroid Style Game
// COPYRIGHT    : Andy Thomas (C) 2019
// WEB URL      : https://kuiper.zone
// LICENSE      : GPLv3
//---------------------------------------------------------------------------

#include "big_rock.h"
#include "universe.h"

#include <math.h>

using namespace Game::Internal;

//---------------------------------------------------------------------------
// CLASS BigRock : PUBLIC MEMBERS
//---------------------------------------------------------------------------
BigRock::BigRock(Universe *owner)
    : Rotator(owner)
{
    setPolygon(30.0);
    setFragility(0.5);
    setFragmentKind(EntityKind::MediumRock);
    setFragmentCount(2);
    setExplosionSound(SoundId::BigExplosion);

    // Range +/- 1 deg per tick
    setRotation(owner->random(-M_PI / 180.0, +M_PI / 180.0));

    // Eventually explodes by itself
    setMaxSeconds(owner->random(10, 150));
}
