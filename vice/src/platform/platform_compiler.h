/*
 * platform_compiler.h - compiler discovery macros.
 *
 * Written by
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

/* Compilers supported:
 *
 * compiler       | support
 * ------------------------------------------------------
 * clang          | yes (untested)
 * comeau c++     | yes (untested)
 * compaq/dec     | yes, but wrong version format for now (untested)
 * dignus systems | yes (untested)
 * EKOPath        | yes (untested)
 * gcc            | yes
 * green hill     | yes (untested)
 * hp uc          | yes (untested)
 * intel cc       | yes (untested)
 * llvm           | yes
 * metrowerks     | yes (untested)
 * MIPSpro        | yes (untested)
 * MSVC           | yes
 * RealView C     | yes (untested)
 * SAS/C          | yes (untested)
 * Sun Studio     | yes (untested)
 * Tiny C         | yes (untested)
 * xLC            | yes (untested)
 */

#ifndef VICE_PLATFORM_COMPILER_H
#define VICE_PLATFORM_COMPILER_H

#undef XQUOTE
#undef QUOTE
#define QUOTE(x) XQUOTE(x)
#define XQUOTE(x) #x

/* clang discovery */
#if !defined(PLATFORM_COMPILER) && defined(__clang__)
#define PLATFORM_COMPILER "clang " QUOTE(__clang_major__) "." QUOTE(__clang_minor__) "." QUOTE(__clang_patchlevel__)
#endif

/* Intel compiler discovery */
#if !defined(PLATFORM_COMPILER) && defined(__INTEL_COMPILER)
#  define PLATFORM_COMPILER "Intel Compiler " QUOTE(__INTEL_COMPILER)
#  define PLATFORM_COMPILER_NAME "Intel Compiler"
#  define PLATFORM_COMPILER_VERSION __INTEL_COMPILER
#  define PLATFORM_COMPILER_MAJOR_MASK 100
#  define PLATFORM_COMPILER_MINOR_MASK 10
#  define PLATFORM_COMPILER_PATCHLEVEL_MASK 1
#endif

/* GCC discovery */
#if !defined(PLATFORM_COMPILER) && defined(__GNUC__)
#  if (__GNUC__>2)
#    define PLATFORM_COMPILER "GCC-" QUOTE(__GNUC__) "." QUOTE(__GNUC_MINOR__) "." QUOTE(__GNUC_PATCHLEVEL__)
#  else
#    ifdef __GNUC_MINOR__
#      define PLATFORM_COMPILER "GCC-" QUOTE(__GNUC__) "." QUOTE(__GNUC_MINOR__)
#    else
#      define PLATFORM_COMPILER "GCC-" QUOTE(__GNUC__) ".x"
#    endif
#  endif
#endif

/* ACK discovery */
#if !defined(PLATFORM_COMPILE) && defined(__ACK__)
#  define PLATFORM_COMPILER "ACK"
#endif

/* llvm discovery */
#if !defined(PLATFORM_COMPILER) && defined(__APPLE__) && defined(llvm)
#  define PLATFORM_COMPILER  "llvm"
#endif

/* xLC discovery */
#if !defined(PLATFORM_COMPILER) && defined( _AIX) && defined(__TOS_AIX__)
#  define PLATFORM_COMPILER "xLC"
#endif

/* HP UPC discovery */
#if !defined(PLATFORM_COMPILER) && defined(_hpux)
#  define PLATFORM_COMPILER "HP UPC"
#endif

/* Comeau compiler discovery */
#if !defined(PLATFORM_COMPILER) && defined(__COMO__)
#  define PLATFORM_COMPILER "Comeau c++ " QUOTE(__COMO_VERSION__)
#  define PLATFORM_COMPILER_NAME "Comeau c++"
#  define PLATFORM_COMPILER_VERSION __COMO_VERSION__
#  define PLATFORM_COMPILER_MAJOR_MASK 100
#  define PLATFORM_COMPILER_MINOR_MASK 1
#endif

/* compaq/dec compiler discovery */
#if !defined(PLATFORM_COMPILER) && defined(__DECC)
#  define PLATFORM_COMPILER "Compaq/DEC compiler " QUOTE(__DECC_VER)
#endif

/* Dignus Systems compiler discovery */
#if !defined(PLATFORM_COMPILER) && defined(__SYSC__)
#  define PLATFORM_COMPILER "Dignus Systems compiler " QUOTE(__SYSC_VER__)
#  define PLATFORM_COMPILER_NAME "Dignus Systems compiler"
#  define PLATFORM_COMPILER_VERSION __SYSC_VER__
#  define PLATFORM_COMPILER_MAJOR_MASK 10000
#  define PLATFORM_COMPILER_MINOR_MASK 100
#  define PLATFORM_COMPILER_PATCHLEVEL_MASK 1
#endif

/* EKOPath compiler discovery */
#if !defined(PLATFORM_COMPILER) && defined(__PATHCC__)
#  define PLATFORM_COMPILER "EKOPath compiler " QUOTE(__PATHCC__) "." QUOTE(__PATHCC_MINOR__) "." QUOTE(__PATHCC_PATCHLEVEL__)
#endif

/* Green Hill C/C++ compiler discovery */
#if !defined(PLATFORM_COMPILER) && defined(__ghs__)
#  define PLATFORM_COMPILER "Green Hill C/C++ " QUOTE(__GHS_VERSION_NUMBER__)
#  define PLATFORM_COMPILER_NAME "Green Hill C/C++"
#  define PLATFORM_COMPILER_VERSION __GHS_VERSION_NUMBER__
#  define PLATFORM_COMPILER_MAJOR_MASK 100
#  define PLATFORM_COMPILER_MINOR_MASK 10
#  define PLATFORM_COMPILER_PATCHLEVEL_MASK 1
#endif

/* MetroWerks compiler discovery */
#if !defined(PLATFORM_COMPILER) && defined(__MWERKS__)
#  if (__MWERKS__>1)
#    define PLATFORM_COMPILER "MetroWerks " QUOTE(__MWERKS__)
#    define PLATFORM_COMPILER_NAME "MetroWerks"
#    define PLATFORM_COMPILER_VERSION __MWERKS__
#    define PLATFORM_COMPILER_MAJOR_MASK 0x1000
#    define PLATFORM_COMPILER_MINOR_MASK 0x100
#  else
#    define PLATFORM_COMPILER "MetroWerks"
#  endif
#endif

/* MIPSpro compiler discovery */
#if !defined(PLATFORM_COMPILER) && (defined(__sgi) || defined(sgi))
#  if defined(_COMPILER_VERSION) || defined(_SGI_COMPILER_VERSION)
#    ifdef _COMPILER_VERSION
#      define PLATFORM_COMPILER "MIPSpro " QUOTE(_COMPILER_VERSION)
#      define PLATFORM_COMPILER_VERSION _COMPILER_VERSION
#    else
#      define PLATFORM_COMPILER "MIPSpro" QUOTE(_SGI_COMPILER_VERSION)
#      define PLATFORM_COMPILER_VERSION _SGI_COMPILER_VERSION
#    endif
#    define PLATFORM_COMPILER_NAME "MIPSpro"
#    define PLATFORM_COMPILER_MAJOR_MASK 100
#    define PLATFORM_COMPILER_MINOR_MASK 10
#    define PLATFORM_COMPILER_PATCHLEVEL_MASK 1
#  else
#    define PLATFORM_COMPILER "MIPSpro"
#  endif
#endif

/* RealView C compiler discovery */
#if !defined(PLATFORM_COMPILER) && defined(__CC_ARM)
#  define PLATFORM_COMPILER "RealView C " QUOTE(__ARMCC_VERSION)
#  define PLATFORM_COMPILER_NAME "RealView C"
#  define PLATFORM_COMPILER_VERSION __ARMCC_VERSION
#  define PLATFORM_COMPILER_MAJOR_MASK 100000
#  define PLATFORM_COMPILER_MINOR_MASK 10000
#  define PLATFORM_COMPILER_PATCHLEVEL_MASK 1000
#endif

/* SAS/C compiler discovery */
#if !defined(PLATFORM_COMPILER) && defined(__SASC)
#  ifdef __VERSION__
#    define PLATFORM_COMPILER "SAS/C " QUOTE(__VERSION__) "." QUOTE(__REVISION__)
#  else
#    define PLATFORM_COMPILER "SASC " QUOTE(__SASC__)
#    define PLATFORM_COMPILER_NAME "SAS/C"
#    define PLATFORM_COMPILER_VERSION ___SASC__
#    define PLATFORM_COMPILER_MAJOR_MASK 100
#    define PLATFORM_COMPILER_MINOR_MASK 1
#  endif
#endif

/* Sun Studio compiler discovery */
#if !defined(PLATFORM_COMPILER) && defined(__SUNPRO_C)
#  if (__SUNPRO_C==20496 || __SUNPRO_C==1296)
#    define PLATFORM_COMPILER "Sun WorkShop 6"
#  endif
#  if (__SUNPRO_C==20512 || __SUNPRO_C==1312)
#    define PLATFORM_COMPILER "Sun WorkShop 6 Update 1"
#  endif
#  if (__SUNPRO_C==20528 || __SUNPRO_C==1328)
#    define PLATFORM_COMPILER "Sun WorkShop 6 Update 2"
#  endif
#  if (__SUNPRO_C==20544 || __SUNPRO_C==1344)
#    define PLATFORM_COMPILER "Forte Developer 7"
#  endif
#  if (__SUNPRO_C==20560 || __SUNPRO_C==1360)
#    define PLATFORM_COMPILER "Sun Studio 8"
#  endif
#  if (__SUNPRO_C==20576 || __SUNPRO_C==1376)
#    define PLATFORM_COMPILER "Sun Studio 9"
#  endif
#  if (__SUNPRO_C==20592 || __SUNPRO_C==1392)
#    define PLATFORM_COMPILER "Sun Studio 10"
#  endif
#  if (__SUNPRO_C==20608 || __SUNPRO_C==1408)
#    define PLATFORM_COMPILER "Sun Studio 11"
#  endif
#  if (__SUNPRO_C==20624 || __SUNPRO_C==1424)
#    define PLATFORM_COMPILER "Sun Studio 12"
#  endif
#  if (__SUNPRO_C==20736)
#    define PLATFORM_COMPILER "Sun Studio 12 Update 1"
#  endif
#  if (__SUNPRO_C==20752)
#    define PLATFORM_COMPILER "Oracle Solaris Studio 12.2"
#  endif
#  if (__SUNPRO_C==20768)
#    define PLATFORM_COMPILER "Oracle Solaris Studio 12.3"
#  endif
#  if (__SUNPRO_C==20784)
#    define PLATFORM_COMPILER "Oracle Solaris Studio 12.4"
#  endif
#  ifndef PLATFORM_COMPILER
#    define PLATFORM_COMPILER "Sun Studio Compiler " QUOTE(__SUNPRO_C)
#  endif
#  define PLATFORM_COMPILER_NAME "Sun Studio Compiler"
#  define PLATFORM_COMPILER_VERSION __SUNPRO_C
#  define PLATFORM_COMPILER_MAJOR_MASK 0x100
#  define PLATFORM_COMPILER_MINOR_MASK 0x10
#  define PLATFORM_COMPILER_PATCHLEVEL_MASK 1
#endif

/* Tiny C compiler discovery */
#if !defined(PLATFORM_COMPILER) && defined(__TINYC__)
#  define PLATFORM_COMPILER "Tiny C"
#endif

/* OW compiler discovery */
#if !defined(PLATFORM_COMPILER) && defined(WATCOM_COMPILE)
#  define PLATFORM_COMPILER "OpenWatcom"
#endif

/* MSVC compiler discovery */
#if !defined(PLATFORM_COMPILER) && defined(_MSC_VER)
#  if (_MSC_VER == 1100)
#    define PLATFORM_COMPILER "msvc5/vs97"
#  endif
#  if (_MSC_VER == 1200)
#    define PLATFORM_COMPILER "msvc6/vs98"
#  endif
#  if (_MSC_VER == 1300)
#    define PLATFORM_COMPILER "msvc7.0/vs2002"
#  endif
#  if (_MSC_VER == 1310)
#    define PLATFORM_COMPILER "msvc7.1/vs2003"
#  endif
#  if (_MSC_VER == 1400)
#    define PLATFORM_COMPILER "msvc8/vs2005"
#  endif
#  if (_MSC_VER == 1500)
#    define PLATFORM_COMPILER "msvc9/vs2008"
#  endif
#  if (_MSC_VER == 1600)
#    define PLATFORM_COMPILER "msvc10/vs2010"
#  endif
#  if (_MSC_VER == 1700)
#    define PLATFORM_COMPILER "msvc11/vs2012"
#  endif
#  if (_MSC_VER == 1800)
#    define PLATFORM_COMPILER "msvc12/vs2013"
#  endif
#  if (_MSC_VER == 1900)
#    define PLATFORM_COMPILER "msvc14/vs2015"
#  endif
#  ifndef PLATFORM_COMPILER
#    define PLATFORM_COMPILER "msvc"
#  endif
#endif

/* SCO native compiler discovery */
#ifndef PLATFORM_COMPILER
#  if defined(SCO4UNIX_COMPILE) || defined(OPENSERVER5_COMPILE) || defined(OPENSERVER6_COMPILE) || defined(UNIXWARE_COMPILE)
#    define PLATFORM_COMPILER "SCO C"
# endif
#endif

/* SunOS/Solaris native compiler discovery */
#ifndef PLATFORM_COMPILER
#  if defined(sun) || defined(__sun)
#    if defined(__SVR4) || defined(__svr4__)
#      define PLATFORM_COMPILER "Solaris C"
#    else
#      define PLATFORM_COMPILER "SunOS C"
#    endif
#  endif
#endif


/* Ultrix native compiler discovery */
#ifndef PLATFORM_COMPILER
#  if defined(ultrix) || defined(__ultrix) || defined(__ultrix__)
#    define PLATFORM_COMPILER "DEC C"
#  endif
#endif


/* VBCC discovery */
#ifdef __VBCC__
#define PLATFORM_COMPILER "VBCC"
#endif

#endif
