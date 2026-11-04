/**
 * State
 */

/**
 * Includes
 */

#include <inttypes.h>
#include <stdbool.h>
#include <stddef.h>
#include <stdio.h>

#include "StateMachine.h"

#if 0
/**
 * Definitions
 */

#define VOLTAGE_THRESHOLD_MV (10000U)

/**
 * Types
 */

typedef enum
{
  S_Start = 0U,
  S_Stop,
  S_Count
} FsmStateType;

typedef enum
{
  E_START = 0U,
  E_STOP,
  E_Count
} FsmEventType;

typedef struct Fsm FsmType;

typedef struct
{
  void (*OnEntry)(FsmType * FsmPtr);
  void (*OnExit)(FsmType * FsmPtr);
  void (*Execute)(FsmType * FsmPtr);
} FsmStateDescriptorType;

typedef struct
{
  FsmStateType NextState;
  bool (*Guard)(FsmType * FsmPtr);
  void (*OnTransitionAllowed)(FsmType * FsmPtr);
  void (*OnTransitionRejected)(FsmType * FsmPtr);
} FsmTransitionType;

typedef struct Fsm
{
  FsmStateType CurrentState;
  int16_t Voltage_mV;
} FsmType;


/**
 * State-Machine Public Functions Declaration
 */

void FsmInit(FsmType * FsmPtr);
void Fsm_Main(FsmType * FsmPtr);


/**
 * State-Machine Static Functions Declaration
 */

static void FsmExecute(FsmType * FsmPtr);
static void FsmHandleEvent(FsmType * FsmPtr, FsmEventType Event);

static const FsmTransitionType * FsmGetTransition(FsmStateType State, FsmEventType Event);
static bool FsmGetEvent(FsmEventType * EventPtr);
static bool FsmVoltageOkGuard(FsmType * FsmPtr);

static void FsmStartEntry(FsmType * FsmPtr);
static void FsmStartExit(FsmType * FsmPtr);
static void FsmStartExecute(FsmType * FsmPtr);

static void FsmStopEntry(FsmType * FsmPtr);
static void FsmStopExit(FsmType * FsmPtr);
static void FsmStopExecute(FsmType * FsmPtr);

static void FsmOnStartToStartTransition(FsmType * FsmPtr);
static void FsmOnStartToStopTransition(FsmType * FsmPtr);
static void FsmOnStopToStartTransition(FsmType * FsmPtr);
static void FsmOnStopToStopTransition(FsmType * FsmPtr);


/**
 * State Table
 */
static const FsmStateDescriptorType FsmStateTable[S_Count] =
{
  [S_Start] =
  {
    .OnEntry = FsmStartEntry,
    .OnExit  = FsmStartExit,
    .Execute = FsmStartExecute
  },

  [S_Stop] =
  {
    .OnEntry = FsmStopEntry,
    .OnExit  = FsmStopExit,
    .Execute = FsmStopExecute
  }
};

/**
 * Transition Table
 */
static const FsmTransitionType FsmTransitionTable[S_Count][E_Count] =
{
  /* Current state = S_Start */
  [S_Start] =
  {
    /* E_START */
    [E_START] =
    {
      .NextState = S_Start,
      .Guard = NULL,
      .OnTransitionAllowed = FsmOnStartToStartTransition,
      .OnTransitionRejected = NULL,
    },

    /* E_STOP */
    [E_STOP] =
    {
      .NextState = S_Stop,
      .Guard = FsmVoltageOkGuard,
      .OnTransitionAllowed = FsmOnStartToStopTransition,
      .OnTransitionRejected = NULL,
    }
  },

  /* Current state = S_Stop */
  [S_Stop] =
  {
    /* E_START */
    [E_START] =
    {
      .NextState = S_Start,
      .Guard = FsmVoltageOkGuard,
      .OnTransitionAllowed = FsmOnStopToStartTransition,
      .OnTransitionRejected = NULL,
    },

    /* E_STOP */
    [E_STOP] =
    {
      .NextState = S_Stop,
      .Guard = NULL,
      .OnTransitionAllowed = FsmOnStopToStopTransition,
      .OnTransitionRejected = NULL,
    }
  }
};

/**
 * State-Machine Public Functions Definition
 */

void FsmInit(FsmType * FsmPtr)
{
  if (FsmPtr == NULL)
  {
    // Error.
    // Avoid multiple return;
    // Det_ReportError(...);
    printf("FsmInit: Invalid Argument FsmPtr.\n");
  }
  else
  {
    FsmPtr->CurrentState = S_Start;
    FsmPtr->Voltage_mV = 10000;

    if(FsmStateTable[FsmPtr->CurrentState].OnEntry != NULL)
    {
      FsmStateTable[FsmPtr->CurrentState].OnEntry(FsmPtr);
    }
  }
}


void Fsm_Main(FsmType * FsmPtr)
{
  FsmEventType Event;

  FsmExecute(FsmPtr);

  if (FsmGetEvent(&Event))
  {
    FsmHandleEvent(FsmPtr, Event);
  }
}


/**
 * State-Machine Static Functions Definition
 */

static void FsmExecute(FsmType * FsmPtr)
{
  FsmStateType CurrentState;

  if (FsmPtr == NULL)
  {
    // Error.
    // Avoid multiple return;
    // Det_ReportError(...);
    printf("FsmExecute: Invalid Argument FsmPtr.\n");
  }
  else
  {
    CurrentState = FsmPtr->CurrentState;

    if(FsmStateTable[CurrentState].Execute != NULL)
    {
      FsmStateTable[CurrentState].Execute(FsmPtr);
    }
  }
}


static void FsmHandleEvent(FsmType * FsmPtr, FsmEventType Event)
{
  const FsmTransitionType * TransitionPtr;
  FsmStateType CurrentState;
  FsmStateType NextState;

  if (FsmPtr == NULL)
  {
    // Error.
    // Avoid multiple return;
    // Det_ReportError(...);
    printf("FsmHandleEvent: Invalid Argument FsmPtr.\n");
  }
  else if(Event >= E_Count)
  {
    // Error.
    // Avoid multiple return;
    // Det_ReportError(...);
    printf("FsmHandleEvent: Invalid Argument Event %d.\n", Event);
  }
  else
  {
    CurrentState = FsmPtr->CurrentState;

    if(CurrentState >= S_Count)
    {
      // Error.
      // Avoid multiple return;
      // Det_ReportError(...);
      printf("FsmHandleEvent: Invalid FsmPtr->State %d.\n", CurrentState);
    }
    else
    {
      TransitionPtr = FsmGetTransition(CurrentState, Event);

      if (TransitionPtr == NULL)
      {
        printf("FsmHandleEvent: TransitionPtr is NULL for the %d Event.\n", Event);
      }
      else if((TransitionPtr->Guard != NULL) && (TransitionPtr->Guard(FsmPtr) == false))
      {
        if(TransitionPtr->OnTransitionRejected != NULL)
        {
          TransitionPtr->OnTransitionRejected(FsmPtr);
        }
      }
      else
      {
        NextState = TransitionPtr->NextState;

        if(NextState != CurrentState)
        {
          if(FsmStateTable[CurrentState].OnExit != NULL)
          {
            FsmStateTable[CurrentState].OnExit(FsmPtr);
          }
        }

        if(TransitionPtr->OnTransitionAllowed != NULL)
        {
          TransitionPtr->OnTransitionAllowed(FsmPtr);
        }

        if(NextState != CurrentState)
        {
          FsmPtr->CurrentState = NextState;

          if(FsmStateTable[NextState].OnEntry != NULL)
          {
            FsmStateTable[NextState].OnEntry(FsmPtr);
          }
        }
      }
    }
  }
}


static const FsmTransitionType * FsmGetTransition(FsmStateType State, FsmEventType Event)
{
  const FsmTransitionType * TransitionPtr = NULL;

  if (State >= S_Count)
  {
    TransitionPtr = NULL;
    printf("FsmGetTransition: Invalid Argument State %d.\n", State);
  }
  else if (Event >= E_Count)
  {
    TransitionPtr = NULL;
    printf("FsmGetTransition: Invalid Argument Event %d.\n", Event);
  }
  else
  {
    TransitionPtr = &FsmTransitionTable[State][Event];
  }

  return TransitionPtr;
}

static bool FsmVoltageOkGuard(FsmType * FsmPtr)
{
  bool Result;

  Result = false;

  if (FsmPtr == NULL)
  {
    // Error.
    // Avoid multiple return;
    // Det_ReportError(...);
    printf("FsmVoltageOkGuard: Invalid Argument FsmPtr.\n");
  }
  else
  {
    Result = (FsmPtr->Voltage_mV <= VOLTAGE_THRESHOLD_MV);
    printf("FsmVoltageOkGuard: Voltage is %" PRId16 " mV, that is %sallowed\n", FsmPtr->Voltage_mV, (Result) ? "" : "NOT ");
  }

  return Result;
}


static void FsmStartEntry(FsmType * FsmPtr)
{
  (void)FsmPtr;
  printf("Enter State START\n");
}

static void FsmStartExit(FsmType * FsmPtr)
{
  (void)FsmPtr;
  printf("Exit State START\n");
}

static void FsmStartExecute(FsmType * FsmPtr)
{
  (void)FsmPtr;
  printf("Execute State START\n");
}

static void FsmStopEntry(FsmType * FsmPtr)
{
  (void)FsmPtr;
  printf("Enter State STOP\n");
}

static void FsmStopExit(FsmType * FsmPtr)
{
  (void)FsmPtr;
  printf("Exit State STOP\n");
}

static void FsmStopExecute(FsmType * FsmPtr)
{
  (void)FsmPtr;
  printf("Execute State STOP\n");
}

static void FsmOnStartToStartTransition(FsmType * FsmPtr)
{
  (void)FsmPtr;
  printf("Transition State START->START\n");
}

static void FsmOnStartToStopTransition(FsmType * FsmPtr)
{
  (void)FsmPtr;
  printf("Transition State START->STOP\n");
}

static void FsmOnStopToStartTransition(FsmType * FsmPtr)
{
  (void)FsmPtr;
  printf("Transition State STOP->START\n");
}

static void FsmOnStopToStopTransition(FsmType * FsmPtr)
{

  printf("Transition State STOP->STOP\n");
}
#endif

static FsmEventType FsmEventQueueMock;

static bool Fsm_GetEvent(FsmEventType * EventPtr)
{
  bool Result;

  Result = false;

  if (EventPtr == NULL)
  {
    printf("FsmGetEvent: Invalid Argument EventPtr.\n");
  }
  else
  {
    *EventPtr = FsmEventQueueMock;
    Result = true;
  }

  return Result;
}

/**
 * Main Function
 */
int main(int argc, const char * argv[])
{
  FsmType Fsm;

  if (Fsm_Init(&Fsm, Fsm_GetEvent, printf) != FSM_OK)
  {
    return 0;
  }

  {
    FsmEventQueueMock = E_START;
    Fsm.Voltage_mV = 15000;

    Fsm_Main(&Fsm);
  }

  {
    FsmEventQueueMock = E_START;
    Fsm.Voltage_mV = 0;

    Fsm_Main(&Fsm);
  }

  {
    FsmEventQueueMock = E_STOP;
    Fsm.Voltage_mV = 15000;
    Fsm_Main(&Fsm);
  }

  {
    FsmEventQueueMock = E_STOP;
    Fsm.Voltage_mV = 0;

  Fsm_Main(&Fsm);
  }

  {
    FsmEventQueueMock = E_STOP;
    Fsm.Voltage_mV = 0;

    Fsm_Main(&Fsm);
  }

  return 0;
}
