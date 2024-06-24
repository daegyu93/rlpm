#include "rlpmApi.h"
#include "iio.h"
#include "iio_dev.h"
#include "iio_dev.h"

CRlpmApi::CRlpmApi()
{
	memset(this, 0, sizeof(*this));
}

CRlpmApi::~CRlpmApi() {
	if (mCtx)
	{
		iio_context_destroy(mCtx);
		mCtx = NULL;
	}

	if (mpWaveDev)
	{
		delete mpWaveDev;
		mpWaveDev = NULL;
	}
		
	if (mpCalcDev)
	{
		delete mpCalcDev;
		mpCalcDev = NULL;
	}
		
	if (mpParaDev)
	{
		delete mpParaDev;
		mpParaDev = NULL;
	}
}

bool CRlpmApi::Open()
{
	mCtx = iio_create_context_from_uri("ip:analog.local");
	if (!mCtx)
		return false;
	else 
	{
		int nRet;
		nRet = iio_context_get_version(mCtx, &mnContextVersion.major, &mnContextVersion.minor, mnContextVersion.git_tag);
		memcpy(mstrContextDescription, iio_context_get_description(mCtx),sizeof(mstrContextDescription));
		
		int nDevCount = iio_context_get_devices_count(mCtx);
		iio_device *pDev;

		char DevName[64];
		for (int i = 0; i < nDevCount-1; i++)
		{
			pDev = iio_context_get_device(mCtx, i);
			strcpy_s(DevName, iio_device_get_name(pDev));

			if (strcmp(DevName, "rlpm-calc") == 0)
				mpCalcDev = new Ciio_dev(pDev);
			else if(strcmp(DevName, "rlpm-wave") == 0)
				mpParaDev = new Ciio_dev(pDev);
			else if(strcmp(DevName, "axi-ad9680-hpc") == 0)
				mpWaveDev = new Ciio_dev(pDev);
		}
		if (mpCalcDev && mpParaDev && mpWaveDev)
		{
			Init();
			return true;
		}

		return false;
	}
}
void CRlpmApi::Close()
{
	if (mCtx)
	{
		iio_context_destroy(mCtx);
		mCtx = NULL;
	}
		
	if (mpWaveDev)
	{
		delete mpWaveDev;
		mpWaveDev = NULL;
	}
		
	if (mpCalcDev)
	{
		delete mpCalcDev;
		mpCalcDev = NULL;
	}
		
	if (mpParaDev)
	{
		delete mpParaDev;
		mpParaDev = NULL;
	}
		
}

bool CRlpmApi::IsOpen()
{
	if (mCtx && mpCalcDev && mpParaDev && mpWaveDev)
		return true;
	else
		return false;
}

void CRlpmApi::Init()
{
	mpParaDev->write_reg(0x1c, 64);
	mpParaDev->write_reg(0x20, 64);
	mpCalcDev->write_reg(0x1c, 64);
	mpCalcDev->write_reg(0x20, 64);
}


void CRlpmApi::Calc_CreateBuffer(unsigned int _nuBufferSize)
{
	mpCalcDev->create_buffer(_nuBufferSize);
}
void CRlpmApi::Calc_DistroyBuffer()
{
	mpCalcDev->distroy_buffer();
}
int CRlpmApi::Calc_RefillBuffer()
{
	return mpCalcDev->refill_buffer();
}
void* CRlpmApi::Calc_GetBufferData()
{
	return mpCalcDev->get_buffer();
}
void CRlpmApi::Wave_CreateBuffer(unsigned int _nuBufferSize)
{
	mpWaveDev->create_buffer(_nuBufferSize);
}
void CRlpmApi::Wave_DistroyBuffer()
{
	mpWaveDev->distroy_buffer();
}
int CRlpmApi::Wave_RefillBuffer()
{
	return mpWaveDev->refill_buffer();
}
void* CRlpmApi::Wave_GetBufferData()
{
	return mpWaveDev->get_buffer();
}

bool CRlpmApi::GetRegister(EDEVICE _eDev, unsigned long addr, unsigned int* value)
{
	Ciio_dev *pDev = NULL;
	switch (_eDev)
	{
	case EDEVICE_WAVE:
		pDev = mpWaveDev;
		break;
	case EDEVICE_CALC:
		pDev = mpCalcDev;
		break;
	case EDEVICE_PARA:
		pDev = mpParaDev;
		break;
	default:
		return false;
	}

	if (!pDev)
		return false;

	if (pDev->read_reg(addr, value) == 0)
		return true;
	else
		return false;
}

bool CRlpmApi::SetRegitser(EDEVICE _eDev, unsigned long addr, unsigned int value)
{
	Ciio_dev *pDev = NULL;
	switch (_eDev)
	{
	case EDEVICE_WAVE:
		pDev = mpWaveDev;
		break;
	case EDEVICE_CALC:
		pDev = mpCalcDev;
		break;
	case EDEVICE_PARA:
		pDev = mpParaDev;
		break;
	default:
		return false;
	}

	if (pDev)
		return false;

	if (pDev->write_reg(addr, value) == 0)
		return true;
	else
		return false;
}

bool CRlpmApi::SetRlpmMode(ERLPM_MODE _eRlpmMode, int nUseChannel)
{
	if (_eRlpmMode > 1 || nUseChannel > 1)
		return false;
	mpParaDev->write_reg(0x0024, _eRlpmMode + (nUseChannel << 1));
	return true;
}

bool CRlpmApi::SetSyncMode(ESYNC_MODE _eSyncMode)
{
	if (_eSyncMode > ESYNC_MODE_SW)
		return false;

	mpParaDev->write_reg(0x0104, _eSyncMode);
	mpCalcDev->write_reg(0x0104, _eSyncMode);
	return true;
}

void CRlpmApi::SetFmaxThreshold(unsigned int _min_LSB, unsigned int _max_LSB)
{
	mpParaDev->write_reg(0x0280, _min_LSB);
	mpParaDev->write_reg(0x0284, _max_LSB);
	mpCalcDev->write_reg(0x0280, _min_LSB);
	mpCalcDev->write_reg(0x0284, _max_LSB);
}

void CRlpmApi::SetLatencyThreshold(unsigned int _min_ns, unsigned int _max_ns)
{
	mpParaDev->write_reg(0x0288, _min_ns);
	mpParaDev->write_reg(0x028c, _max_ns);
	mpCalcDev->write_reg(0x0288, _min_ns);
	mpCalcDev->write_reg(0x028c, _max_ns);
}

void CRlpmApi::SetFWHMDutyThreshold(unsigned int _min_ns, unsigned int _max_ns)
{
	mpParaDev->write_reg(0x0290, _min_ns);
	mpParaDev->write_reg(0x0294, _max_ns);
	mpCalcDev->write_reg(0x0290, _min_ns);
	mpCalcDev->write_reg(0x0294, _max_ns);
}

void CRlpmApi::SetGateEnable(bool _bEnable)
{
	mpParaDev->write_reg(0x0028, _bEnable);
	mpCalcDev->write_reg(0x0028, _bEnable);
}
void CRlpmApi::SetADCOffset(short _nOffset)
{
	mpParaDev->write_reg(0x002c, _nOffset);
	mpCalcDev->write_reg(0x002c, _nOffset);
}
void CRlpmApi::SetADCCaptureDelay(unsigned int _nDelay)
{
	mpParaDev->write_reg(0x0030, _nDelay);
	mpCalcDev->write_reg(0x0030, _nDelay);
}

void CRlpmApi::GetGateEnable(bool *_bEnable)
{
	unsigned int nValue;
	mpParaDev->read_reg(0x0028, &nValue);
	*_bEnable = (bool)(nValue & 0x01);
}
void CRlpmApi::GetADCOffset(short *_nOffset)
{
	unsigned int nValue;
	mpParaDev->read_reg(0x002c, &nValue);
	*_nOffset = (short)(nValue & 0xFFFF);
}
void CRlpmApi::GetADCCaptureDelay(unsigned int *_nDelay)
{
	mpParaDev->read_reg(0x0030, _nDelay);
}

void CRlpmApi::GetRlpmMode(ERLPM_MODE *_peRlpmMode, int *_pnUseChannel)
{
	unsigned int nValue;
	
	mpParaDev->read_reg(0x0024, &nValue);
	*_peRlpmMode = (ERLPM_MODE)(nValue & 0x1);
	*_pnUseChannel = (nValue >> 1) & 0x1;
}

void CRlpmApi::GetSyncMode(ESYNC_MODE *_peSyncMode)
{
	unsigned int nValue;
	mpParaDev->read_reg(0x0104, &nValue);
	*_peSyncMode = (ESYNC_MODE)(nValue & 0x3);
}

void CRlpmApi::GetFmaxThreshold(unsigned int *_pnMin_LSB, unsigned int *_pnMax_LSB)
{
	mpParaDev->read_reg(0x0280, _pnMin_LSB);
	mpParaDev->read_reg(0x0284, _pnMax_LSB);
}

void CRlpmApi::GetLatencyThreshold(unsigned int *_pnMin_ns, unsigned int *_pnMax_ns)
{
	mpParaDev->read_reg(0x0288, _pnMin_ns);
	mpParaDev->read_reg(0x028c, _pnMax_ns);
}

void CRlpmApi::GetFWHMDutyThreshold(unsigned int *_pnMin_ns, unsigned int *_pnMax_ns)
{
	mpParaDev->read_reg(0x0290, _pnMin_ns);
	mpParaDev->read_reg(0x0294, _pnMax_ns);
}

void CRlpmApi::ClearLmGateCount()
{
	mpParaDev->write_reg(0x008c, 1);
	mpCalcDev->write_reg(0x008c, 1);
}