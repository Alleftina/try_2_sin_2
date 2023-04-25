/*
 * Debounce.h - библиотека обработки дискретных сигналов STM32.
 *
 * Может быть использована для устранения дребезга контактов, цифровой фильтрации сигналов от помех.
 *
 * Работает в фоновом режиме.
 *
 * В параллельном процессе регулярно должен вызываться один из методов обработки:
 *
 * void scanStability(void); // метод ожидания стабильного состояния сигнала
 * void scanAverage(void); // метод фильтрации сигнала по среднему значению
 *
 * В результате формируются признаки состояния сигнала:
 *
 * uint8_t flagLow; // признак СИГНАЛ В НИЗКОМ УРОВНЕ
 * uint8_t flagRising; // признак БЫЛ ПОЛОЖИТЕЛЬНЫЙ ФРОНТ
 * uint8_t flagFalling; // признак БЫЛ ОТРИЦАТЕЛЬНЫЙ ФРОНТ
 *
 * Признаки могут быть прочитаны функциями:
 *
 * uint8_t readFlagLow(void); // чтение признака СИГНАЛ В НИЗКОМ УРОВНЕ
 * uint8_t readFlagRising(void); // чтение признака БЫЛ ПОЛОЖИТЕЛЬНЫЙ ФРОНТ
 * uint8_t readFlagFalling(void); // чтение признака БЫЛ ОТРИЦАТЕЛЬНЫЙ ФРОНТ
 *
 * Пример создания объекта:
 * Debounce button(GPIOC, 1 << 11, 10); // экземпляр класса Debounce
 *
 * Подробно описана http://mypractic.ru/uroki-stm32 в уроках 12, 13, 14
 *
 * Разработана Калининым Эдуардом.
 *
 * http://mypractic.ru
 */

/* Проверка, что библиотека не подключена */
#ifndef DEBOUNCE_H
#define DEBOUNCE_H

#include "stm32f4xx.h"

class Debounce {

	public:
		Debounce(GPIO_TypeDef *GPIOx, uint16_t GPIO_Pin, uint32_t filterTime); // конструктор
		void scanStability(void); // метод ожидания стабильного состояния сигнала
		void scanAverage(void); // метод фильтрации сигнала по среднему значению
		void setTime(uint32_t filterTime); // метод установки времени фильтрации
		uint8_t readFlagLow(void); // чтение признака СИГНАЛ В НИЗКОМ УРОВНЕ
		uint8_t readFlagRising(void); // чтение признака БЫЛ ПОЛОЖИТЕЛЬНЫЙ ФРОНТ
		uint8_t readFlagFalling(void); // чтение признака БЫЛ ОТРИЦАТЕЛЬНЫЙ ФРОНТ

		uint8_t flagLow; // признак СИГНАЛ В НИЗКОМ УРОВНЕ
		uint8_t flagRising; // признак БЫЛ ПОЛОЖИТЕЛЬНЫЙ ФРОНТ
		uint8_t flagFalling; // признак БЫЛ ОТРИЦАТЕЛЬНЫЙ ФРОНТ

	private:
		GPIO_TypeDef *_GPIOx; // порт
		uint16_t _GPIO_Pin; // маска вывода
		uint32_t _filterTime; // время фильтрации
		uint32_t _filterTimeCount; // счетчик времени фильтрации
};

#endif /* DEBOUNCE_H */
