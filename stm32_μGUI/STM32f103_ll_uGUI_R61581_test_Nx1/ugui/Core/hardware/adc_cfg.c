#include "adc_cfg.h"

void ADC_StartCalibration(ADC_TypeDef *ADCx)
{
  LL_ADC_Enable(ADCx);
  for (volatile uint8_t i = 128 ;i; i--)  {}
  LL_ADC_StartCalibration(ADCx);
  while (LL_ADC_IsCalibrationOnGoing(ADCx) != 0) {}
}

void ADC_Init(ADC_TypeDef *ADCx)
{
  LL_RCC_SetADCClockSource(LL_RCC_ADC_CLKSRC_PCLK2_DIV_6);
  LL_ADC_CommonInitTypeDef ADC_CommonInitStruct = {0};
  ADC_CommonInitStruct.Multimode = LL_ADC_MULTI_INDEPENDENT;
  LL_ADC_CommonInit(__LL_ADC_COMMON_INSTANCE(ADCx), &ADC_CommonInitStruct);
  /** Common config 
  */
  LL_ADC_InitTypeDef ADC_InitStruct = {0};
  ADC_InitStruct.DataAlignment = LL_ADC_DATA_ALIGN_RIGHT;
  ADC_InitStruct.SequencersScanMode = LL_ADC_SEQ_SCAN_DISABLE;
  LL_ADC_Init(ADCx, &ADC_InitStruct);

  /* Peripheral clock enable */
  LL_APB2_GRP1_EnableClock(LL_APB2_GRP1_PERIPH_ADC1);

  LL_ADC_REG_InitTypeDef ADC_REG_InitStruct = {0};

  ADC_REG_InitStruct.TriggerSource = LL_ADC_REG_TRIG_SOFTWARE;
  ADC_REG_InitStruct.SequencerLength = 1;
  ADC_REG_InitStruct.SequencerDiscont = LL_ADC_REG_SEQ_DISCONT_DISABLE;
  ADC_REG_InitStruct.ContinuousMode = LL_ADC_REG_CONV_SINGLE;
  ADC_REG_InitStruct.DMATransfer = LL_ADC_REG_DMA_TRANSFER_NONE;
  LL_ADC_REG_Init(ADCx, &ADC_REG_InitStruct);

  if (ADCx == ADC1)
  {
    // vrefint
    LL_ADC_REG_SetSequencerRanks(ADCx, LL_ADC_REG_RANK_1, LL_ADC_CHANNEL_VREFINT);
    LL_ADC_SetChannelSamplingTime(ADCx, LL_ADC_CHANNEL_VREFINT, LL_ADC_SAMPLINGTIME_1CYCLE_5);
    LL_ADC_SetCommonPathInternalCh(__LL_ADC_COMMON_INSTANCE(ADCx), LL_ADC_PATH_INTERNAL_VREFINT);
  }

  ADC_StartCalibration(ADCx);
}

uint16_t adc_get_value(ADC_TypeDef *ADCx, uint8_t channel)
{
  if (ADCx == ADC1)
  {
    LL_ADC_Enable(ADCx);
    LL_APB2_GRP1_EnableClock(LL_APB2_GRP1_PERIPH_ADC1);
  }
  if (ADCx == ADC2)
  {
    LL_ADC_Enable(ADCx);
    LL_APB2_GRP1_EnableClock(LL_APB2_GRP1_PERIPH_ADC2);
  }
  LL_APB2_GRP1_EnableClock(LL_APB2_GRP1_PERIPH_GPIOA);
  LL_APB2_GRP1_EnableClock(LL_APB2_GRP1_PERIPH_GPIOB);

  LL_GPIO_InitTypeDef GPIO_InitStruct = {0};

  if(channel == 0)
  {
    LL_ADC_REG_SetSequencerRanks(ADCx, LL_ADC_REG_RANK_1, LL_ADC_CHANNEL_0);
    LL_ADC_SetChannelSamplingTime(ADCx, LL_ADC_CHANNEL_0, LL_ADC_SAMPLINGTIME_7CYCLES_5);
    GPIO_InitStruct.Pin = LL_GPIO_PIN_0;
    GPIO_InitStruct.Mode = LL_GPIO_MODE_ANALOG;
    LL_GPIO_Init(GPIOA, &GPIO_InitStruct);
  }

  if(channel == 1)
  {
    LL_ADC_REG_SetSequencerRanks(ADCx, LL_ADC_REG_RANK_1, LL_ADC_CHANNEL_1);
    LL_ADC_SetChannelSamplingTime(ADCx, LL_ADC_CHANNEL_1, LL_ADC_SAMPLINGTIME_7CYCLES_5);
    GPIO_InitStruct.Pin = LL_GPIO_PIN_1;
    GPIO_InitStruct.Mode = LL_GPIO_MODE_ANALOG;
    LL_GPIO_Init(GPIOA, &GPIO_InitStruct);
  }

  if(channel == 2)
  {
    LL_ADC_REG_SetSequencerRanks(ADCx, LL_ADC_REG_RANK_1, LL_ADC_CHANNEL_2);
    LL_ADC_SetChannelSamplingTime(ADCx, LL_ADC_CHANNEL_2, LL_ADC_SAMPLINGTIME_7CYCLES_5);
    GPIO_InitStruct.Pin = LL_GPIO_PIN_2;
    GPIO_InitStruct.Mode = LL_GPIO_MODE_ANALOG;
    LL_GPIO_Init(GPIOA, &GPIO_InitStruct);
  }

  if(channel == 3)
  {
    LL_ADC_REG_SetSequencerRanks(ADCx, LL_ADC_REG_RANK_1, LL_ADC_CHANNEL_3);
    LL_ADC_SetChannelSamplingTime(ADCx, LL_ADC_CHANNEL_3, LL_ADC_SAMPLINGTIME_7CYCLES_5);
    GPIO_InitStruct.Pin = LL_GPIO_PIN_3;
    GPIO_InitStruct.Mode = LL_GPIO_MODE_ANALOG;
    LL_GPIO_Init(GPIOA, &GPIO_InitStruct);
  }

  if(channel == 4)
  {
    LL_ADC_REG_SetSequencerRanks(ADCx, LL_ADC_REG_RANK_1, LL_ADC_CHANNEL_4);
    LL_ADC_SetChannelSamplingTime(ADCx, LL_ADC_CHANNEL_4, LL_ADC_SAMPLINGTIME_7CYCLES_5);
    GPIO_InitStruct.Pin = LL_GPIO_PIN_4;
    GPIO_InitStruct.Mode = LL_GPIO_MODE_ANALOG;
    LL_GPIO_Init(GPIOA, &GPIO_InitStruct);
  }

  if(channel == 5)
  {
    LL_ADC_REG_SetSequencerRanks(ADCx, LL_ADC_REG_RANK_1, LL_ADC_CHANNEL_5);
    LL_ADC_SetChannelSamplingTime(ADCx, LL_ADC_CHANNEL_5, LL_ADC_SAMPLINGTIME_7CYCLES_5);
    GPIO_InitStruct.Pin = LL_GPIO_PIN_5;
    GPIO_InitStruct.Mode = LL_GPIO_MODE_ANALOG;
    LL_GPIO_Init(GPIOA, &GPIO_InitStruct);
  }

  if(channel == 6)
  {
    LL_ADC_REG_SetSequencerRanks(ADCx, LL_ADC_REG_RANK_1, LL_ADC_CHANNEL_6);
    LL_ADC_SetChannelSamplingTime(ADCx, LL_ADC_CHANNEL_6, LL_ADC_SAMPLINGTIME_7CYCLES_5);
    GPIO_InitStruct.Pin = LL_GPIO_PIN_6;
    GPIO_InitStruct.Mode = LL_GPIO_MODE_ANALOG;
    LL_GPIO_Init(GPIOA, &GPIO_InitStruct);
  }

  if(channel == 7)
  {
    LL_ADC_REG_SetSequencerRanks(ADCx, LL_ADC_REG_RANK_1, LL_ADC_CHANNEL_7);
    LL_ADC_SetChannelSamplingTime(ADCx, LL_ADC_CHANNEL_7, LL_ADC_SAMPLINGTIME_7CYCLES_5);
    GPIO_InitStruct.Pin = LL_GPIO_PIN_7;
    GPIO_InitStruct.Mode = LL_GPIO_MODE_ANALOG;
    LL_GPIO_Init(GPIOA, &GPIO_InitStruct);
  }

  if(channel == 8)
  {
    LL_ADC_REG_SetSequencerRanks(ADCx, LL_ADC_REG_RANK_1, LL_ADC_CHANNEL_8);
    LL_ADC_SetChannelSamplingTime(ADCx, LL_ADC_CHANNEL_8, LL_ADC_SAMPLINGTIME_7CYCLES_5);
    GPIO_InitStruct.Pin = LL_GPIO_PIN_0;
    GPIO_InitStruct.Mode = LL_GPIO_MODE_ANALOG;
    LL_GPIO_Init(GPIOB, &GPIO_InitStruct);
  }

  if(channel == 9)
  {
    LL_ADC_REG_SetSequencerRanks(ADCx, LL_ADC_REG_RANK_1, LL_ADC_CHANNEL_9);
    LL_ADC_SetChannelSamplingTime(ADCx, LL_ADC_CHANNEL_9, LL_ADC_SAMPLINGTIME_7CYCLES_5);
    GPIO_InitStruct.Pin = LL_GPIO_PIN_1;
    GPIO_InitStruct.Mode = LL_GPIO_MODE_ANALOG;
    LL_GPIO_Init(GPIOB, &GPIO_InitStruct);
  }

  if(channel == 16) // temperature sensor
  {
    LL_ADC_REG_SetSequencerRanks(ADCx, LL_ADC_REG_RANK_1, LL_ADC_CHANNEL_TEMPSENSOR);
    LL_ADC_SetChannelSamplingTime(ADCx, LL_ADC_CHANNEL_TEMPSENSOR, LL_ADC_SAMPLINGTIME_7CYCLES_5);
    LL_ADC_SetCommonPathInternalCh(__LL_ADC_COMMON_INSTANCE(ADCx), LL_ADC_PATH_INTERNAL_TEMPSENSOR);
  }

  if(channel == 17) // vrefint
  {
    LL_ADC_REG_SetSequencerRanks(ADCx, LL_ADC_REG_RANK_1, LL_ADC_CHANNEL_VREFINT);
    LL_ADC_SetChannelSamplingTime(ADCx, LL_ADC_CHANNEL_VREFINT, LL_ADC_SAMPLINGTIME_7CYCLES_5);
    LL_ADC_SetCommonPathInternalCh(__LL_ADC_COMMON_INSTANCE(ADCx), LL_ADC_PATH_INTERNAL_VREFINT);
  }

  uint32_t ADC_value = 0;

  LL_ADC_REG_StartConversionSWStart(ADCx);
  while (LL_ADC_IsActiveFlag_EOS(ADCx) == 0); // Wait until the conversion is completed

  ADC_value = LL_ADC_REG_ReadConversionData12(ADCx);
  LL_ADC_REG_StopConversionExtTrig(ADCx);

  LL_ADC_Disable(ADCx);

  return ADC_value;
}

extern float adc_val_0, adc_val_1, adc_val_2, adc_val_3, adc_val_4, adc_val_5, adc_val_6, adc_temp, adc_vref;

uint16_t read_adc(ADC_TypeDef *ADCx, uint8_t channel)
{
    uint32_t val = 0;

    #define ADCX_COUNT 2

    for (uint8_t i =0; i<ADCX_COUNT; i++)
    {
      val += adc_get_value(ADCx, channel);
    }

    val /= ADCX_COUNT;
    return (uint16_t)val * 3300.0 / 4096.0;
}

