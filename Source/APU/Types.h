/*
** FamiTracker - NES/Famicom sound tracker
** Copyright (C) 2005-2014  Jonathan Liss
**
** 0CC-FamiTracker is (C) 2014-2015 HertzDevil
**
** This program is free software; you can redistribute it and/or modify
** it under the terms of the GNU General Public License as published by
** the Free Software Foundation; either version 2 of the License, or
** (at your option) any later version.
**
** This program is distributed in the hope that it will be useful, 
** but WITHOUT ANY WARRANTY; without even the implied warranty of
** MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU 
** Library General Public License for more details.  To obtain a 
** copy of the GNU Library General Public License, write to the Free 
** Software Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA.
**
** Any permitted reproduction of these routines, in whole or in part,
** must bear this legend.
*/

#pragma once

#include <cstdint>

const int SNDCHIP_NONE = 0;
const int SNDCHIP_VRC6 = 1;				// Konami VRCVI
const int SNDCHIP_VRC7 = 2;				// Konami VRCVII
const int SNDCHIP_FDS  = 4;				// Famicom Disk Sound
const int SNDCHIP_MMC5 = 8;				// Nintendo MMC5
const int SNDCHIP_N163 = 16;			// Namco N-106
const int SNDCHIP_S5B  = 32;			// Sunsoft 5B
const int SNDCHIP_AY8930  = 64;		// Microchip AY8930
const int SNDCHIP_SAA1099 = 128;	// Philips SAA1099
const int SNDCHIP_5E01 = 256;			// Eulous 5E01
const int SNDCHIP_6581 = 512;			// MOS Technology 6581
const int SNDCHIP_8580 = 1024;		// MOS Technology 8580
const int SNDCHIP_POKEY   = 2048;	// Atari POKEY

enum chan_id_t {
	CHANID_SQUARE1,
	CHANID_SQUARE2,
	CHANID_TRIANGLE,
	CHANID_NOISE,
	CHANID_DPCM,

	CHANID_VRC6_PULSE1,
	CHANID_VRC6_PULSE2,
	CHANID_VRC6_SAWTOOTH,

	CHANID_MMC5_SQUARE1,
	CHANID_MMC5_SQUARE2,
	CHANID_MMC5_VOICE,

	CHANID_N163_CH1,		// // //
	CHANID_N163_CH2,
	CHANID_N163_CH3,
	CHANID_N163_CH4,
	CHANID_N163_CH5,
	CHANID_N163_CH6,
	CHANID_N163_CH7,
	CHANID_N163_CH8,

	CHANID_FDS,

	CHANID_VRC7_CH1,
	CHANID_VRC7_CH2,
	CHANID_VRC7_CH3,
	CHANID_VRC7_CH4,
	CHANID_VRC7_CH5,
	CHANID_VRC7_CH6,

	CHANID_S5B_CH1,
	CHANID_S5B_CH2,
	CHANID_S5B_CH3,

	CHANID_AY8930_CH1,
	CHANID_AY8930_CH2,
	CHANID_AY8930_CH3,

	CHANID_SAA1099_CH1,
	CHANID_SAA1099_CH2,
	CHANID_SAA1099_CH3,
	CHANID_SAA1099_CH4,
	CHANID_SAA1099_CH5,
	CHANID_SAA1099_CH6,

	CHANID_5E01_SQUARE1,
	CHANID_5E01_SQUARE2,
	CHANID_5E01_WAVEFORM,
	CHANID_5E01_NOISE,
	CHANID_5E01_DPCM,

	CHANID_6581_CH1,
	CHANID_6581_CH2,
	CHANID_6581_CH3,

	CHANNELS,		/* Total number of channels */

	CHANID_8580_CH1,
	CHANID_8580_CH2,
	CHANID_8580_CH3,

	CHANID_POKEY_CH1,
	CHANID_POKEY_CH2,
	CHANID_POKEY_CH3,
	CHANID_POKEY_CH4,

};

enum apu_machine_t {
	MACHINE_NTSC, 
	MACHINE_PAL
};
