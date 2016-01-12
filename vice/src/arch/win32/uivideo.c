/*
 * uivideo.c - Implementation of video settings dialog box.
 *
 * Written by
 *  Andreas Boose <viceteam@t-online.de>
 *  Ettore Perazzoli <ettore@comm2000.it>
 *  Tibor Biczo <crown@mail.matav.hu>
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

#include "vice.h"

#include <windows.h>
#include <prsht.h>
#include <tchar.h>

#ifdef HAVE_SHLOBJ_H
#include <shlobj.h>
#endif

#include "fullscrn.h"
#include "intl.h"
#include "lib.h"
#include "res.h"
#include "resources.h"
#include "system.h"
#include "translate.h"
#include "ui.h"
#include "uiapi.h"
#include "uilib.h"
#include "uivideo.h"
#include "videoarch.h"
#include "winlong.h"
#include "winmain.h"

static const char *vicii_palettes[] = {
    "default",
    "c64hq",
    "c64s",
    "ccs64",
    "frodo",
    "godot",
    "pc64",
    "vice",
    NULL
};

static const char *vic_palettes[] = {
    "default",
    NULL
};

static const char *crtc_palettes[] = {
    "amber",
    "green",
    "white",
    NULL
};

static const char *vdc_palettes[] = {
    "vdc_deft",
    "vdc_comp",
    NULL
};

static const char *ted_palettes[] = {
    "default",
    "vice",
    NULL
};

typedef struct {
    const char **palette_names;
    char *res_PaletteFile_name;
    char *res_ExternalPalette_name;
    int external_pal;
    char *file_name;
    int color_title;
    char *res_colors_gamma;
    char *res_colors_tint;
    char *res_colors_saturation;
    char *res_colors_contrast;
    char *res_colors_brightness;
    int crt_emu_title;
    char *res_crt_emu_scanlineshade;
    char *res_crt_emu_blur;
    char *res_crt_emu_oddlinephase;
    char *res_crt_emu_oddlineoffset;
    char *res_audio_leak;
    int renderer_title;
    char *res_videocache;
    char *res_doublesize;
    char *res_doublescan;
    char *res_vstretch;
    char *res_render_filter;
} Chip_Parameters;

static Chip_Parameters chip_param_table[] =
{
    { vicii_palettes, "VICIIPaletteFile", "VICIIExternalPalette", 0, NULL,
      IDS_VICII_COLORS, "VICIIColorGamma", "VICIIColorTint",
            "VICIIColorSaturation", "VICIIColorContrast", "VICIIColorBrightness",
      IDS_VICII_CRT_EMULATION, "VICIIPALScanLineShade", "VICIIPALBlur",
            "VICIIPALOddLinePhase", "VICIIPALOddLineOffset", "VICIIAudioLeak",
      IDS_VICII_RENDERER, "VICIIVideoCache", "VICIIDoubleSize", "VICIIDoubleScan", NULL,
            "VICIIFilter",
    },
    { vic_palettes, "VICPaletteFile", "VICExternalPalette", 0, NULL,
      IDS_VIC_COLORS, "VICColorGamma", "VICColorTint",
            "VICColorSaturation", "VICColorContrast", "VICColorBrightness",
      IDS_VIC_CRT_EMULATION, "VICPALScanLineShade", "VICPALBlur",
            "VICPALOddLinePhase", "VICPALOddLineOffset", "VICAudioLeak",
      IDS_VIC_RENDERER, "VICVideoCache", "VICDoubleSize", "VICDoubleScan", NULL,
            "VICFilter",
    },
    { crtc_palettes, "CrtcPaletteFile", "CrtcExternalPalette", 0, NULL,
      IDS_CRTC_COLORS, "CrtcColorGamma", "CrtcColorTint",
            "CrtcColorSaturation", "CrtcColorContrast", "CrtcColorBrightness",
      IDS_CRTC_CRT_EMULATION, "CrtcPALScanLineShade", "CrtcPALBlur",
            "CrtcPALOddLinePhase", "CrtcPALOddLineOffset", "CrtcAudioLeak",
      IDS_CRTC_RENDERER, "CrtcVideoCache", "CrtcDoubleSize", "CrtcDoubleScan", "CrtcStretchVertical",
            "CrtcFilter",
    },
    { vdc_palettes, "VDCPaletteFile", "VDCExternalPalette", 0, NULL,
      IDS_VDC_COLORS, "VDCColorGamma", "VDCColorTint",
            "VDCColorSaturation", "VDCColorContrast", "VDCColorBrightness",
      IDS_VDC_CRT_EMULATION, "VDCPALScanLineShade", "VDCPALBlur",
            "VDCPALOddLinePhase", "VDCPALOddLineOffset", "VDCAudioLeak",
      IDS_VDC_RENDERER, "VDCVideoCache", "VDCDoubleSize", "VDCDoubleScan", "VDCStretchVertical",
            "VDCFilter",
    },
    { ted_palettes, "TEDPaletteFile", "TEDExternalPalette", 0, NULL,
      IDS_TED_COLORS, "TEDColorGamma", "TEDColorTint",
            "TEDColorSaturation", "TEDColorContrast", "TEDColorBrightness",
      IDS_TED_CRT_EMULATION, "TEDPALScanLineShade", "TEDPALBlur",
            "TEDPALOddLinePhase", "TEDPALOddLineOffset", "TEDAudioLeak",
      IDS_TED_RENDERER, "TEDVideoCache", "TEDDoubleSize", "TEDDoubleScan", NULL,
            "TEDFilter",
    }
};

static HWND color_dialog_1 = NULL;
static HWND crt_emu_dialog_1 = NULL;
static HWND renderer_dialog_1 = NULL;

static Chip_Parameters *current_chip_1 = NULL;
static Chip_Parameters *current_chip_2 = NULL;

static uilib_localize_dialog_param color_dialog_trans[] = {
    { IDC_VIDEO_COLORS_GAMMA_LABEL, IDS_COLORS_GAMMA, 0 },
    { IDC_VIDEO_COLORS_TINT_LABEL, IDS_COLORS_TINT, 0 },
    { IDC_VIDEO_COLORS_SATURATION_LABEL, IDS_COLORS_SATURATION, 0 },
    { IDC_VIDEO_COLORS_CONTRAST_LABEL, IDS_COLORS_CONTRAST, 0 },
    { IDC_VIDEO_COLORS_BRIGHTNESS_LABEL, IDS_COLORS_BRIGHTNESS, 0 },
    { IDC_TOGGLE_VIDEO_EXTPALETTE, IDS_TOGGLE_VIDEO_EXTPALETTE, 0 },
    { IDC_VIDEO_CUSTOM_BROWSE, IDS_BROWSE, 0 },
    { 0, 0, 0 }
};

static uilib_dialog_group color_left_group[] = {
    { IDC_VIDEO_COLORS_GAMMA_LABEL, 0 },
    { IDC_VIDEO_COLORS_TINT_LABEL, 0 },
    { IDC_VIDEO_COLORS_SATURATION_LABEL, 0 },
    { IDC_VIDEO_COLORS_CONTRAST_LABEL, 0 },
    { IDC_VIDEO_COLORS_BRIGHTNESS_LABEL, 0 },
    { 0, 0 }
};

static uilib_dialog_group color_right_group[] = {
    { IDC_VIDEO_COLORS_GAMMA, 0 },
    { IDC_VIDEO_COLORS_TINT, 0 },
    { IDC_VIDEO_COLORS_SATURATION, 0 },
    { IDC_VIDEO_COLORS_CONTRAST, 0 },
    { IDC_VIDEO_COLORS_BRIGHTNESS, 0 },
    { 0, 0 }
};

static void init_color_dialog(HWND hwnd, Chip_Parameters *chip_type)
{
    int val, n;
    double fval;
    TCHAR newval[64];
    const char *path;
    TCHAR *st_path;
    HWND filename_hwnd;
    int xstart, xpos, size;

    if (chip_type == current_chip_1) {
        color_dialog_1 = hwnd;
    }

    /* translate all dialog items */
    uilib_localize_dialog(hwnd, color_dialog_trans);

    /* adjust the size of the elements in the left group */
    uilib_adjust_group_width(hwnd, color_left_group);

    /* get the max x of the left group */
    uilib_get_group_max_x(hwnd, color_left_group, &xpos);

    /* move the right group to the correct position */
    uilib_move_group(hwnd, color_right_group, xpos + 10);

    /* adjust the size of the external palette element */
    uilib_adjust_element_width(hwnd, IDC_TOGGLE_VIDEO_EXTPALETTE);

    /* get the min x of the palette combo element */
    uilib_get_element_min_x(hwnd, IDC_VIDEO_CUSTOM_NAME, &xstart);

    /* get the max x of the external palette element */
    uilib_get_element_max_x(hwnd, IDC_TOGGLE_VIDEO_EXTPALETTE, &xpos);

    if (xpos + 10 > xstart) {
        /* get the size of the palette combo element */
        uilib_get_element_size(hwnd, IDC_VIDEO_CUSTOM_NAME, &size);

        /* move and resize the palette combo element */
        uilib_move_and_set_element_width(hwnd, IDC_VIDEO_CUSTOM_NAME, xpos + 10, size - ((xpos + 10) - xstart));
    }

    resources_get_int(chip_type->res_colors_gamma, &val);
    fval = ((double)val) / 1000.0;
    _stprintf(newval, TEXT("%.3f"), (float)fval);
    SetDlgItemText(hwnd, IDC_VIDEO_COLORS_GAMMA, newval);

    resources_get_int(chip_type->res_colors_tint, &val);
    fval = ((double)val) / 1000.0;
    _stprintf(newval, TEXT("%.3f"), (float)fval);
    SetDlgItemText(hwnd, IDC_VIDEO_COLORS_TINT, newval);

    resources_get_int(chip_type->res_colors_saturation, &val);
    fval = ((double)val) / 1000.0;
    _stprintf(newval, TEXT("%.3f"), (float)fval);
    SetDlgItemText(hwnd, IDC_VIDEO_COLORS_SATURATION, newval);

    resources_get_int(chip_type->res_colors_contrast, &val);
    fval = ((double)val) / 1000.0;
    _stprintf(newval, TEXT("%.3f"), (float)fval);
    SetDlgItemText(hwnd, IDC_VIDEO_COLORS_CONTRAST, newval);

    resources_get_int(chip_type->res_colors_brightness, &val);
    fval = ((double)val) / 1000.0;
    _stprintf(newval, TEXT("%.3f"), (float)fval);
    SetDlgItemText(hwnd, IDC_VIDEO_COLORS_BRIGHTNESS, newval);

    /* palette */
    filename_hwnd = GetDlgItem(hwnd, IDC_VIDEO_CUSTOM_NAME);
    SendMessage(filename_hwnd, CB_RESETCONTENT, 0, 0);
    n = 0 ;
    while (chip_type->palette_names[n] != NULL) {
        SendMessage(filename_hwnd, CB_ADDSTRING, 0, (LPARAM)chip_type->palette_names[n]);
        n++;
    }
    resources_get_string(chip_type->res_PaletteFile_name, &path);
    lib_free(chip_type->file_name);
    chip_type->file_name = lib_stralloc(path);
    st_path = system_mbstowcs_alloc(path);
    SetDlgItemText(hwnd, IDC_VIDEO_CUSTOM_NAME, st_path);
    system_mbstowcs_free(st_path);

    resources_get_int(chip_type->res_ExternalPalette_name, &n);
    CheckDlgButton(hwnd, IDC_TOGGLE_VIDEO_EXTPALETTE, n ? BST_CHECKED : BST_UNCHECKED);
    chip_type->external_pal = n;

    EnableWindow(GetDlgItem(hwnd, IDC_VIDEO_CUSTOM_BROWSE), chip_type->external_pal);
    EnableWindow(GetDlgItem(hwnd, IDC_VIDEO_CUSTOM_NAME), chip_type->external_pal);
}

static uilib_localize_dialog_param crt_emulation_dialog_trans[] = {
    { IDC_VIDEO_CRT_SCANLINE_SHADE_LABEL, IDS_CRT_SCANLINE_SHADE, 0 },
    { IDC_VIDEO_CRT_BLUR_LABEL, IDS_CRT_BLUR, 0 },
    { IDC_VIDEO_CRT_ODDLINE_PHASE_LABEL, IDS_CRT_ODDLINE_PHASE, 0 },
    { IDC_VIDEO_CRT_ODDLINE_OFFSET_LABEL, IDS_CRT_ODDLINE_OFFSET, 0 },
    { IDC_TOGGLE_VIDEO_AUDIO_LEAK, IDS_AUDIO_LEAK, 0 },
    { 0, 0, 0 }
};

static uilib_dialog_group crt_emulation_left_group[] = {
    { IDC_VIDEO_CRT_SCANLINE_SHADE_LABEL, 0 },
    { IDC_VIDEO_CRT_BLUR_LABEL, 0 },
    { IDC_VIDEO_CRT_ODDLINE_PHASE_LABEL, 0 },
    { IDC_VIDEO_CRT_ODDLINE_OFFSET_LABEL, 0 },
    { 0, 0 }
};

static uilib_dialog_group crt_emulation_right_group[] = {
    { IDC_VIDEO_CRT_SCANLINE_SHADE, 0 },
    { IDC_VIDEO_CRT_BLUR, 0 },
    { IDC_VIDEO_CRT_ODDLINE_PHASE, 0 },
    { IDC_VIDEO_CRT_ODDLINE_OFFSET, 0 },
    { 0, 0 }
};

static void init_crt_emulation_dialog(HWND hwnd, Chip_Parameters *chip_type)
{
    int val;
    double fval;
    TCHAR newval[64];
    int xpos;

    if (chip_type == current_chip_1) {
        crt_emu_dialog_1 = hwnd;
    }

    /* translate all dialog items */
    uilib_localize_dialog(hwnd, crt_emulation_dialog_trans);

    /* adjust the size of the elements in the left group */
    uilib_adjust_group_width(hwnd, crt_emulation_left_group);

    /* get the max x of the left group */
    uilib_get_group_max_x(hwnd, crt_emulation_left_group, &xpos);

    /* move the right group to the correct position */
    uilib_move_group(hwnd, crt_emulation_right_group, xpos + 10);

    resources_get_int(chip_type->res_crt_emu_scanlineshade, &val);
    fval = ((double)val) / 1000.0;
    _stprintf(newval, TEXT("%.3f"), (float)fval);
    SetDlgItemText(hwnd, IDC_VIDEO_CRT_SCANLINE_SHADE, newval);

    resources_get_int(chip_type->res_crt_emu_blur, &val);
    fval = ((double)val) / 1000.0;
    _stprintf(newval, TEXT("%.3f"), (float)fval);
    SetDlgItemText(hwnd, IDC_VIDEO_CRT_BLUR, newval);

    resources_get_int(chip_type->res_crt_emu_oddlinephase, &val);
    fval = ((double)val) / 1000.0;
    _stprintf(newval, TEXT("%.3f"), (float)fval);
    SetDlgItemText(hwnd, IDC_VIDEO_CRT_ODDLINE_PHASE, newval);

    resources_get_int(chip_type->res_crt_emu_oddlineoffset, &val);
    fval = ((double)val) / 1000.0;
    _stprintf(newval, TEXT("%.3f"), (float)fval);
    SetDlgItemText(hwnd, IDC_VIDEO_CRT_ODDLINE_OFFSET, newval);

    resources_get_int(chip_type->res_audio_leak, &val);
    CheckDlgButton(hwnd, IDC_TOGGLE_VIDEO_AUDIO_LEAK, val ? BST_CHECKED : BST_UNCHECKED);
}

static uilib_localize_dialog_param renderer_dialog_trans[] = {
    {IDC_TOGGLE_VIDEO_CACHE, IDS_VIDEO_CACHE, 0},
    {IDC_TOGGLE_VIDEO_DOUBLE_SIZE, IDS_DOUBLE_SIZE, 0},
    {IDC_TOGGLE_VIDEO_DOUBLE_SCAN, IDS_DOUBLE_SCAN, 0},
    {IDC_TOGGLE_VIDEO_VSTRETCH, IDS_VSTRETCH, 0},
    {IDC_VIDEO_RENDER_FILTER_LABEL, IDS_RENDER_FILTER, 0},
    {0, 0, 0}
};

static uilib_dialog_group renderer_group[] = {
    {IDC_TOGGLE_VIDEO_CACHE,  1},
    {IDC_TOGGLE_VIDEO_DOUBLE_SIZE,  1},
    {IDC_TOGGLE_VIDEO_DOUBLE_SCAN,  1},
    {IDC_TOGGLE_VIDEO_VSTRETCH,  1},
    {0, 0}
};

static uilib_dialog_group renderer_left_group[] = {
    {IDC_VIDEO_RENDER_FILTER_LABEL,  0},
    {0, 0}
};
static uilib_dialog_group renderer_right_group[] = {
    {IDC_VIDEO_RENDER_FILTER,  0},
    {0, 0}
};

static void init_renderer_dialog(HWND hwnd, Chip_Parameters *chip_type)
{
    int n, xpos;
    int doublesize;
    int has_vstretch;
    HWND setting_hwnd;

    if (chip_type == current_chip_1) {
        renderer_dialog_1 = hwnd;
    }

    /* translate all dialog items */
    uilib_localize_dialog(hwnd, renderer_dialog_trans);

    /* adjust the size of the elements in the group */
    uilib_adjust_group_width(hwnd, renderer_left_group);

    /* get the max x of the left group */
    uilib_get_group_max_x(hwnd, renderer_group, &xpos);

    /* set the position of the right group */
    uilib_move_group(hwnd, renderer_right_group, xpos + 10);

    has_vstretch = (chip_type->res_vstretch != NULL);

    resources_get_int(chip_type->res_videocache, &n);
    CheckDlgButton(hwnd, IDC_TOGGLE_VIDEO_CACHE, n ? BST_CHECKED : BST_UNCHECKED);

    resources_get_int(chip_type->res_doublesize, &doublesize);
    CheckDlgButton(hwnd, IDC_TOGGLE_VIDEO_DOUBLE_SIZE, doublesize ? BST_CHECKED : BST_UNCHECKED);

    resources_get_int(chip_type->res_doublescan, &n);
    CheckDlgButton(hwnd, IDC_TOGGLE_VIDEO_DOUBLE_SCAN, n ? BST_CHECKED : BST_UNCHECKED);
    EnableWindow(GetDlgItem(hwnd, IDC_TOGGLE_VIDEO_DOUBLE_SCAN), doublesize);

    if (has_vstretch) {
        resources_get_int(chip_type->res_vstretch, &n);
        CheckDlgButton(hwnd, IDC_TOGGLE_VIDEO_VSTRETCH, n ? BST_CHECKED : BST_UNCHECKED);
    } else {
        CheckDlgButton(hwnd, IDC_TOGGLE_VIDEO_VSTRETCH, 0);
    }
    EnableWindow(GetDlgItem(hwnd, IDC_TOGGLE_VIDEO_VSTRETCH), has_vstretch);

    /* render filter */
    setting_hwnd = GetDlgItem(hwnd, IDC_VIDEO_RENDER_FILTER);
    SendMessage(setting_hwnd, CB_ADDSTRING, 0, (LPARAM)translate_text(IDS_NONE));
    SendMessage(setting_hwnd, CB_ADDSTRING, 0, (LPARAM)translate_text(IDS_CRT_EMULATION));
    SendMessage(setting_hwnd, CB_ADDSTRING, 0, (LPARAM)translate_text(IDS_SCALE2X));

    resources_get_int(chip_type->res_render_filter, &n);
    SendMessage(setting_hwnd, CB_SETCURSEL, (WPARAM)n, 0);
}

static INT_PTR CALLBACK dialog_color_proc(HWND hwnd, UINT msg, WPARAM wparam, LPARAM lparam)
{
    int type;
    int ivalgamma;
    int ivaltint;
    int ivalsaturation;
    int ivalcontrast;
    int ivalbrightness;

    float tf;
    TCHAR s[100];
    TCHAR st[MAX_PATH];

    Chip_Parameters *chip_type = (hwnd == color_dialog_1) ? current_chip_1 : current_chip_2;

    switch (msg) {
        case WM_NOTIFY:
            if (((NMHDR FAR *)lparam)->code == (UINT)PSN_APPLY) {
                GetDlgItemText(hwnd, IDC_VIDEO_COLORS_GAMMA, s, 100);
                _stscanf(s, TEXT("%f"), &tf);
                ivalgamma = (int)(tf * 1000.0 + 0.5);
                if (ivalgamma < 0) {
                    ui_error(translate_text(IDS_VAL_F_FOR_S_OUT_RANGE_USE_F), tf, translate_text(IDS_COLORS_GAMMA), 0.0f);
                    ivalgamma = 0;
                }
                if (ivalgamma > 4000) {
                    ui_error(translate_text(IDS_VAL_F_FOR_S_OUT_RANGE_USE_F), tf, translate_text(IDS_COLORS_GAMMA), 4.0f);
                    ivalgamma = 4000;
                }

                GetDlgItemText(hwnd, IDC_VIDEO_COLORS_TINT, s, 100);
                _stscanf(s, TEXT("%f"), &tf);
                ivaltint = (int)(tf * 1000.0 + 0.5);
                if (ivaltint < 0) {
                    ui_error(translate_text(IDS_VAL_F_FOR_S_OUT_RANGE_USE_F), tf, translate_text(IDS_COLORS_TINT), 0.0f);
                    ivaltint = 0;
                }
                if (ivaltint > 2000) {
                    ui_error(translate_text(IDS_VAL_F_FOR_S_OUT_RANGE_USE_F), tf, translate_text(IDS_COLORS_TINT), 2.0f);
                    ivaltint = 2000;
                }

                GetDlgItemText(hwnd, IDC_VIDEO_COLORS_SATURATION, s, 100);
                _stscanf(s, TEXT("%f"), &tf);
                ivalsaturation = (int)(tf * 1000.0 + 0.5);
                if (ivalsaturation < 0) {
                    ui_error(translate_text(IDS_VAL_F_FOR_S_OUT_RANGE_USE_F), tf, translate_text(IDS_COLORS_SATURATION), 0.0f);
                    ivalsaturation = 0;
                }
                if (ivalsaturation > 2000) {
                    ui_error(translate_text(IDS_VAL_F_FOR_S_OUT_RANGE_USE_F), tf, translate_text(IDS_COLORS_SATURATION), 2.0f);
                    ivalsaturation = 2000;
                }

                GetDlgItemText(hwnd, IDC_VIDEO_COLORS_CONTRAST, s, 100);
                _stscanf(s, TEXT("%f"), &tf);
                ivalcontrast = (int)(tf * 1000.0 + 0.5);
                if (ivalcontrast < 0) {
                    ui_error(translate_text(IDS_VAL_F_FOR_S_OUT_RANGE_USE_F), tf, translate_text(IDS_COLORS_CONTRAST), 0.0f);
                    ivalcontrast = 0;
                }
                if (ivalcontrast > 2000) {
                    ui_error(translate_text(IDS_VAL_F_FOR_S_OUT_RANGE_USE_F), tf, translate_text(IDS_COLORS_CONTRAST), 2.0f);
                    ivalcontrast = 2000;
                }

                GetDlgItemText(hwnd, IDC_VIDEO_COLORS_BRIGHTNESS, s, 100);
                _stscanf(s, TEXT("%f"), &tf);
                ivalbrightness = (int)(tf * 1000.0 + 0.5);
                if (ivalbrightness < 0) {
                    ui_error(translate_text(IDS_VAL_F_FOR_S_OUT_RANGE_USE_F), tf, translate_text(IDS_COLORS_BRIGHTNESS), 0.0f);
                    ivalbrightness = 0;
                }
                if (ivalbrightness > 2000) {
                    ui_error(translate_text(IDS_VAL_F_FOR_S_OUT_RANGE_USE_F), tf, translate_text(IDS_COLORS_BRIGHTNESS), 2.0f);
                    ivalbrightness = 2000;
                }

                resources_set_int(chip_type->res_colors_gamma, ivalgamma);
                resources_set_int(chip_type->res_colors_tint, ivaltint);
                resources_set_int(chip_type->res_colors_saturation, ivalsaturation);
                resources_set_int(chip_type->res_colors_contrast, ivalcontrast);
                resources_set_int(chip_type->res_colors_brightness, ivalbrightness);
                /* palette */
                GetDlgItemText(hwnd, IDC_VIDEO_CUSTOM_NAME, st, 100);
                lib_free(chip_type->file_name);
                chip_type->file_name = system_wcstombs_alloc(st);
                querynewpalette = 1;
                if (resources_set_string(chip_type->res_PaletteFile_name, chip_type->file_name) < 0) {
                    ui_error(translate_text(IDS_COULD_NOT_LOAD_PALETTE));
                    resources_set_int(chip_type->res_ExternalPalette_name, 0);
                    SetWindowLongPtr(hwnd, DWLP_MSGRESULT, TRUE);
                    lib_free(chip_type->file_name);
                    chip_type->file_name = NULL;
                    return TRUE;
                }
                lib_free(chip_type->file_name);
                chip_type->file_name = NULL;
                resources_set_int(chip_type->res_ExternalPalette_name, chip_type->external_pal);
                querynewpalette = 1;
                color_dialog_1 = NULL;
                SetWindowLongPtr(hwnd, DWLP_MSGRESULT, FALSE);
                return TRUE;
            }
            return FALSE;
        case WM_INITDIALOG:
            init_color_dialog(hwnd, (Chip_Parameters*)((PROPSHEETPAGE*)lparam)->lParam);
            return TRUE;
        case WM_COMMAND:
            type = LOWORD(wparam);
            switch (type) {
                case IDC_VIDEO_COLORS_GAMMA:
                case IDC_VIDEO_COLORS_TINT:
                case IDC_VIDEO_COLORS_SATURATION:
                case IDC_VIDEO_COLORS_CONTRAST:
                case IDC_VIDEO_COLORS_BRIGHTNESS:
                    break;
                case IDC_TOGGLE_VIDEO_EXTPALETTE:
                    chip_type->external_pal = !chip_type->external_pal;
                    EnableWindow(GetDlgItem(hwnd, IDC_VIDEO_CUSTOM_BROWSE), chip_type->external_pal);
                    EnableWindow(GetDlgItem(hwnd, IDC_VIDEO_CUSTOM_NAME), chip_type->external_pal);
                    break;
                case IDC_VIDEO_CUSTOM_BROWSE:
                    {
                        TCHAR *st_name;

                        if ((st_name = uilib_select_file(hwnd, translate_text(IDS_LOAD_VICE_PALETTE_FILE), UILIB_FILTER_ALL | UILIB_FILTER_PALETTE,
                                                         UILIB_SELECTOR_TYPE_FILE_LOAD, UILIB_SELECTOR_STYLE_DEFAULT)) != NULL) {
                            char *name;

                            SetDlgItemText(hwnd, IDC_VIDEO_CUSTOM_NAME, st_name);
                            name = system_wcstombs_alloc(st_name);
                            lib_free(chip_type->file_name);
                            chip_type->file_name = name;
                            chip_type->external_pal = 1;
                            CheckDlgButton(hwnd, IDC_TOGGLE_VIDEO_EXTPALETTE, BST_CHECKED);
                            lib_free(st_name);
                        }
                    }
                    break;
                case IDC_VIDEO_CUSTOM_NAME:
                    {
                        TCHAR st[100];

                        GetDlgItemText(hwnd, IDC_VIDEO_CUSTOM_NAME, st, 100);
                        lib_free(chip_type->file_name);
                        chip_type->file_name = system_wcstombs_alloc(st);

                        chip_type->external_pal = 1;
                        CheckDlgButton(hwnd, IDC_TOGGLE_VIDEO_EXTPALETTE, BST_CHECKED);

                        break;
                    }
            }
            return TRUE;
    }
    return FALSE;
}

static INT_PTR CALLBACK dialog_crt_emulation_proc(HWND hwnd, UINT msg, WPARAM wparam, LPARAM lparam)
{
    int type;
    int ivalscanlineshade;
    int ivalblur;
    int ivaloddlinephase;
    int ivaloddlineoffset;
    float tf;
    TCHAR s[100];

    Chip_Parameters *chip_type = (hwnd == crt_emu_dialog_1) ? current_chip_1 : current_chip_2;

    switch (msg) {
        case WM_NOTIFY:
            if (((NMHDR FAR *)lparam)->code == PSN_APPLY) {
                GetDlgItemText(hwnd, IDC_VIDEO_CRT_SCANLINE_SHADE, s, 100);
                _stscanf(s, TEXT("%f"), &tf);
                ivalscanlineshade = (int)(tf * 1000.0 + 0.5);
                if (ivalscanlineshade < 0) {
                    ui_error(translate_text(IDS_VAL_F_FOR_S_OUT_RANGE_USE_F), tf, translate_text(IDS_CRT_SCANLINE_SHADE), 0.0f);
                    ivalscanlineshade = 0;
                }
                if (ivalscanlineshade > 1000) {
                    ui_error(translate_text(IDS_VAL_F_FOR_S_OUT_RANGE_USE_F), tf, translate_text(IDS_CRT_SCANLINE_SHADE), 1.0f);
                    ivalscanlineshade = 1000;
                }

                GetDlgItemText(hwnd, IDC_VIDEO_CRT_BLUR, s, 100);
                _stscanf(s, TEXT("%f"), &tf);
                ivalblur = (int)(tf * 1000.0 + 0.5);
                if (ivalblur < 0) {
                    ui_error(translate_text(IDS_VAL_F_FOR_S_OUT_RANGE_USE_F), tf, translate_text(IDS_CRT_BLUR), 0.0f);
                    ivalblur = 0;
                }
                if (ivalblur > 1000) {
                    ui_error(translate_text(IDS_VAL_F_FOR_S_OUT_RANGE_USE_F), tf, translate_text(IDS_CRT_BLUR), 1.0f);
                    ivalblur = 1000;
                }

                GetDlgItemText(hwnd, IDC_VIDEO_CRT_ODDLINE_PHASE, s, 100);
                _stscanf(s, TEXT("%f"), &tf);
                ivaloddlinephase = (int)(tf * 1000.0 + 0.5);
                if (ivaloddlinephase < 0) {
                    ui_error(translate_text(IDS_VAL_F_FOR_S_OUT_RANGE_USE_F), tf, translate_text(IDS_CRT_ODDLINE_PHASE), 0.0f);
                    ivaloddlinephase = 0;
                }
                if (ivaloddlinephase > 2000) {
                    ui_error(translate_text(IDS_VAL_F_FOR_S_OUT_RANGE_USE_F), tf, translate_text(IDS_CRT_ODDLINE_PHASE), 2.0f);
                    ivaloddlinephase = 2000;
                }

                GetDlgItemText(hwnd, IDC_VIDEO_CRT_ODDLINE_OFFSET, s, 100);
                _stscanf(s, TEXT("%f"), &tf);
                ivaloddlineoffset = (int)(tf * 1000.0 + 0.5);
                if (ivaloddlineoffset < 0) {
                    ui_error(translate_text(IDS_VAL_F_FOR_S_OUT_RANGE_USE_F), tf, translate_text(IDS_CRT_ODDLINE_OFFSET), 0.0f);
                    ivaloddlineoffset = 0;
                }
                if (ivaloddlineoffset > 2000) {
                    ui_error(translate_text(IDS_VAL_F_FOR_S_OUT_RANGE_USE_F), tf, translate_text(IDS_CRT_ODDLINE_OFFSET), 2.0f);
                    ivaloddlineoffset = 2000;
                }

                resources_set_int(chip_type->res_crt_emu_scanlineshade, ivalscanlineshade);
                resources_set_int(chip_type->res_crt_emu_blur, ivalblur);
                resources_set_int(chip_type->res_crt_emu_oddlinephase, ivaloddlinephase);
                resources_set_int(chip_type->res_crt_emu_oddlineoffset, ivaloddlineoffset);
                resources_set_int(chip_type->res_audio_leak, (IsDlgButtonChecked(hwnd, IDC_TOGGLE_VIDEO_AUDIO_LEAK) == BST_CHECKED ? 1 : 0));
                querynewpalette = 1;
                crt_emu_dialog_1 = NULL;
                SetWindowLongPtr(hwnd, DWLP_MSGRESULT, FALSE);
                return TRUE;
            }
            return FALSE;
        case WM_INITDIALOG:
            init_crt_emulation_dialog(hwnd, (Chip_Parameters*)((PROPSHEETPAGE*)lparam)->lParam);
            return TRUE;
        case WM_COMMAND:
            type = LOWORD(wparam);
            switch (type) {
                case IDC_VIDEO_CRT_SCANLINE_SHADE:
                case IDC_VIDEO_CRT_BLUR:
                case IDC_VIDEO_CRT_ODDLINE_PHASE:
                case IDC_VIDEO_CRT_ODDLINE_OFFSET:
                    break;
            }
            return TRUE;
    }
    return FALSE;
}

static INT_PTR CALLBACK dialog_renderer_proc(HWND hwnd, UINT msg, WPARAM wparam, LPARAM lparam)
{
    int type, index;

    Chip_Parameters *chip_type = (hwnd == renderer_dialog_1) ? current_chip_1 : current_chip_2;

    switch (msg) {
        case WM_NOTIFY:
            if (((NMHDR FAR *)lparam)->code == (UINT)PSN_APPLY) {
                resources_set_int(chip_type->res_videocache, (IsDlgButtonChecked(hwnd, IDC_TOGGLE_VIDEO_CACHE) == BST_CHECKED ? 1 : 0));
                resources_set_int(chip_type->res_doublesize, (IsDlgButtonChecked(hwnd, IDC_TOGGLE_VIDEO_DOUBLE_SIZE) == BST_CHECKED ? 1 : 0));
                resources_set_int(chip_type->res_doublescan, (IsDlgButtonChecked(hwnd, IDC_TOGGLE_VIDEO_DOUBLE_SCAN) == BST_CHECKED ? 1 : 0));
                if (chip_type->res_vstretch) {
                    resources_set_int(chip_type->res_vstretch, (IsDlgButtonChecked(hwnd, IDC_TOGGLE_VIDEO_VSTRETCH) == BST_CHECKED ? 1 : 0));
                }
                index = (int)SendMessage(GetDlgItem(hwnd, IDC_VIDEO_RENDER_FILTER), CB_GETCURSEL, 0, 0);
                resources_set_int(chip_type->res_render_filter, index);
                renderer_dialog_1 = NULL;
                SetWindowLongPtr(hwnd, DWLP_MSGRESULT, FALSE);
                return TRUE;
            }
            return FALSE;
        case WM_INITDIALOG:
            init_renderer_dialog(hwnd, (Chip_Parameters*)((PROPSHEETPAGE*)lparam)->lParam);
            return TRUE;
        case WM_COMMAND:
            type = LOWORD(wparam);
            switch (type) {
                case IDC_TOGGLE_VIDEO_DOUBLE_SIZE:
                    EnableWindow(GetDlgItem(hwnd, IDC_TOGGLE_VIDEO_DOUBLE_SCAN), (IsDlgButtonChecked(hwnd, IDC_TOGGLE_VIDEO_DOUBLE_SIZE) == BST_CHECKED) ? 1 : 0);
                break;
                case IDC_TOGGLE_VIDEO_CACHE:
                case IDC_TOGGLE_VIDEO_DOUBLE_SCAN:
                case IDC_TOGGLE_VIDEO_VSTRETCH:
                break;
            }
            return TRUE;
    }
    return FALSE;
}

#define MAXTABS 7
void ui_video_settings_dialog(HWND hwnd, int chip_type1, int chip_type2)
{
    PROPSHEETPAGE psp[MAXTABS];
    PROPSHEETHEADER psh;
    int i;
    Chip_Parameters *chip_param;

    for (i = 0; i < MAXTABS; i++) {
        psp[i].dwSize = sizeof(PROPSHEETPAGE);
        psp[i].dwFlags = PSP_USETITLE /*| PSP_HASHELP*/ ;
        psp[i].hInstance = winmain_instance;
#ifdef _ANONYMOUS_UNION
        psp[i].pszIcon = NULL;
#else
        psp[i].u2.pszIcon = NULL;
#endif
        psp[i].lParam = 0;
        psp[i].pfnCallback = NULL;
    }

    chip_param = &chip_param_table[chip_type1];
    current_chip_1 = chip_param;

    psp[0].pfnDlgProc = dialog_fullscreen_proc;
    psp[0].pszTitle = translate_text(IDS_FULLSCREEN);

    psp[1].pfnDlgProc = dialog_crt_emulation_proc;
    psp[1].pszTitle = system_mbstowcs_alloc(translate_text(chip_param->crt_emu_title));
    psp[1].lParam = (LPARAM)chip_param;
    psp[2].pfnDlgProc = dialog_renderer_proc;
    psp[2].pszTitle = system_mbstowcs_alloc(translate_text(chip_param->renderer_title));
    psp[2].lParam = (LPARAM)chip_param;
    psp[3].pfnDlgProc = dialog_color_proc;
    psp[3].pszTitle = system_mbstowcs_alloc(translate_text(chip_param->color_title));
    psp[3].lParam = (LPARAM)chip_param;

#ifdef _ANONYMOUS_UNION
    if (video_dx9_enabled()) {
        psp[0].pszTemplate = MAKEINTRESOURCE(IDD_FULLSCREEN_SETTINGS_DX9_DIALOG);
    } else {
        psp[0].pszTemplate = MAKEINTRESOURCE(IDD_FULLSCREEN_SETTINGS_DIALOG);
    }
    psp[1].pszTemplate = MAKEINTRESOURCE(IDD_VIDEO_CRT_EMULATION_DIALOG);
    psp[2].pszTemplate = MAKEINTRESOURCE(IDD_VIDEO_RENDERER_DIALOG);
    psp[3].pszTemplate = MAKEINTRESOURCE(IDD_VIDEO_COLORS_DIALOG);
#else
    if (video_dx9_enabled()) {
        psp[0].DUMMYUNIONNAME.pszTemplate = MAKEINTRESOURCE(IDD_FULLSCREEN_SETTINGS_DX9_DIALOG);
    } else {
        psp[0].DUMMYUNIONNAME.pszTemplate = MAKEINTRESOURCE(IDD_FULLSCREEN_SETTINGS_DIALOG);
    }
    psp[1].DUMMYUNIONNAME.pszTemplate = MAKEINTRESOURCE(IDD_VIDEO_CRT_EMULATION_DIALOG);
    psp[2].DUMMYUNIONNAME.pszTemplate = MAKEINTRESOURCE(IDD_VIDEO_RENDERER_DIALOG);
    psp[3].DUMMYUNIONNAME.pszTemplate = MAKEINTRESOURCE(IDD_VIDEO_COLORS_DIALOG);
#endif
    psh.nPages = 4;

    if (chip_type2 != UI_VIDEO_CHIP_NONE) {
        chip_param = &chip_param_table[chip_type2];
        current_chip_2 = chip_param;

        /* colors */
        psp[4].pfnDlgProc = dialog_color_proc;
        psp[4].pszTitle = system_mbstowcs_alloc(translate_text(chip_param->color_title));
        psp[4].lParam = (LPARAM)chip_param;

#ifdef _ANONYMOUS_UNION
        psp[4].pszTemplate = MAKEINTRESOURCE(IDD_VIDEO_COLORS_DIALOG);
#else
        psp[4].DUMMYUNIONNAME.pszTemplate = MAKEINTRESOURCE(IDD_VIDEO_COLORS_DIALOG);
#endif
        /* renderer */
        psp[5].pfnDlgProc = dialog_renderer_proc;
        psp[5].pszTitle = system_mbstowcs_alloc(translate_text(chip_param->renderer_title));
        psp[5].lParam = (LPARAM)chip_param;

#ifdef _ANONYMOUS_UNION
        psp[5].pszTemplate = MAKEINTRESOURCE(IDD_VIDEO_RENDERER_DIALOG);
#else
        psp[5].DUMMYUNIONNAME.pszTemplate = MAKEINTRESOURCE(IDD_VIDEO_RENDERER_DIALOG);
#endif
        /* CRT emulation */
        psp[6].pfnDlgProc = dialog_crt_emulation_proc;
        psp[6].pszTitle = system_mbstowcs_alloc(translate_text(chip_param->crt_emu_title));
        psp[6].lParam = (LPARAM)chip_param;

#ifdef _ANONYMOUS_UNION
        psp[6].pszTemplate = MAKEINTRESOURCE(IDD_VIDEO_CRT_EMULATION_DIALOG);
#else
        psp[6].DUMMYUNIONNAME.pszTemplate = MAKEINTRESOURCE(IDD_VIDEO_CRT_EMULATION_DIALOG);
#endif

        psh.nPages += 3;
    }

    psh.dwSize = sizeof(PROPSHEETHEADER);
    psh.dwFlags = PSH_PROPSHEETPAGE | PSH_NOAPPLYNOW;
    psh.hwndParent = hwnd;
    psh.hInstance = winmain_instance;
    psh.pszCaption = translate_text(IDS_VIDEO_SETTINGS);
#ifdef _ANONYMOUS_UNION
    psh.pszIcon = NULL;
    psh.nStartPage = 0;
    psh.ppsp = psp;
#else
    psh.DUMMYUNIONNAME.pszIcon = NULL;
    psh.u2.nStartPage = 0;
    psh.u3.ppsp = psp;
#endif
    psh.pfnCallback = NULL;

    PropertySheet(&psh);
    system_mbstowcs_free((char *)psp[1].pszTitle);
}
#undef MAXTABS
