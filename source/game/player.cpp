//---------------------------------------------------------------------------
// PROJECT      : Asteroid Style Game
// COPYRIGHT    : Andy Thomas (C) 2019
// WEB URL      : https://kuiper.zone
// LICENSE      : GPLv3
//---------------------------------------------------------------------------

#include "player.h"
#include "internal/universe.h"
#include "internal/scaled_canvas.h"
#include "internal/ship.h"
#include "internal/ufo.h"
#include "internal/label.h"

#include <cmath>
#include <cstdlib>
#include <algorithm>

using namespace Game;
using namespace Game::Internal;

//---------------------------------------------------------------------------
// CLASS Player : PUBLIC MEMBERS
//---------------------------------------------------------------------------

// Universe defines this value, but knowledge
// of it is needed by the calling application.
const int Player::PollInterval = Universe::PollInterval;

Player::Player(CanvasInterface *widget)
{
    // Universe will delete ScaledCanvas, but
    // ScaledCanvas won't delete widget.
    _universe = new Internal::Universe(new Internal::ScaledCanvas(widget));
    _universe->canvas()->setSoundOn(true);

    // Keep a separate universe for the demo.
    // We should be OK sharing the same widget.
    _demo = new Internal::Universe(new Internal::ScaledCanvas(widget));
    _demo->canvas()->setSoundOn(false);

    // Initialise state
    for(int n = 0; n < KeyCount; ++n)
    {
        _keyDown[n] = false;
    }
}

Player::~Player()
{
    delete _demo;
    delete _universe;
}

bool Player::soundOn() const
{
    return _universe->canvas()->soundOn();
}

void Player::setSoundOn(bool on)
{
    _universe->canvas()->setSoundOn(on);
}

void Player::startGame()
{
    _page = PageId::Game;
    _universe->start();

    Label* lab = new Label(_universe, "NEW GAME", 2);
    lab->setRem(2);
    _universe->add(lab, Universe::Position::Upper);

    // Ensure reset
    _paused = false;
    _ufoFlag = false;
}

bool Player::inPlay() const
{
    return _page == PageId::Game;
}

void Player::advance()
{
    if (_page != PageId::Game)
    {
        _ticker += 1;

        if (_ticker > 1000 * PageSeconds / PollInterval)
        {
            // Auto next page
            _ticker = 0;
            _page = static_cast<PageId>(static_cast<int>(_page) + 1);

            if (_page > PageId::Demo)
            {
                // Rotate
                _page = PageId::Intro0;
            }
        }
    }

    if (_page == PageId::Intro0 && !_introPlayed)
    {
        // Play once here - at start only
        _introPlayed = true;
        _universe->canvas()->playSound(SoundId::IntroMusic, SoundOpt::None);
    }

    if (_page == PageId::Game)
    {
        if (!_paused)
        {
            _universe->advance();
        }

        if (_universe->gameOver())
        {
            // Game finished
            _ticker = 0;
            _page = PageId::Intro0;
        }
    }

    if (_page == PageId::Demo)
    {
        driveDemo();
    }
}

void Player::draw()
{
    if (_page == PageId::Game)
    {
        // Draw game universe
        _universe->draw();
    }
    else
    if (_page == PageId::Demo )
    {
        // Draw demo universe
        _demo->draw();
    }
    else
    {
        // Draw intro
        drawIntro(_page);
    }
}

bool Player::inkey(KeyId key, bool down)
{
    if (key == KeyId::Sound)
    {
        if (down)
        {
            setSoundOn(!soundOn());
        }

        return true;
    }

    if (key == KeyId::Pause)
    {
        if (down)
        {
            _paused = !_paused;

            if (_paused)
            {
                // An easy way to add a pause indicator
                Label *lab = new Label(_universe, "PAUSED", 0.1);
                lab->setRem(2);
                _universe->add(lab, Universe::Position::Upper);
            }
        }

        return true;
    }

    if (key == KeyId::Quit)
    {
        if (down)
        {
            _ticker = 0;
            _page = PageId::Intro0;
        }

        return true;
    }

    if (_page == PageId::Game)
    {
        auto *ship = _universe->ship();

        if (ship != nullptr)
        {
            switch(key)
            {
            case KeyId::Left:
                ship->rotate(down ? -1 : 0);
                return true;
            case KeyId::Right:
                ship->rotate(down ? +1 : 0);
                return true;
            case KeyId::Thrust:
                ship->thrust(down);
                return true;
            case KeyId::Fire:
                ship->fire(down);
                return true;
            default:
                break;
            }
        }

        return false;
    }

    if (key == KeyId::Fire)
    {
        if (down)
        {
            startGame();
        }

        return true;
    }

    return false;
}

std::string Player::keyName(KeyId key) const
{
    switch(key)
    {
    case KeyId::Left: return "LEFT or J";
    case KeyId::Right: return "RIGHT or L";
    case KeyId::Thrust: return "UP or I";
    case KeyId::Fire: return "SPACE or CTRL";
    case KeyId::Quit: return "ESC";
    case KeyId::Pause: return "P";
    case KeyId::Sound: return "S";
    default: return "";
    }
}

//---------------------------------------------------------------------------
// CLASS Player : PRIVATE MEMBERS
//---------------------------------------------------------------------------
void Player::drawIntro(PageId page)
{
    // Draw onto canvas supplied to the universe
    auto c = _universe->canvas();

    c->beginDraw();

    double x = c->width() / 2;
    double y = c->height() / 8;
    y += c->drawText(PairXy(x, y), 2.5, GameTitle);

    y = std::max(y, c->height() / 4);

    if (page == PageId::Intro0)
    {
        y += c->drawText(PairXy(x, y), 1.5, "CONTROLS");
        y += c->drawText(PairXy(x, y), 1.2, keyName(KeyId::Left) + " = Rotate Left");
        y += c->drawText(PairXy(x, y), 1.2, keyName(KeyId::Right) + " = Rotate Right");
        y += c->drawText(PairXy(x, y), 1.2, keyName(KeyId::Thrust) + " = Thrust");
        y += c->drawText(PairXy(x, y), 1.2, keyName(KeyId::Fire) + " = Fire");
        y += c->drawText(PairXy(x, y), 1.2, keyName(KeyId::Pause) + " = Pause/Resume");
        y += 2.0 * c->drawText(PairXy(x, y), 1.2, keyName(KeyId::Quit) + " = Quit Game");

        std::string s = "(OFF)";
        if (soundOn()) s = "(ON)";
        y += c->drawText(PairXy(x, y), 1.2, keyName(KeyId::Sound) + " = Toggle Sound " + s);
    }
    else
    if (page == PageId::Intro1)
    {
        y += 2.0 * c->drawText(PairXy(x, y), 1.5, "INSERT COIN");
        y += c->drawText(PairXy(x, y), 1.2, "HIGHEST SCORE: " + std::to_string(_universe->hiScore()));
        y += c->drawText(PairXy(x, y), 1.2, "LAST SCORE: " + std::to_string(_universe->score()));
    }
    else
    {
        y += 2.0 * c->drawText(PairXy(x, y), 1.5, "WHERE WORLDS COLLIDE");
        y += c->drawText(PairXy(x, y), 1.2, Copyright);
        y += 2.0 * c->drawText(PairXy(x, y), 1.2, WebUrl);

        y += c->drawText(PairXy(x, y), 1.0, "LICENSE: " + LicenseName);
        y += c->drawText(PairXy(x, y), 1.0, "See About Box for Information");
    }

    y = 7.0 * c->height() / 8;
    c->drawText(PairXy(x, y), AlignHorz::Center, AlignVert::Bottom,
        1.2, "Press " + keyName(KeyId::Fire) + " to Play");

    c->endDraw();
}

void Player::driveDemo()
{
    if (_demo->gameOver())
    {
        // Restart
        _demo->start(1);

        Label* lab = new Label(_demo, "INSERT COIN", -1);
        lab->setRem(2);
        _demo->add(lab, Universe::Position::Upper);
    }

    Ship *ship = _demo->ship();

    if (ship != nullptr)
    {
        double r = _demo->random();

        if (r < 0.1)
        {
            ship->fire(true);
        }
        else
        if (r < 0.15)
        {
            ship->rotate(-1);
            ship->thrust(false);
        }
        else
        if (r < 0.30)
        {
            ship->rotate(+1);
        }
        else
        if (r < 0.45)
        {
            ship->rotate(0);
            ship->thrust(true);
        }
        else
        if (r < 0.50)
        {
            ship->rotate(0);
            ship->fire(false);
        }
        else
        if (r < 0.60)
        {
            ship->fire(false);
            ship->thrust(false);
        }
    }

    _demo->advance();
}
