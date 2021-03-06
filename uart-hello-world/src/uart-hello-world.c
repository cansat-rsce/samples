/*
 * Базовый пример использования UART
 * Программа - UART hello-world. Отправляет в UART строку hello world каждые пол секунлы
 * UART запускается с настройками baudRate: 9600, stopBits: 1, parity: none, charSize: 8bit
 * МК должен быть запущен на 16 мГц
 */

#include <avr/io.h>
#include <util/delay.h>


void uartWrite(const char * message) {
	// идем в цикле, пока строка сообщения не закончится нулевым байтом
	while (*message) {
		while (!(UCSR0A & (1 << UDRE0))) // ждем, пока предыущий байт не покинет буфер (на случай, если он там по-какой-то причине задержался)
		{}

		UDR0 = *message; // отправляем текущий символ в модуль
		message++; // переходим к следующему символу сообщения
	}
}

int main() {
	// Инициализация UART baud 9600, stopBits: 1, parity: None
	// ==========================================================
	UCSR0B = (1 << TXEN0) | (1 << RXEN0);  // включаем TX и RX

	UCSR0C = (1 << UCSZ00) | (1 << UCSZ01) // Размер символа - 8 бит
		| (0 << UPM00) | (0 << UPM01)      // Бит четности отключен
		| (0 << USBS0) // 1 стоп бит
	;

	// baud на 9600 по таблице на частоте в 16мгц
	UBRR0H = 103 / 0xFF;
	UBRR0L = 103 % 0xFF;

	// Работаем в режиме hello-world
	// ==========================================================
	while(1) {
		uartWrite("hello world!\r\n"); // \r - символ возврата каретки в начало строки, \n - переход на новую строку.
		_delay_ms(500);
	}

	return 0;
}
