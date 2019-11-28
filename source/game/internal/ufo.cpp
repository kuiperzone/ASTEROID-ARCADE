//---------------------------------------------------------------------------
// PROJECT      : Asteroid Style Game
// COPYRIGHT    : Andy Thomas (C) 2019
// WEB URL      : https://kuiper.zone
// LICENSE      : GPLv3
//---------------------------------------------------------------------------

#include "ufo.h"
#include "bullet.h"
#include "universe.h"

#include <math.h>

using namespace Game::Internal;

//---------------------------------------------------------------------------
// CLASS Ufo : PUBLIC MEMBERS
//---------------------------------------------------------------------------
Ufo::Ufo(Universe *owner)
    : Exploder(owner)
{
    std::vector<PairXy> poly(20);

    poly[0] = PairXy(5.0, -5.0);
    poly[1] = PairXy(10.0, -2.0);
    poly[2] = PairXy(10.0, 2.0);
    poly[3] = PairXy(8.0, 4.0);
    poly[4] = PairXy(-2.0, 4.0);
    poly[5] = PairXy(-2.0, 2.0);
    poly[6] = PairXy(2.0, 2.0);
    poly[7] = PairXy(2.0, 4.0);
    poly[8] = PairXy(-8.0, 4.0);
    poly[9] = PairXy(-10.0, 2.0);
    poly[10] = PairXy(-10.0, -2.0);
    poly[11] = PairXy(10.0, -2.0);
    poly[12] = PairXy(10.0, 2.0);
    poly[13] = PairXy(-10.0, 2.0);
    poly[14] = PairXy(-10.0, -2.0);
    poly[15] = PairXy(-5.0, -5.0);
    poly[16] = PairXy(0.0, -5.0);
    poly[17] = PairXy(-7.0, -2.0);
    poly[18] = PairXy(-5.0, -5.0);
    poly[19] = PairXy(5.0, -5.0);

    setPolygon(poly);

    setFragility(0);
    setFragmentKind(EntityKind::Debris);
    setFragmentCount(5);
    setExplosionSound(SoundId::SmallExplosion);
}

bool Ufo::crunch(GameEntity *other)
{
    if (other->mass() > mass())
    {
        // Stay clear
        PairXy vec = position() - other->position();
        double abs = vec.abs();

        // Delta is ratio where: if delta < +1 = seriously close.
        double delta = abs / (radius() + other->radius() * 5.0);

        if (_avoidRockDelta <= 0 || delta < _avoidRockDelta)
        {
            _avoidRockDelta = delta;
            _avoidRockVector = vec / abs;
        }
    }

    if (other->kind() == EntityKind::Ufo)
    {
        PairXy vec = position() - other->position();
        double abs = vec.abs();

        double delta = abs / (radius() + other->radius() * 2.0);

        if (_avoidOtherDelta <= 0 || delta < _avoidOtherDelta)
        {
            _avoidOtherDelta = delta;
            _avoidOtherVector = vec / abs;
        }

        _flockCount += 1;
        _flockVector += vec;
        _flockVelocity += other->velocity();
    }

    return Exploder::crunch(other);
}

bool Ufo::advance()
{
    static const double SwirlyThrust = 0.25;
    static const double SwirlyRotate = 2.0 * M_PI / 180.0;

    static const double AvoidRockThrust = 0.4;
    static const double AvoidOtherThrust = 0.2;

    static const double FlockCoalesseThrust = 0.2;
    static const double FlockCohesionThrust = 0.1;

    if (Exploder::advance())
    {
        // Prevents UFOs from staying alone in a single place.
        PairXy thrust = _thrustAngle * SwirlyThrust;
        _thrustAngle.rotate(SwirlyRotate);

        if (_avoidRockDelta > 0)
        {
           thrust += _avoidRockVector * AvoidRockThrust / _avoidRockDelta;
        }

        if (_avoidOtherDelta > 0)
        {
           thrust += _avoidOtherVector * AvoidOtherThrust / _avoidOtherDelta;
        }

        if (_flockCount > 0)
        {
            _flockVector /= _flockCount;
            _flockVector /= _flockVector.abs();
            thrust += _flockVector * FlockCoalesseThrust;

            _flockVelocity /= _flockCount;
            thrust += _flockVelocity * FlockCohesionThrust;
        }

        setVelocity((velocity() + thrust).throttle(MaxSpeed));

        // Reset
        _avoidRockDelta = -1;
        _avoidOtherDelta = -1;

        _flockCount = 0;
        _flockVector = PairXy();
        _flockVelocity = PairXy();
        return true;
    }

    return false;
}

//---------------------------------------------------------------------------
// CLASS Ufo : PROTECTED MEMBERS
//---------------------------------------------------------------------------

//---------------------------------------------------------------------------
// CLASS Ufo : PRIVATE MEMBERS
//---------------------------------------------------------------------------
