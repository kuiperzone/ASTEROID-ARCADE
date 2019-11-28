//---------------------------------------------------------------------------
// PROJECT      : Asteroid Style Game
// COPYRIGHT    : Andy Thomas (C) 2019
// WEB URL      : https://kuiper.zone
// LICENSE      : GPLv3
//---------------------------------------------------------------------------

#include "rotator.h"
#include "universe.h"

#include <math.h>

using namespace Game::Internal;

//---------------------------------------------------------------------------
// CLASS Rotator : PUBLIC MEMBERS
//---------------------------------------------------------------------------
Rotator::Rotator(Universe *owner)
    : Exploder(owner)
{
}

double Rotator::rotation() const
{
    return _rotation;
}

void Rotator::setRotation(double value)
{
    _rotation = value;
}

bool Rotator::advance()
{
    if (Exploder::advance())
    {
        setAlpha(alpha() + _rotation);
        return true;
    }

    return false;
}
