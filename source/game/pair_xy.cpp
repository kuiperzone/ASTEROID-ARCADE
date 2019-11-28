//---------------------------------------------------------------------------
// PROJECT      : Asteroid Style Game
// COPYRIGHT    : Andy Thomas (C) 2019
// WEB URL      : https://kuiper.zone
// LICENSE      : GPLv3
//---------------------------------------------------------------------------

#include "pair_xy.h"

#include <cmath>

using namespace Game;

//---------------------------------------------------------------------------
// CLASS PairXy : PUBLIC MEMBERS
//---------------------------------------------------------------------------
PairXy::PairXy(double x, double y)
    : _x{x}, _y{y}
{
}

PairXy::PairXy(bool nan)
{
    if (nan)
    {
        _x = NAN;
        _y = NAN;
    }
}

bool PairXy::isNaN() const
{
    return std::isnan(_x) || std::isnan(_y);
}

double PairXy::abs() const
{
    return std::sqrt(_x * _x + _y * _y);
}

const PairXy PairXy::rotate(double alpha) const
{
    if (alpha == 0 || isNaN())
    {
        return PairXy(_x, _y);
    }

    double cs = std::cos(alpha);
    double sn = std::sin(alpha);
    return PairXy(cs * _x - sn * _y, sn * _x + cs * _y);
}

const PairXy PairXy::throttle(double max) const
{
    PairXy rslt(_x, _y);

    if (!isNaN())
    {
        if (_x > max) rslt._x = max;
        if (_x < -max) rslt._x = -max;
        if (_y > max) rslt._y = max;
        if (_y < -max) rslt._y = -max;
    }

    return rslt;
}

//---------------------------------------------------------------------------
// NON-CLASS FUNCTIONS
//---------------------------------------------------------------------------
const PairXy Game::operator+(const PairXy &p1, const PairXy &p2)
{
    return PairXy(p1.x() + p2.x(), p1.y() + p2.y());
}

//! Non-member subration operator.
const PairXy Game::operator-(const PairXy &p1, const PairXy &p2)
{
    return PairXy(p1.x() - p2.x(), p1.y() - p2.y());
}

//! Non-member multiplication operator.
const PairXy Game::operator*(const PairXy &p1, double value)
{
    return PairXy(p1.x() * value, p1.y() * value);
}

//! Non-member division operator.
const PairXy Game::operator/(const PairXy &p1, double value)
{
    return PairXy(p1.x() / value, p1.y() / value);
}
