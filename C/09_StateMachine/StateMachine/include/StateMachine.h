/**
 * Utils.h
 *
 * Notes:
 * - Compare*, CompareMore* and CompareLess* do not check pointers for NULL. Passing NULL is undefined behavior.
 */

#ifndef STATEMACHINE_H
#define STATEMACHINE_H

#include <inttypes.h>
#include <stdbool.h>
#include <stddef.h>

/**
 * Data Types
 */

typedef enum
{
  FSM_OK = 0,
  FSM_NOT_OK,
  FSM_NULL_PTR,
  FSM_INVALID_ARGUMENT
} FsmResultType;

typedef enum
{
  S_START = 0U,
  S_STOP,
  S_COUNT
} FsmStateType;

typedef enum
{
  E_START = 0U,
  E_STOP,
  E_COUNT
} FsmEventType;

//typedef struct Fsm FsmType;
typedef bool (* FsmGetEventFunctionType)(FsmEventType * EventPtr);
typedef int (*FsmLogFunctionType)(const char *, ...);

typedef struct Fsm
{
  FsmGetEventFunctionType GetEvent;
  FsmLogFunctionType Log;
  FsmStateType State;
  uint16_t Voltage_mV;
} FsmType;



/**
 * State-Machine Public Functions Declaration
 */

FsmResultType Fsm_Init(FsmType * Fsm, FsmGetEventFunctionType FsmGetEvent, FsmLogFunctionType FsmLog);
FsmResultType Fsm_Deinit(FsmType * Fsm);
FsmResultType Fsm_Main(FsmType * Fsm);

#endif // STATEMACHINE_H
