#include "stm32f10x.h"                  // Device header

static void USART1_Init(void)
{
	//USART Clock
  SET_BIT(RCC->APB2ENR, RCC_APB2ENR_USART1EN);
	//Set GPIO_TX_RX
  SET_BIT(RCC->APB2ENR, RCC_APB2ENR_IOPAEN);
  MODIFY_REG(GPIOA->CRH, GPIO_CRH_MODE10 | GPIO_CRH_CNF10_1 | GPIO_CRH_CNF9_0,
                          GPIO_CRH_CNF10_0 | GPIO_CRH_MODE9 | GPIO_CRH_CNF9_1);
	////USART needs to be in disabled state, in order to be able to configure some bits in CRx registers
  if(READ_BIT(USART1->CR1, USART_CR1_UE) != (USART_CR1_UE))
  {
    MODIFY_REG(USART1->CR1,
           USART_CR1_M | USART_CR1_PCE | USART_CR1_PS ,USART_CR1_TE |USART_CR1_RE);
  }
	//Async Mode
  CLEAR_BIT(USART1->CR2, (USART_CR2_LINEN | USART_CR2_CLKEN));
  CLEAR_BIT(USART1->CR3, (USART_CR3_SCEN | USART_CR3_IREN | USART_CR3_HDSEL));
	//115200
  WRITE_REG(USART1->BRR, ((0x27)<<4)|0x01);
	//Enable
  SET_BIT(USART1->CR1, USART_CR1_UE);
}

static void USART_TX (uint8_t* dt, uint16_t sz)
{
  uint16_t ind = 0;
  while (ind<sz)
  {
    while (READ_BIT(USART1->SR, USART_SR_TXE) != (USART_SR_TXE)) {}
    USART1->DR = (uint16_t)dt[ind];
    ind++;
  }
}

static uint8_t USART_RX()
{
	while (READ_BIT(USART1->SR, USART_SR_RXNE) != (USART_SR_RXNE)) {}
  return (uint8_t)(USART1->DR & 0x00FF);	
}

int main()
{
	uint8_t ch;
	
	USART1_Init();
	
	USART_TX((uint8_t*)"Press any kay... \r\n",19);
	
	while(1)
	{
		ch = USART_RX();
		USART_TX(&ch,1);
  }
}
