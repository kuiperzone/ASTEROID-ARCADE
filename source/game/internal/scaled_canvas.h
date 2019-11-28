//---------------------------------------------------------------------------
// PROJECT      : Asteroid Style Game
// COPYRIGHT    : Andy Thomas (C) 2019
// WEB URL      : https://kuiper.zone
// LICENSE      : GPLv3
//---------------------------------------------------------------------------

#ifndef GAME_SCALED_CANVAS_H
#define GAME_SCALED_CANVAS_H

#include "../canvas_interface.h"

namespace Game { namespace Internal {

//! A concrete class which mediates calls to an internal CanvasInterface
//! widget in order to scale dimensions from those of an internal game
//! space to the dynamic dimensions of a GUI widget. This class both
//! encapsulates a CanvasInterface instance, provided on construction,
//! and implements its public methods. Additionally, the class provides
//! an audio enable/disable switch.
class ScaledCanvas final : public CanvasInterface
{
public:

    //! A theoretical internal game width.
    static const int ScaledWidth = 800;

    //! A theoretical internal game height.
    static const int ScaledHeight = 600;

    //! The caller must supply a concrete instance of CanvasInterface which
    //! must remain valid for the lifetime of this instance. Note the widget
    //! is not destroyed by ScaledCanvas and must be deleted by the caller.
    ScaledCanvas(CanvasInterface *widget);

    //! Enable game sounds. If sound is off, the playSound() method
    //! does nothing.
    bool soundOn() const;
    void setSoundOn(bool on);

    //! Equivalent to: drawText(pos, AlignHorz::Center, AlignVert::Top, rem, text)
    inline double drawText(const PairXy &pos, double rem, const std::string &text)
    {
        return drawText(pos, AlignHorz::Center, AlignVert::Top, rem, text);
    }

    // Implements CanvasInterface.
    double width() const override;
    double height() const override;
    void beginDraw() override;
    void endDraw() override;
    void drawLine(const PairXy &p1, const PairXy &p2) override;
    double drawText(const PairXy &pos, AlignHorz horz, AlignVert vert,
        double rem, const std::string &text) override;
    void playSound(SoundId id, SoundOpt opt) override;
    void stopSound(SoundId id) override;

private:

    static const int GameArea = ScaledWidth * ScaledHeight;

    bool _soundOn {true};
    mutable double _scale {-1};
    mutable double _width {-1};
    mutable double _height {-1};
    CanvasInterface *_widget;

    void calcDimensions(bool setScale) const;
};

}} // namespace
#endif
