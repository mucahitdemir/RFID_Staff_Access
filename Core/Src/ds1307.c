#include "main.h"
#define DS1307_ADDRESS 0XD0


/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/
I2C_HandleTypeDef hi2c1;
RTC_HandleTypeDef hrtc;



// Convert normal decimal numbers to binary coded decimal
uint8_t decToBcd(int val)
{
  return (uint8_t)( (val/10*16) + (val%10) );
}
// Convert binary coded decimal to normal decimal numbers
int bcdToDec(uint8_t val)
{
  return (int)( (val/16*10) + (val%16) );
}



typedef struct {
	uint8_t seconds;
	uint8_t minutes;
	uint8_t hour;
	uint8_t dayofweek;
	uint8_t dayofmonth;
	uint8_t month;
	uint8_t year;
} TIME;

TIME time;

// function to set time

void Set_Time (uint8_t sec, uint8_t min, uint8_t hour, uint8_t dow, uint8_t dom, uint8_t month, uint8_t year)
{
	uint8_t set_time[7];
	set_time[0] = decToBcd(sec);
	set_time[1] = decToBcd(min);
	set_time[2] = decToBcd(hour);
	set_time[3] = decToBcd(dow);
	set_time[4] = decToBcd(dom);
	set_time[5] = decToBcd(month);
	set_time[6] = decToBcd(year);

	HAL_I2C_Mem_Write(&hi2c1, DS1307_ADDRESS, 0x00, 1, set_time, 7, 1000);
}

void Get_Time (void)
{
	uint8_t get_time[7];
	HAL_I2C_Mem_Read(&hi2c1, DS1307_ADDRESS, 0x00, 1, get_time, 7, 1000);
	time.seconds = bcdToDec(get_time[0]);
	time.minutes = bcdToDec(get_time[1]);
	time.hour = bcdToDec(get_time[2]);
	time.dayofweek = bcdToDec(get_time[3]);
	time.dayofmonth = bcdToDec(get_time[4]);
	time.month = bcdToDec(get_time[5]);
	time.year = bcdToDec(get_time[6]);
}

float Get_Temp (void)
{
	uint8_t temp[2];

	HAL_I2C_Mem_Read(&hi2c1, DS1307_ADDRESS, 0x11, 1, temp, 2, 1000);
	return ((temp[0])+(temp[1]>>6)/4.0);
}

void force_temp_conv (void)
{
	uint8_t status=0;
	uint8_t control=0;
	HAL_I2C_Mem_Read(&hi2c1, DS1307_ADDRESS, 0x0F, 1, &status, 1, 100);  // read status register
	if (!(status&0x04))
	{
		HAL_I2C_Mem_Read(&hi2c1, DS1307_ADDRESS, 0x0E, 1, &control, 1, 100);  // read control register
		HAL_I2C_Mem_Write(&hi2c1, DS1307_ADDRESS, 0x0E, 1, (uint8_t *)(control|(0x20)), 1, 100);
	}
}

float TEMP;

char buffer[20];
char buffer2[20];


/* USER CODE END 0 */

/**
  * @brief  The application entry point.
  * @retval int
  */
int main(void)
{
  /* USER CODE BEGIN 1 */
 int start, stop = 0;

start = HAL_GetTick();

  /* USER CODE END 1 */

  /* MCU Configuration--------------------------------------------------------*/

  /* Reset of all peripherals, Initializes the Flash interface and the Systick. */
  HAL_Init();

  /* USER CODE BEGIN Init */

  /* USER CODE END Init */

  /* Configure the system clock */
  SystemClock_Config();

  /* USER CODE BEGIN SysInit */

  /* USER CODE END SysInit */

  /* Initialize all configured peripherals */
  MX_GPIO_Init();
  MX_I2C1_Init();
  MX_USART2_UART_Init();
  /* USER CODE BEGIN 2 */

  Set_Time(45,45,10,2,16,3,22);

int index = 0;

  while(1)
  {
	Get_Time();
  sprintf (buffer, "Time: %02d:%02d:%02d\t", time.hour, time.minutes, time.seconds);

  sprintf (buffer2, "Date: %02d-%02d-20%02d\n", time.dayofmonth, time.month, time.year);


  HAL_Delay(1000);

  //index++;
  }

 // stop = HAL_GetTick()-start;
  //sprintf (stop, "%", stop);

  //HAL_UART_Transmit(&huart2, &stop, sizeof(stop), HAL_MAX_DELAY);


  HAL_Delay(500);

  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
  while (1)
  {
    /* USER CODE END WHILE */

    /* USER CODE BEGIN 3 */
  }
  /* USER CODE END 3 */
}
