//---------------------------------------------------------------------------
// PROJECT      : Asteroid Style Game
// COPYRIGHT    : Andy Thomas (C) 2019
// WEB URL      : https://kuiper.zone
// LICENSE      : GPLv3
//---------------------------------------------------------------------------

#ifndef ABOUT_DIALOG_H
#define ABOUT_DIALOG_H

#include <QtWidgets>

namespace Ui { class AboutDialog; }

//! About dialog box.
class AboutDialog : public QDialog
{
    Q_OBJECT

public:

    explicit AboutDialog(QWidget *parent = nullptr);
    ~AboutDialog();

private slots:

    void on_okButton_clicked();

private:

    Ui::AboutDialog *_ui;

    void populate();
    void setFixedSizeEm(double widthEm, double heightEm);

    double getEmMultiple(int fontSize) const;
    static double getPixPerPt();
};

#endif
