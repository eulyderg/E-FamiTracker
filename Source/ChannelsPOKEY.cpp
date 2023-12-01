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

// MOS Technology POKEY

#include "stdafx.h"
#include "FamiTrackerTypes.h"		// // //
#include "APU/Types.h"		// // //
#include "Sequence.h"		// // //
#include "Instrument.h"		// // //
#include "ChannelHandler.h"
#include "ChannelsPOKEY.h"
#include "APU/APU.h"
#include "InstHandler.h"		// // //
#include "SeqInstHandler.h"		// // //
#include <map>

// Static member variables, for the shared stuff in POKEY
unsigned char			  CChannelHandlerPOKEY::s_iGlobalVolume = 15;
unsigned char			  CChannelHandlerPOKEY::s_iFilterResonance = 0;
unsigned int			  CChannelHandlerPOKEY::s_iFilterCutoff = 0;
unsigned char			  CChannelHandlerPOKEY::s_iFilterMode = 0;
unsigned char			  CChannelHandlerPOKEY::s_iFilterEnable = 0;

// Class functions


//void CChannelHandlerPOKEY::UpdateRegs()		// // //
//{
	// Done only once
	//if (s_iNoiseFreq != s_iNoisePrev)		// // //
	//	WriteReg(0x06, (s_iNoisePrev = s_iNoiseFreq) ^ 0xFF);
	//WriteReg(0x07, s_iModes);
//}

// Instance functions

CChannelHandlerPOKEY::CChannelHandlerPOKEY() : 
	CChannelHandler(0xFFFF, 0xF),
	m_bUpdate(false),
	m_iEnvAD(0),
	m_iEnvSR(0)
{
	m_iDefaultDuty = 4;		// // //
	SetLinearPitch(true);
}



const char CChannelHandlerPOKEY::MAX_DUTY = 0xF;

int CChannelHandlerPOKEY::getDutyMax() const {
	return MAX_DUTY;
}


bool CChannelHandlerPOKEY::HandleEffect(effect_t EffNum, unsigned char EffParam)
{
	switch (EffNum) {
	case EF_DUTY_CYCLE: {
		m_iDefaultDuty = m_iDutyPeriod = EffParam;
		break;
	}
	case EF_AY8930_PULSE_WIDTH: {
		m_iPulseWidth = EffParam * 16;
		break;
	}
	case EF_SID_FILTER_RESONANCE: {
		s_iFilterResonance = EffParam & 0xF;
		break;
	}
	case EF_SID_FILTER_CUTOFF_HI: {
		s_iFilterCutoff = (s_iFilterCutoff & 0xFF) | ((EffParam & 0xF) << 8);
		break;
	}
	case EF_SID_FILTER_CUTOFF_LO: {
		s_iFilterCutoff = (s_iFilterCutoff & 0xF00) | (EffParam & 0xFF);
		break;
	}
	case EF_SID_FILTER_MODE: {
		if (EffParam == 0)
			s_iFilterEnable &= ~(1U << (m_iChannelID - CHANID_POKEY_CH1));
		else {
			s_iFilterEnable |= (1U << (m_iChannelID - CHANID_POKEY_CH1));
			s_iFilterMode = EffParam & 0xF;
		}
		break;
	}
	default: return CChannelHandler::HandleEffect(EffNum, EffParam);
	}

	return true;
}

void CChannelHandlerPOKEY::HandleNote(int Note, int Octave)		// // //
{
	CChannelHandler::HandleNote(Note, Octave);

	/*
	Vxx is handled above: CChannelHandlerPOKEY::HandleEffect, case EF_DUTY_CYCLE
	m_iDefaultDuty is Vxx.
	m_iDutyPeriod is Vxx plus instrument bit-flags. But it's not fully
		initialized yet (instruments are handled after notes) which is bad.
	https://docs.google.com/document/d/e/2PACX-1vQ8osh6mm4c4Ay_gVMIJCH8eRB5gBE180Xyeda1T5U6owG7BbKM-yNKVB8azg27HUD9QZ9Vf88crplE/pub
  */
}


void CChannelHandlerPOKEY::HandleNoteData(stChanNote* pNoteData, int EffColumns)
{
	CChannelHandler::HandleNoteData(pNoteData, EffColumns);
	if (pNoteData->Note != 0 && pNoteData->Note != RELEASE && pNoteData->Note != HALT)
		m_iGateCounter = (pNoteData->Instrument != HOLD_INSTRUMENT) ? 1 : 3;

	if (pNoteData->Vol < MAX_VOLUME) {
		s_iGlobalVolume = pNoteData->Vol & 15;
	}
}


void CChannelHandlerPOKEY::HandleEmptyNote()
{

}

void CChannelHandlerPOKEY::HandleCut()
{
	CutNote();
	m_iDutyPeriod = 4;
	m_iNote = 0;
	m_iGateBit = 0;
	m_iEnvAD = 0;
	m_iEnvSR = 0;
}


void CChannelHandlerPOKEY::HandleRelease()
{
	if (!m_bRelease) {
		m_iGateBit = 0;
		ReleaseNote();		// // //
	}
}

bool CChannelHandlerPOKEY::CreateInstHandler(inst_type_t Type)
{
	switch (Type) {
	case INST_2A03: case INST_VRC6: case INST_N163: case INST_S5B: case INST_FDS: case INST_SID:
		switch (m_iInstTypeCurrent) {
		case INST_2A03: case INST_VRC6: case INST_N163: case INST_S5B: case INST_FDS: case INST_SID: break;
		default:
			m_pInstHandler.reset(new CSeqInstHandler(this, 0x0F, 0x01));
			return true;
		}
	}
	return false;
}

void CChannelHandlerPOKEY::WriteReg(int Reg, int Value)
{
	WriteRegister(0xD400+Reg, Value);
}

void CChannelHandlerPOKEY::ResetChannel()
{
	CChannelHandler::ResetChannel();

	m_iDefaultDuty = m_iDutyPeriod = 0xA;
}

int CChannelHandlerPOKEY::CalculateVolume() const		// // //
{
	return LimitVolume((m_iVolume >> VOL_COLUMN_SHIFT) - GetTremolo() + m_iInstVolume - 15);
}

int CChannelHandlerPOKEY::ConvertDuty(int Duty)		// // //
{
	return Duty;
}

void CChannelHandlerPOKEY::ClearRegisters()
{
//	WriteReg(0x18, 15);
}

CString CChannelHandlerPOKEY::GetSlideEffectString() const		// // //
{
	CString str = _T("");

	return str;
}

CString CChannelHandlerPOKEY::GetCustomEffectString() const		// // //
{
	CString str = _T("");

	return str;
}

void CChannelHandlerPOKEY::RefreshChannel()
{

	// Channel address offset
	unsigned int Offset = 7 * (m_iChannelID - CHANID_6581_CH1);

	// Calculate values
	int Period = CalculatePeriod();
	unsigned char Freq = (Period & 0xFF);

	unsigned char LoPW = (m_iPulseWidth & 0xFF);
	unsigned char HiPW = (m_iPulseWidth >> 8);

	WriteReg(0x05 + Offset, m_iEnvAD);


	unsigned char Waveform = (m_iDutyPeriod & 15) << 4;
	Waveform |= m_iGateBit | (m_iTestBit << 1);

	WriteReg(0x00 + Offset, LoFreq);

}

void CChannelHandlerPOKEY::SetADSR(unsigned char EnvAD, unsigned char EnvSR)
{
	m_iEnvAD = EnvAD;
	m_iEnvSR = EnvSR;
}

void CChannelHandlerPOKEY::SetPulseWidth(unsigned int PulseWidth)
{
	m_iPulseWidth = PulseWidth;
}

unsigned int CChannelHandlerPOKEY::GetPulseWidth() const
{
	return m_iPulseWidth;
}