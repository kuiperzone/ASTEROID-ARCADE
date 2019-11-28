//---------------------------------------------------------------------------
// PROJECT      : Asteroid Style Game
// COPYRIGHT    : Andy Thomas (C) 2019
// WEB URL      : https://kuiper.zone
// LICENSE      : GPLv3
//---------------------------------------------------------------------------

#include "spark.h"
#include "universe.h"

using namespace Game::Internal;

//---------------------------------------------------------------------------
// CLASS Spark : PUBLIC MEMBERS
//---------------------------------------------------------------------------
Spark::Spark(Universe *owner)
    : GameEntity(owner)
{
    std::vector<PairXy> poly(2);
    poly[0] = PairXy(0.0, 1.0);
    poly[1] = PairXy(0.0, -1.0);

    setPolygon(poly);
    setMaxSeconds(owner->random(0.05, 0.15));
}

