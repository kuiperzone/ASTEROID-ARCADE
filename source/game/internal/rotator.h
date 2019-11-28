//---------------------------------------------------------------------------
// PROJECT      : Asteroid Style Game
// COPYRIGHT    : Andy Thomas (C) 2019
// WEB URL      : https://kuiper.zone
// LICENSE      : GPLv3
//---------------------------------------------------------------------------

#ifndef GAME_ROTATOR_H
#define GAME_ROTATOR_H

#include "exploder.h"

namespace Game { namespace Internal {

//! An abstract base class for rotating objects. It is derived from Exploder.
class Rotator : public Exploder
{
public:

    Rotator(Universe *owner);

    //! Rotation rate in radians per time tick. The initial value is 0.
    double rotation() const;
    void setRotation(double value);
    bool advance() override;

private:

    double _rotation {0};

};

}} // namespace
#endif
