//---------------------------------------------------------------------------
// PROJECT      : Asteroid Style Game
// COPYRIGHT    : Andy Thomas (C) 2019
// WEB URL      : https://kuiper.zone
// LICENSE      : GPLv3
//---------------------------------------------------------------------------

#ifndef GAME_CANVAS_INTERFACE_H
#define GAME_CANVAS_INTERFACE_H

#include "pair_xy.h"
#include "sound_id.h"

#include <string>

namespace Game {

//! Text horizontal alignment options.
enum class AlignHorz
{
    Left,
    Center,
    Right,
};

//! Text vertical alignment options.
enum class AlignVert
{
    Top,
    Middle,
    Bottom,
};

//! Options used with playSound().
enum class SoundOpt
{
    None, //!< The sound plays and stops when finished. Does nothing if already playing.
    Restart, //!< If already playing, the sound is stopped and played again.
    Loop, //!< Loops until stop is called. If already playing, it is not restarted until it completes.
};

//! A pure virtual class which provides a minimal interface needed to render
//! game objects using only a foreground and background color, as designated
//! at the application level. Additionally, it also defines the methods to play
//! game related sounds.  A concrete subclass of CanvasInterface may, for example,
//! be implemented using Qt, wxWidgets or any API or framwork capable of drawing
//! on a 2D surface and playing audio. The drawing area is that of a standard
//! "window", where the point (0, 0) represents the top-left of the window area.
class CanvasInterface
{
public:

    //! Virtual destructor.
    virtual ~CanvasInterface() {}

    //! The current canvas width in "canvas units". For a direct GDI implementation,
    //! the units are pixels.
    virtual double width() const = 0;

    //! The current canvas height in "canvas units". For a direct GDI implementation,
    //! the units are pixels.
    virtual double height() const = 0;

    //! Activates the "draw state" and clears the widget area by painting a solid
    //! background of a designated background color. For example, in Qt, this
    //! method would create an instance QPainter and call fillRect(). Drawing must
    //! be terminated with a subsequent call to endDraw(). Draw methods should be
    //! called only between these begin and end calls.
    virtual void beginDraw() = 0;

    //! Terminates a previous call to beginDraw(). For example, in Qt, this
    //! method would delete the instance of QPainter created by the previous
    //! beginDraw() call.
    virtual void endDraw() = 0;

    //! Draws a line from position p1 to p2. It is an error to call this method
    //! without first calling beginDraw().
    virtual void drawLine(const PairXy &p1, const PairXy& p2) = 0;

    //! Draws text at position pos. The text is to be aligned horizontally and
    //! vertically according to horz and vert respectively. The text string is
    //! not expected to contain new-line characters and the implementation need
    //! not implement wrapping. The font size is denoted by the relative size
    //! rem, where a value of 1.0 indicates a default "normal" window text size.
    //! The return value is the height of the text drawn in canvas units. It is
    //! an error to call this method without first calling beginDraw().
    virtual double drawText(const PairXy &pos, AlignHorz horz, AlignVert vert,
        double rem, const std::string &text)  = 0;

    //! Asynchronously (non-blocking) plays the sound indicated by id. If audio
    //! is not supported, this method may be implemented such that it does nothing.
    virtual void playSound(SoundId id, SoundOpt opt) = 0;

    //! Stops the sound indicated by id. Does nothing if the sound is not
    //! currently playing.
    virtual void stopSound(SoundId id) = 0;

};

} // namespace
#endif
