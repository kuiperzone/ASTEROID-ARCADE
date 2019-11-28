//---------------------------------------------------------------------------
// PROJECT      : Asteroid Style Game
// COPYRIGHT    : Andy Thomas (C) 2019
// WEB URL      : https://kuiper.zone
// LICENSE      : GPLv3
//---------------------------------------------------------------------------

#ifndef GAME_SHIP_H
#define GAME_SHIP_H

#include "exploder.h"

namespace Game { namespace Internal {

//! The ship.
class Ship : public Exploder
{
public:

    //! Fully charged value.
    static const int FullCharge = 8;

    Ship(Universe *owner);

    //! Rotate control. Rotates left if dir is negative,
    //! right if position and stops rotation if 0.
    void rotate(int dir);

    //! Starts firing if down is true. Stops if false.
    void fire(bool down);

    //! Starts thrusting if down is true. Stops if false.
    void thrust(bool down);

    //! Gets amount of charge available for firing.
    int charge() const;

    // Overrides
    EntityKind kind() const override { return EntityKind::Ship; }
    double mass() const override { return 20; }
    bool advance() override;

private:

    // NB. The values pertain to tick rate and were
    // determined on basis of 25ms poll interval.
    static const int MaxSpeed = 4;
    static const int BulletSpeed = 8;
    static const int ExhaustSpeed = 5;

    // Controls rate of fire (per tick).
    static const int LockInc = 2;
    static const int ChargeInc = 8;

    int _rotating {0};
    int _charge {FullCharge};
    bool _firing {false};
    int _fireLock {0};
    bool _thrusting {false};
    bool _thrustSound {false};
    PairXy _nosePos;
    PairXy _thrustPos;
    PairXy _thrustPlane;

};

}} // namespace
#endif
