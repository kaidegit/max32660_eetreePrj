//
// Created by yekai on 2021/3/10.
//

#include "temperature.h"
#include "sht30.h"
#include "screen.h"

float temperature = 0;
extern enum ActName nowAct;

void GetTemperature() {
    float humidity = 0.0;
    uint8_t recv_dat[30];
    SHT30_Read_Dat(recv_dat);
    SHT30_Dat_To_Float(recv_dat, &temperature, &humidity);
}

void ShowTemperature(){
    GetTemperature();
    nowAct = Temperature;
}