//---------------------------------------------------------------------------
// PROJECT      : Asteroid Style Game
// COPYRIGHT    : Andy Thomas (C) 2019
// WEB URL      : https://kuiper.zone
// LICENSE      : GPLv3
//---------------------------------------------------------------------------

#ifndef GAME_UNIVERSE_H
#define GAME_UNIVERSE_H

#include "../pair_xy.h"
#include "../key_id.h"
#include "entity_kind.h"

#include <vector>
#include <string>
#include <cstdint>
#include <random>

namespace Game { namespace Internal {

// Forwards
class GameEntity;
class ScaledCanvas;
class Ship;

//! Maintains game objects, their interactions and core game logic. The start()
//! method must called to initiate a game, and advance() must called every
//! PollInterval seconds, followed by draw(). The ship() accessor should be used
//! to drive the ship in response to user input. The gameOver() flag goes high
//! when finished.
class Universe
{
public:

    //! The advance() poll interval in milliseconds.
    static const int PollInterval = 25;

    //! Position when adding entities to the universe. See the add() method.
    enum class Position
    {
        Kuiper, //!< Instance is added off-screen in the Kuiper zone.
        Center, //!< Added to screen center.
        Upper, //!< Added to the upper part of the screen.
        Lower, //!< Added to the lower part of the screen.
    };

    //! Constructor. The caller must supply an instance ScaledCanvas. This
    //! will be deleted by the class destructor. The start() method should
    //! be called to initialise a new game.
    Universe(ScaledCanvas *canvas);

    //! Destructor.
    virtual ~Universe();

    //! Returns the pointer to ScaledCanvas supplied to the constructor.
    ScaledCanvas* canvas() const;

    //! Starts a new game. If a game is in play, it is restarted.
    void start(int lives = 3);

    //! The number of called to advance().
    std::int64_t ticker() const;

    //! Whether the game has finished It goes to false when state() is called,
    //! and becomes true when the game has ended. The initial value is true.
    bool gameOver() const;

    //! The current number of ship lives.
    int lifeCount() const;

    //! Current or last score.
    int score() const;

    //! Increments the score.
    void incScore(int value);

    //! The highest value of score().
    int hiScore() const;

    //! Gets a pointer to the Ship instance (if it exists). Note that the caller must
    //! check whether the value is null.
    Ship* ship() const;

    //! Advances the game state and increments ticker().
    void advance();

    //! Draws the game on the canvas() instance.
    void draw();

    //! Creates an instance of the given entity kind. The universe state is unchanged.
    GameEntity* create(EntityKind kind);

    //! Adds the entity to the universe. The entity pointer is returned as the result.
    GameEntity* add(GameEntity *entity, const PairXy& pos);

    //! Adds entity and sets both position and velocity.
    GameEntity* add(GameEntity *entity, const PairXy& pos, const PairXy& vel);

    //! Adds the entity to the universe at the position specifed.
    //! The entity pointer is returned as the result.
    GameEntity* add(GameEntity *entity, Position pos);

    //! Generates a pseudo random number in the range [0, 1.0]. The PRNG state
    //! is held by the Universe instance and seeded on construction.
    double random() const;

    //! Generates a pseudo random number in the range [min, max].
    double random(double min, double max) const;

    //! Maps seconds to game ticks.
    static inline std::int64_t secondsToTicks(double sec)
    {
        return static_cast<std::int64_t>(1000.0 * sec / PollInterval);
    }

private:

    // Start delay in seconds.
    static const int RestartDelay = 2;

    // End of game delay in seconds.
    static const int GameEndDelay = 5;

    // Number of seconds to half-way point in game difficulty.
    // Used to determine rock generation rates and speeds etc.
    static const int MidDifficulty = 75;

    // Initial number of rocks
    static const int StartRocks = 12;

    // Maximum rock speed.
    static const int MaxRockSpeed = 5;

    // Maximum number of UFOs at same time.
    static const int MaxUfoCount = 4;

    // Maximum rate at which asteroids can be added to the universe.
    const double MaxRockPerSecond = 1.0;

    // Maximum rate at which UFOs can be added to the universe.
    const double MaxUfoPerSecond = 0.15;

    // A factor used to determine Kuiper region size.
    const double KuiperZone = 0.2;

    int _lifeCount {0};
    int _deathCount {0};
    int _score {0};
    int _hiScore {0};
    bool _gameOver {true};
    std::int64_t _ticker {0};
    std::int64_t _startTick {0};
    ScaledCanvas* _canvas;
    Ship *_ship {nullptr};
    std::vector<GameEntity*> _entities;

    mutable std::ranlux24 _random;

    void clear(int lifeCount);
    void restart(int lifeCount);

    // Generate random velocity.
    PairXy randomXy(double max) const
    {
        return PairXy(random(-max, max), random(-max, max));
    }

    // Returns 0 value [0, 1.0], where result is 0 when game
    // time is 0, and 0.5 when time equals MidDifficulty seconds.
    inline double tickFactor() const
    {
        static double MidTicks = 1000.0 * MidDifficulty / PollInterval;
        return 1.0 - 1.0 / (1.0 + static_cast<double>(_ticker) / MidTicks);
    }

    // Returns true if the kind can enter the off-screen "kuiper zone".
    static inline bool isDeepRoaming(EntityKind kind)
    {
        return kind != EntityKind::Ship && kind != EntityKind::Bullet;
    }
};

}} // namespace
#endif
