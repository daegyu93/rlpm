#pragma once

class CRlpmReg {
public:
	unsigned int offset;
	char name[32];
	CRlpmReg(unsigned int _offset, const char _name[]);
};

extern CRlpmReg rlpm_regmaps[32];