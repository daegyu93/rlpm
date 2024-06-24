#include "pch.h"
#include "log.h"

#ifndef __USE_CONSOLE__
#include "resource.h"
#include "rlpm_app.h"
#define LOG_LISTBOX_ID IDC_LIST_LOGGER
#define CONTEXT_LISTBOX_ID IDC_LIST_CONTEXT
#define DEVICE_LISTBOX_ID IDC_LIST_DEVICE_INFO
#endif


void PrintConsol(char *format, ...)
{
	char buf[512];

	size_t ret;
	va_list arg;
	va_start(arg, format);
	vsprintf_s(buf, format, arg);
	//strcat_s(buf, "\n");
	va_end(arg);

	printf("%s", buf);
}
void Printlog(char *format, ...)
{
	char buf[512];

	size_t ret;
	va_list arg;
	va_start(arg, format);
	vsprintf_s(buf, format, arg);
	strcat_s(buf, "\n");
	va_end(arg);

	CListBox* pBox = (CListBox*)theApp.m_pMainWnd->GetDlgItem(LOG_LISTBOX_ID);
	pBox->InsertString(-1, CA2W(buf));
	int cnt = pBox->GetCount();
	if (cnt > 0)
		pBox->SetCurSel(cnt - 1);
}

void printContextinfo(char *format, ...)
{
	char buf[512];
	//wchar_t wbuf[512];
	size_t ret;
	va_list arg;
	va_start(arg, format);
	vsprintf_s(buf, format, arg);
	strcat_s(buf, "\n");
	va_end(arg);
	//mbstowcs_s(&ret, wbuf, 512, buf, 512);
#ifdef __USE_CONSOLE__
	printf("Context : %s\n", buf);
#else
	CListBox* pBox = (CListBox*)theApp.m_pMainWnd->GetDlgItem(CONTEXT_LISTBOX_ID);
	pBox->InsertString(-1, CA2W(buf));
	int cnt = pBox->GetCount();
	if (cnt > 0)
		pBox->SetCurSel(cnt - 1);
#endif
}
void printDeviceinfo(char *format, ...)
{
	char buf[512];
	//wchar_t wbuf[512];
	size_t ret;
	va_list arg;
	va_start(arg, format);
	vsprintf_s(buf, format, arg);
	strcat_s(buf, "\n");
	va_end(arg);
	//mbstowcs_s(&ret, wbuf, 512, buf, 512);
#ifdef __USE_CONSOLE__
	printf("Device : %s\n", buf);
#else
	CListBox* pBox = (CListBox*)theApp.m_pMainWnd->GetDlgItem(DEVICE_LISTBOX_ID);
	pBox->InsertString(-1, CA2W(buf));
	int cnt = pBox->GetCount();
	if (cnt > 0)
		pBox->SetCurSel(cnt - 1);
#endif
}