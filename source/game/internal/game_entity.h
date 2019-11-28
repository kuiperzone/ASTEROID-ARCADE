//---------------------------------------------------------------------------
// PROJECT      : Asteroid Style Game
// COPYRIGHT    : Andy Thomas (C) 2019
// WEB URL      : https://kuiper.zone
// LICENSE      : GPLv3
//---------------------------------------------------------------------------

#ifndef GAME_ENTITY_H
#define GAME_ENTITY_H

#include "../pair_xy.h"
#include "../sound_id.h"
#include "entity_kind.h"

#include <vector>
#include <cstdint>

namespace Game { namespace Internal {

// Forward declarations
class Universe;

//! An abstract base class for all objects in the game universe.
class GameEntity
{
public:

    //! Maximum value of velocity axes.
    static const int SpeedOfLight = 20;

    //! Constructor with the Universe to which the entity belongs.
    GameEntity(Universe *owner);

    //! Virtual destructor.
    virtual ~GameEntity();

    //! Gets the owning Universe of this instance.
    Universe* owner() const;

    //! Identifies the entity kind. To be implemented by subclass.
    virtual EntityKind kind() const = 0;

    //! Position getter and setter as measured in arbitrary game units.
    //! The initial value is (0, 0).
    PairXy position() const;
    void setPosition(const PairXy &value);

    //! Velocity getter and setter as measured in arbitrary game distance units
    //! per time "tick". The maximum speed is always restricted to MaxSpeed.
    //! The initial value is (0, 0).
    PairXy velocity() const;
    void setVelocity(const PairXy &value);

    //! Rotation angle in radians. The range is restricted to [0, 2 * M_PI) and
    //! setting values outside this range sets the remainder. The initial value is 0.
    double alpha() const;
    void setAlpha(double rads);

    //! Amount added to score if entity is hit by fire and destroyed.
    //! The base class implementation returns 0.
    virtual int score() const { return 0; }

    //! Mass in arbitrary game units. Objects with zero mass are ghost entities
    //! which do not collide. The base implementation returns 0.
    virtual double mass() const { return 0; }

    //! The object's size expressed as a radius. It is calculated
    //! automatically from the polygon and is used in collision detection.
    double radius() const;

    //! The age of object in ticks. It is 0 on creation and incremented on
    //! each call to advance().
    std::int64_t ticker() const;

    //! The mamimum allowed age of the instance in seconds after which advance()
    //! returns false indicating that the object should be removed. A value of 0
    //! or less indicates no maximum age. The value can be a fraction. The initial
    //! value is 0.
    double maxSeconds() const;

    //! Calculates the object's reaction to another game object. If the two object's
    //! are in collision, this instance will be rebound using conservation of momentum
    //! laws (the state of other is unaffected). The result is true if the object is
    //! in collision. It is anticipated that crunch() be called on every game entity
    //! for every other entity prior to calling the advance() method. The method is
    //! virtual so it may be overridden.
    virtual bool crunch(GameEntity *other);

    //! Advances the object's state by one tick. This means that ticker() will be
    //! incremented by +1 and position() will be incremented by velocity(). The
    //! result is true if the object is "alive" on return, or false if the object
    //! has ceased to exist and should be removed from the game state. Note that
    //! this method may call owner()->add() to add items to the universe.
    virtual bool advance();

    //! Draws the entity. The object is drawn on the owner() canvas as a
    //! polygon defined by the setPolygon() method property.
    virtual void draw();

protected:

    //! Protected setter for maxSeconds().
    void setMaxSeconds(double sec);

    //! Sets polygon draw points. The polygon will be drawn as a sequence of lines
    //! starting at points[0]. A PaiyXy value where isNaN() is true will serve as
    //! a break in the sequence. This method is expected to be called in the
    //! constructor of the subclass only in order to define the object's appearance.
    void setPolygon(const std::vector<PairXy> &points);

    //! Sets circular polygon points derived from a radius value and a point count.
    //! If randomize is true, the random variation is added to the point positions.
    //! This method is typically used for generating asteroids.
    void setPolygon(double radius, int count = 21, bool randomize = true);

    //! Gets the polygon points as rotated by alpha().
    const std::vector<PairXy>& polygon() const;

private:

    void setAlpha(double rads, bool force);

    Universe * _owner;
    bool _isAlive {true};
    PairXy _position;
    PairXy _velocity;
    double _alpha {0};
    PairXy _nextVelocity;
    double _radius {0};
    std::int64_t _ticker {0};
    double _maxSeconds {-1};
    std::int64_t _maxTicks {-1};
    std::vector<PairXy> _polyAlpha;
    std::vector<PairXy> _polySource;
};

}} // namespace
#endif
