//---------------------------------------------------------------------------
// PROJECT      : Asteroid Style Game
// COPYRIGHT    : Andy Thomas (C) 2019
// WEB URL      : https://kuiper.zone
// LICENSE      : GPLv3
//---------------------------------------------------------------------------

#ifndef GAME_PLAYER_H
#define GAME_PLAYER_H

#include "canvas_interface.h"
#include "pair_xy.h"
#include "key_id.h"

#include <vector>
#include <string>

namespace Game {

//! The name of the game.
const std::string GameTitle = "ASTEROID ARCADE";

//! Copyright string.
const std::string Copyright = "Copyright \u00a9 Andy Thomas 2019";

//! License name.
const std::string LicenseName = "GPLv3";

//! Contact address
const std::string WebUrl = "https://kuiper.zone";

// Forwards
namespace Internal {
class Universe;
}

//! Player is a platform and GUI framwork independent class which encapsulates
//! all game state and logic. It depends on C++11 only. In order to the draw
//! game and play game sounds, the application level code must provide
//! Player with a concrete implementation of CanvasInterface on instantiation.
//! CanvasInterface can be implemented using Qt, WxWidgets or any suitable
//! GUI/GDI API. A Player class instance is single threaded and is "poll driven".
//! The application must create a timer in order to call Player::advance() at
//! an interval of Player::PollInterval milliseconds. Keyboard input should
//! be provided to the class instance with the inkey() method.
class Player
{
public:

    //! The advance() poll interval in milliseconds.
    static const int PollInterval;

    //! Constructor. The caller must supply a concrete instance of CanvasInterface
    //! which must remain valid for the lifetime of this instance. Universe will
    //! not delete it on destruction.
    Player(CanvasInterface *widget);

    //! Destructor.
    virtual ~Player();

    //! Called to start a new game. If a game is in play, it is reset.
    void startGame();

    //! Returns true if a game is in play.
    bool inPlay() const;

    //! Enable game sounds.
    bool soundOn() const;

    //! Enable game sounds.
    void setSoundOn(bool on);

    //! Advances the game state, but does not draw the game on the CanvasInterface.
    //! This should be called by a timer every PollInterval milliseconds.
    void advance();

    //! Draws the game on the CanvasInstance instance supplied to the constructor.
    //! It would typically be called immediately after advance(), but may also be
    //! called at any time to refresh to widget area.
    void draw();

    //! Inputs an action ID. The method should be called on key down and
    //! key up events. Down should be false when the key is raised. The
    //! result is true if key is not KeyId::Count.
    bool inkey(KeyId key, bool down);

    //! Gets a short descriptive name for the given action key. This is used
    //! in the introductory screens to display game keys. It may potentially
    //! by overridden to display different values.
    virtual std::string keyName(KeyId key) const;

private:

    // Seconds per page
    static const int PageSeconds = 8;

    static const int KeyCount = static_cast<int>(KeyId::Count);

    // Screen mode state IDs
    enum class PageId {Intro0 = 0, Intro1, Intro2, Demo, Game};

    int _ticker {0};
    bool _introPlayed {false};
    bool _keyDown[KeyCount];
    bool _ufoFlag {false};
    bool _paused {false};
    KeyId _simkey {KeyId::Count};

    PageId _page {PageId::Intro0};
    Internal::Universe *_universe;
    Internal::Universe *_demo;

    void drawIntro(PageId page);
    void driveDemo();
};

} // namespace
#endif
