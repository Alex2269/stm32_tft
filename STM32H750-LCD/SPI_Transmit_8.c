
/* --------------------------SPI_Transmit_8 bits----------------------------------------*/
static inline
void SPI_Transmit_8(SPI_HandleTypeDef *hspi, uint8_t *pData)
{
  SPI_1LINE_TX(hspi);  /* Configure communication direction : 1Line */
  MODIFY_REG(hspi->Instance->CR2, SPI_CR2_TSIZE, 1);  /* Set the number of data at current transfer */
  SET_BIT((hspi)->Instance->CR1 , SPI_CR1_SPE);  /* Enable SPI peripheral */
  SET_BIT(hspi->Instance->CR1, SPI_CR1_CSTART);    /* Master transfer start */

  *((volatile uint8_t *)&hspi->Instance->TXDR) = *((uint8_t *)pData); // SPI_Transmit

  // CloseTransfer(hspi);
  SET_BIT((hspi)->Instance->IFCR , SPI_IFCR_EOTC);
  SET_BIT((hspi)->Instance->IFCR , SPI_IFCR_TXTFC);
  CLEAR_BIT((hspi)->Instance->CR1 , SPI_CR1_SPE);
}
/* -------------------------------------------------------------------------------------*/
