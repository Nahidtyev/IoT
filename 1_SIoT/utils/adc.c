#include "adc.h"
#include <math.h>

void ADC_SetSHTime(unsigned char us);

void ADC_init() {
    ADC10CTL0 = ADC10ON | ADC10IE;
    ADC10CTL1 = ADC10SSEL_3;
    ADC10CTL0 |= 0x0002;
    adcStatus.channel = 0;
}

void ADC_SetChannel(unsigned char ch)
{
    if (adcStatus.channel == ch)
        return;

    if ((ch > 4 && ch < 10) || ch > 15)
        return;

    ADC10CTL0 &= 0xFFFD;

    if (ch == 10) {
        ADC10CTL0 &= 0x1FFF;
        ADC10CTL0 |= 0x2020;
        ADC10CTL1 &= 0x0FFF;
        ADC10CTL1 |= 0xA000;
        ADC_SetSHTime(30);
    }
    else {
        ADC10CTL0 &= 0x1FFF;
        ADC10CTL1 &= 0x0FFF;
        ADC10CTL1 |= (ch << 12) & 0xF000;
    }

    adcStatus.channel = ch;
    ADC10CTL0 |= 0x0002;
}

inline int ADC_getVal()
{
    adcStatus.DataReady = 0;
    ADC10CTL0 |= ADC10SC | ADC10IE;
    while (!adcStatus.DataReady)
        wait();
    return ADC10MEM;
}


void ADC_SetSHTime(unsigned char us)
{
    unsigned char i = 1;
    unsigned char st = 0;

    for (; i < 9; i++) {
        float T = 1.0 / (8.0 / (float)i);
        if (4.0 * T >= us) { st = 1; break; }
        if (8.0 * T >= us) { st = 2; break; }
        if (16.0 * T >= us) { st = 3; break; }
        if (64.0 * T >= us) { st = 4; break; }
    }

    if (st == 0) return;

    ADC10CTL0 &= 0xFFFD;
    ADC10CTL1 &= 0xFF1F;
    ADC10CTL1 |= (i - 1) * 0x20;
    ADC10CTL0 &= 0xE7FF;

    switch (st) {
        case 1: { ADC10CTL0 |= ADC10SHT_0; break; }
        case 2: { ADC10CTL0 |= ADC10SHT_1; break; }
        case 3: { ADC10CTL0 |= ADC10SHT_2; break; }
        case 4: { ADC10CTL0 |= ADC10SHT_3; break; }
    }
    ADC10CTL0 |= 0x0002;
}

void ADC_GetInternalTemp(float *temp)
{
    ADC_SetChannel(10);
    adcStatus.DataReady = 0;
    ADC10CTL0 |= ADC10SC;
    while (!adcStatus.DataReady)
        wait();
    int raw = ADC10MEM;

    *temp = ((float)raw) * 0.0014648;
    *temp = *temp - 0.986;
    *temp = *temp / 0.00355;
}


void ADC_GetR1(float *R1)
{

}

void ADC_GetR2(float *R2)
{

}


void convert_RtoTemp(float R, float* temp)
{

}
