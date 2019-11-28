//---------------------------------------------------------------------------
// PROJECT      : Asteroid Style Game
// COPYRIGHT    : Andy Thomas (C) 2019
// WEB URL      : https://kuiper.zone
// LICENSE      : GPLv3
//---------------------------------------------------------------------------

#include "scaled_canvas.h"

#include <cmath>
#include <algorithm>

using namespace Game::Internal;

//---------------------------------------------------------------------------
// CLASS ScaledCanvas : PUBLIC MEMBERS
//---------------------------------------------------------------------------
ScaledCanvas::ScaledCanvas(CanvasInterface *widget)
    : _widget(widget)
{
}

bool ScaledCanvas::soundOn() const
{
    return _soundOn;
}

void ScaledCanvas::setSoundOn(bool on)
{
    if (_soundOn != on)
    {
        _soundOn = on;

        if (!_soundOn)
        {
            // Ensure music is stopped
            _widget->stopSound(SoundId::IntroMusic);
        }
    }
}

double ScaledCanvas::width() const
{
    if (_width < 0)
    {
        calcDimensions(false);
    }

    return _width;
}

double ScaledCanvas::height() const
{
    if (_height < 0)
    {
        calcDimensions(false);
    }

    return _height;
}

void ScaledCanvas::beginDraw()
{
    if (_scale <= 0)
    {
        calcDimensions(true);
        _widget->beginDraw();
    }
}

void ScaledCanvas::endDraw()
{
    _widget->endDraw();
    _scale = -1;
}

void ScaledCanvas::drawLine(const PairXy &p1, const PairXy &p2)
{
    if (_scale > 0)
    {
        _widget->drawLine(p1 * _scale, p2 * _scale);
    }
}

double ScaledCanvas::drawText(const PairXy &pos, AlignHorz horz, AlignVert vert,
    double rem, const std::string &text)
{
    if (_scale > 0)
    {
        return _widget->drawText(pos * _scale, horz, vert, rem * _scale, text) / _scale;
    }

    return 0;
}

void ScaledCanvas::playSound(SoundId id, SoundOpt opt)
{
    if (_soundOn)
    {
        _widget->playSound(id, opt);
    }
}

void ScaledCanvas::stopSound(SoundId id)
{
    _widget->stopSound(id);
}

//---------------------------------------------------------------------------
// CLASS ScaledCanvas : PRIVATE MEMBERS
//---------------------------------------------------------------------------
void ScaledCanvas::calcDimensions(bool setScale) const
{
    // Follow device aspect ratio of the internal dimensions,
    // while keeping the area approximately constant. This seems to
    // work well, allowing the window size to changed while
    // keeping the game in motion.
    _width = _widget->width();
    _height = _widget->height();

    if (_height > 0)
    {
      double ratio = _width / _height;
      _width = std::sqrt(ratio * GameArea);
      _height = _width / ratio;
    }

    if (setScale)
    {
        if (_width > 0 && _height > 0)
        {
            _scale = std::sqrt((_widget->width() * _widget->height()) / (_width * _height));
        }
        else
        {
            // Not expected,
            // but let's not crash.
            _scale = 1.0;
        }
    }
}
