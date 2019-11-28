//---------------------------------------------------------------------------
// PROJECT      : Asteroid Style Game
// COPYRIGHT    : Andy Thomas (C) 2019
// WEB URL      : https://kuiper.zone
// LICENSE      : GPLv3
//---------------------------------------------------------------------------

#include "main_window.h"
#include "ui_main_window.h"

#include "about_dialog.h"

#include "device_canvas.h"
#include "game/player.h"

//---------------------------------------------------------------------------
// CLASS MainWindow : PUBLIC MEMBERS
//---------------------------------------------------------------------------
MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent)
{
    _ui = new Ui::MainWindow();
    _ui->setupUi(this);

    setMinimumSize(300, 225);
    setWindowTitle(QString::fromStdString(Game::GameTitle));

    // DeviceCanvas is a QObject and will be deleted by parent
    Game::DeviceCanvas *canvas = new Game::DeviceCanvas(this, menuBar()->height(), 5);

    _player = new Game::Player(canvas);
    connect(&_pollTimer, &QTimer::timeout, this, &MainWindow::advanceUniverse);

    // Set default background color to match canvas
    QPalette pal = palette();
    pal.setColor(QPalette::Background, canvas->background());
    this->setPalette(pal);

    _dialog = new AboutDialog(this);
}

MainWindow::~MainWindow()
{
    delete _player;
}

//---------------------------------------------------------------------------
// CLASS MainWindow : PROTECTED MEMBERS
//---------------------------------------------------------------------------
void MainWindow::showEvent(QShowEvent *event)
{
    QMainWindow::showEvent(event);

    // Start timer
    _pollTimer.start(Game::Player::PollInterval);
}

void MainWindow::hideEvent(QHideEvent *event)
{
    // Stop updates
    _pollTimer.stop();
    QMainWindow::hideEvent(event);
}

void MainWindow::paintEvent(QPaintEvent *)
{
    _player->draw();
}

void MainWindow::keyPressEvent(QKeyEvent *event)
{
    if (!_player->inkey(gameKey(event->key()), true))
    {
        QMainWindow::keyPressEvent(event);
    }
}

void MainWindow::keyReleaseEvent(QKeyEvent *event)
{
    if (!_player->inkey(gameKey(event->key()), false))
    {
        QMainWindow::keyPressEvent(event);
    }
}

//---------------------------------------------------------------------------
// CLASS MainWindow : PRIVATE SLOTS
//---------------------------------------------------------------------------
void MainWindow::on_actionGame_StartGame_triggered()
{
    _player->startGame();
}

void MainWindow::on_actionGame_QuitGame_triggered()
{
    _player->inkey(Game::KeyId::Quit, true);
}

void MainWindow::on_actionGame_Sounds_triggered()
{
    _player->setSoundOn(!_player->soundOn());
}

void MainWindow::on_actionGame_Exit_triggered()
{
    close();
}

void MainWindow::on_actionHelp_Website_triggered()
{
    QDesktopServices::openUrl(QString::fromStdString(Game::WebUrl));
}

void MainWindow::on_actionHelp_SourceCode_triggered()
{
    // Assume runtime in "bin" side-by-side with doc directory
    QUrl url = QUrl::fromLocalFile(QApplication::applicationDirPath() + "/../doc/index.html");
    QDesktopServices::openUrl(url);
}

void MainWindow::on_actionHelp_About_triggered()
{
    _dialog->show();
}

//---------------------------------------------------------------------------
// CLASS MainWindow : PRIVATE MEMBERS
//---------------------------------------------------------------------------
void MainWindow::advanceUniverse()
{
    _player->advance();
    update();

    updateMenuState();
}

void MainWindow::updateMenuState()
{
    bool playing = _player->inPlay();
    _ui->actionGame_StartGame->setEnabled(!playing);
    _ui->actionGame_QuitGame->setEnabled(playing);
    _ui->actionGame_Sounds->setChecked(_player->soundOn());
}

Game::KeyId MainWindow::gameKey(int key)
{
    // Map Qt key to GameKey::KeyId
    switch(key)
    {
    case Qt::Key_J:
    case Qt::Key_Left:
        return Game::KeyId::Left;

    case Qt::Key_L:
    case Qt::Key_Right:
        return Game::KeyId::Right;

    case Qt::Key_I:
    case Qt::Key_Up:
        return Game::KeyId::Thrust;

    case Qt::Key_Space:
    case Qt::Key_Control:
        return Game::KeyId::Fire;

    case Qt::Key_Escape:
        return Game::KeyId::Quit;

    case Qt::Key_P:
        return Game::KeyId::Pause;

    case Qt::Key_S:
        return Game::KeyId::Sound;

    // N/A
    default:
        return Game::KeyId::Count;
    }
}
