//---------------------------------------------------------------------------
// PROJECT      : Asteroid Style Game
// COPYRIGHT    : Andy Thomas (C) 2019
// WEB URL      : https://kuiper.zone
// LICENSE      : GPLv3
//---------------------------------------------------------------------------

#ifndef DEVICE_CANVAS_H
#define DEVICE_CANVAS_H

#include "game/canvas_interface.h"

#include <QObject>
#include <QFont>
#include <QColor>
#include <QMediaPlayer>
#include <QPaintDevice>

namespace Game {

//! Implements CanvasInterface for QPaintDevice. We derive this from both
//! CanvasInterface and QObject in order to take advantage of slots.
class DeviceCanvas : public QObject, public CanvasInterface
{
    Q_OBJECT

public:

    //! Constructor. Padding adds extra space around top and bottom in pixels.
    //! This is needed to cope with menu bar in Qt.
    explicit DeviceCanvas(QWidget *parent, int padTop = 0, int padBottom = 0);
    ~DeviceCanvas();

    //! Foreground color.
    QColor foreground() const;
    void setForeground(QColor col);

    //! Background color.
    QColor background() const;
    void setBackground(QColor col);

    //! The font family name used with drawText().
    QString canvasFont() const;
    bool setCanvasFont(const QString& family);

    // Impement CanvasInterface
    double width() const override;
    double height() const override;
    void beginDraw() override;
    void endDraw() override;
    void drawLine(const PairXy &p1, const PairXy& p2) override;
    double drawText(const PairXy &pos, AlignHorz horz, AlignVert vert,
        double rem, const std::string &text) override;
    void playSound(SoundId id, SoundOpt opt) override;
    void stopSound(SoundId id) override;

private:

    QPaintDevice *_device;
    int _padTop {0};
    int _padBottom {0};
    QPainter *_painter {nullptr};
    QColor _foreground {0x45C6D6};
    QColor _background {0x2E2F30};
    double _fontSize;

    QString _canvasFont;

    // We keep a separate QMediaPlayer instance for each sound type
    // as it allows sounds to overlap. This seems to be fine, although
    // we don't want too many instances as there may be a resource
    // limit. For reference, here's a guy running into problems
    // with +100 QMediaPlayer instances:
    // https://stackoverflow.com/questions/42272618/qt-qmediaplayer-qthreadstart-failed-to-create-thread
    QMediaPlayer *_introPlayer;
    bool _introLoop {false};

    QMediaPlayer *_startPlayer;
    bool _startLoop {false};

    QMediaPlayer *_bigPlayer;
    bool _bigLoop {false};

    QMediaPlayer *_mediumPlayer;
    bool _mediumLoop {false};

    QMediaPlayer *_smallPlayer;
    bool _smallLoop {false};

    QMediaPlayer *_thrustPlayer;
    bool _thrustLoop {false};

    QMediaPlayer *_gunPlayer;
    bool _gunLoop {false};

    QMediaPlayer* getPlayer(SoundId id, bool loop);

    // Used to loop play
    void introStateChanged(QMediaPlayer::State state);
    void startStateChanged(QMediaPlayer::State state);
    void bigStateChanged(QMediaPlayer::State state);
    void mediumStateChanged(QMediaPlayer::State state);
    void smallStateChanged(QMediaPlayer::State state);
    void thrustStateChanged(QMediaPlayer::State state);
    void gunStateChanged(QMediaPlayer::State state);
};

} // namespace
#endif

