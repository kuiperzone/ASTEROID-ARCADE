//---------------------------------------------------------------------------
// PROJECT      : Asteroid Style Game
// COPYRIGHT    : Andy Thomas (C) 2019
// WEB URL      : https://kuiper.zone
// LICENSE      : GPLv3
//---------------------------------------------------------------------------

#ifndef GAME_PAIR_XY_H
#define GAME_PAIR_XY_H

namespace Game {

//! An x-y value pair which can be used to specify both position and
//! velocity. A basic set of operators are also provided.
class PairXy
{
public:

    //! Default constructor. Both x and y are assigned 0.
    PairXy() = default;

     //! Copy constructor.
    PairXy(const PairXy &other) = default;

    //! Assignment constructor.
    PairXy(double x, double y);

    //! Explicity constructor. If nan is true, both x and y are
    //! assigned the value NAN. If nan is false, both are assigned 0.
    explicit PairXy(bool nan);

    //! Gets and sets x coordinate.
    double x() const { return _x; }
    void setX(double x) { _x = x; }

    //! Gets and sets y coordinate.
    double y() const { return _y; }
    void setY(double y) { _y = y; }

    //! Returns true if either x or y are NAN.
    bool isNaN() const;

    //! Returns the absolute magntitude.
    double abs() const;

    //! Returns a copy in which each component is no greater in
    //! magnitude than max. Max should be a positive value.
    const PairXy throttle(double max) const;

    //! Rotates the point around the origin by angle alpha (radians).
    const PairXy rotate(double alpha) const;

    //! Addition operator.
    PairXy& operator+= (const PairXy &other)
    {
        _x += other._x;
        _y += other._y;
        return *this;
    }

    //! Subtraction operator.
    PairXy& operator-= (const PairXy &other)
    {
        _x -= other._x;
        _y -= other._y;
        return *this;
    }

    //! Multiplication operator.
    PairXy& operator*= (double value)
    {
        _x *= value;
        _y *= value;
        return *this;
    }

    //! Division operator.
    PairXy& operator/= (double value)
    {
        _x /= value;
        _y /= value;
        return *this;
    }

    //! Equality operator.
    bool operator== (const PairXy &other) const
    {
        return _x == other._x && _y == other._y;
    }

    //! Inequality operator.
    bool operator!= (const PairXy &other) const
    {
        return _x != other._x || _y != other._y;
    }

private:

    double _x {0};
    double _y {0};

};

//! Non-member addition operator.
const PairXy operator+(const PairXy &p1, const PairXy &p2);

//! Non-member subration operator.
const PairXy operator-(const PairXy &p1, const PairXy &p2);

//! Non-member multiplication operator.
const PairXy operator*(const PairXy &p1, double value);

//! Non-member division operator.
const PairXy operator/(const PairXy &p1, double value);

} // namespace
#endif
