/*******************************/
/* CrystalCPUID for HariboteOS */
/*******************************/

#include <stdio.h>
#include <string.h>
#include "apilib.h"
#include "asm.h"

void HariMain(void)
{
	char *buf;
	int win, timer;

	char vendor_string[13];
	char name_string[49];
	char str[256];
	unsigned int eax, ebx, ecx, edx;
	unsigned int family, model, stepping;
	unsigned int family_ex, model_ex, stepping_ex;
	unsigned int max_cpuid, max_cpuid_ex;
	unsigned int tsc_start_high, tsc_start_low;
	unsigned int tsc_end_high, tsc_end_low;
	unsigned int clock;

	api_initmalloc();
	buf = api_malloc(400 * 120 * 2);
	win = api_openwin(buf, 400, 120, -1, "CrystalCPUID for HariboteOS");
	timer = api_alloctimer();
	api_inittimer(timer, 128);

	asm_cpuid(0x00000000, 0x0, &eax, &ebx, &ecx, &edx);

	max_cpuid = eax;
	memcpy(vendor_string    , &ebx, 4);
	memcpy(vendor_string + 4, &edx, 4);
	memcpy(vendor_string + 8, &ecx, 4);
	vendor_string[12] = '\0';
	api_putstrwin(win, 14, 27, 0, 12, vendor_string);

	asm_cpuid(0x00000001, 0x0, &eax, &ebx, &ecx, &edx);
	family   = (((eax >> 20) & 0xFF) << 4)
             + ((eax >> 8) & 0xF);
	model    = (((eax >> 16) & 0xF) << 4)
             + ((eax >> 4) & 0xF);
	stepping = (eax >> 0) & 0xF;
	sprintf(str, "Family = %Xh, Model = %Xh, Stepping = %Xh",
					family, model, stepping);
	api_putstrwin(win, 14, 27 + 16 * 1, 0, strlen(str), str);

	asm_cpuid(0x80000000, 0x0, &eax, &ebx, &ecx, &edx);
	max_cpuid_ex = eax;
	if(max_cpuid_ex >= 0x80000001){
		asm_cpuid(0x80000001, 0x0, &eax, &ebx, &ecx, &edx);
		family_ex   = (((eax >> 20) & 0xFF) << 4)
                    + ((eax >> 8) & 0xF);
		model_ex    = (((eax >> 16) & 0xF) << 4)
                    + ((eax >> 4) & 0xF);
		stepping_ex = (eax >> 0) & 0xF;
		sprintf(str, "FamilyEx = %Xh, ModelEx = %Xh, SteppingEx = %Xh",
				family_ex, model_ex, stepping_ex);
		api_putstrwin(win, 14, 27 + 16 * 2, 0, strlen(str), str);
	}
	if(max_cpuid_ex >= 0x80000004){
		asm_cpuid(0x80000002, 0x0, &eax, &ebx, &ecx, &edx);
		memcpy(name_string      , &eax, 4);
		memcpy(name_string  +  4, &ebx, 4);
		memcpy(name_string  +  8, &ecx, 4);
		memcpy(name_string  + 12, &edx, 4);
		asm_cpuid(0x80000003, 0x0, &eax, &ebx, &ecx, &edx);
		memcpy(name_string  + 16, &eax, 4);
		memcpy(name_string  + 20, &ebx, 4);
		memcpy(name_string  + 24, &ecx, 4);
		memcpy(name_string  + 28, &edx, 4);
		asm_cpuid(0x80000004, 0x0, &eax, &ebx, &ecx, &edx);
		memcpy(name_string  + 32, &eax, 4);
		memcpy(name_string  + 36, &ebx, 4);
		memcpy(name_string  + 40, &ecx, 4);
		memcpy(name_string  + 44, &edx, 4);
		name_string[49] = '\0';
		api_putstrwin(win, 14, 27 + 16 * 3, 0, 48, name_string);
	}

/* クロック周波数の測定精度は劣悪です。 */

	asm_rdtsc(&tsc_start_high, &tsc_start_low);
	for (;;) {
		api_settimer(timer, 100);	/* 1000ms */
		if (api_getkey(1) != 128) {
			break;
		}

		asm_rdtsc(&tsc_end_high, &tsc_end_low);
		clock = (tsc_end_low - tsc_start_low) / 1000 / 1000;
		sprintf(str, "Clock = %dMHz", clock);
		api_boxfilwin(win, 14, 27 + 16 * 4, 14 + 20 * 8, 41 + 16 * 4, 0xc638);
		api_putstrwin(win, 14, 27 + 16 * 4, 0, strlen(str), str);
		tsc_start_high = tsc_end_high;
		tsc_start_low = tsc_end_low;
	}
	api_end();
}
