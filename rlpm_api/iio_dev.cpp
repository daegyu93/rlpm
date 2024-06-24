#include "iio.h"
#include "iio_dev.h"
#include <mutex>

std::mutex g_mutex;
std::mutex g_mutex_gatheringSize;

Ciio_dev::Ciio_dev(iio_device* _pHDev)
{
	mpHDev = _pHDev;

	mnChCnt = iio_device_get_channels_count(mpHDev);
	mCh0 = iio_device_get_channel(mpHDev, 0);

	for (int i = 0; i < mnChCnt; i++)
	{
		iio_channel* pch = iio_device_get_channel(mpHDev, i);
		if (!iio_channel_is_output(pch))
			iio_channel_enable(pch);
	}
}

Ciio_dev::~Ciio_dev()
{
}

int Ciio_dev::read_reg(unsigned long addr, unsigned int* value)
{
	return iio_device_reg_read(mpHDev, addr, value);
}
int Ciio_dev::write_reg(unsigned long addr, unsigned int value)
{
	return iio_device_reg_write(mpHDev, addr, value);
}

void Ciio_dev::create_buffer(unsigned int _nuBufferSize)
{
	mbuf = iio_device_create_buffer(mpHDev, _nuBufferSize, false);
}

void Ciio_dev::distroy_buffer()
{
	iio_buffer_destroy(mbuf);
}

int Ciio_dev::refill_buffer()
{
	return iio_buffer_refill(mbuf);
}
void* Ciio_dev::get_buffer()
{
	return iio_buffer_first(mbuf, mCh0);
}

int Ciio_dev::get_buffer_step()
{
	return iio_buffer_step(mbuf);
}

