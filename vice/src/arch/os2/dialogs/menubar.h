/*
 * dialogs.h - The dialog windows.
 *
 * Written by
 *  Thomas Bretz <tbretz@gsi.de>
 *  Marco van den Heuvel <blackystardust68@yahoo.com>
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

/*----------------------------------------------------------------- */
/*                           Menubar                                */
/*----------------------------------------------------------------- */

#define FID_STATUS                   0x002

#define IDM_FILEOPEN                 0x101
#define IDM_FILESAVE                 0x102
#define IDM_FILE                     0x103
#define IDM_ATTACH                   0x104
#define IDM_DETACH                   0x105
#define IDM_SNAPSHOT                 0x106
#define IDM_JAM_ACTION               0x353
#define IDM_RESET                    0x107
#define IDM_VIEW                     0x108
#define IDM_EMULATOR                 0x109
#define IDM_DRIVE                    0x10a
#define IDM_JOYSTICK                 0x10b
#define IDM_SIDCART                  0x29a
#define IDM_SIDCART_ENABLE           0x29b
#define IDM_SIDCART_MODEL            0x29c
#define IDM_SIDCART_6581             0x29d
#define IDM_SIDCART_8580             0x29e
#define IDM_SIDCART_FILTERS          0x29f
#define IDM_SIDCART_ADDRESS          0x2a4
#define IDM_SIDCART_ADDRESS_1        0x2a5
#define IDM_SIDCART_ADDRESS_2        0x2a6
#define IDM_SIDCART_CLOCK            0x2a7
#define IDM_SIDCART_CLOCK_C64        0x2b6
#define IDM_SIDCART_CLOCK_NATIVE     0x2b7
#define IDM_DIGIBLASTER              0x2be
#define IDM_SIDCART_JOY              0x255
#define IDM_USERPORT_JOY             0x256
#define IDM_USERPORT_JOY_TYPE        0x257
#define IDM_USERPORT_JOY_CGA         0x28e
#define IDM_USERPORT_JOY_PET         0x28f
#define IDM_USERPORT_JOY_HUMMER      0x295
#define IDM_USERPORT_JOY_OEM         0x296
#define IDM_USERPORT_JOY_HIT         0x297
#define IDM_USERPORT_JOY_KINGSOFT    0x298
#define IDM_USERPORT_JOY_STARBYTE    0x299
#define IDM_USERPORT_JOY_SETUP       0x191
#define IDM_ALLOW_OPPOSITE_JOY       0x1ca
#define IDM_DATASETTE                0x10c
#define IDM_LOGWIN                   0x10d
#define IDM_COLOR                    0x10e
#define IDM_COLOR2                   0x2bf

#define IDM_SCREENSHOTS              0x372
#define IDM_SCREENSHOT_BMP           0x12b
#define IDM_SCREENSHOT_DOODLE        0x373
#define IDM_SCREENSHOT_DOODLE_CMPR   0x374
#define IDM_SCREENSHOT_GODOT         0x375
#define IDM_SCREENSHOT_IFF           0x376
#define IDM_SCREENSHOT_KOALA         0x377
#define IDM_SCREENSHOT_KOALA_CMPR    0x378
#define IDM_SCREENSHOT_PCX           0x379
#define IDM_SCREENSHOT_PPM           0x37a

#define IDM_JAM_ACTION_DIALOG        0x354
#define IDM_JAM_ACTION_CONTINUE      0x355
#define IDM_JAM_ACTION_MONITOR       0x356
#define IDM_JAM_ACTION_RESET         0x357
#define IDM_JAM_ACTION_HARD_RESET    0x358
#define IDM_JAM_ACTION_QUIT          0x359

#define IDM_BURSTMOD                 0x2e2
#define IDM_BURST_NONE               0x2e3
#define IDM_BURST_CIA1               0x2e4
#define IDM_BURST_CIA2               0x2e5

#define IDM_SCPU64_SETTINGS          0x32c
#define IDM_SCPU64_SIMM_SIZE         0x32d
#define IDM_SCPU64_SIMM_0            0x32e
#define IDM_SCPU64_SIMM_1            0x32f
#define IDM_SCPU64_SIMM_4            0x330
#define IDM_SCPU64_SIMM_8            0x331
#define IDM_SCPU64_SIMM_16           0x332
#define IDM_SCPU64_JIFFY_SWITCH      0x333
#define IDM_SCPU64_SPEED_SWITCH      0x334

#define IDM_SOUND_OUTPUT_MODE        0x2e6
#define IDM_SOUND_OUTPUT_SYSTEM      0x2e7
#define IDM_SOUND_OUTPUT_MONO        0x2e8
#define IDM_SOUND_OUTPUT_STEREO      0x2e9
#define IDM_DRIVE_SOUND              0x2ea

#define IDM_SETUP                    0x10f
#define IDM_SOUND                    0x110
#define IDM_SIDCHIP                  0x111
#define IDM_SAMPLINGRATE             0x113
#define IDM_SOUNDSYNC                0x114
#define IDM_SOUNDFRAG                0x1c6
#define IDM_SOUNDDEV                 0x115
#define IDM_VDCMEMORY                0x116
#define IDM_VIDEOSTD                 0x117
#define IDM_COLLISION                0x118
#define IDM_REFRATE                  0x119
#define IDM_BUFFER                   0x11a
#define IDM_STRETCH                  0x11b
#define IDM_MONITOR                  0x11c
#define IDM_HELP                     0x11d
#define IDM_CMDLINE                  0x11e
#define IDM_FSMODES                  0x11f
#define IDM_ABOUT                    0x13c
#define IDM_VICE_FEATURES            0x492

#define IDM_ATTACHTAPE               0x120
#define IDM_ATTACH8                  0x121
#define IDM_ATTACH9                  0x122
#define IDM_ATTACH10                 0x123
#define IDM_ATTACH11                 0x124

#define IDM_DETACHTAPE               0x130
#define IDM_DETACH8                  0x138
#define IDM_DETACH9                  0x139
#define IDM_DETACH10                 0x13a
#define IDM_DETACH11                 0x13b
#define IDM_DETACHALL                0x13f

#define IDM_SNAPLOAD                 0x125
#define IDM_SNAPSAVE                 0x126

#define IDM_CONFIG                   0x127
#define IDM_READCONFIG               0x128
#define IDM_WRITECONFIG              0x129
#define IDM_DEFAULTCONFIG            0x12a
#define IDM_SOFTRESET                0x12c
#define IDM_HARDRESET                0x12d
#define IDM_EXIT                     0x12f

#define IDM_VICII_VSP_BUG            0x369

#define IDM_DSIZE                    0x131
#define IDM_DSCAN                    0x132
#define IDM_EXTERNALPAL              0x135
#define IDM_PALETTES                 0x470
#define IDM_EXTERNALPAL_FILE         0x2bb
#define IDM_AUDIO_LEAK               0x2bd
#define IDM_CRT                      0x2c4
#define IDM_RENDER_FILTER            0x2cc
#define IDM_RENDER_NONE              0x2cd
#define IDM_RENDER_CRT               0x2ce
#define IDM_RENDER_SCALE2X           0x2cf
#define IDM_VIDEO_SETTINGS           0x133
#define IDM_VDC_VIDEO_SETTINGS       0x134

#define IDM_PALETTE_C64HQ            0x471
#define IDM_PALETTE_C64S             0x472
#define IDM_PALETTE_CCS64            0x473
#define IDM_PALETTE_COMMUNITY_COLORS 0x474
#define IDM_PALETTE_DEEKAY           0x475
#define IDM_PALETTE_FRODO            0x477
#define IDM_PALETTE_GODOT            0x478
#define IDM_PALETTE_PC64             0x479
#define IDM_PALETTE_PEPTO_NTSC       0x47a
#define IDM_PALETTE_PEPTO_NTSC_SONY  0x47b
#define IDM_PALETTE_PEPTO_PAL        0x47c
#define IDM_PALETTE_PEPTO_PAL_OLD    0x47d
#define IDM_PALETTE_PTOING           0x47e
#define IDM_PALETTE_RGB              0x47f
#define IDM_PALETTE_SPIFF            0x482
#define IDM_PALETTE_VICE             0x483
#define IDM_PALETTE_AMBER            0x484
#define IDM_PALETTE_GREEN            0x485
#define IDM_PALETTE_WHITE            0x486
#define IDM_PALETTE_MIKE_NTSC        0x487
#define IDM_PALETTE_MIKE_PAL         0x488
#define IDM_VDC_PALETTES             0x489
#define IDM_PALETTE_VDC_COMP         0x48a
#define IDM_PALETTE_VDC_DEFT         0x48b
#define IDM_PALETTE_YAPE_NTSC        0x4a6
#define IDM_PALETTE_YAPE_PAL         0x4a7


#define IDM_BORDER_MODE              0x2eb
#define IDM_BORDER_NORMAL            0x2ec
#define IDM_BORDER_FULL              0x2ed
#define IDM_BORDER_DEBUG             0x2ee
#define IDM_BORDER_NONE              0x2ef

#define IDM_VDCDSIZE                 0x136
#define IDM_VDCDSCAN                 0x137
#define IDM_VDCEXTERNALPAL           0x2b5
#define IDM_VDCEXTERNALPAL_FILE      0x2bc
#define IDM_VDCAUDIO_LEAK            0x2c3
#define IDM_CRT2                     0x2c5
#define IDM_VDCRENDER_FILTER         0x2d0
#define IDM_VDCRENDER_NONE           0x2d1
#define IDM_VDCRENDER_CRT            0x2d2
#define IDM_VDCRENDER_SCALE2X        0x2d3

#define IDM_VERTICAL_STRETCH         0x2d4

#define IDM_VDC16K                   0x141
#define IDM_VDC64K                   0x142

#define IDM_FUNCROM                  0x143
#define IDM_INTFUNCROM               0x144
#define IDM_INTFUNCROM_NONE          0x327
#define IDM_INTFUNCROM_ROM           0x328
#define IDM_INTFUNCROM_RAM           0x329
#define IDM_INTFUNCROM_RTC           0x32a
#define IDM_INTFUNCROM_RTC_SAVE      0x349

#define IDM_EXTFUNCROM               0x145
#define IDM_EXTFUNCROM_NONE          0x344
#define IDM_EXTFUNCROM_ROM           0x345
#define IDM_EXTFUNCROM_RAM           0x346
#define IDM_EXTFUNCROM_RTC           0x347
#define IDM_EXTFUNCROM_RTC_SAVE      0x34a

#define IDM_PAL                      0x146
#define IDM_NTSC                     0x147
#define IDM_NTSCOLD                  0x148

#define IDM_AUTOSTART_SETTINGS       0x1aa
#define IDM_AUTOSTART_WARP           0x1ab
#define IDM_RUN_WITH_COLON           0x1ac
#define IDM_LOAD_TO_BASIC_START      0x21a
#define IDM_AUTOSTART_DELAY          0x4a8
#define IDM_AUTOSTART_RANDOM_DELAY   0x2b4
#define IDM_AUTOSTART_PRG_MODE       0x1ad
#define IDM_AUTOSTART_PRG_VIRTUAL_FS 0x1ae
#define IDM_AUTOSTART_PRG_INJECT     0x1af
#define IDM_AUTOSTART_PRG_DISK_IMAGE 0x1b0
#define IDM_AUTOSTART_DISK_FILE      0x1c5

#define IDM_C64DTV_FLASHROM_ATTACH   0x1a4
#define IDM_C64DTV_FLASHROM_DETACH   0x1a5
#define IDM_C64DTV_FLASHROM_RW       0x1a6

#define IDM_C64DTV_REVISION          0x1a7
#define IDM_DTV2                     0x1a8
#define IDM_DTV3                     0x1a9

#define IDM_C64DTV_HUMMER_ADC        0x1cb

#define IDM_SAMPLER_SETTINGS         0x382
#define IDM_SAMPLER_GAIN             0x383
#define IDM_SAMPLER_GAIN_5           0x384
#define IDM_SAMPLER_GAIN_10          0x385
#define IDM_SAMPLER_GAIN_25          0x386
#define IDM_SAMPLER_GAIN_50          0x387
#define IDM_SAMPLER_GAIN_75          0x388
#define IDM_SAMPLER_GAIN_100         0x389
#define IDM_SAMPLER_GAIN_105         0x38a
#define IDM_SAMPLER_GAIN_110         0x38b
#define IDM_SAMPLER_GAIN_125         0x38c
#define IDM_SAMPLER_GAIN_150         0x38d
#define IDM_SAMPLER_GAIN_175         0x38e
#define IDM_SAMPLER_GAIN_200         0x38f
#define IDM_SAMPLER_FILE             0x390

#define IDM_REU_SETTINGS             0x1eb
#define IDM_REU                      0x149
#define IDM_REUSIZE                  0x150
#define IDM_REU128                   0x601
#define IDM_REU256                   0x602
#define IDM_REU512                   0x604
#define IDM_REU1024                  0x608
#define IDM_REU2048                  0x610
#define IDM_REU4096                  0x620
#define IDM_REU8192                  0x640
#define IDM_REU16384                 0x680
#define IDM_SAVE_REU                 0x26a
#define IDM_REUFILE                  0x14a

#define IDM_GEORAM_SETTINGS          0x1ec
#define IDM_GEORAM                   0x16b
#define IDM_GEORAMSIZE               0x16c
#define IDM_GEORAM64                 0x901
#define IDM_GEORAM128                0x902
#define IDM_GEORAM256                0x904
#define IDM_GEORAM512                0x908
#define IDM_GEORAM1024               0x910
#define IDM_GEORAM2048               0x920
#define IDM_GEORAM4096               0x940
#define IDM_SAVE_GEORAM              0x26b
#define IDM_GEORAMFILE               0x14b
#define IDM_GEORAM_IO_SWAP           0x2c6

#define IDM_RAMCART_SETTINGS         0x1ed
#define IDM_RAMCART                  0x12e
#define IDM_RAMCARTSIZE              0x16f
#define IDM_RAMCART64                0x941
#define IDM_RAMCART128               0x942
#define IDM_SAVE_RAMCART             0x26c
#define IDM_RAMCARTFILE              0x14c

#define IDM_MAGICVOICE_SETTINGS      0x1f6
#define IDM_MAGICVOICE               0x1f7
#define IDM_MAGICVOICEFILE           0x1f8

#define IDM_V364SPEECH_SETTINGS      0x1f9
#define IDM_V364SPEECH               0x1fa
#define IDM_V364SPEECHFILE           0x1fb

#define IDM_IDE64_SETTINGS           0x2a0
#define IDM_IDE64_REVISION           0x2a1
#define IDM_IDE64_VER_3              0x740
#define IDM_IDE64_VER_41             0x741
#define IDM_IDE64_VER_42             0x742
#define IDM_IDE64_HD1                0x2a8
#define IDM_IDE64_IMAGE1             0x2a9
#define IDM_IDE64_AUTO1              0x2aa
#define IDM_IDE64_GEO1               0x2ab
#define IDM_IDE64_HD2                0x2b0
#define IDM_IDE64_IMAGE2             0x2b1
#define IDM_IDE64_AUTO2              0x2b2
#define IDM_IDE64_GEO2               0x2ac
#define IDM_IDE64_HD3                0x2b8
#define IDM_IDE64_IMAGE3             0x2b9
#define IDM_IDE64_AUTO3              0x2ba
#define IDM_IDE64_GEO3               0x2ad
#define IDM_IDE64_HD4                0x2c0
#define IDM_IDE64_IMAGE4             0x2c1
#define IDM_IDE64_AUTO4              0x2c2
#define IDM_IDE64_GEO4               0x2b3
#define IDM_IDE64_RTC_SAVE           0x34c
#define IDM_IDE64_SB_DIGIMAX         0x393
#define IDM_IDE64_SB_DIGIMAX_BASE    0x394
#define IDM_IDE64_SB_DIGIMAX_DE40    0x395
#define IDM_IDE64_SB_DIGIMAX_DE48    0x396
#define IDM_IDE64_SB_ETFE            0x45f
#define IDM_IDE64_SB_ETFE_BASE       0x460
#define IDM_IDE64_SB_ETFE_DE00       0x461
#define IDM_IDE64_SB_ETFE_DE10       0x462
#define IDM_IDE64_SB_ETFE_DF00       0x463
#define IDM_IDE64_CLOCKPORT_DEVICE   0x48c
#define IDM_IDE64_CLOCKPORT_NONE     0x48d
#define IDM_IDE64_CLOCKPORT_RRNET    0x48e
#define IDM_IDE64_CLOCKPORT_MP3AT64  0x48f
#define IDM_IDE64_USB_SRVR_SETTINGS  0x4a4
#define IDM_IDE64_USB_SERVER_ENABLE  0x4a5
#define IDM_IDE64_USB_SERVER_ADDR    0x4a6

#define IDM_MMC64_SETTINGS           0x229
#define IDM_MMC64                    0x22a
#define IDM_MMC64_REVISION           0x22b
#define IDM_MMC64_REV_A              0x22c
#define IDM_MMC64_REV_B              0x22d
#define IDM_MMC64_FLASH_JUMPER       0x22e
#define IDM_MMC64_BIOS_WRITE         0x23e
#define IDM_MMC64_BIOS_FILE          0x243
#define IDM_MMC64_SD_MMC_READONLY    0x244
#define IDM_MMC64_SD_MMC_FILE        0x246
#define IDM_MMC64_SD_TYPE            0x247
#define IDM_MMC64_SD_TYPE_AUTO       0x248
#define IDM_MMC64_SD_TYPE_MMC        0x249
#define IDM_MMC64_SD_TYPE_SD         0x24a
#define IDM_MMC64_SD_TYPE_SDHC       0x24b
#define IDM_MMC64_CLOCKPORT_DEVICE   0x464
#define IDM_MMC64_CLOCKPORT_NONE     0x465
#define IDM_MMC64_CLOCKPORT_RRNET    0x466
#define IDM_MMC64_CLOCKPORT_MP3AT64  0x467

#define IDM_MMCR_SETTINGS            0x24c
#define IDM_MMCR_RESCUE_MODE         0x24d
#define IDM_MMCR_EEPROM_WRITE        0x24e
#define IDM_MMCR_EEPROM_FILE         0x24f
#define IDM_MMCR_EEPROM_READWRITE    0x253
#define IDM_MMCR_SD_MMC_READWRITE    0x259
#define IDM_MMCR_SD_MMC_FILE         0x265
#define IDM_MMCR_SD_TYPE             0x25a
#define IDM_MMCR_SD_TYPE_AUTO        0x25b
#define IDM_MMCR_SD_TYPE_MMC         0x25c
#define IDM_MMCR_SD_TYPE_SD          0x25d
#define IDM_MMCR_SD_TYPE_SDHC        0x25e
#define IDM_MMCR_CLOCKPORT_DEVICE    0x468
#define IDM_MMCR_CLOCKPORT_NONE      0x469
#define IDM_MMCR_CLOCKPORT_RRNET     0x46a
#define IDM_MMCR_CLOCKPORT_MP3AT64   0x46b

#define IDM_GMOD2_SETTINGS           0x16d
#define IDM_GMOD2_EEPROM_WRITE       0x17c
#define IDM_GMOD2_EEPROM_FILE        0x18b
#define IDM_GMOD2_FLASH_READWRITE    0x2a2

#define IDM_RR_SETTINGS              0x2f3
#define IDM_RR_FLASH_WRITE           0x2f4
#define IDM_RR_FLASH_JUMPER          0x2f5
#define IDM_RR_BANK_JUMPER           0x2f6
#define IDM_RR_REVISION              0x2f7
#define IDM_RR_REV_RR                0x2f8
#define IDM_RR_REV_NR                0x2f9
#define IDM_RR_CLOCKPORT_DEVICE      0x46c
#define IDM_RR_CLOCKPORT_NONE        0x46d
#define IDM_RR_CLOCKPORT_RRNET       0x46e
#define IDM_RR_CLOCKPORT_MP3AT64     0x46f

#define IDM_DIGIMAX_SETTINGS         0x1ee
#define IDM_DIGIMAX                  0x1a1
#define IDM_DIGIMAXBASE              0x1a2
#define IDM_DIGIMAXDE00              0x950
#define IDM_DIGIMAXDE20              0x951
#define IDM_DIGIMAXDE40              0x952
#define IDM_DIGIMAXDE60              0x953
#define IDM_DIGIMAXDE80              0x954
#define IDM_DIGIMAXDEA0              0x955
#define IDM_DIGIMAXDEC0              0x956
#define IDM_DIGIMAXDEE0              0x957
#define IDM_DIGIMAXDF00              0x958
#define IDM_DIGIMAXDF20              0x959
#define IDM_DIGIMAXDF40              0x95a
#define IDM_DIGIMAXDF60              0x95b
#define IDM_DIGIMAXDF80              0x95c
#define IDM_DIGIMAXDFA0              0x95d
#define IDM_DIGIMAXDFC0              0x95e
#define IDM_DIGIMAXDFE0              0x95f

#define IDM_DS12C887RTC_SETTINGS     0x30f
#define IDM_DS12C887RTC              0x31c
#define IDM_DS12C887RTCBASE          0x31d
#define IDM_DS12C887RTCD500          0x31e
#define IDM_DS12C887RTCD600          0x31f
#define IDM_DS12C887RTCD700          0x321
#define IDM_DS12C887RTCDE00          0x322
#define IDM_DS12C887RTCDF00          0x323
#define IDM_DS12C887RTC9800          0x324
#define IDM_DS12C887RTC9C00          0x325
#define IDM_DS12C887RTC_SAVE         0x34b
#define IDM_DS12C887RTCRUNMODE       0x366
#define IDM_DS12C887RTC_HALTED       0x367
#define IDM_DS12C887RTC_RUNNING      0x368

#define IDM_SFX_SE_SETTINGS          0x1f0
#define IDM_SFX_SE                   0x192
#define IDM_SFX_SE_TYPE              0x1c1
#define IDM_SFX_SE_3526              0x193
#define IDM_SFX_SE_3812              0x1c0
#define IDM_SFX_SE_IO_SWAP           0x2c7

#define IDM_SFX_SS_SETTINGS          0x2c8
#define IDM_SFX_SS                   0x112
#define IDM_SFX_SS_IO_SWAP           0x2c9

#define IDM_SS5_32K_ADDON            0x37b

#define IDM_C64_CPM                  0x381

#define IDM_RRNETMK3_SETTINGS        0x392
#define IDM_RRNETMK3_FLASH_JUMPER    0x1a3
#define IDM_RRNETMK3_BIOS_WRITE      0x391

#define IDM_EASYFLASH_SETTINGS       0x1ef
#define IDM_EASYFLASH_JUMPER         0x1c2
#define IDM_EASYFLASH_OPTIMIZE_CRT   0x371
#define IDM_EASYFLASH_AUTOSAVE       0x1c3
#define IDM_EASYFLASH_SAVE_NOW       0x1c4

#define IDM_VIC20_FE_WRITEBACK       0x21b
#define IDM_VIC20_VFP_WRITEBACK      0x21c
#define IDM_VIC20_MC_WRITEBACK       0x21d
#define IDM_VIC20_MC_FILE            0x21e
#define IDM_VIC20_UM_WRITEBACK       0x37d

#define IDM_VIC20_IO2_RAM            0x37e
#define IDM_VIC20_IO3_RAM            0x37f

#define IDM_VIC20_VFLI_MOD           0x3fe

#define IDM_MEMORY_HACKS_SETTINGS    0x1f3

#define IDM_PLUS60KBASE              0x17d
#define IDM_PLUS60KD040              0x17e
#define IDM_PLUS60KD100              0x17f
#define IDM_PLUS60KFILE              0x14d

#define IDM_PLUS256KFILE             0x14e

#define IDM_MEMPLUS4_HACKS_SETTINGS  0x36a
#define IDM_PLUS4_MEMORY_HACK_DEVICE 0x36b
#define IDM_MEMORY_PLUS4HACK_NONE    0x36c
#define IDM_MEMORY_PLUS4HACK_C256K   0x36d
#define IDM_MEMORY_PLUS4HACK_H256K   0x36e
#define IDM_MEMORY_PLUS4HACK_H1024K  0x36f
#define IDM_MEMORY_PLUS4HACK_H4096K  0x370

#define IDM_DQBB_SETTINGS            0x1f1
#define IDM_DQBB                     0x1b1
#define IDM_SAVE_DQBB                0x26d
#define IDM_DQBBFILE                 0x1b2

#define IDM_ISEPIC_SETTINGS          0x1f2
#define IDM_ISEPIC                   0x1b3
#define IDM_ISEPIC_SWITCH            0x1b4
#define IDM_SAVE_ISEPIC              0x26e
#define IDM_ISEPICFILE               0x26f

#define IDM_EXPERT_SETTINGS          0x27e
#define IDM_EXPERT                   0x287
#define IDM_EXPERT_MODE              0x288
#define IDM_EXPERT_MODE_OFF          0x289
#define IDM_EXPERT_MODE_PRG          0x28a
#define IDM_EXPERT_MODE_ON           0x28b
#define IDM_SAVE_EXPERT              0x28c
#define IDM_EXPERTFILE               0x28d

#define IDM_C64_MEMORY_HACK_DEVICE   0x1f4

#define IDM_MEMORY_HACK_NONE         0x1f5
#define IDM_MEMORY_HACK_C64_256K     0x350
#define IDM_MEMORY_HACK_PLUS60K      0x351
#define IDM_MEMORY_HACK_PLUS256K     0x352

#define IDM_C64_256K_BASE            0x16e
#define IDM_C64_256K_BASEDE00        0x18c
#define IDM_C64_256K_BASEDE80        0x18d
#define IDM_C64_256K_BASEDF00        0x18e
#define IDM_C64_256K_BASEDF80        0x18f
#define IDM_C64_256KFILE             0x14f

#define IDM_PETREU_SETTINGS          0x1e9
#define IDM_PETREU                   0x19b
#define IDM_PETREUSIZE               0x19c
#define IDM_PETREU128                0x19d
#define IDM_PETREU512                0x19e
#define IDM_PETREU1024               0x19f
#define IDM_PETREU2048               0x1a0
#define IDM_PETREUFILE               0x159

#define IDM_PETDWW_SETTINGS          0x1ea
#define IDM_PETDWW                   0x153
#define IDM_PETDWWFILE               0x1e8

#define IDM_PETHRE                   0x335

#define IDM_ETHERNETCART_SETTINGS    0x2ca
#define IDM_ETHERNETCART             0x158
#define IDM_ETHERNETCART_MODE        0x2cb
#define IDM_ETHERNETCART_MODE_TFE    0x140
#define IDM_ETHERNETCART_MODE_RRNET  0x15a
#define IDM_ETHERNETCART_ADDRESS     0x15b
#define IDM_ETHERNETCART_ADDR_000    0x15c
#define IDM_ETHERNETCART_ADDR_010    0x15d
#define IDM_ETHERNETCART_ADDR_020    0x15e
#define IDM_ETHERNETCART_ADDR_030    0x15f
#define IDM_ETHERNETCART_ADDR_040    0x18a
#define IDM_ETHERNETCART_ADDR_050    0x2ae
#define IDM_ETHERNETCART_ADDR_060    0x2af
#define IDM_ETHERNETCART_ADDR_070    0x30e
#define IDM_ETHERNETCART_ADDR_080    0x32b
#define IDM_ETHERNETCART_ADDR_090    0x341
#define IDM_ETHERNETCART_ADDR_0A0    0x449
#define IDM_ETHERNETCART_ADDR_0B0    0x44a
#define IDM_ETHERNETCART_ADDR_0C0    0x44b
#define IDM_ETHERNETCART_ADDR_0D0    0x44c
#define IDM_ETHERNETCART_ADDR_0E0    0x44d
#define IDM_ETHERNETCART_ADDR_0F0    0x44e
#define IDM_ETHERNETCART_ADDR_100    0x44f
#define IDM_ETHERNETCART_ADDR_110    0x450
#define IDM_ETHERNETCART_ADDR_120    0x451
#define IDM_ETHERNETCART_ADDR_130    0x452
#define IDM_ETHERNETCART_ADDR_140    0x453
#define IDM_ETHERNETCART_ADDR_150    0x454
#define IDM_ETHERNETCART_ADDR_160    0x455
#define IDM_ETHERNETCART_ADDR_170    0x456
#define IDM_ETHERNETCART_ADDR_180    0x457
#define IDM_ETHERNETCART_ADDR_190    0x458
#define IDM_ETHERNETCART_ADDR_1A0    0x459
#define IDM_ETHERNETCART_ADDR_1B0    0x45a
#define IDM_ETHERNETCART_ADDR_1C0    0x45b
#define IDM_ETHERNETCART_ADDR_1D0    0x45c
#define IDM_ETHERNETCART_ADDR_1E0    0x45d
#define IDM_ETHERNETCART_ADDR_1F0    0x45e

#define IDM_MOUSE                    0x151
#define IDM_HIDEMOUSE                0x152

#define IDM_SMART_MOUSE_RTC_SAVE     0x34f

#define IDM_VCACHE                   0x154
#define IDM_SBCOLL                   0x155
#define IDM_SSCOLL                   0x156
#define IDM_VDCVCACHE                0x157
#define IDM_C128FULLBANKS            0x190
#define IDM_REFRATEAUTO              0x160
#define IDM_REFRATE1                 0x161
#define IDM_REFRATE2                 0x162
#define IDM_REFRATE3                 0x163
#define IDM_REFRATE4                 0x164
#define IDM_REFRATE5                 0x165
#define IDM_REFRATE6                 0x166
#define IDM_REFRATE7                 0x167
#define IDM_REFRATE8                 0x168
#define IDM_REFRATE9                 0x169
#define IDM_REFRATE10                0x16a

#define IDM_SC6581                   0x170
#define IDM_SC8580                   0x171
#define IDM_SC8580DB                 0x172
#define IDM_SCDTV                    0x1b5
#define IDM_SC6581R3_4885            0x1b6
#define IDM_SC6581R3_0486S           0x1b7
#define IDM_SC6581R3_3984            0x1b8
#define IDM_SC6581R4AR_3789          0x1b9
#define IDM_SC6581R3_4485            0x1ba
#define IDM_SC6581R4_1986S           0x1bb
#define IDM_SC8580R5_3691            0x1bc
#define IDM_SC8580R5_3691DB          0x1bd
#define IDM_SC8580R5_1489            0x1be
#define IDM_SC8580R5_1489DB          0x1bf

#define IDM_SOUNDON                  0x173
#define IDM_SIDFILTER                0x174
#define IDM_RESID                    0x175
#define IDM_RESIDMETHOD              0x176
#define IDM_RESIDFAST                0x177
#define IDM_RESIDINTERPOL            0x178
#define IDM_RESIDRESAMPLE            0x179
#define IDM_RESIDFASTRES             0x17a
#define IDM_STEREO                   0x17b
#define IDM_SID_AMOUNT               0x2f0
#define IDM_TRIPLE                   0x2f1
#define IDM_NO_EXTRA_SID             0x2f2

#define IDM_RESIDBAND                0x13d
#define IDM_RESIDPASS0               0x180
#define IDM_RESIDPASS10              0x181
#define IDM_RESIDPASS20              0x182
#define IDM_RESIDPASS30              0x183
#define IDM_RESIDPASS40              0x184
#define IDM_RESIDPASS50              0x185
#define IDM_RESIDPASS60              0x186
#define IDM_RESIDPASS70              0x187
#define IDM_RESIDPASS80              0x188
#define IDM_RESIDPASS90              0x189

#define IDM_SR8000                   0x194
#define IDM_SR11025                  0x195
#define IDM_SR22050                  0x196
#define IDM_SR44100                  0x197

#define IDM_SYNCFLEX                 0x198
#define IDM_SYNCADJUST               0x199
#define IDM_SYNCEXACT                0x19a

#define IDM_FRAGVERYSMALL            0x342
#define IDM_FRAGSMALL                0x1c7
#define IDM_FRAGMEDIUM               0x1c8
#define IDM_FRAGLARGE                0x1c9
#define IDM_FRAGVERYLARGE            0x343

#define IDM_BUF010                   0x202
#define IDM_BUF025                   0x205
#define IDM_BUF040                   0x208
#define IDM_BUF055                   0x20b
#define IDM_BUF070                   0x20e
#define IDM_BUF085                   0x211
#define IDM_BUF100                   0x214

#define IDM_DEVDART                  0x215
#define IDM_DEVSID                   0x216
#define IDM_DEVWAV                   0x217
#define IDM_DEVRAW                   0x218
#define IDM_DEVDUMMY                 0x219

#define IDM_VOLUME                   0x400
#define IDM_VOL100                   0x40a
#define IDM_VOL90                    0x409
#define IDM_VOL80                    0x408
#define IDM_VOL70                    0x407
#define IDM_VOL60                    0x406
#define IDM_VOL50                    0x405
#define IDM_VOL40                    0x404
#define IDM_VOL30                    0x403
#define IDM_VOL20                    0x402
#define IDM_VOL10                    0x401

#define IDM_STATUSBAR                0x21f
#define IDM_MENUBAR                  0x220

#define IDM_STRETCH1                 0x221
#define IDM_STRETCH2                 0x222
#define IDM_STRETCH3                 0x223

#define IDM_PAUSE                    0x224

#define IDM_HLPINDEX                 0x225
#define IDM_HLPGENERAL               0x226
#define IDM_HLPUSINGHLP              0x227
#define IDM_HLPKEYBOARD              0x228

#define IDM_IEEE                     0x22f

#define IDM_MODEL                    0x230

#define IDM_MODELLINE                0x23f

#define IDM_CBM510PAL                0x311
#define IDM_CBM510NTSC               0x312
#define IDM_CBM610PAL                0x313
#define IDM_CBM610NTSC               0x314
#define IDM_CBM620PAL                0x315
#define IDM_CBM620NTSC               0x316
#define IDM_CBM620PLUSPAL            0x317
#define IDM_CBM620PLUSNTSC           0x318
#define IDM_CBM710NTSC               0x319
#define IDM_CBM720NTSC               0x31a
#define IDM_CBM720PLUSNTSC           0x31b

#define IDM_MODEL750                 0x240
#define IDM_MODEL660                 0x241
#define IDM_MODEL650                 0x242
      
#define IDM_RAMSIZE                  0x250

#define IDM_RAM64                    0x251
#define IDM_RAM128                   0x252
#define IDM_RAM256                   0x254
#define IDM_RAM512                   0x258
#define IDM_RAM1024                  0x260
      
#define IDM_RAMMAPPING               0x13e
      
#define IDM_RAM08                    0x238
#define IDM_RAM10                    0x239
#define IDM_RAM20                    0x23a
#define IDM_RAM40                    0x23b
#define IDM_RAM60                    0x23c
#define IDM_RAMC0                    0x23d

#define IDM_CHARSET                  0x25f
#define IDM_EOI                      0x261
#define IDM_ROMPATCH                 0x262
#define IDM_DIAGPIN                  0x263
#define IDM_SUPERPET                 0x264
#define IDM_MAP9                     0x245
#define IDM_MAPA                     0x266

#define IDM_IOMEM                    0x267
#define IDM_IOMEM256                 0x268
#define IDM_IOMEM2K                  0x269

#define IDM_PETRAM4                  0x304
#define IDM_PETRAM8                  0x308
#define IDM_PETRAM16                 0x310
#define IDM_PETRAM32                 0x320
#define IDM_PETRAM96                 0x360
#define IDM_PETRAM128                0x380

#define IDM_C64_MODEL                0x1cd
#define IDM_C64PAL                   0x1ce
#define IDM_C64CPAL                  0x1cf
#define IDM_C64OLDPAL                0x1d0
#define IDM_C64NTSC                  0x1d1
#define IDM_C64CNTSC                 0x1d2
#define IDM_C64OLDNTSC               0x1d3
#define IDM_DREAN                    0x1d4
#define IDM_C64SXPAL                 0x336
#define IDM_C64SXNTSC                0x337
#define IDM_C64JAP                   0x338
#define IDM_C64GS                    0x339
#define IDM_PET64PAL                 0x33a
#define IDM_PET64NTSC                0x33b
#define IDM_ULTIMAX                  0x33c
#define IDM_CUSTOM_C64_MODEL         0x1d5
#define IDM_VICII_MODEL              0x1d6
#define IDM_6569_PAL                 0x1d7
#define IDM_8565_PAL                 0x1d8
#define IDM_6569R1_OLD_PAL           0x1d9
#define IDM_6567_NTSC                0x1da
#define IDM_8562_NTSC                0x1db
#define IDM_6567R56A_OLD_NTSC        0x1dc
#define IDM_6572_PAL_N               0x1dd
#define IDM_PALG                     0x35a
#define IDM_OLD_PALG                 0x35b
#define IDM_NTSCM                    0x35c
#define IDM_OLD_NTSCM                0x35d
#define IDM_PALN                     0x35e
#define IDM_VICII_NEW_LUMINANCES     0x1de
#define IDM_CIA1_MODEL               0x1df
#define IDM_CIA1_6526_OLD            0x1e0
#define IDM_CIA1_6526A_NEW           0x1e1
#define IDM_CIA2_MODEL               0x1e2
#define IDM_CIA2_6526_OLD            0x1e3
#define IDM_CIA2_6526A_NEW           0x1e4
#define IDM_GLUE_LOGIC               0x1e5
#define IDM_DISCRETE                 0x1e6
#define IDM_CUSTOM_IC                0x1e7

#define IDM_KERNAL_REVISION          0x35f
#define IDM_KERNAL_REV_1             0x361
#define IDM_KERNAL_REV_2             0x362
#define IDM_KERNAL_REV_3             0x363
#define IDM_KERNAL_SX64              0x364
#define IDM_KERNAL_4064              0x365

#define IDM_C128_MODEL               0x2fa
#define IDM_C128PAL                  0x2fb
#define IDM_C128DCRPAL               0x2fc
#define IDM_C128NTSC                 0x2fd
#define IDM_C128DCRNTSC              0x2fe

#define IDM_PLUS4_MODEL              0x2d5
#define IDM_C16PAL                   0x2d6
#define IDM_C16NTSC                  0x2d7
#define IDM_PLUS4PAL                 0x2d8
#define IDM_PLUS4NTSC                0x2d9
#define IDM_V364NTSC                 0x2da
#define IDM_C232NTSC                 0x2db

#define IDM_C64DTV_MODEL             0x2dc
#define IDM_DTV2PAL                  0x2dd
#define IDM_DTV2NTSC                 0x2de
#define IDM_DTV3PAL                  0x2df
#define IDM_DTV3NTSC                 0x2e0
#define IDM_HUMMERNTSC               0x2e1

#define IDM_PETMODEL                 0x270

#define IDM_PET2001                  0x271
#define IDM_PET3008                  0x272
#define IDM_PET3016                  0x273
#define IDM_PET3032                  0x274
#define IDM_PET3032B                 0x275
#define IDM_PET4016                  0x276
#define IDM_PET4032                  0x277
#define IDM_PET4032B                 0x278
#define IDM_PET8032                  0x279
#define IDM_PET8096                  0x27a
#define IDM_PET8296                  0x27b
#define IDM_PETSUPER                 0x27c

#define IDM_VIC20_MODEL              0x33d
#define IDM_VIC20PAL                 0x33e
#define IDM_VIC20NTSC                0x33f
#define IDM_VIC21                    0x340

#define IDM_CRTC                     0x27d

#define IDM_VIDEOSIZE                0x27f

#define IDM_VSDETECT                 0x280
#define IDM_VS40                     0x281
#define IDM_VS80                     0x282

#define IDM_TRUEDRIVE                0x283

#define IDM_FLIPLIST                 0x284

#define IDM_PLUS                     0x285
#define IDM_MINUS                    0x286

#define IDM_FLIPPREV8                0x480
#define IDM_FLIPNEXT8                0x481

#define IDM_FLIPPREV9                0x490
#define IDM_FLIPNEXT9                0x491

#define IDM_KERNALREV                0x290
#define IDM_KERNALREV0               0x291
#define IDM_KERNALREV3               0x292
#define IDM_KERNALREVSX              0x293
#define IDM_KERNALREV4064            0x294

#define IDM_CARTRIDGE                0x500
#define IDM_CARTRIDGE_GENERIC_GROUP  0x40b
#define IDM_CRTGEN                   0x501
#define IDM_CRTGEN8KB                0x502
#define IDM_CRTGEN16KB               0x503
#define IDM_CRTGENULTI               0x40c
#define IDM_CARTRIDGE_FREEZER_GROUP  0x40d
#define IDM_CRTACTREPL               0x504
#define IDM_CRTACTREPL2              0x40e
#define IDM_CRTACTREPL3              0x40f
#define IDM_CRTACTREPL4              0x410
#define IDM_CRTATOMPOW               0x505
#define IDM_CRTCAPTURE               0x411
#define IDM_CRTDIASHOWMAKER          0x412
#define IDM_CRTEXPERT                0x413
#define IDM_CRTFC1                   0x414
#define IDM_CRTFC3                   0x415
#define IDM_CRTFCPLUS                0x416
#define IDM_CRTFORMEL64              0x417
#define IDM_CRTFREEZEFRAME           0x418
#define IDM_CRTFREEZEMACHINE         0x419
#define IDM_CRTGAMEKILLER            0x41a
#define IDM_CRTKCSPOWER              0x41b
#define IDM_CRTMAGICFORMEL           0x41c
#define IDM_CRTMMCR                  0x41d
#define IDM_CRTRR                    0x41e
#define IDM_SS64                     0x41f
#define IDM_CRTSSSHOT                0x507
#define IDM_CRTSSSHOT5               0x508
#define IDM_CARTRIDGE_UTIL_GROUP     0x420
#define IDM_CRTCOMAL80               0x421
#define IDM_CRTDELAEP256             0x422
#define IDM_CRTDELAEP64              0x423
#define IDM_CRTDELAEP7x8             0x424
#define IDM_CRTEASYCALC              0x425
#define IDM_CRTEASYFLASH             0x426
#define IDM_CRTEPYX                  0x506
#define IDM_CRTEXOS                  0x427
#define IDM_CRTIDE64                 0x50b
#define IDM_CRTIEEE                  0x50a
#define IDM_CRTKINGSOFT              0x428
#define IDM_CRTMACH5                 0x429
#define IDM_CRTMAGICDESK             0x42a
#define IDM_CRTMAGICVOICE            0x42b
#define IDM_CRTMIKROASS              0x42c
#define IDM_CRTMMC64                 0x42d
#define IDM_CRTP64                   0x42e
#define IDM_CRTPAGEFOX               0x42f
#define IDM_CRTREX                   0x430
#define IDM_CRTREXEP256              0x431
#define IDM_CRTRRNETMK3              0x432
#define IDM_CRTROSS                  0x433
#define IDM_CRTSIMON                 0x434
#define IDM_CRTSTARDOS               0x435
#define IDM_CRTSBASIC                0x436
#define IDM_CRTSE5                   0x437
#define IDM_CRTWARPSPEED             0x438
#define IDM_CRTWEST                  0x509
#define IDM_CARTRIDGE_GAME_GROUP     0x439
#define IDM_CRTDINAMIC               0x43a
#define IDM_CRTFUNPLAY               0x43b
#define IDM_CRTGMOD2                 0x43c
#define IDM_CRTGS                    0x43d
#define IDM_CRTOCEAN                 0x43e
#define IDM_CRTRGCD                  0x43f
#define IDM_CRTSILVERROCK128         0x440
#define IDM_CRTSUPERGAMES            0x441
#define IDM_CRTZAXXON                0x442
#define IDM_CARTRIDGE_RAMEX_GROUP    0x443
#define IDM_CRTDQBB                  0x444
#define IDM_CRTGEORAM                0x445
#define IDM_CRTISEPIC                0x446
#define IDM_CRTRAMCART               0x447
#define IDM_CRTREU                   0x448
#define IDM_CRTFREEZE                0x50d
#define IDM_CRTSAVEIMG               0x50e
#define IDM_CARTRIDGEDET             0x50f

#define IDM_VIC20_ATTACH_CARTRIDGE   0x348
#define IDM_VIC20_CART_GENERIC       0x200
#define IDM_VIC20_CART_BEHR_BONZ     0x2a3
#define IDM_VIC20_CART_MEGACART      0x201
#define IDM_VIC20_CART_FINAL_EXP     0x203
#define IDM_VIC20_CART_VFP           0x204
#define IDM_VIC20_CART_UM            0x37c
#define IDM_VIC20_CART_ADD_GENERIC   0x206
#define IDM_VIC20_CART_SMART_ATTACH  0x207
#define IDM_VIC20_CART_4_8_16KB_2000 0x209
#define IDM_VIC20_CART_4_8_16KB_4000 0x20a
#define IDM_VIC20_CART_4_8_16KB_6000 0x20c
#define IDM_VIC20_CART_4_8KB_A000    0x20d
#define IDM_VIC20_CART_4KB_B000      0x20f
#define IDM_VIC20_SET_DEFAULT_CART   0x210
#define IDM_VIC20_RESET_ON_CART_CHNG 0x212
#define IDM_VIC20_DETACH_CART        0x213

#define IDM_PLUS4_ATTACH_CARTRIDGE   0x231
#define IDM_PLUS4_CART_SMART_ATTACH  0x232
#define IDM_PLUS4_C0_LOW             0x233
#define IDM_PLUS4_C0_HIGH            0x234
#define IDM_PLUS4_C1_LOW             0x235
#define IDM_PLUS4_C1_HIGH            0x236
#define IDM_PLUS4_C2_LOW             0x237
#define IDM_PLUS4_C2_HIGH            0x2ff
#define IDM_PLUS4_RESET_ON_CART_CHNG 0x300
#define IDM_PLUS4_DETACH_CART        0x301

#define IDM_CBM2_ATTACH_CARTRIDGE    0x302
#define IDM_CBM2_LOAD_1XXX           0x303
#define IDM_CBM2_UNLOAD_1XXX         0x305
#define IDM_CBM2_LOAD_2_3XXX         0x306
#define IDM_CBM2_UNLOAD_2_3XXX       0x307
#define IDM_CBM2_LOAD_4_5XXX         0x309
#define IDM_CBM2_UNLOAD_4_5XXX       0x30a
#define IDM_CBM2_LOAD_6_7XXX         0x30b
#define IDM_CBM2_UNLOAD_6_7XXX       0x30c
#define IDM_CBM2_RESET_ON_CART_CHNG  0x30d

#define IDM_PRINTER                  0x6ff

#define IDM_PRINTER4                 0x700
#define IDM_PRT4IEC                  0x701
#define IDM_PRT4DRV                  0x702
#define IDM_PRT4ASCII                0x703
#define IDM_PRT4MPS803               0x704
#define IDM_PRT4NL10                 0x705
#define IDM_PRT4RAW                  0x1fc
#define IDM_PRT4OUT                  0x706
#define IDM_PRT4TXT                  0x707
#define IDM_PRT4GFX                  0x708

#define IDM_PRINTER5                 0x710
#define IDM_PRT5IEC                  0x711
#define IDM_PRT5DRV                  0x712
#define IDM_PRT5ASCII                0x713
#define IDM_PRT5MPS803               0x714
#define IDM_PRT5NL10                 0x715
#define IDM_PRT5RAW                  0x1fd
#define IDM_PRT5OUT                  0x716
#define IDM_PRT5TXT                  0x717
#define IDM_PRT5GFX                  0x718

#define IDM_PRINTER6                 0x720
#define IDM_PRT6IEC                  0x721
#define IDM_PRT6DRV                  0x722
#define IDM_PRT6_1520                0x723
#define IDM_PRT6RAW                  0x1fe
#define IDM_PRT6OUT                  0x726
#define IDM_PRT6TXT                  0x727
#define IDM_PRT6GFX                  0x728

#define IDM_PRINTERUP                0x730
#define IDM_PRTUP                    0x731
#define IDM_PRTUPDRV                 0x732
#define IDM_PRTUPASCII               0x733
#define IDM_PRTUPMPS803              0x734
#define IDM_PRTUPNL10                0x735
#define IDM_PRTUPRAW                 0x1ff
#define IDM_PRTUPOUT                 0x736
#define IDM_PRTUPTXT                 0x737
#define IDM_PRTUPGFX                 0x738

#define IDM_C128TYPE                 0x7ff
#define IDM_C128INT                  0x800
#define IDM_C128FI                   0x801
#define IDM_C128FR                   0x802
#define IDM_C128GE                   0x803
#define IDM_C128IT                   0x804
#define IDM_C128NO                   0x805
#define IDM_C128SW                   0x806

#define IDM_USERPORT_DEVICE_SETTINGS     0x397
#define IDM_USERPORT_DAC                 0x1cc
#define IDM_USERPORT_DIGIMAX             0x398
#define IDM_USERPORT_4BIT_SAMPLER        0x399
#define IDM_USERPORT_8BSS                0x39a
#define IDM_USERPORT_RTC58321A_SETTINGS  0x39b
#define IDM_USERPORT_RTC58321A           0x39c
#define IDM_USERPORT_RTC58321A_SAVE      0x39d
#define IDM_USERPORT_DS1307_RTC_SETTINGS 0x39e
#define IDM_USERPORT_DS1307_RTC          0x39f
#define IDM_USERPORT_DS1307_RTC_SAVE     0x3a0

#define IDM_JOYPORT_DEVICES            0x326

#define IDM_JOYPORT_1_DEVICE           0x34d
#define IDM_JOYPORT_1_NONE             0x34e
#define IDM_JOYPORT_1_JOYSTICK         0x3a1
#define IDM_JOYPORT_1_PADDLES          0x3a2
#define IDM_JOYPORT_1_MOUSE_1351       0x3a3
#define IDM_JOYPORT_1_MOUSE_NEOS       0x3a4
#define IDM_JOYPORT_1_MOUSE_AMIGA      0x3a5
#define IDM_JOYPORT_1_MOUSE_CX22       0x3a6
#define IDM_JOYPORT_1_MOUSE_ST         0x3a7
#define IDM_JOYPORT_1_MOUSE_SMART      0x3a8
#define IDM_JOYPORT_1_MOUSE_MICROMYS   0x3a9
#define IDM_JOYPORT_1_KOALAPAD         0x3aa
#define IDM_JOYPORT_1_LIGHTPEN_U       0x3ab
#define IDM_JOYPORT_1_LIGHTPEN_L       0x3ac
#define IDM_JOYPORT_1_LIGHTPEN_DATEL   0x3ad
#define IDM_JOYPORT_1_LIGHTGUN_Y       0x3ae
#define IDM_JOYPORT_1_LIGHTGUN_L       0x3af
#define IDM_JOYPORT_1_LIGHTPEN_INKWELL 0x3b0
#define IDM_JOYPORT_1_SAMPLER_2BIT     0x3b1
#define IDM_JOYPORT_1_SAMPLER_4BIT     0x3b2
#define IDM_JOYPORT_1_BBRTC            0x3b3
#define IDM_JOYPORT_1_PAPERCLIP64      0x3b4
#define IDM_JOYPORT_1_COPLIN_KEYPAD    0x3b5
#define IDM_JOYPORT_1_CARDCO_KEYPAD    0x3b6
#define IDM_JOYPORT_1_CX85_KEYPAD      0x3b7
#define IDM_JOYPORT_1_RUSHWARE_KEYPAD  0x3b8
#define IDM_JOYPORT_1_CX21_KEYPAD      0x3b9

#define IDM_JOYPORT_2_DEVICE           0x3ba
#define IDM_JOYPORT_2_NONE             0x3bb
#define IDM_JOYPORT_2_JOYSTICK         0x3bc
#define IDM_JOYPORT_2_PADDLES          0x3bd
#define IDM_JOYPORT_2_MOUSE_1351       0x3be
#define IDM_JOYPORT_2_MOUSE_NEOS       0x3bf
#define IDM_JOYPORT_2_MOUSE_AMIGA      0x3c0
#define IDM_JOYPORT_2_MOUSE_CX22       0x3c1
#define IDM_JOYPORT_2_MOUSE_ST         0x3c2
#define IDM_JOYPORT_2_MOUSE_SMART      0x3c3
#define IDM_JOYPORT_2_MOUSE_MICROMYS   0x3c4
#define IDM_JOYPORT_2_KOALAPAD         0x3c5
#define IDM_JOYPORT_2_SAMPLER_2BIT     0x3c6
#define IDM_JOYPORT_2_SAMPLER_4BIT     0x3c7
#define IDM_JOYPORT_2_BBRTC            0x3c8
#define IDM_JOYPORT_2_PAPERCLIP64      0x3c9
#define IDM_JOYPORT_2_COPLIN_KEYPAD    0x3ca
#define IDM_JOYPORT_2_CARDCO_KEYPAD    0x3cb
#define IDM_JOYPORT_2_CX85_KEYPAD      0x3cc
#define IDM_JOYPORT_2_RUSHWARE_KEYPAD  0x3cd
#define IDM_JOYPORT_2_CX21_KEYPAD      0x3ce

#define IDM_JOYPORT_3_DEVICE           0x3cf
#define IDM_JOYPORT_3_NONE             0x3d0
#define IDM_JOYPORT_3_JOYSTICK         0x3d1
#define IDM_JOYPORT_3_MOUSE_NEOS       0x3d2
#define IDM_JOYPORT_3_MOUSE_AMIGA      0x3d3
#define IDM_JOYPORT_3_MOUSE_CX22       0x3d4
#define IDM_JOYPORT_3_MOUSE_ST         0x3d5
#define IDM_JOYPORT_3_SAMPLER_2BIT     0x3d6
#define IDM_JOYPORT_3_SAMPLER_4BIT     0x3d7
#define IDM_JOYPORT_3_BBRTC            0x3d8
#define IDM_JOYPORT_3_PAPERCLIP64      0x3d9
#define IDM_JOYPORT_3_COPLIN_KEYPAD    0x3da
#define IDM_JOYPORT_3_RUSHWARE_KEYPAD  0x3db

#define IDM_JOYPORT_4_DEVICE           0x3dc
#define IDM_JOYPORT_4_NONE             0x3dd
#define IDM_JOYPORT_4_JOYSTICK         0x3de
#define IDM_JOYPORT_4_MOUSE_NEOS       0x3df
#define IDM_JOYPORT_4_MOUSE_AMIGA      0x3e0
#define IDM_JOYPORT_4_MOUSE_CX22       0x3e1
#define IDM_JOYPORT_4_MOUSE_ST         0x3e2
#define IDM_JOYPORT_4_SAMPLER_2BIT     0x3e3
#define IDM_JOYPORT_4_SAMPLER_4BIT     0x3e4
#define IDM_JOYPORT_4_BBRTC            0x3e5
#define IDM_JOYPORT_4_PAPERCLIP64      0x3e6
#define IDM_JOYPORT_4_COPLIN_KEYPAD    0x3e7
#define IDM_JOYPORT_4_RUSHWARE_KEYPAD  0x3e8

#define IDM_JOYPORT_5_DEVICE           0x3e9
#define IDM_JOYPORT_5_NONE             0x3ea
#define IDM_JOYPORT_5_JOYSTICK         0x3eb
#define IDM_JOYPORT_5_PADDLES          0x3ec
#define IDM_JOYPORT_5_MOUSE_1351       0x3ed
#define IDM_JOYPORT_5_MOUSE_NEOS       0x3ee
#define IDM_JOYPORT_5_MOUSE_AMIGA      0x3ef
#define IDM_JOYPORT_5_MOUSE_CX22       0x3f0
#define IDM_JOYPORT_5_MOUSE_ST         0x3f1
#define IDM_JOYPORT_5_MOUSE_SMART      0x3f2
#define IDM_JOYPORT_5_MOUSE_MICROMYS   0x3f3
#define IDM_JOYPORT_5_KOALAPAD         0x3f4
#define IDM_JOYPORT_5_SAMPLER_2BIT     0x3f5
#define IDM_JOYPORT_5_SAMPLER_4BIT     0x3f6
#define IDM_JOYPORT_5_BBRTC            0x3f7
#define IDM_JOYPORT_5_PAPERCLIP64      0x3f8
#define IDM_JOYPORT_5_COPLIN_KEYPAD    0x3f9
#define IDM_JOYPORT_5_CARDCO_KEYPAD    0x3fa
#define IDM_JOYPORT_5_CX85_KEYPAD      0x3fb
#define IDM_JOYPORT_5_RUSHWARE_KEYPAD  0x3fc
#define IDM_JOYPORT_5_CX21_KEYPAD      0x3fd

#define IDM_JOYPORT_BBRTC_SAVE         0x3ff

#define IDM_TAPEPORT_DEVICES           0x476
#define IDM_TAPE_SENSE_DONGLE_ENABLE   0x4a2
#define IDM_DTL_BASIC_DONGLE_ENABLE    0x4a3

#define IDM_DATASETTE_SETTINGS         0x493
#define IDM_DATASETTE_ENABLE           0x494
#define IDM_DATASETTE_RESET_WITH_CPU   0x495
#define IDM_DATASETTE_ZERO_GAP_DELAY   0x496
#define IDM_DATASETTE_SPEED_TUNING     0x497
#define IDM_DATASETTE_TAPE_WOBBLE      0x498

#define IDM_TAPELOG_SETTINGS           0x499
#define IDM_TAPELOG_ENABLE             0x49a
#define IDM_TAPELOG_DESTINATION        0x49b
#define IDM_TAPELOG_EMULOG             0x49c
#define IDM_TAPELOG_USERLOG            0x49d
#define IDM_TAPELOG_FILENAME           0x49e

#define IDM_CP_CLOCK_F83_SETTINGS      0x49f
#define IDM_CP_CLOCK_F83_ENABLE        0x4a0
#define IDM_CP_CLOCK_F83_SAVE          0x4a1


void menu_action(HWND hwnd, USHORT item); //, MPARAM mp2);
void menu_select(HWND hwnd, USHORT item);
