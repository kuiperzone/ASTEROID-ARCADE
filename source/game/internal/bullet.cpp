//---------------------------------------------------------------------------
// PROJECT      : Asteroid Style Game
// COPYRIGHT    : Andy Thomas (C) 2019
// WEB URL      : https://kuiper.zone
// LICENSE      : GPLv3
//---------------------------------------------------------------------------

#include "bullet.h"
#include "universe.h"
#include "scaled_canvas.h"

using namespace Game::Internal;

//---------------------------------------------------------------------------
// CLASS Bullet : PUBLIC MEMBERS
//---------------------------------------------------------------------------
Bullet::Bullet(Universe *owner, const PairXy& vel)
    : GameEntity(owner)
{
    setVelocity(vel);

    // Determine age based on game space and speed
    double abs = vel.abs();

    if (abs > 0)
    {
        double dim = 0.75 * std::min(owner->canvas()->width(), owner->canvas()->height());

        double ticks = dim / abs;
        setMaxSeconds(ticks * Universe::PollInterval / 1000);
    }
    else
    {
        // Stationary bullets are not expected (die quickly)
        setMaxSeconds(0.1);
    }
}

bool Bullet::crunch(GameEntity *other)
{
    if (GameEntity::crunch(other))
    {
        _hit = true;
        return true;
    }

    return false;
}

bool Bullet::advance()
{
    if (_hit)
    {
        return false;
    }

    return GameEntity::advance();
}

void Bullet::draw()
{
    // Draw in same direction as motion.
    static const double Radius = 1.0;

    PairXy dv = velocity();
    double va = dv.abs();

    if (va > 0)
    {
        dv /= va * Radius;
    }
    else
    {
        dv = PairXy(owner()->random(-Radius, Radius), owner()->random(-Radius, Radius));
    }

    owner()->canvas()->drawLine(position() - dv, position() + dv);
}
