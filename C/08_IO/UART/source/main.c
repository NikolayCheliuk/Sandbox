/**
 * UART
 *
 * 1. idle - High
 * 2. Start Bit - Low
 * 3. Data [5-9] bits
 * 4. Parity Bit
 * 5. Stop Bit - High
 */

#include <inttypes.h>
#include <stdbool.h>
#include <stddef.h>
#include <stdio.h>
#include <string.h>

#define GPIO_UART_RX 13
#define TIMER_ID 0

typedef enum
{
  GPIO_DIRECTION_IN = 0U,
  GPIO_DIRECTION_OUT,
  GPIO_DIRECTION_COUNT
} gpio_direction_t;

typedef enum
{
  GPIO_LEVEL_LOW = 0U,
  GPIO_LEVEL_HIGH = 1U,
  GPIO_LEVEL_COUNT
} gpio_level_t;

typedef enum
{
  IRQ_EDGE_FALLING = 0U,
  IRQ_EDGE_RISING,
  IRQ_EDGE_COUNT
} irq_edge_t;

void gpio_set_direction(uint32_t gpio_channel, gpio_direction_t gpio_direction);
gpio_level_t gpio_get_level(uint32_t gpio_channel);
void gpio_set_isr(uint32_t gpio_channel, irq_edge_t gpio_edge, void (*handler)(void));

void timer_start(uint32_t timer_id, uint32_t us);
void timer_set_isr(uint32_t timer_id, void (*handler)(void));

/**
 * Stubs
 */

void gpio_set_direction(uint32_t gpio_channel, gpio_direction_t gpio_direction)
{

}

gpio_level_t gpio_get_level(uint32_t gpio_channel)
{
  gpio_level_t level = 0U;

  return level;
}

void gpio_set_isr(uint32_t gpio_channel, irq_edge_t gpio_edge, void (*handler)(void))
{

}

void timer_start(uint32_t timer_id, uint32_t us)
{

}

void timer_set_isr(uint32_t timer_id, void (*handler)(void))
{

}


#define BIT(p) (1UL << (p))
#define BIT_MASK(n) (BIT(n) - 1U)

#define SET_BIT(x, p)  ((x) |=  BIT(p))
#define CLR_BIT(x, p)  ((x) &= ~BIT(p))
#define TGL_BIT(x, p)  ((x) ^=  BIT(p))
#define CHK_BIT(x, p) (((x) &   BIT(p)) != 0U)

#define SET_BITS(x, mask) ((x) |= (mask))
#define CLR_BITS(x, mask) ((x) &= ~(mask))

#define UART_USEC 1000000U
#define UART_PERIOD(UART_BAUDRATE) (UART_USEC / (UART_BAUDRATE))
#define UART_DATA_SIZE 8U

typedef enum
{
  UART_BAUDRATE_4800    = 4800U,
  UART_BAUDRATE_9600    = 9600U,
  UART_BAUDRATE_19200   = 19200U,
  UART_BAUDRATE_57600   = 57600U,
  UART_BAUDRATE_115200  = 115200U
} uart_baudrate_t;

typedef enum
{
  UART_STATE_IDLE = 0U,
  UART_STATE_WAIT_START_BIT,
  UART_STATE_WAIT_PAYLOAD,
  UART_STATE_WAIT_PARITY_BIT,
  UART_STATE_WAIT_STOP_BIT,
  UART_STATE_COUNT
} uart_state_t;



typedef struct
{
  uart_baudrate_t baudrate;
  uart_state_t state;
  uint16_t reg;
  uint8_t bit_index;
  uint8_t sum;
  uint8_t parity_bit;
  bool data_ready;
} uart_driver_t;

volatile uart_driver_t uart;

void gpio_isr(void)
{
  if (uart.state == UART_STATE_IDLE)
  {
    timer_start(TIMER_ID, UART_PERIOD(uart.baudrate) / 2U);

    uart.state = UART_STATE_WAIT_START_BIT;
  }
}

void timer_isr(void)
{
  gpio_level_t level = GPIO_LEVEL_LOW;

  switch (uart.state)
  {
    case UART_STATE_WAIT_START_BIT:
    {
      level = gpio_get_level(GPIO_UART_RX);

      // Start Bit is NOT confirmed. Miscall.
      if (level == GPIO_LEVEL_HIGH)
      {
        uart.state = UART_STATE_IDLE;
      }
      // Start Bit is confirmed
      else
      {
        timer_start(TIMER_ID, UART_PERIOD(uart.baudrate));

        uart.reg = 0U;
        uart.bit_index = 0U;
        uart.sum = 0U;
        uart.parity_bit = 0U;
        uart.data_ready = false;

        uart.state = UART_STATE_WAIT_PAYLOAD;
      }

      break;
    }

    case UART_STATE_WAIT_PAYLOAD:
    {
      level = gpio_get_level(GPIO_UART_RX);

      timer_start(TIMER_ID, UART_PERIOD(uart.baudrate));

      if (level == GPIO_LEVEL_HIGH)
      {
        SET_BIT(uart.reg, uart.bit_index); // Or UART_DATA_SIZE - bit_index
        uart.sum++;
      }
      else
      {
        CLR_BIT(uart.reg, uart.bit_index); // Or UART_DATA_SIZE - bit_index
      }

      uart.bit_index++;

      if (uart.bit_index == UART_DATA_SIZE)
      {
        uart.state = UART_STATE_WAIT_PARITY_BIT;
      }

      break;
    }

    case UART_STATE_WAIT_PARITY_BIT:
    {
      level = gpio_get_level(GPIO_UART_RX);

      timer_start(TIMER_ID, UART_PERIOD(uart.baudrate));

      uart.parity_bit = (level == GPIO_LEVEL_HIGH) ? 1U : 0U;

      uart.state = UART_STATE_WAIT_STOP_BIT;

      break;
    }

    case UART_STATE_WAIT_STOP_BIT:
    {
      level = gpio_get_level(GPIO_UART_RX);

      // Data Validation.
      if (level == GPIO_LEVEL_HIGH)
      {
        uart.data_ready = (uart.sum % 2U == uart.parity_bit) ? true : false;
      }

      uart.state = UART_STATE_IDLE;

      break;
    }

    default:
    {
      break;
    }
  }
}

void setup(void)
{
  gpio_set_direction(GPIO_UART_RX, GPIO_DIRECTION_IN);
  gpio_set_isr(GPIO_UART_RX, IRQ_EDGE_FALLING, gpio_isr);
  timer_set_isr(TIMER_ID, timer_isr);

  uart.baudrate = UART_BAUDRATE_9600;
  uart.state = UART_STATE_IDLE;
  uart.reg = 0U;
  uart.bit_index = 0U;
  uart.sum = 0U;
  uart.parity_bit = 0U;
  uart.data_ready = false;
}

int main(int argc, const char * argv[])
{
  return 0;
}
