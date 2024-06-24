#include "pch.h"
#include "rlpm_reg.h"

CRlpmReg::CRlpmReg(unsigned int _offset, const char _name[]) {
	offset = _offset;
	memcpy(name, _name, sizeof(name));
}

CRlpmReg rlpm_regmaps[32] = {
	CRlpmReg(0x0000, "VERSION"),
	CRlpmReg(0x0004, "PERIPHERAL_ID"),
	CRlpmReg(0x0008, "SCRATCH"),
	CRlpmReg(0x000c, "IDENTIFICATION"),
	CRlpmReg(0x0010, "SYNTHESIS_CONFIG"),
	CRlpmReg(0x0014, "MEMORY_SIZE_LSB"),
	CRlpmReg(0x0018, "MEMORY_SIZE_MSB"),
	CRlpmReg(0x001c, "WAVE_TRANSFER_LENGTH"),
	CRlpmReg(0x0020, "CALC_TRANSFER_LENGTH"),
	CRlpmReg(0x0024, "RLPM_CONFIG"),

	CRlpmReg(0x0080, "MEM_PHY_STATE"),
	CRlpmReg(0x0084, "RESETN_OFFLOAD"),
	CRlpmReg(0x0088, "CONTROL"),

	CRlpmReg(0x0100, "SYNC_TRIGGER"),
	CRlpmReg(0x0104, "SYNC_CONFIG"),

	CRlpmReg(0x0200, "FSM_DEBUG"),
	CRlpmReg(0x0204, "SAMPLE_COUNT_LSB"),
	CRlpmReg(0x0208, "SAMPLE_COUNT_MSB"),

	CRlpmReg(0x0240, "LM_COUNT"),
	CRlpmReg(0x0244, "GATE_COUNT"),
	CRlpmReg(0x0248, "FMAX_VALUE_LSB"),
	CRlpmReg(0x024c, "LATENCY_NS"),
	CRlpmReg(0x0250, "FWHM_COUNT"),
	CRlpmReg(0x0254, "FWHM_DUTY"),
	CRlpmReg(0x0258, "FWHM_2ND_NS"),
	CRlpmReg(0x025c, "FWHM_1ST_NS"),
	CRlpmReg(0x0280, "FMAX_VALUE_LSB_MIN"),

	CRlpmReg(0x0284, "FMAX_VALUE_LSB_MAX"),
	CRlpmReg(0x0288, "FMAX_LATENCY_LSB_MIN"),
	CRlpmReg(0x028c, "FMAX_LATENCY_LSB_MAX"),
	CRlpmReg(0x0290, "FMAX_DUTY_LSB_MIN"),
	CRlpmReg(0x0294, "FMAX_DUTY_LSB_MAX"),
};