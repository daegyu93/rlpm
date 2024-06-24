#pragma once

#include <thread>
#include <mutex>

struct iio_context;
struct iio_device;
struct iio_channel;
struct iio_buffer;

#define MAX_BUFF_SIZE 65535
class Ciio_dev
{
public:
	Ciio_dev(iio_device* _pHDev);
	~Ciio_dev();

	iio_device* mpHDev;
	iio_channel *mCh0;
	iio_buffer	*mbuf;
	int mnChCnt;

private:
	int mnData[2][MAX_BUFF_SIZE];
public:
	void	create_buffer(unsigned int _nuBufferSize);
	void	distroy_buffer();
	int		refill_buffer();	//return gathering buffer size 
	void*	get_buffer();		//return gathering buffer 
	int		get_buffer_step();

	int		read_reg(unsigned long addr, unsigned int* value);
	int		write_reg(unsigned long addr, unsigned int value);
};

