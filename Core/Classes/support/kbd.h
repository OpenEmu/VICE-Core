/*
 Copyright (c) 2016, OpenEmu Team
 
 Redistribution and use in source and binary forms, with or without
 modification, are permitted provided that the following conditions are met:
 * Redistributions of source code must retain the above copyright
 notice, this list of conditions and the following disclaimer.
 * Redistributions in binary form must reproduce the above copyright
 notice, this list of conditions and the following disclaimer in the
 documentation and/or other materials provided with the distribution.
 * Neither the name of the OpenEmu Team nor the
 names of its contributors may be used to endorse or promote products
 derived from this software without specific prior written permission.
 
 THIS SOFTWARE IS PROVIDED BY OpenEmu Team ''AS IS'' AND ANY
 EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
 WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
 DISCLAIMED. IN NO EVENT SHALL OpenEmu Team BE LIABLE FOR ANY
 DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
 (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
 LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND
 ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
 SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */


#ifndef VICE_KBD_H
#define VICE_KBD_H

extern void kbd_arch_init(void);
extern int kbd_arch_get_host_mapping(void);

extern signed long kbd_arch_keyname_to_keynum(char *keyname);
extern const char *kbd_arch_keynum_to_keyname(signed long keynum);

/* FIXME: remove these */
#define KBD_C64_SYM_US  "osx_sym_us.vkm"
#define KBD_C64_SYM_DE  "osx_sym_de.vkm"
#define KBD_C64_POS     "osx_pos.vkm"
#define KBD_C128_SYM    "osx_sym.vkm"
#define KBD_C128_POS    "osx_pos.vkm"
#define KBD_VIC20_SYM   "osx_sym.vkm"
#define KBD_VIC20_POS   "osx_pos.vkm"
#define KBD_PET_SYM_UK  "osx_sym_uk.vkm"
#define KBD_PET_POS_UK  "osx_pos_uk.vkm"
#define KBD_PET_SYM_DE  "osx_sym_de.vkm"
#define KBD_PET_POS_DE  "osx_pos_de.vkm"
#define KBD_PET_SYM_GR  "osx_sym_gr.vkm"
#define KBD_PET_POS_GR  "osx_pos_gr.vkm"
#define KBD_PLUS4_SYM   "osx_sym.vkm"
#define KBD_PLUS4_POS   "osx_pos.vkm"
#define KBD_CBM2_SYM_UK "osx_sym_uk.vkm"
#define KBD_CBM2_POS_UK "osx_pos_uk.vkm"
#define KBD_CBM2_SYM_DE "osx_sym_de.vkm"
#define KBD_CBM2_POS_DE "osx_pos_de.vkm"
#define KBD_CBM2_SYM_GR "osx_sym_gr.vkm"
#define KBD_CBM2_POS_GR "osx_pos_gr.vkm"

#define KBD_INDEX_C64_DEFAULT   KBD_INDEX_C64_SYM
#define KBD_INDEX_C128_DEFAULT  KBD_INDEX_C128_SYM
#define KBD_INDEX_VIC20_DEFAULT KBD_INDEX_VIC20_SYM
#define KBD_INDEX_PET_DEFAULT   KBD_INDEX_PET_BUKP
#define KBD_INDEX_PLUS4_DEFAULT KBD_INDEX_PLUS4_SYM
#define KBD_INDEX_CBM2_DEFAULT  KBD_INDEX_CBM2_BUKP

#define KBD_PORT_PREFIX "osx"

#endif  /* _KBD_H */
