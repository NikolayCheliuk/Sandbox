/**
 * PWM
 */

#include <inttypes.h>
#include <stddef.h>
#include <stdio.h>

typedef enum
{
  PWM_ERROR = 0,
  PWM_OK = 1
} pwm_result_t;

typedef struct
{
  int32_t max;
  int32_t cmp;
  int32_t cnt;
  int32_t step;
  int32_t lvl;
} pwm_t;

pwm_result_t Pwm_Init(pwm_t * pwm)
{
  pwm_result_t result = PWM_ERROR;

  if (pwm == NULL)
  {
    // Error
  }
  else
  {
    pwm->max  = 10;
    pwm->cmp  = 3;
    pwm->cnt  = 0;
    pwm->step = 1;
    pwm->lvl  = 0;

    result = PWM_OK;
  }

  return result;
}

void Pwm_Main(pwm_t * pwm)
{
  if (pwm == NULL)
  {
    // Error
  }
  else
  {
    pwm->lvl = (pwm->cnt < pwm->cmp) ? 0 : 1;

    printf("%-3" PRId32 " : %" PRId32 "\n", pwm->cnt, pwm->lvl);

    pwm->cnt += pwm->step;

    if ((pwm->cnt == 0) || (pwm->cnt == pwm->max))
    {
      pwm->step = -pwm->step;
    }
  }
}

int main(int argc, const char * argv[])
{
  pwm_result_t pwm_status;
  pwm_t pwm;
  size_t n = 40;
  
  pwm_status = Pwm_Init(&pwm);

  if (pwm_status == PWM_ERROR)
  {
    // Error
  }
  else
  {
    for (size_t i = 0; i < n; i++)
    {
      Pwm_Main(&pwm);
    }
  }

  return 0;
}

