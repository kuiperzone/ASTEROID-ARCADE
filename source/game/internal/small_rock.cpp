//---------------------------------------------------------------------------
// PROJECT      : Asteroid Style Game
// COPYRIGHT    : Andy Thomas (C) 2019
// WEB URL      : https://kuiper.zone
// LICENSE      : GPLv3
//---------------------------------------------------------------------------

#include "small_rock.h"
#include "universe.h"

#include <math.h>

using namespace Game::Internal;

//---------------------------------------------------------------------------
// CLASS SmallRock : PUBLIC MEMBERS
//---------------------------------------------------------------------------
SmallRock::SmallRock(Universe *owner)
    : Rotator(owner)
{
    setPolygon(10.0);
    setFragility(0.3);
    setFragmentKind(EntityKind::Debris);
    setFragmentCount(3);
    setExplosionSound(SoundId::SmallExplosion);

    // Range +/- 4 deg per tick
    setRotation(owner->random(-M_PI / 45.0, +M_PI / 45.0));
}
