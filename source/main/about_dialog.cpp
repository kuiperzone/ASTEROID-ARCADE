//---------------------------------------------------------------------------
// PROJECT      : Asteroid Style Game
// COPYRIGHT    : Andy Thomas (C) 2019
// WEB URL      : https://kuiper.zone
// LICENSE      : GPLv3
//---------------------------------------------------------------------------

#include "about_dialog.h"
#include "ui_about_dialog.h"

#include "game/player.h"

#include <algorithm>
#include <cmath>

//---------------------------------------------------------------------------
// CLASS AboutDialog : PUBLIC MEMBERS
//---------------------------------------------------------------------------
AboutDialog::AboutDialog(QWidget *parent)
    : QDialog(parent, (Qt::Dialog | Qt::WindowCloseButtonHint
        | Qt::CustomizeWindowHint | Qt::WindowTitleHint))
{
    // Limit window size
    _ui = new Ui::AboutDialog();
    _ui->setupUi(this);

    // Set fixed width based on font size multiple.
    // This means that the about box will scale according to DPI.
    setFixedSizeEm(30, 40);

    // Initialise content
    populate();
}

AboutDialog::~AboutDialog()
{
    delete _ui;
}

//---------------------------------------------------------------------------
// CLASS AboutDialog : PROTECTED MEMBERS
//---------------------------------------------------------------------------
//---------------------------------------------------------------------------
// CLASS AboutDialog : PRIVATE SLOTS
//---------------------------------------------------------------------------
void AboutDialog::on_okButton_clicked()
{
    hide();
}

//---------------------------------------------------------------------------
// CLASS AboutDialog : PRIVATE MEMBERS
//---------------------------------------------------------------------------
void AboutDialog::populate()
{
    // Call once only

    // Populate text using a template
    QString text = "<html><body style=\"color:$(FORE_COL);\">"
        "<p>Version : $(APP_VERSION)</p>"

        "<p><a href=\"$(LINK_URL)\"><span style=\"text-decoration: underline; "
            "color:$(LINK_COL);\">$(LINK_NAME)</span></a><br/>"
        "$(COPYRIGHT_STATEMENT)</p>"

        "<p><a href=\"$(PROJ_URL)\"><span style=\"text-decoration: underline; "
            "color:$(LINK_COL);\">$(PROJ_NAME)</span></a><br/>"
        "License : $(LICENSE_NAME)</p>"

        "<p><b>Attribution</b><br/>"
        "$(ATTRIB_TEXT)</p>"
        "</body></html>";

    // Colors
    text.replace("$(FORE_COL)", "#B2B29B");
    text.replace("$(LINK_COL)", "#FF6A00");

    // Follow Game constants
    text.replace("$(APP_NAME)", QString::fromStdString(Game::GameTitle));
    text.replace("$(COPYRIGHT_STATEMENT)", QString::fromStdString(Game::Copyright));
    text.replace("$(LICENSE_NAME)", QString::fromStdString(Game::LicenseName));
    text.replace("$(LINK_NAME)", QString::fromStdString(Game::WebUrl));
    text.replace("$(LINK_URL)", QString::fromStdString(Game::WebUrl));

    text.replace("$(PROJ_NAME)", "Github Page");
    text.replace("$(PROJ_URL)", "https://github.com/kuiperzone/ASTEROID-ARCADE");

    // Version is set in PRO file
    text.replace("$(APP_VERSION)", APP_VERSION);

    QString attrib = "Intro Music: Seung Hee Oh (CC-BY)<br/>"
        "Sound Effects: Golgotha (Public Domain)<br/>"
        "This software uses the Qt Framework © The Qt Company."
        "<br/>";

    text.replace("$(ATTRIB_TEXT)", attrib);
    _ui->aboutLabel->setText(text);

    // Set text label to match background.
    _ui->aboutLabel->setStyleSheet("background-color:#27536B");
}

void AboutDialog::setFixedSizeEm(double widthEm, double heightEm)
{
    // Set window size based on font (em units)
    double mult = getEmMultiple(0);
    setFixedSize(widthEm * mult, heightEm * mult);
}

double AboutDialog::getEmMultiple(int fontSize) const
{
    static double pixPerPt = getPixPerPt();

    if (fontSize > 0)
    {
        return fontSize * pixPerPt;
    }

    if (fontSize == 0)
    {
        double f = font().pointSizeF();

        if (f >= 0)
        {
            return f * pixPerPt;
        }

        return font().pixelSize();
    }

    return std::abs(fontSize);
}

double AboutDialog::getPixPerPt()
{
    QFont temp;
    temp.setPointSize(100);
    return static_cast<double>(QFontMetrics(temp).height()) / 100.0;
}
