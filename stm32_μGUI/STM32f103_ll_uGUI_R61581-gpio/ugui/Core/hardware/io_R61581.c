// /***
//  * 
//  * io_R61581.h
//  *
// ***/
// 
// #include "io_R61581."
// 
// #pragma GCC push_options
// #pragma GCC optimize("O3")
// 
// void
// tft_write_reg(uint8_t eightbit)
// {
//   static uint16_t control = 0b0000000000000111 | (1<<7) | (0<<6) | (0<<5) | (0<<4) | (1<<3);
//   GPIOB->ODR = control | (eightbit<<8);
//   LL_GPIO_SetOutputPin(LCD_WR_GPIO_Port, LCD_WR_Pin); // raising edge
// }
// 
// void
// tft_write_data(uint8_t eightbit)
// {
//   static uint16_t control = 0b0000000000000111 | (1<<7) | (0<<6) | (1<<5) | (0<<4) | (1<<3);
//   GPIOB->ODR = control | (eightbit<<8);
//   LL_GPIO_SetOutputPin(LCD_WR_GPIO_Port, LCD_WR_Pin); // raising edge
// }
// 
// #pragma GCC pop_options
// 
// /* [] END OF FILE */
