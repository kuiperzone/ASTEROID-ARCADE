//---------------------------------------------------------------------------
// PROJECT      : Asteroid Style Game
// COPYRIGHT    : Andy Thomas (C) 2019
// WEB URL      : https://kuiper.zone
// LICENSE      : GPLv3
//---------------------------------------------------------------------------

#include "universe.h"
#include "scaled_canvas.h"

#include "game_entity.h"
#include "big_rock.h"
#include "medium_rock.h"
#include "small_rock.h"
#include "debris.h"
#include "spark.h"
#include "ship.h"
#include "ufo.h"
#include "bullet.h"
#include "label.h"

#include <cmath>
#include <cstdlib>
#include <ctime>
#include <algorithm>

using namespace Game;
using namespace Game::Internal;

//---------------------------------------------------------------------------
// CLASS Universe : PUBLIC MEMBERS
//---------------------------------------------------------------------------
Universe::Universe(ScaledCanvas *canvas)
    : _canvas {canvas}
{
    _random.seed(static_cast<unsigned long>(std::time(0)));
}

Universe::~Universe()
{
    clear(0);
    delete _canvas;
}

ScaledCanvas* Universe::canvas() const
{
    return _canvas;
}

void Universe::start(int lives)
{
    clear(std::max(lives, 1));

    _ticker = 0;
    _score = 0;
    _gameOver = false;
    _startTick = secondsToTicks(RestartDelay);
}

bool Universe::gameOver() const
{
    return _gameOver;
}

std::int64_t Universe::ticker() const
{
    return _ticker;
}

int Universe::lifeCount() const
{
    return _lifeCount;
}

int Universe::score() const
{
    return _score;
}

void Universe::incScore(int value)
{
    _score += value;

    if (_score > _hiScore)
    {
        _hiScore = _score;
    }
}

int Universe::hiScore() const
{
    return _hiScore;
}

Ship* Universe::ship() const
{
    return _ship;
}

void Universe::advance()
{
    if (_startTick > 0 && _ticker > _startTick)
    {
        if (_lifeCount > 0)
        {
            // New ship
            restart(_lifeCount);
        }
        else
        {
            // Raise finished flag
            _gameOver = true;
        }
    }

    // Give every game object "knowledge" of every other object in the universe.
    // This allows for collisions, hits and UFO awareness behaviour.
    std::size_t count = _entities.size();

    for(std::size_t x = 0; x < count; ++x)
    {
        GameEntity *ex = _entities[x];

        for(std::size_t y = 0; y < count; ++y)
        {
            if (x != y && ex->crunch(_entities[y]))
            {
                GameEntity *ey = _entities[y];

                // Keep score
                if (ey->kind() == EntityKind::Bullet && ex->score() > 0)
                {
                    incScore(ex->score());

                    // Floating score label
                    Label *lab = new Label(this, std::to_string(ex->score()));
                    lab->setVelocity(ex->velocity());
                    add(lab, ex->position());
                }
            }
        }
    }


    // Advance state of every entity.
    int ufoCount = 0;
    double cx = _canvas->width();
    double cy = _canvas->height();

    std::size_t n = 0;
    while(n < _entities.size())
    {
        GameEntity *entity = _entities[n];

        if (entity->advance())
        {
            // Move to next
            n += 1;

            if (entity->kind() == EntityKind::Ufo)
            {
                ufoCount += 1;
            }

            // Toroidal space restricton
            PairXy pos = entity->position();

            if (isDeepRoaming(entity->kind()))
            {
                // Can roam in Kuiper zone
                double kx = KuiperZone * cx;
                double ky = KuiperZone * cy;

                if (pos.x() < -kx) pos.setX(cx + kx);
                else if (pos.x() > cx + kx) pos.setX(-kx);

                if (pos.y() < -ky) pos.setY(cy + ky);
                else if (pos.y() > cy + ky) pos.setY(-ky);
            }
            else
            {
                // Visible region only
                double r = entity->radius();

                if (pos.x() < -r) pos.setX(cx + r);
                else if (pos.x() > cx + r) pos.setX(-r);

                if (pos.y() < -r) pos.setY(cy + r);
                else if (pos.y() > cy + r) pos.setY(-r);
            }

            entity->setPosition(pos);
        }
        else
        {
            if (entity->kind() == EntityKind::Ship)
            {
                _ship = nullptr;
                _lifeCount -= 1;

                if (_lifeCount == 0)
                {
                    _startTick = _ticker + secondsToTicks(GameEndDelay);
                    _canvas->playSound(SoundId::IntroMusic, SoundOpt::None);

                    Label *lab = new Label(this, "GAME OVER", -1);
                    lab->setRem(2);
                    add(lab, Position::Center);
                }
                else
                {
                    _startTick = _ticker + secondsToTicks(RestartDelay);
                }
            }

            // Remove if dead
            delete entity;
            _entities.erase(_entities.begin() + n);
        }
    }

    // Add new rock to game?
    static const double RockRate = MaxRockPerSecond * PollInterval / 1000.0;

    if (random() < RockRate * tickFactor())
    {
        PairXy vel = randomXy(MaxRockSpeed * tickFactor());
        add(new BigRock(this), Position::Kuiper)->setVelocity(vel);
    }

    // Add Ufo to game?
    static const double UfoRate = MaxUfoPerSecond * PollInterval / 1000.0;

    if (ufoCount < MaxUfoCount && random() < UfoRate)
    {
        add(new Ufo(this), Position::Kuiper);
    }

    _ticker += 1;
}

void Universe::draw()
{
    _canvas->beginDraw();

    for(std::size_t x = 0; x < _entities.size(); ++x)
    {
        _entities[x]->draw();
    }

    double cx = _canvas->width();
    double sx = std::max(cx * 0.01, 2.0);
    double cy = _canvas->height();
    double sy = std::max(cy * 0.01, 2.0);

    _canvas->drawText(PairXy(sx, sy), AlignHorz::Left, AlignVert::Top, 1.0,
        "SCORE " + std::to_string(_score));

    _canvas->drawText(PairXy(cx - sx, sy), AlignHorz::Right, AlignVert::Top, 1.0,
        "HISCORE " + std::to_string(_hiScore));

    if (_ship != nullptr)
    {
        _canvas->drawText(PairXy(sx, cy - sy), AlignHorz::Left, AlignVert::Bottom, 1.0,
            std::string(_ship->charge(), '|'));
    }

    _canvas->drawText(PairXy(cx - sx, cy - sy), AlignHorz::Right, AlignVert::Bottom, 1.0,
        "SHIPS " + std::to_string(_lifeCount));

    _canvas->endDraw();
}

GameEntity* Universe::create(EntityKind kind)
{
    switch (kind)
    {
    case EntityKind::Ship: return new Ship(this);
    case EntityKind::BigRock: return new BigRock(this);
    case EntityKind::MediumRock: return new MediumRock(this);
    case EntityKind::SmallRock: return new SmallRock(this);
    case EntityKind::Bullet: return new Bullet(this);
    case EntityKind::Debris: return new Debris(this);
    case EntityKind::Spark: return new Spark(this);
    case EntityKind::Ufo: return nullptr;
    case EntityKind::Label: return new Label(this);
    default: return nullptr;
    }
}

GameEntity* Universe::add(GameEntity *entity, const PairXy& pos)
{
    entity->setPosition(pos);
    _entities.push_back(entity);
    return entity;
}

GameEntity* Universe::add(GameEntity *entity, const PairXy& pos, const PairXy& vel)
{
    entity->setVelocity(vel);
    return add(entity, pos);
}

GameEntity* Universe::add(GameEntity *entity, Position pos)
{
    double cx = _canvas->width();
    double cy = _canvas->height();

    if (pos == Position::Kuiper)
    {
        // Random but in Kuiper region
        double x, y;
        double r = random();

        if (r < 0.25)
        {
            x = -KuiperZone * random(0.2, 0.8) * cx;
            y = random(0, cy);
        }
        else
        if (r < 0.5)
        {
            x = cx + KuiperZone * random(0.2, 0.8) * cx;
            y = random(0, cy);
        }
        else
        if (r < 0.75)
        {
            x = random(0, cx);
            y = -KuiperZone * random(0.2, 0.8) * cy;
        }
        else
        {
            x = random(0, cx);
            y = cy + KuiperZone * random(0.2, 0.8) * cy;
        }

        return add(entity, PairXy(x, y));
    }

    // X center
    cx /= 2;

    if (pos == Position::Upper)
    {
        cy /= 4;
    }
    else
    if (pos == Position::Upper)
    {
        cy *= 3;
        cy /= 4;
    }
    else
    {
        cy /= 2;
    }

    return add(entity, PairXy(cx, cy));
}

double Universe::random() const
{
    std::uniform_real_distribution<double> unif(0.0, 1.0);
    return unif(_random);
}

double Universe::random(double min, double max) const
{
    std::uniform_real_distribution<double> unif(min, max);
    return unif(_random);
}

//---------------------------------------------------------------------------
// CLASS Universe : PRIVATE MEMBERS
//---------------------------------------------------------------------------
void Universe::clear(int lives)
{
    for(std::size_t x = 0; x < _entities.size(); ++x)
    {
        delete _entities[x];
    }

    _lifeCount = lives;
    _startTick = 0;
    _ship = nullptr;
    _entities.clear();
}

void Universe::restart(int lives)
{
    clear(lives);

    _canvas->playSound(SoundId::Start, SoundOpt::None);

    // Faster and more initial rocks when using lives
    int count = StartRocks + StartRocks * _deathCount / 2;
    double speed = MaxRockSpeed * (0.2 + static_cast<double>(_deathCount) / 10);

    for(int n = 0; n < count; ++n)
    {
        add(new BigRock(this), Position::Kuiper)->setVelocity(randomXy(speed));
    }

    _ship = new Ship(this);
    add(_ship, Position::Center);
}
