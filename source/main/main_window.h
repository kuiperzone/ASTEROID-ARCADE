//---------------------------------------------------------------------------
// PROJECT      : Asteroid Style Game
// COPYRIGHT    : Andy Thomas (C) 2019
// WEB URL      : https://kuiper.zone
// LICENSE      : GPLv3
//---------------------------------------------------------------------------

#ifndef MAIN_WINDOW_H
#define MAIN_WINDOW_H

#include "game/key_id.h"

#include <QtWidgets>

// Forwards
class AboutDialog;
namespace Ui { class MainWindow; }

namespace Game {
class Player;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:

    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

protected:

    void showEvent(QShowEvent *event) override;
    void hideEvent(QHideEvent *event) override;
    void paintEvent(QPaintEvent *event) override;
    void keyPressEvent(QKeyEvent *event) override;
    void keyReleaseEvent(QKeyEvent *event) override;

private slots:

    void on_actionGame_StartGame_triggered();
    void on_actionGame_QuitGame_triggered();
    void on_actionGame_Sounds_triggered();
    void on_actionGame_Exit_triggered();

    void on_actionHelp_Website_triggered();
    void on_actionHelp_About_triggered();

private:

    Ui::MainWindow *_ui;
    QTimer _pollTimer;
    Game::Player *_player;
    AboutDialog *_dialog;

    void advanceUniverse();
    void updateMenuState();
    static Game::KeyId gameKey(int key);

};

#endif
