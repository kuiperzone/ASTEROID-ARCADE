//---------------------------------------------------------------------------
// PROJECT      : Asteroid Style Game
// COPYRIGHT    : Andy Thomas (C) 2019
// WEB URL      : https://kuiper.zone
// LICENSE      : GPLv3
//---------------------------------------------------------------------------

#include "game_entity.h"

#include "scaled_canvas.h"
#include "universe.h"

#include <cmath>
#include <algorithm>

using namespace Game;
using namespace Game::Internal;

//---------------------------------------------------------------------------
// CLASS GameEntity : PUBLIC MEMBERS
//---------------------------------------------------------------------------
GameEntity::GameEntity(Universe *owner)
    : _owner {owner}
{
}

GameEntity::~GameEntity()
{
}

Universe* GameEntity::owner() const
{
    return _owner;
}

PairXy GameEntity::position() const
{
    return _position;
}

void GameEntity::setPosition(const PairXy &value)
{
    _position = value;
}

PairXy GameEntity::velocity() const
{
    return _velocity;
}

void GameEntity::setVelocity(const PairXy &value)
{
    _velocity = value.throttle(SpeedOfLight);
    _nextVelocity = _velocity;
}

double GameEntity::alpha() const
{
    return _alpha;
}

void GameEntity::setAlpha(double rads)
{
    setAlpha(rads, false);
}

double GameEntity::radius() const
{
    return _radius;
}

std::int64_t GameEntity::ticker() const
{
    return _ticker;
}

double GameEntity::maxSeconds() const
{
    return _maxSeconds;
}

bool GameEntity::crunch(GameEntity *other)
{
    // Coefficient of resitution
    static const double CR = 0.90;

    double m0 = mass();
    double m1 = other->mass();

    if (!_isAlive || m0 <= 0 || m1 <= 0)
    {
        // Ghost particles
        return false;
    }

    // Do objects overlap?
    auto dpos = position() - other->position();

    if (dpos.abs() <= radius() + other->radius())
    {
        auto v0 = velocity();
        auto v1 = other->velocity();

        // Check moving toward each other?
        if (dpos.abs() > (dpos + (v0 - v1) * 0.1).abs())
        {
            // Rebound - conservation of momentum
            // https://physics.info/momentum-energy/
            // https://en.wikipedia.org/wiki/Coefficient_of_restitution
            _nextVelocity = (v0 * m0 + v1 * m1 + (v1 - v0) * m1 * CR) / (m0 + m1);

            // Assume an energy loss (partially elastic)

            // Fire kills
            if (other->kind() == EntityKind::Bullet)
            {
                _isAlive = false;
            }

            return true;
        }
    }

    return false;
}

bool GameEntity::advance()
{
    _ticker += 1;

    // Next holds result of collision
    _velocity = _nextVelocity;

    // New position
    _position += _velocity;

    if (_maxTicks > 0 && _ticker > _maxTicks)
    {
        _isAlive = false;
    }

    return _isAlive;
}

void GameEntity::draw()
{
    if (_isAlive)
    {
        PairXy last(true);
        auto canvas = _owner->canvas();

        auto &poly = polygon();

        for(std::size_t n = 0; n < poly.size(); ++n)
        {
            PairXy p = poly[n] + position();

            if (!p.isNaN() && !last.isNaN())
            {
                canvas->drawLine(last, p);
            }

            last = p;
        }
    }
}

//---------------------------------------------------------------------------
// CLASS GameEntity : PROTECTED MEMBERS
//---------------------------------------------------------------------------
void GameEntity::setMaxSeconds(double sec)
{
    _maxSeconds = sec;
    _maxTicks = sec > 0 ? Universe::secondsToTicks(sec) : -1;
}

void GameEntity::setPolygon(const std::vector<PairXy> &points)
{
    _polySource = points;
    setAlpha(_alpha, true);

    // Determine radius
    _radius = 0;
    int count = 0;

    for(std::size_t n = 0; n < points.size(); ++n)
    {
        // NaN serves as a break, so count
        // non-NaNs rather than rely on size().
        if (!points[n].isNaN())
        {
            count += 1;
            _radius += points[n].abs();
        }
    }

    if (count != 0)
    {
        // Average
        _radius /= count;
    }
}

void GameEntity::setPolygon(double radius, int count, bool randomize)
{
    count = std::max(count, 4);
    std::vector<PairXy> poly(count);

    double alpha = 0.0;
    double delta = 2.0 * M_PI / (count - 1);

    for(int n = 0; n < count - 1; ++n)
    {
        PairXy p = PairXy(std::sin(alpha), std::cos(alpha)) * radius;

        if (randomize && n > 0 && n < count - 1)
        {
            p.setX(_owner->random(p.x() * 0.8, p.x() * 1.2));
            p.setY(_owner->random(p.y() * 0.8, p.y() * 1.2));
        }

        poly[n] = p;
        alpha += delta;
    }

    // Connect final point
    poly[count - 1] = poly[0];

    setPolygon(poly);

}

const std::vector<PairXy>& GameEntity::polygon() const
{
    if (_polyAlpha.size() != 0)
    {
        return _polyAlpha;
    }

    return _polySource;
}

//---------------------------------------------------------------------------
// CLASS GameEntity : PROTECTED MEMBERS
//---------------------------------------------------------------------------
void GameEntity::setAlpha(double rads, bool force)
{
    if (_alpha != rads || force)
    {
        _alpha = rads;

        if (rads != 0)
        {
            // Rotate
            _polyAlpha.resize(_polySource.size());

            for(std::size_t n = 0; n < _polySource.size(); ++n)
            {
                _polyAlpha[n] = _polySource[n].rotate(rads);
            }
        }
        else
        {
            // Not needed. See polygon().
            _polyAlpha.clear();
        }
    }
}
