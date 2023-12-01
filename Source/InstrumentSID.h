/*
** FamiTracker - NES/Famicom sound tracker
** Copyright (C) 2005-2014  Jonathan Liss
**
** 0CC-FamiTracker is (C) 2014-2016 HertzDevil
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


#include <vector>
#include <memory>

#include "SeqInstrument.h"		// // //

enum envparam_t {
	ENV_ATTACK,
	ENV_DECAY,
	ENV_SUSTAIN,
	ENV_RELEASE
};


enum pwmdirection_t {
	PWM_DISABLED,
	PWM_LOOP,
	PWM_PINGPONG,
	PWM_ONCE,
	PWM_SUSTAIN,
};


class CInstrumentSID : public CSeqInstrument {
public:
	CInstrumentSID();
	CInstrument* Clone() const;
	void	Setup();
	void	Store(CDocumentFile *pDocFile);
	bool	Load(CDocumentFile *pDocFile);
	void	SaveFile(CInstrumentFile *pFile);
	bool	LoadFile(CInstrumentFile *pFile, int iVersion);
	int		Compile(CChunk *pChunk, int Index);
	bool	CanRelease() const;

public:
	int  	GetEnvParam(int EnvParam) const;		// // //
	void 	SetEnvParam(int EnvParam, int Value);		// // //
	int  	GetPWMStart() const { return m_pPWMStart; };		// // //
	void 	SetPWMStart(int Value) { m_pPWMStart = Value; InstrumentChanged(); };		// // //
	int  	GetPWMEnd() const { return m_pPWMEnd; };		// // //
	void 	SetPWMEnd(int Value) { m_pPWMEnd = Value; InstrumentChanged(); };		// // //
	int  	GetPWMSpeed() const { return m_pPWMSpeed; };		// // //
	void 	SetPWMSpeed(int Value) { m_pPWMSpeed = Value; InstrumentChanged(); };		// // //
	int  	GetPWMMode() const { return m_pPWMMode; };		// // //
	void 	SetPWMMode(int Value) { m_pPWMMode = Value; InstrumentChanged(); };		// // //
	int  	GetFilterStart() const { return m_pFilterStart; };		// // //
	void 	SetFilterStart(int Value) { m_pFilterStart = Value; InstrumentChanged(); };		// // //
	int  	GetFilterEnd() const { return m_pFilterEnd; };		// // //
	void 	SetFilterEnd(int Value) { m_pFilterEnd = Value; InstrumentChanged(); };		// // //
	int  	GetFilterSpeed() const { return m_pFilterSpeed; };		// // //
	void 	SetFilterSpeed(int Value) { m_pFilterSpeed = Value; InstrumentChanged(); };		// // //
	int  	GetFilterMode() const { return m_pFilterMode; };		// // //
	void 	SetFilterMode(int Value) { m_pFilterMode = Value; InstrumentChanged(); };		// // //
	int  	GetFilterPass() const { return m_pFilterPass; };		// // //
	void 	SetFilterPass(int Value) { m_pFilterPass = Value; InstrumentChanged(); };		// // //

protected:
	virtual void	CloneFrom(const CInstrument *pInst);		// // //


public:
	static const int SEQUENCE_COUNT = 6;		// // //
	static LPCTSTR SEQUENCE_NAME[];
	LPCTSTR	GetSequenceName(int Index) const { return SEQUENCE_NAME[Index]; }		// // //
	
private:
	// Instrument data
	int m_pEnvelopeAD;
	int m_pEnvelopeSR;

	int m_pPWMStart;
	int m_pPWMEnd;
	int m_pPWMSpeed;
	int m_pPWMMode;

	int m_pFilterStart;
	int m_pFilterEnd;
	int m_pFilterSpeed;
	int m_pFilterMode;
	int m_pFilterPass;
	
};
