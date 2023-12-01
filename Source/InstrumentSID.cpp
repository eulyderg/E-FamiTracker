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

#include <vector>
#include <memory>
#include "stdafx.h"
#include "Sequence.h"		// // //
#include "ModuleException.h"		// // //
#include "Instrument.h"
#include "SeqInstrument.h"		// // //
#include "InstrumentSID.h"		// // //
#include "Chunk.h"
#include "ChunkRenderText.h"		// // //
#include "DocumentFile.h"

// https://stackoverflow.com/a/14997413/2683842
inline int modulo(int i, int n) {
	return (i % n + n) % n;
}


LPCTSTR CInstrumentSID::SEQUENCE_NAME[] = {_T("Global Volume"), _T("Arpeggio"), _T("Pitch"), _T("Hi-pitch"), _T("Waveform"), _T("Pulse Width")};

CInstrumentSID::CInstrumentSID() : CSeqInstrument(INST_SID), // // //
m_pEnvelopeAD(0x0A),
m_pEnvelopeSR(0x00),
m_pPWMStart(0x800),
m_pPWMEnd(0x800),
m_pPWMSpeed(0x00),
m_pPWMMode(PWM_DISABLED),
m_pFilterStart(0x800),
m_pFilterEnd(0x800),
m_pFilterSpeed(0x00),
m_pFilterMode(PWM_DISABLED)
{
}

CInstrument * CInstrumentSID::Clone() const
{
	CInstrumentSID*inst = new CInstrumentSID();		// // //
	inst->CloneFrom(this);
	return inst;
}

void CInstrumentSID::CloneFrom(const CInstrument *pInst)
{
	CSeqInstrument::CloneFrom(pInst);
	
	if (auto pNew = dynamic_cast<const CInstrumentSID*>(pInst)) {
	// Copy parameters
		SetEnvParam(ENV_ATTACK, pNew->GetEnvParam(ENV_ATTACK));
		SetEnvParam(ENV_DECAY, pNew->GetEnvParam(ENV_DECAY));
		SetEnvParam(ENV_SUSTAIN, pNew->GetEnvParam(ENV_SUSTAIN));
		SetEnvParam(ENV_RELEASE, pNew->GetEnvParam(ENV_RELEASE));

		SetPWMStart(pNew->GetPWMStart());
		SetPWMEnd(pNew->GetPWMEnd());
		SetPWMSpeed(pNew->GetPWMSpeed());
		SetPWMMode(pNew->GetPWMMode());
		SetFilterStart(pNew->GetFilterStart());
		SetFilterEnd(pNew->GetFilterEnd());
		SetFilterSpeed(pNew->GetFilterSpeed());
		SetFilterMode(pNew->GetFilterMode());

	}
}

void CInstrumentSID::Setup()
{
}

void CInstrumentSID::Store(CDocumentFile *pDocFile)
{

	pDocFile->WriteBlockInt(4);
	pDocFile->WriteBlockChar(m_pEnvelopeAD);
	pDocFile->WriteBlockChar(m_pEnvelopeSR);
	pDocFile->WriteBlockInt(m_pPWMStart);
	pDocFile->WriteBlockInt(m_pPWMEnd);
	pDocFile->WriteBlockChar(m_pPWMSpeed);
	pDocFile->WriteBlockChar(m_pPWMMode);
	pDocFile->WriteBlockInt(m_pFilterStart);
	pDocFile->WriteBlockInt(m_pFilterEnd);
	pDocFile->WriteBlockChar(m_pFilterSpeed);
	pDocFile->WriteBlockChar(m_pFilterMode);

	// Store sequences
	CSeqInstrument::Store(pDocFile);		// // //
}

bool CInstrumentSID::Load(CDocumentFile *pDocFile)
{
	
	unsigned int instversion = pDocFile->GetBlockInt();
	if (instversion <= 255) {
		m_pEnvelopeAD = pDocFile->GetBlockChar();
		m_pEnvelopeSR = pDocFile->GetBlockChar();
		if (instversion >= 2) {
			m_pPWMStart = pDocFile->GetBlockInt();
			m_pPWMEnd = pDocFile->GetBlockInt();
			m_pPWMSpeed = pDocFile->GetBlockChar();
			m_pPWMMode = pDocFile->GetBlockChar();
		}
		if (instversion >= 3) {
			m_pFilterStart = pDocFile->GetBlockInt();
			m_pFilterEnd = pDocFile->GetBlockInt();
			m_pFilterSpeed = pDocFile->GetBlockChar();
			m_pFilterMode = pDocFile->GetBlockChar();
		}
		if (instversion >= 4) {
			CSeqInstrument::Load(pDocFile);
		}
	} else {
		pDocFile->RollbackPointer(4);
		unsigned int a = pDocFile->GetBlockInt();
		unsigned int b = pDocFile->GetBlockInt();
		pDocFile->RollbackPointer(8);
		if (a < 256 && (b & 0xFF) != 0x00) {
		}
		else {
		}
	}

//	}

	// Older files was 0-15, new is 0-31
	//if (pDocFile->GetBlockVersion() <= 3) DoubleVolume();

	return true;
}

void CInstrumentSID::SaveFile(CInstrumentFile *pFile)
{

	// Sequences
	CSeqInstrument::SaveFile(pFile);		// // //
}

bool CInstrumentSID::LoadFile(CInstrumentFile *pFile, int iVersion)
{
	// Sequences
	CSeqInstrument::LoadFile(pFile, iVersion);		// // //

	return true;
}

int CInstrumentSID::Compile(CChunk *pChunk, int Index)
{
	return 0;
}

bool CInstrumentSID::CanRelease() const
{
	const CSequence *pVol = GetSequence(SEQ_VOLUME);
	return pVol && pVol->GetItemCount() && pVol->GetReleasePoint() != -1;
}




int CInstrumentSID::GetEnvParam(int EnvParam) const
{
	switch (EnvParam) {
	case ENV_ATTACK:
		return (m_pEnvelopeAD & 0xF0) >> 4;
	case ENV_DECAY:
		return (m_pEnvelopeAD & 0x0F);
	case ENV_SUSTAIN:
		return (m_pEnvelopeSR & 0xF0) >> 4;
	case ENV_RELEASE:
		return (m_pEnvelopeSR & 0x0F);
	}
	return 0;
}

void CInstrumentSID::SetEnvParam(int EnvParam, int Value)
{
	switch (EnvParam) {
	case ENV_ATTACK:
		m_pEnvelopeAD = (m_pEnvelopeAD & 0x0F) | ((Value & 0x0F) << 4);
		break;
	case ENV_DECAY:
		m_pEnvelopeAD = (m_pEnvelopeAD & 0xF0) | (Value & 0x0F);
		break;
	case ENV_SUSTAIN:
		m_pEnvelopeSR = (m_pEnvelopeSR & 0x0F) | ((Value & 0x0F) << 4);
		break;
	case ENV_RELEASE:
		m_pEnvelopeSR = (m_pEnvelopeSR & 0xF0) | (Value & 0x0F);
		break;
	}
	InstrumentChanged();
}

