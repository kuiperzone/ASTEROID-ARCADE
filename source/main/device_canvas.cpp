//---------------------------------------------------------------------------
// PROJECT      : Asteroid Style Game
// COPYRIGHT    : Andy Thomas (C) 2019
// WEB URL      : https://kuiper.zone
// LICENSE      : GPLv3
//---------------------------------------------------------------------------

#include "device_canvas.h"

#include <QtWidgets>

using namespace Game;

//---------------------------------------------------------------------------
// CLASS DeviceCanvas : PUBLIC MEMBERS
//---------------------------------------------------------------------------
DeviceCanvas::DeviceCanvas(QWidget *parent, int padTop, int padBottom)
    : QObject(parent)
{
    _device = parent;
    _padTop = padTop;
    _padBottom = padBottom;

    // Create and initialise media players
    _introPlayer = new QMediaPlayer(this);
    _introPlayer->setAudioRole(QAudio::Role::GameRole);
    _introPlayer->setMedia(QUrl("qrc:/intro_music.mp3"));
    connect(_introPlayer, &QMediaPlayer::stateChanged, this, &DeviceCanvas::introStateChanged);

    _startPlayer = new QMediaPlayer(this, QMediaPlayer::LowLatency);
    _startPlayer->setAudioRole(QAudio::Role::GameRole);
    _startPlayer->setMedia(QUrl("qrc:/start.wav"));
    connect(_startPlayer, &QMediaPlayer::stateChanged, this, &DeviceCanvas::startStateChanged);

    _bigPlayer = new QMediaPlayer(this, QMediaPlayer::LowLatency);
    _bigPlayer->setAudioRole(QAudio::Role::GameRole);
    _bigPlayer->setMedia(QUrl("qrc:/big_explosion.wav"));
    connect(_bigPlayer, &QMediaPlayer::stateChanged, this, &DeviceCanvas::bigStateChanged);

    _mediumPlayer = new QMediaPlayer(this, QMediaPlayer::LowLatency);
    _mediumPlayer->setAudioRole(QAudio::Role::GameRole);
    _mediumPlayer->setMedia(QUrl("qrc:/medium_explosion.wav"));
    connect(_mediumPlayer, &QMediaPlayer::stateChanged, this, &DeviceCanvas::mediumStateChanged);

    _smallPlayer = new QMediaPlayer(this, QMediaPlayer::LowLatency);
    _smallPlayer->setAudioRole(QAudio::Role::GameRole);
    _smallPlayer->setMedia(QUrl("qrc:/small_explosion.wav"));
    connect(_smallPlayer, &QMediaPlayer::stateChanged, this, &DeviceCanvas::smallStateChanged);

    _thrustPlayer = new QMediaPlayer(this, QMediaPlayer::LowLatency);
    _thrustPlayer->setAudioRole(QAudio::Role::GameRole);
    _thrustPlayer->setMedia(QUrl("qrc:/thrust.wav"));
    connect(_thrustPlayer, &QMediaPlayer::stateChanged, this, &DeviceCanvas::thrustStateChanged);

    _gunPlayer = new QMediaPlayer(this, QMediaPlayer::LowLatency);
    _gunPlayer->setAudioRole(QAudio::Role::GameRole);
    _gunPlayer->setMedia(QUrl("qrc:/gun_fire.wav"));
    connect(_gunPlayer, &QMediaPlayer::stateChanged, this, &DeviceCanvas::gunStateChanged);

    // Default
    _canvasFont = QFont().family();

    // Prefered (common on Windows)
    if (!setCanvasFont("Segoe UI Light"))
    {
        // Fall back (common on linux)
        setCanvasFont("DejaVu Sans Light");
    }
}

DeviceCanvas::~DeviceCanvas()
{
    // NB. Parent will delete media players
    delete _painter;
}

QColor DeviceCanvas::foreground() const
{
    return _foreground;
}

void DeviceCanvas::setForeground(QColor col)
{
    _foreground = col;
}

QColor DeviceCanvas::background() const
{
    return _background;
}

void DeviceCanvas::setBackground(QColor col)
{
    _background = col;
}

QString DeviceCanvas::canvasFont() const
{
    return _canvasFont;
}

bool DeviceCanvas::setCanvasFont(const QString& family)
{
    if (QFontDatabase().families().indexOf(family) >= 0)
    {
        // Exists
        _canvasFont = family;
        return true;
    }

    return false;
}

double DeviceCanvas::width() const
{
    return _device->width();
}

double DeviceCanvas::height() const
{
    return _device->height() - _padTop - _padBottom;
}

void DeviceCanvas::beginDraw()
{
    delete _painter;
    _painter = new QPainter(_device);

    QFont f = _painter->font();

    // We can leave empty to use default font
    if (!_canvasFont.isEmpty())
    {
        f.setFamily(_canvasFont);
    }

    _fontSize = f.pointSizeF();
    _painter->setFont(f);

    _painter->setPen(QPen(_foreground));
    _painter->fillRect(0, 0, width(), height(), _background);
}

void DeviceCanvas::endDraw()
{
    delete _painter;
    _painter = nullptr;
}

void DeviceCanvas::drawLine(const PairXy& p1, const PairXy& p2)
{
    if (_painter != nullptr)
    {
        _painter->drawLine(static_cast<int>(p1.x()), static_cast<int>(p1.y()) + _padTop,
            static_cast<int>(p2.x()), static_cast<int>(p2.y()) + _padTop);
    }
}

double DeviceCanvas::drawText(const PairXy& pos, AlignHorz horz, AlignVert vert,
    double rem, const std::string& text)
{
    if (_painter != nullptr && rem > 0)
    {
        QFont f = _painter->font();
        f.setPointSizeF(_fontSize * rem);
        _painter->setFont(f);

        QString s = QString::fromStdString(text);
        QSize qz = QFontMetrics(f, _device).size(0, s);
        QPoint qp = QPoint(static_cast<int>(pos.x()), static_cast<int>(pos.y()) + _padTop);

        switch(horz)
        {
        case AlignHorz::Center:
            qp.setX(qp.x() - qz.width() / 2);
            break;
        case AlignHorz::Right:
            qp.setX(qp.x() - qz.width());
            break;
        default:
            break;
        }

        switch(vert)
        {
        case AlignVert::Top:
            qp.setY(qp.y() + qz.height());
            break;
        case AlignVert::Middle:
            qp.setY(qp.y() + qz.height() / 2);
            break;
        default:
            break;
        }

        _painter->drawText(qp, s);
        return qz.height();
    }

    return 0;
}

void DeviceCanvas::playSound(SoundId id, SoundOpt opt)
{
    QMediaPlayer* player = getPlayer(id, opt == SoundOpt::Loop);

    if (player != nullptr)
    {
        if (player->state() == QMediaPlayer::State::PlayingState)
        {
            if (opt == SoundOpt::Restart)
            {
                player->stop();
                player->play();
            }
        }
        else
        {
            player->play();
        }
    }
}

void DeviceCanvas::stopSound(SoundId id)
{
    QMediaPlayer* player = getPlayer(id, false);

    if (player != nullptr)
    {
        player->stop();
    }
}

//---------------------------------------------------------------------------
// CLASS DeviceCanvas : PRIVATE MEMBERS
//---------------------------------------------------------------------------
QMediaPlayer* DeviceCanvas::getPlayer(SoundId id, bool loop)
{

    switch(id)
    {
    case SoundId::IntroMusic:
        _introLoop = loop;
        return _introPlayer;
    case SoundId::Start:
        _startLoop = loop;
        return _startPlayer;
    case SoundId::BigExplosion:
        _bigLoop = loop;
        return _bigPlayer;
    case SoundId::MediumExplosion:
        _mediumLoop = loop;
        return _mediumPlayer;
    case SoundId::SmallExplosion:
        _smallLoop = loop;
        return _smallPlayer;
    case SoundId::Thrust:
        _thrustLoop = loop;
        return _thrustPlayer;
    case SoundId::GunFire:
        _gunLoop = loop;
        return _gunPlayer;
    default:
        return nullptr;
    }
}

void DeviceCanvas::introStateChanged(QMediaPlayer::State state)
{
    if (_introLoop && state == QMediaPlayer::State::StoppedState)
    {
        // Restart
        _introPlayer->play();
    }
}

void DeviceCanvas::startStateChanged(QMediaPlayer::State state)
{
    if (_startLoop && state == QMediaPlayer::State::StoppedState)
    {
        _startPlayer->play();
    }
}

void DeviceCanvas::bigStateChanged(QMediaPlayer::State state)
{
    if (_bigLoop && state == QMediaPlayer::State::StoppedState)
    {
        _bigPlayer->play();
    }
}

void DeviceCanvas::mediumStateChanged(QMediaPlayer::State state)
{
    if (_mediumLoop && state == QMediaPlayer::State::StoppedState)
    {
        _mediumPlayer->play();
    }
}

void DeviceCanvas::smallStateChanged(QMediaPlayer::State state)
{
    if (_smallLoop && state == QMediaPlayer::State::StoppedState)
    {
        _smallPlayer->play();
    }
}

void DeviceCanvas::thrustStateChanged(QMediaPlayer::State state)
{
    if (_thrustLoop && state == QMediaPlayer::State::StoppedState)
    {
        _thrustPlayer->play();
    }
}

void DeviceCanvas::gunStateChanged(QMediaPlayer::State state)
{
    if (_gunLoop && state == QMediaPlayer::State::StoppedState)
    {
        _gunPlayer->play();
    }
}
