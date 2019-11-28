//---------------------------------------------------------------------------
// PROJECT      : Asteroid Style Game
// COPYRIGHT    : Andy Thomas (C) 2019
// WEB URL      : https://kuiper.zone
// LICENSE      : GPLv3
//---------------------------------------------------------------------------

#include "medium_rock.h"
#include "universe.h"

#include <math.h>

using namespace Game::Internal;

//---------------------------------------------------------------------------
// CLASS MediumRock : PUBLIC MEMBERS
//---------------------------------------------------------------------------
MediumRock::MediumRock(Universe *owner)
    : Rotator(owner)
{
    setPolygon(18.0);
    setFragility(0.4);
    setFragmentKind(EntityKind::SmallRock);
    setFragmentCount(2);
    setExplosionSound(SoundId::MediumExplosion);

    // Range +/- 2 deg per tick
    setRotation(owner->random(-M_PI / 90.0, +M_PI / 90.0));

    // Eventually explodes by itself
    setMaxSeconds(owner->random(10, 150));
}
