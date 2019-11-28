//---------------------------------------------------------------------------
// PROJECT      : Asteroid Style Game
// COPYRIGHT    : Andy Thomas (C) 2019
// WEB URL      : https://kuiper.zone
// LICENSE      : GPLv3
//---------------------------------------------------------------------------

#include "ship.h"
#include "bullet.h"
#include "spark.h"
#include "universe.h"
#include "scaled_canvas.h"

#include <math.h>

using namespace Game::Internal;

//---------------------------------------------------------------------------
// CLASS Ship : PUBLIC MEMBERS
//---------------------------------------------------------------------------
Ship::Ship(Universe *owner)
    : Exploder(owner)
{
    std::vector<PairXy> poly(5);

    poly[0] = PairXy(0, -10);
    poly[1] = PairXy(7, 10);
    poly[2] = PairXy(0, 7);
    poly[3] = PairXy(-7, 10);
    poly[4] = PairXy(0, -10);

    setPolygon(poly);

    // Gun & thrust postions
    _nosePos = PairXy(0, -14);
    _thrustPos = PairXy(3, 10);
    _thrustPlane = PairXy(-6, 0);

    setFragility(1.0);
    setFragmentKind(EntityKind::Debris);
    setFragmentCount(5);
    setExplosionSound(SoundId::SmallExplosion);
}

void Ship::rotate(int dir)
{
    _rotating = dir;
}

void Ship::fire(bool down)
{
    _firing = down;
}

void Ship::thrust(bool down)
{
    _thrusting = down;
}

int Ship::charge() const
{
    return _charge;
}

bool Ship::advance()
{
    static const double FireRecoil = 0.0075;
    static const double ThrustFactor = 0.06;
    static const double RotateFactor = 10.0 / 360.0;

    if (Exploder::advance())
    {
        double rads = alpha();

        if (_rotating != 0)
        {
            // Reset on rotation
            setAlpha(rads + M_PI * RotateFactor * _rotating);
            rads = alpha();
        }

        if (_thrusting)
        {
            // Rotate thrust position and plane
            PairXy tpos = _thrustPos.rotate(rads);
            PairXy tplane = _thrustPlane.rotate(rads);

            // Determine thrust vector
            PairXy tvec(std::sin(rads), -std::cos(rads));

            // Add thrust to velocity.
            setVelocity((velocity() + tvec * ThrustFactor).throttle(MaxSpeed));

            // Exhaust vector
            PairXy xvec(tvec * -ExhaustSpeed + velocity());

            for(int n = 0; n < 2; ++n)
            {
                // Generate exhaust along the plane
                PairXy temp(tpos + tplane * owner()->random());
                owner()->add(new Spark(owner()), position() + temp, xvec)->setAlpha(rads);
            }

            if (!_thrustSound)
            {
                _thrustSound = true;
                owner()->canvas()->playSound(SoundId::Thrust, SoundOpt::Loop);
            }
        }
        else
        if (_thrustSound)
        {
            _thrustSound = false;
            owner()->canvas()->stopSound(SoundId::Thrust);
        }

        // Fire! Stops once charge exhausted.
        // Lock limit the firing rate.
        if (_firing && _fireLock == 0 && _charge > 0)
        {
            // Create bullet heading
            PairXy bvec(std::sin(rads), -std::cos(rads));
            bvec *= BulletSpeed;
            bvec += velocity();

            // Recoil
            setVelocity(velocity() - bvec * FireRecoil);

            // Fire
            _charge -= 1;
            _fireLock = LockInc;

            PairXy temp = _nosePos.rotate(rads);
            owner()->add(new Bullet(owner(), bvec), position() + temp, bvec);

            owner()->canvas()->playSound(SoundId::GunFire, SoundOpt::Restart);
        }
        else
        {
            // Control fire rate
            if (_fireLock > 0)
            {
                _fireLock -= 1;
            }

            // Replenish charge
            if (_charge < FullCharge && ticker() % ChargeInc == 0)
            {
                _charge += 1;
            }
        }

        return true;
    }

    _thrustSound = false;
    owner()->canvas()->stopSound(SoundId::Thrust);
    return false;
}

//---------------------------------------------------------------------------
// CLASS Ship : PROTECTED MEMBERS
//---------------------------------------------------------------------------

//---------------------------------------------------------------------------
// CLASS Ship : PRIVATE MEMBERS
//---------------------------------------------------------------------------
