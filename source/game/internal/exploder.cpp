//---------------------------------------------------------------------------
// PROJECT      : Asteroid Style Game
// COPYRIGHT    : Andy Thomas (C) 2019
// WEB URL      : https://kuiper.zone
// LICENSE      : GPLv3
//---------------------------------------------------------------------------

#include "exploder.h"
#include "scaled_canvas.h"
#include "universe.h"

#include <cmath>

using namespace Game::Internal;

//---------------------------------------------------------------------------
// CLASS Exploder : PUBLIC MEMBERS
//---------------------------------------------------------------------------
bool Exploder::crunch(GameEntity *other)
{
    // Max impact
    static const double MaxImpact = SpeedOfLight / 4;

    if (GameEntity::crunch(other))
    {
        // Destruction if mass of other is same or larger
        if (_fragility > 0 && (other->mass() > mass()
            || (other->mass() == mass() && owner()->random() < 0.5)))
        {
            // Actual destruction depends of velocity of impact and fragility
            if ((velocity() - other->velocity()).abs() > MaxImpact * (1.0 - _fragility))
            {
                _destruction = true;
            }
        }

        return true;
    }

    return false;
}

bool Exploder::advance()
{
    // We still want to call base if collision is true
    // as it is needed to calculate the rebound velocity.
    if (!GameEntity::advance() || _destruction)
    {
        if (_fragmentCount > 0)
        {
            double r = radius();
            auto p0 = position();

            // Don't play sound if no ship
            if (owner()->ship() != nullptr
                && p0.x() > -r && p0.y() > -r
                && p0.x() < owner()->canvas()->width() + r
                && p0.y() < owner()->canvas()->height() + r)
            {
                // Sound only in visible region
                owner()->canvas()->playSound(_explosionSound, SoundOpt::Restart);
            }

            // Fragments
            fragment(_fragmentKind, _fragmentCount, 0);

            if (_fragmentKind == EntityKind::BigRock || _fragmentKind == EntityKind::MediumRock)
            {
                // Additional debris
                fragment(EntityKind::Debris, 5, 0.2);
            }
        }

        return false;
    }

    return true;
}

//---------------------------------------------------------------------------
// CLASS Exploder : PROTECTED MEMBERS
//---------------------------------------------------------------------------
void Exploder::setFragility(double value)
{
    _fragility = std::min(std::max(value, 0.0), 1.0);
}

void Exploder::setFragmentKind(EntityKind kind)
{
    _fragmentKind = kind;
}

void Exploder::setFragmentCount(int count)
{
    _fragmentCount = count;
}

void Exploder::setExplosionSound(SoundId id)
{
    _explosionSound = id;
}

//---------------------------------------------------------------------------
// CLASS Exploder : PRIVATE MEMBERS
//---------------------------------------------------------------------------
void Exploder::fragment(EntityKind kind, int count, double sf)
{
    if (count > 0)
    {
        // Creates count items of given kind, where the net velocity
        // remains equal (roughly) to this->velocity() while fanning
        // out in all directions. It is important to try and get
        // the objects away from this->position() by a distance of
        // radius() so that fragments do not immediately collide.

        PairXy v0 = velocity();

        // Velocity direction with abs() of 1.
        // A default is used if velocity is zero.
        PairXy dir(0.1, 0.1);
        double vabs = v0.abs();
        if (vabs > 0) dir = v0 / vabs;

        // Delta velocity and position
        PairXy dv = dir * sf;
        PairXy dp = dir * radius();

        // Rotate initial position delta by 45 deg
        dp = dp.rotate(M_PI / 2);

        // Fan out
        double fanStep = 2.0 * M_PI / count;

        for(int n = 0; n < count; ++n)
        {
            GameEntity *entity = owner()->create(kind);
            owner()->add(entity, position() + dp, velocity() + dv);
            dp = dp.rotate(fanStep);
            dv = dv.rotate(fanStep);
        }
    }

}
