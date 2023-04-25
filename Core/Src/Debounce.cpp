#include "Debounce.h"
#include "stm32f4xx_hal_rcc.h"

//----------------- методы класса Debounce

// конструктор
Debounce::Debounce(GPIO_TypeDef *GPIOx, uint16_t GPIO_Pin, uint32_t filterTime) {

	_GPIOx = GPIOx;
	_GPIO_Pin = GPIO_Pin;
	_filterTime = filterTime;
	_filterTimeCount = _filterTime /2;
	flagLow = 0;
	flagRising = 0;
	flagFalling = 0;

	// разрешение тактирования порта
	if( _GPIOx == GPIOA ) { __HAL_RCC_GPIOA_CLK_ENABLE();}
	else if( _GPIOx == GPIOB ) {__HAL_RCC_GPIOB_CLK_ENABLE();}
	else if( _GPIOx == GPIOC ) {__HAL_RCC_GPIOC_CLK_ENABLE();}
	else if( _GPIOx == GPIOD ) {__HAL_RCC_GPIOD_CLK_ENABLE();}
	else if( _GPIOx == GPIOE ) {__HAL_RCC_GPIOE_CLK_ENABLE();}

	// устанавливаем конфигурацию вывода на вход с подтягивающим резистором
	GPIO_InitTypeDef GPIO_InitStruct;
	GPIO_InitStruct.Pin = _GPIO_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
  GPIO_InitStruct.Pull = GPIO_PULLUP;
  HAL_GPIO_Init(_GPIOx, &GPIO_InitStruct);
};
// метод ожидания стабильного состояния сигнала
// flagLow; // признак СИГНАЛ В НИЗКОМ УРОВНЕ
// flagRising; // признак БЫЛ ПОЛОЖИТЕЛЬНЫЙ ФРОНТ
// flagFalling; // признак БЫЛ ОТРИЦАТЕЛЬНЫЙ ФРОНТ
void Debounce::scanStability(void) {

	// если состояние вывода не изменилось, то счетчик = 0
	if( (flagLow == 0) == ((_GPIOx->IDR & _GPIO_Pin) != 0)) _filterTimeCount = 0;

	// состояние вывода изменилось
	else {
		_filterTimeCount++;

		if( _filterTimeCount >= _filterTime ) {
			// состояние кнопки не мянялось в течение заданного времени
		    // состояние кнопки стало устойчивым
			flagLow = (~ flagLow) & 1; // инверсия признака состояния
			_filterTimeCount = 0;  // сброс счетчика времени

	        if ( flagLow != 0 ) flagFalling = 1; // признак фронта кнопки на нажатие
	        else flagRising = 1; // признак фронта кнопки на отжатие
		}
	}
}

// метод фильтрации сигнала по среднему значению
// flagLow; // признак СИГНАЛ В НИЗКОМ УРОВНЕ
// flagRising; // признак БЫЛ ПОЛОЖИТЕЛЬНЫЙ ФРОНТ
// flagFalling; // признак БЫЛ ОТРИЦАТЕЛЬНЫЙ ФРОНТ
void Debounce::scanAverage(void) {

	if( (flagLow == 0) == ((_GPIOx->IDR & _GPIO_Pin) != 0)) {
		// состояние сигнала осталось прежним
		if ( _filterTimeCount != 0 ) _filterTimeCount--; // счетчик времени - 1 с ограничением на 0
	}

	else {
		// состояние сигнала изменилось
		_filterTimeCount++;   // +1 к счетчику времени

		if ( _filterTimeCount >= _filterTime ) {
			// порог
			flagLow = (~ flagLow) & 1; // инверсия признака состояния
			_filterTimeCount = 0;  // сброс счетчика времени

	        if ( flagLow != 0 ) flagFalling = 1; // признак фронта кнопки на нажатие
	        else flagRising = 1; // признак фронта кнопки на отжатие
		}
	}
}

// метод установки времени фильтрации
void Debounce::setTime(uint32_t filterTime) {
	_filterTime = filterTime;
}

// чтение признака СИГНАЛ В НИЗКОМ УРОВНЕ
uint8_t Debounce::readFlagLow(void) {
	return(flagLow);
}

// чтение признака БЫЛ ПОЛОЖИТЕЛЬНЫЙ ФРОНТ
uint8_t Debounce::readFlagRising(void) {
	if(flagRising != 0) {
		flagRising = 0;
		return(1);
	}
	else return(0);
}

// чтение признака БЫЛ ОТРИЦАТЕЛЬНЫЙ ФРОНТ
uint8_t Debounce::readFlagFalling(void) {
	if(flagFalling != 0) {
		flagFalling = 0;
		return(1);
	}
	else return(0);
}
