#pragma once
#define MAX_DEVICE_COUNT 5

typedef enum 
{
	ERLPM_MODE_WAVEFORM = 0,
	ERLPM_MODE_CALCULATION
}ERLPM_MODE;

typedef enum
{
	ESYNC_MODE_NONE =0,
	ESYNC_MODE_LM,
	ESYNC_MODE_CALC,
	ESYNC_MODE_SW
}ESYNC_MODE;

typedef enum
{
	EDEVICE_WAVE,
	EDEVICE_CALC,
	EDEVICE_PARA
}EDEVICE;

typedef struct
{
	unsigned int major;
	unsigned int minor;
	char git_tag[8];
}TContextVersion;

struct iio_context;
class Ciio_dev;

class CRlpmApi
{
protected:
	iio_context *mCtx;

	TContextVersion mnContextVersion;
	char mstrContextDescription[64];

	Ciio_dev *mpWaveDev;
	Ciio_dev *mpCalcDev;
	Ciio_dev *mpParaDev;
	
public:

	CRlpmApi();
	~CRlpmApi();

	bool	Open();
	void	Close();
	bool	IsOpen();
	void	Init();

	void	Calc_CreateBuffer(unsigned int _nuBufferSize);
	void	Calc_DistroyBuffer();
	int		Calc_RefillBuffer();		//return gathering buffer size 
	void*	Calc_GetBufferData();		//return gathering buffer 

	void	Wave_CreateBuffer(unsigned int _nuBufferSize);
	void	Wave_DistroyBuffer();
	int		Wave_RefillBuffer();		//return gathering buffer size 
	void*	Wave_GetBufferData();		//return gathering buffer 

	bool	GetRegister(EDEVICE _eDev, unsigned long addr, unsigned int* value);
	bool	SetRegitser(EDEVICE _eDev, unsigned long addr, unsigned int value);

	bool	SetRlpmMode(ERLPM_MODE _eRlpmMode, int nUseChannel);
	bool	SetSyncMode(ESYNC_MODE _eSyncMode);
	void	SetFmaxThreshold(unsigned int _nMin_LSB, unsigned int _nMax_LSB);
	void	SetLatencyThreshold(unsigned int _nMin_ns, unsigned int _nMax_ns);
	void	SetFWHMDutyThreshold(unsigned int _nMin_ns, unsigned int _nMax_ns);

	void	SetGateEnable(bool _bEnable);
	void	SetADCOffset(short _nOffset);
	void	SetADCCaptureDelay(unsigned int _nDelay);
	
	void	GetGateEnable(bool *_bEnable);
	void	GetADCOffset(short *_nOffset);
	void	GetADCCaptureDelay(unsigned int *_nDelay);

	void	GetRlpmMode(ERLPM_MODE *_peRlpmMode, int *_pnUseChannel);
	void	GetSyncMode(ESYNC_MODE *_peSyncMode);
	void	GetFmaxThreshold(unsigned int *_pnMin_LSB, unsigned int *_pnMax_LSB);
	void	GetLatencyThreshold(unsigned int *_pnMin_ns, unsigned int *_pnMax_ns);
	void	GetFWHMDutyThreshold(unsigned int *_pnMin_ns, unsigned int *_pnMax_ns);

	void	ClearLmGateCount();
};

