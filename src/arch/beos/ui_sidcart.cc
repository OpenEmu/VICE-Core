/*
 * ui_sidcart.cc - SID cart settings
 *
 * Written by
 *  Marcus Sutton <loggedoubt@gmail.com>
 *
 * This file is part of VICE, the Versatile Commodore Emulator.
 * See README for copyright notice.
 *
 *  This program is free software; you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation; either version 2 of the License, or
 *  (at your option) any later version.
 *
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with this program; if not, write to the Free Software
 *  Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA
 *  02111-1307  USA.
 *
 */

#include <Box.h>
#include <CheckBox.h>
#include <OptionPopUp.h>
#include <RadioButton.h>
#include <Slider.h>
#include <string.h>
#include <Window.h>

#include <stdlib.h>

extern "C" {
#include "constants.h"
#include "machine.h"
#include "resources.h"
#include "sid.h"
#include "ui.h"
#include "ui_sidcart.h"
#include "util.h"
#include "vsync.h"
}

static char *samplingmode[] = {
    "fast sampling",
    "interpolating",
    "resampling",
    "fast resampling",
    NULL
};

static sid_engine_model_t **sid_engine_model_list;

static char **sidaddresspair;
static char **sidclockpair;

class SidCartWindow : public BWindow {
        BOptionPopUp *engine_model_popup;
        BSlider *passbandslider;
        BBox *addressbox;
        BBox *clockbox;
        BBox *residbox;

        void EnableBoxedControls(BBox *box, int enable);
        void EnableReSidControls(int engine);
        void EnableControls();
    public:
        SidCartWindow();
        ~SidCartWindow();
        virtual void MessageReceived(BMessage *msg);
};

static SidCartWindow *sidcartwindow = NULL;

void SidCartWindow::EnableBoxedControls(BBox *box, int enable)
{
    int32 children, i;

    children = box->CountChildren();
    for (i = 0; i < children; i++) {
        ((BControl *)box->ChildAt(i))->SetEnabled(enable);
    }

}

void SidCartWindow::EnableReSidControls(int engine)
{
    int enable;

    enable = ((BControl *)FindView("SidCart"))->Value();
    EnableBoxedControls(residbox, enable && ((engine == SID_ENGINE_RESID) || (engine == SID_ENGINE_RESID_FP)));
}

void SidCartWindow::EnableControls()
{
    int enable, engine;

    enable = ((BControl *)FindView("SidCart"))->Value();
    engine_model_popup->SetEnabled(enable);
    ((BControl *)FindView("SidFilters"))->SetEnabled(enable);
    EnableBoxedControls(addressbox, enable);
    EnableBoxedControls(clockbox, enable);
    if (machine_class == VICE_MACHINE_PLUS4) {
        ((BControl *)FindView("DIGIBLASTER"))->SetEnabled(enable);
    }

    resources_get_int("SidEngine", &engine);
    EnableReSidControls(engine);
}

SidCartWindow::SidCartWindow() 
    : BWindow(BRect(250, 50, 500, 340), "SID cartridge settings", B_TITLED_WINDOW_LOOK, B_MODAL_APP_WINDOW_FEEL, B_NOT_ZOOMABLE | B_NOT_RESIZABLE) 
{
    BMessage *msg;
    BCheckBox *checkbox;
    BRect r;
    BRadioButton *radiobutton;
    BView *background;
    int engine, res_val, i;

    r = Bounds();
    background = new BView(r, NULL,  B_FOLLOW_NONE, B_WILL_DRAW);
    background->SetViewColor(220, 220, 220, 0);
    AddChild(background);

    /* SID cart emulation */
    resources_get_int("SidCart", &res_val);
    checkbox = new BCheckBox(BRect(10, 10, 240, 25), "SidCart", "Enable SID cartridge", new BMessage(MESSAGE_SIDCART_ENABLE));
    checkbox->SetValue(res_val);
    background->AddChild(checkbox);

    /* SID model */
    sid_engine_model_list = sid_get_engine_model_list();
    resources_get_int("SidModel", &i);
    resources_get_int("SidEngine", &engine);
    res_val = engine << 8;
    res_val |= i;
    engine_model_popup = new BOptionPopUp(BRect(10, 35, 240, 59), "SID Engine/Model", "SID Engine/Model", new BMessage(MESSAGE_SIDCART_MODEL));
    for (i = 0; sid_engine_model_list[i] != NULL; i++) {
        engine_model_popup->AddOption(sid_engine_model_list[i]->name, sid_engine_model_list[i]->value);
    }
    engine_model_popup->SelectOptionFor(res_val);
    background->AddChild(engine_model_popup);

    /* SID filter */
    resources_get_int("SidFilters", &res_val);
    checkbox = new BCheckBox(BRect(10, 60, 240, 75), "SidFilters", "SID filters", new BMessage(MESSAGE_SIDCART_FILTERS));
    checkbox->SetValue(res_val);
    background->AddChild(checkbox);

    /* SID address */
    addressbox = new BBox(BRect(10, 85, 120, 145), "SID address");
    addressbox->SetViewColor(220, 220, 220, 0);
    addressbox->SetLabel("SID address");
    background->AddChild(addressbox);

    resources_get_int("SidAddress", &res_val);
    for (i = 0; i < 2; i++) {
        msg = new BMessage(MESSAGE_SIDCART_ADDRESS);
        msg->AddInt32("address", i);
        radiobutton = new BRadioButton(BRect(10, 15 + i * 20, 100, 30 + i * 20), sidaddresspair[i], sidaddresspair[i], msg);
        radiobutton->SetValue(res_val == i);
        addressbox->AddChild(radiobutton);
    }

    /* SID clock */
    clockbox = new BBox(BRect(130, 85, 240, 145), "SID clock");
    clockbox->SetViewColor(220, 220, 220, 0);
    clockbox->SetLabel("SID clock");
    background->AddChild(clockbox);

    resources_get_int("SidClock", &res_val);
    for (i = 0; i < 2; i++) {
        msg = new BMessage(MESSAGE_SIDCART_CLOCK);
        msg->AddInt32("clock", i);
        radiobutton = new BRadioButton(BRect(10, 15 + i * 20, 100, 30 + i * 20), sidclockpair[i], sidclockpair[i], msg);
        radiobutton->SetValue(res_val == i);
        clockbox->AddChild(radiobutton);
    }

    /* DIGIBLASTER */
    if (machine_class == VICE_MACHINE_PLUS4) {
        resources_get_int("DIGIBLASTER", &res_val);
        checkbox = new BCheckBox(BRect(10, 155, 240, 170), "DIGIBLASTER", "Enable digiblaster add-on", new BMessage(MESSAGE_SIDCART_DIGIBLASTER));
        checkbox->SetValue(res_val);
        background->AddChild(checkbox);
    } else {
        ResizeTo(250, 265);
    }

    /* reSID settings */
    r = Bounds();
    r.InsetBy(10, 10);
    r.top = r.bottom - 100;
    residbox = new BBox(r, "reSID/reSID-fp settings");
    residbox->SetViewColor(220, 220, 220, 0);
    residbox->SetLabel("reSID/reSID-fp settings");
    background->AddChild(residbox);
    r = residbox->Bounds();

    /* sampling method */
    resources_get_int("SidResidSampling", &res_val);
    for (i = 0; samplingmode[i] != NULL; i++) {
        msg = new BMessage(MESSAGE_SIDCART_RESIDSAMPLING);
        msg->AddInt32("mode", i);
        radiobutton = new BRadioButton(BRect(10, 15 + i * 20, r.Width() / 2 - 10, 30 + i * 20), samplingmode[i], samplingmode[i], msg);
        radiobutton->SetValue(res_val == i);
        residbox->AddChild(radiobutton);
    }

    resources_get_int("SidResidPassband", &res_val);
    passbandslider = new BSlider(BRect(r.Width() / 2 + 10, 20, r.Width() - 10, 60), "Passband", "Passband", new BMessage(MESSAGE_SIDCART_RESIDPASSBAND), 0, 90, B_TRIANGLE_THUMB);
    passbandslider->SetValue(res_val);
    passbandslider->SetHashMarkCount(10);
    passbandslider->SetHashMarks(B_HASH_MARKS_BOTTOM);
    passbandslider->SetLimitLabels("0", "90");
    residbox->AddChild(passbandslider);

    EnableControls();

    Show();
}

SidCartWindow::~SidCartWindow() 
{
    sidcartwindow = NULL;
}

void SidCartWindow::MessageReceived(BMessage *msg)
{
    int32 engine, val;

    switch (msg->what) {
        case MESSAGE_SIDCART_ENABLE:
            resources_toggle("SidCart", (int *)&val);
            EnableControls();
            break;
        case MESSAGE_SIDCART_MODEL:
            val = engine_model_popup->Value();
            engine = val >> 8;
            val &= 0xff;
            sid_set_engine_model(engine, val);
            EnableReSidControls(engine);
            break;
        case MESSAGE_SIDCART_FILTERS:
            resources_toggle("SidFilters", (int *)&val);
            break;
        case MESSAGE_SIDCART_ADDRESS:
            val = msg->FindInt32("address");
            resources_set_int("SidAddress", (int)val);
            break;
        case MESSAGE_SIDCART_CLOCK:
            val = msg->FindInt32("clock");
            resources_set_int("SidClock", (int)val);
            break;
        case MESSAGE_SIDCART_DIGIBLASTER:
            resources_toggle("DIGIBLASTER", (int *)&val);
            break;
        case MESSAGE_SIDCART_RESIDSAMPLING:
            val = msg->FindInt32("mode");
            resources_set_int("SidResidSampling", val);
            break;
        case MESSAGE_SIDCART_RESIDPASSBAND:
            resources_set_int("SidResidPassband", passbandslider->Value());
            break;
        default:
            BWindow::MessageReceived(msg);
    }
}

void ui_sidcart(char **cartaddresspair, char **cartclockpair)
{
    thread_id sidcartthread;
    status_t exit_value;

    if (sidcartwindow != NULL) {
        return;
    }

    sidaddresspair = cartaddresspair;
    sidclockpair = cartclockpair;

    sidcartwindow = new SidCartWindow;

    vsync_suspend_speed_eval();

    /* wait until window closed */
    sidcartthread = sidcartwindow->Thread();
    wait_for_thread(sidcartthread, &exit_value);
}
