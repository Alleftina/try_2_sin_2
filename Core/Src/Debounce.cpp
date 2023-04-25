#include "Debounce.h"
#include "stm32f4xx_hal_rcc.h"

//----------------- ������ ������ Debounce

// �����������
Debounce::Debounce(GPIO_TypeDef *GPIOx, uint16_t GPIO_Pin, uint32_t filterTime) {

	_GPIOx = GPIOx;
	_GPIO_Pin = GPIO_Pin;
	_filterTime = filterTime;
	_filterTimeCount = _filterTime /2;
	flagLow = 0;
	flagRising = 0;
	flagFalling = 0;

	// ���������� ������������ �����
	if( _GPIOx == GPIOA ) { __HAL_RCC_GPIOA_CLK_ENABLE();}
	else if( _GPIOx == GPIOB ) {__HAL_RCC_GPIOB_CLK_ENABLE();}
	else if( _GPIOx == GPIOC ) {__HAL_RCC_GPIOC_CLK_ENABLE();}
	else if( _GPIOx == GPIOD ) {__HAL_RCC_GPIOD_CLK_ENABLE();}
	else if( _GPIOx == GPIOE ) {__HAL_RCC_GPIOE_CLK_ENABLE();}

	// ������������� ������������ ������ �� ���� � ������������� ����������
	GPIO_InitTypeDef GPIO_InitStruct;
	GPIO_InitStruct.Pin = _GPIO_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
  GPIO_InitStruct.Pull = GPIO_PULLUP;
  HAL_GPIO_Init(_GPIOx, &GPIO_InitStruct);
};
// ����� �������� ����������� ��������� �������
// flagLow; // ������� ������ � ������ ������
// flagRising; // ������� ��� ������������� �����
// flagFalling; // ������� ��� ������������� �����
void Debounce::scanStability(void) {

	// ���� ��������� ������ �� ����������, �� ������� = 0
	if( (flagLow == 0) == ((_GPIOx->IDR & _GPIO_Pin) != 0)) _filterTimeCount = 0;

	// ��������� ������ ����������
	else {
		_filterTimeCount++;

		if( _filterTimeCount >= _filterTime ) {
			// ��������� ������ �� �������� � ������� ��������� �������
		    // ��������� ������ ����� ����������
			flagLow = (~ flagLow) & 1; // �������� �������� ���������
			_filterTimeCount = 0;  // ����� �������� �������

	        if ( flagLow != 0 ) flagFalling = 1; // ������� ������ ������ �� �������
	        else flagRising = 1; // ������� ������ ������ �� �������
		}
	}
}

// ����� ���������� ������� �� �������� ��������
// flagLow; // ������� ������ � ������ ������
// flagRising; // ������� ��� ������������� �����
// flagFalling; // ������� ��� ������������� �����
void Debounce::scanAverage(void) {

	if( (flagLow == 0) == ((_GPIOx->IDR & _GPIO_Pin) != 0)) {
		// ��������� ������� �������� �������
		if ( _filterTimeCount != 0 ) _filterTimeCount--; // ������� ������� - 1 � ������������ �� 0
	}

	else {
		// ��������� ������� ����������
		_filterTimeCount++;   // +1 � �������� �������

		if ( _filterTimeCount >= _filterTime ) {
			// �����
			flagLow = (~ flagLow) & 1; // �������� �������� ���������
			_filterTimeCount = 0;  // ����� �������� �������

	        if ( flagLow != 0 ) flagFalling = 1; // ������� ������ ������ �� �������
	        else flagRising = 1; // ������� ������ ������ �� �������
		}
	}
}

// ����� ��������� ������� ����������
void Debounce::setTime(uint32_t filterTime) {
	_filterTime = filterTime;
}

// ������ �������� ������ � ������ ������
uint8_t Debounce::readFlagLow(void) {
	return(flagLow);
}

// ������ �������� ��� ������������� �����
uint8_t Debounce::readFlagRising(void) {
	if(flagRising != 0) {
		flagRising = 0;
		return(1);
	}
	else return(0);
}

// ������ �������� ��� ������������� �����
uint8_t Debounce::readFlagFalling(void) {
	if(flagFalling != 0) {
		flagFalling = 0;
		return(1);
	}
	else return(0);
}
