/**
 * Utils.c
 */

#include "StateMachine.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define FSM_LOG_I(format, ...) \
  do { \
    if (Fsm && Fsm->Log) \
    { \
        Fsm->Log("[%s]: " format "\n", __func__, ##__VA_ARGS__); \
    } \
  } while (0)

#define FSM_LOG_E(Error) \
  do { \
    if (Fsm && Fsm->Log) \
    { \
        Fsm->Log("[%s]: %s\n", __func__, FsmErrorDecoder[Error]); \
    } \
  } while (0)

#define VOLTAGE_THRESHOLD_MV (10000U)

/**
 * Data Types
 */

typedef struct
{
  void (*OnEntry)(FsmType * Fsm);
  void (*OnExit)(FsmType * Fsm);
  void (*Execute)(FsmType * Fsm);
} FsmStateDescriptorType;

typedef struct
{
  FsmStateType NextState;
  void (*OnTransition)(FsmType * Fsm);
  void (*OnTransitionRejected)(FsmType * Fsm);
  bool (*Guard)(FsmType * Fsm);
} FsmTransitionDescriptorType;

/**
 * State-Machine Static Functions Declaration
 */

static FsmResultType Fsm_Execute(FsmType * Fsm);
static FsmResultType Fsm_HandleEvent(FsmType * Fsm, FsmEventType Event);

static FsmStateDescriptorType * Fsm_State(FsmStateType State);
static FsmTransitionDescriptorType * Fsm_Transition(FsmStateType State, FsmEventType Event);

static void Fsm_OnStartEntry(FsmType * Fsm);
static void Fsm_OnStartExit(FsmType * Fsm);
static void Fsm_StartExecute(FsmType * Fsm);

static void Fsm_OnStopEntry(FsmType * Fsm);
static void Fsm_OnStopExit(FsmType * Fsm);
static void Fsm_StopExecute(FsmType * Fsm);

static void Fsm_OnStartToStartTransition(FsmType * Fsm);
static void Fsm_OnStartToStopTransition(FsmType * Fsm);
static void Fsm_OnStopToStartTransition(FsmType * Fsm);
static void Fsm_OnStopToStopTransition(FsmType * Fsm);

static bool Fsm_VoltageGuard(FsmType * Fsm);

/**
 * Error Decoder
 */
const char * FsmErrorDecoder[] =
{
  "FSM_OK",
  "FSM_NOT_OK",
  "FSM_NULL_PTR",
  "FSM_INVALID_ARGUMENT"
};

/**
 * State Table
 */
FsmStateDescriptorType FsmStateTable [S_COUNT] =
{
  [S_START] =
  {
    .OnEntry = Fsm_OnStartEntry,
    .OnExit = Fsm_OnStartExit,
    .Execute = Fsm_StartExecute
  },

  [S_STOP] =
  {
    .OnEntry = Fsm_OnStopEntry,
    .OnExit = Fsm_OnStopExit,
    .Execute = Fsm_StopExecute
  }
};

/**
 * Transition Table
 */
FsmTransitionDescriptorType FsmTransitionTable[S_COUNT][E_COUNT] =
{
  [S_START] =
  {
    [E_START] =
    {
      .NextState = S_START,
      .OnTransition = Fsm_OnStartToStartTransition,
      .OnTransitionRejected = NULL,
      .Guard = NULL
    },

    [E_STOP] =
    {
      .NextState = S_STOP,
      .OnTransition = Fsm_OnStartToStopTransition,
      .OnTransitionRejected = NULL,
      .Guard = Fsm_VoltageGuard
    }
  },

  [S_STOP] =
  {
    [E_START] =
    {
      .NextState = S_START,
      .OnTransition = Fsm_OnStopToStartTransition,
      .OnTransitionRejected = NULL,
      .Guard = Fsm_VoltageGuard
    },

    [E_STOP] =
    {
      .NextState = S_STOP,
      .OnTransition = Fsm_OnStopToStopTransition,
      .OnTransitionRejected = NULL,
      .Guard = NULL
    }
  }
};

/**
 * State-Machine Public Functions Definition
 */

FsmResultType Fsm_Init(FsmType * Fsm, FsmGetEventFunctionType FsmGetEvent, FsmLogFunctionType FsmLog)
{
  FsmResultType Result = FSM_OK;

  if (Fsm == NULL || FsmGetEvent == NULL || FsmLog == NULL)
  {
    Result = FSM_NULL_PTR;
  }
  else
  {
    Fsm->GetEvent = FsmGetEvent;
    Fsm->Log = FsmLog;
    Fsm->State = S_STOP;
    Fsm->Voltage_mV = 10000U;

    if(FsmStateTable[Fsm->State].OnEntry != NULL)
    {
      FsmStateTable[Fsm->State].OnEntry(Fsm);
    }
  }

  FSM_LOG_E(Result);
  return Result;
}

FsmResultType Fsm_Deinit(FsmType * Fsm)
{
  FsmResultType Result = FSM_OK;

  FSM_LOG_I("");

  if (Fsm == NULL)
  {
    Result = FSM_NULL_PTR;
  }
  else
  {
    Fsm->GetEvent = NULL;
    Fsm->State = S_STOP;
    Fsm->Voltage_mV = 0U;
  }

  FSM_LOG_E(Result);
  return Result;
}

FsmResultType Fsm_Main(FsmType * Fsm)
{
  FsmResultType Result = FSM_OK;
  FsmEventType Event;

  FSM_LOG_I("");

  if (Fsm == NULL)
  {
    Result = FSM_NULL_PTR;
  }
  else if (Fsm->GetEvent == NULL)
  {
    Result = FSM_INVALID_ARGUMENT;
  }
  else if (Fsm->State >= S_COUNT)
  {
    Result = FSM_INVALID_ARGUMENT;
  }
  else
  {
    Fsm_Execute(Fsm);

    if (Fsm->GetEvent(&Event))
    {
      if (Event >= E_COUNT)
      {
        Result = FSM_NOT_OK;
      }
      else
      {
        Fsm_HandleEvent(Fsm, Event);
      }
    }
  }

  FSM_LOG_E(Result);
  return Result;
}

/**
 * State-Machine Static Functions Definition
 */

static FsmResultType Fsm_Execute(FsmType * Fsm)
{
  FsmResultType Result = FSM_OK;

  FSM_LOG_I("");

  if(FsmStateTable[Fsm->State].Execute != NULL)
  {
    FsmStateTable[Fsm->State].Execute(Fsm);
  }

  FSM_LOG_E(Result);
  return Result;
}

static FsmResultType Fsm_HandleEvent(FsmType * Fsm, FsmEventType Event)
{
  FsmResultType Result = FSM_OK;

  FSM_LOG_I("");

  if (Fsm == NULL)
  {
    Result = FSM_NULL_PTR;
  }
  else if(Event >= E_COUNT)
  {
    Result = FSM_INVALID_ARGUMENT;
  }
  else
  {
    if(Fsm->State >= S_COUNT)
    {
      Result = FSM_INVALID_ARGUMENT;
    }
    else
    {
      if (&FsmTransitionTable[Fsm->State][Event] == NULL)
      {
        FSM_LOG_I("Transition for the %d Event is unavailable.", Event);
      }
      else if((FsmTransitionTable[Fsm->State][Event].Guard != NULL) && (FsmTransitionTable[Fsm->State][Event].Guard(Fsm) == false))
      {
        if(FsmTransitionTable[Fsm->State][Event].OnTransitionRejected != NULL)
        {
          FsmTransitionTable[Fsm->State][Event].OnTransitionRejected(Fsm);
        }
      }
      else
      {
        if(FsmTransitionTable[Fsm->State][Event].NextState != Fsm->State)
        {
          if(FsmStateTable[Fsm->State].OnExit != NULL)
          {
            FsmStateTable[Fsm->State].OnExit(Fsm);
          }
        }

        if(FsmTransitionTable[Fsm->State][Event].OnTransition != NULL)
        {
          FsmTransitionTable[Fsm->State][Event].OnTransition(Fsm);
        }

        if(FsmTransitionTable[Fsm->State][Event].NextState != Fsm->State)
        {
          Fsm->State = FsmTransitionTable[Fsm->State][Event].NextState;

          if(FsmStateTable[Fsm->State].OnEntry != NULL)
          {
            FsmStateTable[Fsm->State].OnEntry(Fsm);
          }
        }
      }
    }
  }

  FSM_LOG_E(Result);
  return Result;
}

static FsmStateDescriptorType * Fsm_State(FsmStateType State)
{
  return &FsmStateTable[State];
}

static FsmTransitionDescriptorType * Fsm_Transition(FsmStateType State, FsmEventType Event)
{
  return &FsmTransitionTable[State][Event];
}

static void Fsm_OnStartEntry(FsmType * Fsm)
{
  (void)Fsm;
  FSM_LOG_I("");
}

static void Fsm_OnStartExit(FsmType * Fsm)
{
  (void)Fsm;
  FSM_LOG_I("");
}

static void Fsm_StartExecute(FsmType * Fsm)
{
  (void)Fsm;
  FSM_LOG_I("");
}

static void Fsm_OnStopEntry(FsmType * Fsm)
{
  (void)Fsm;
  FSM_LOG_I("");
}

static void Fsm_OnStopExit(FsmType * Fsm)
{
  (void)Fsm;
  FSM_LOG_I("");
}

static void Fsm_StopExecute(FsmType * Fsm)
{
  (void)Fsm;
  FSM_LOG_I("");
}

static void Fsm_OnStartToStartTransition(FsmType * Fsm)
{
  (void)Fsm;
  FSM_LOG_I("");
}

static void Fsm_OnStartToStopTransition(FsmType * Fsm)
{
  (void)Fsm;
  FSM_LOG_I("");
}

static void Fsm_OnStopToStartTransition(FsmType * Fsm)
{
  (void)Fsm;
  FSM_LOG_I("");
}

static void Fsm_OnStopToStopTransition(FsmType * Fsm)
{
  (void)Fsm;
  FSM_LOG_I("");
}

static bool Fsm_VoltageGuard(FsmType * Fsm)
{
  bool Result = false;

  Result = (Fsm->Voltage_mV <= VOLTAGE_THRESHOLD_MV);
  FSM_LOG_I("Voltage is %" PRId16 " mV, that is %sallowed", Fsm->Voltage_mV, (Result) ? "" : "NOT ");

  return Result;
}
