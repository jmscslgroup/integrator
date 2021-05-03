//
// Academic License - for use in teaching, academic research, and meeting
// course requirements at degree granting institutions only.  Not for
// government, commercial, or other organizational use.
//
// File: integrator.cpp
//
// Code generated for Simulink model 'integrator'.
//
// Model version                  : 1.12
// Simulink Coder version         : 9.5 (R2021a) 14-Nov-2020
// C/C++ source code generated on : Sun May  2 21:34:09 2021
//
// Target selection: ert.tlc
// Embedded hardware selection: Generic->Unspecified (assume 32-bit Generic)
// Code generation objectives: Unspecified
// Validation result: Not run
//
#include "integrator.h"
#include "integrator_private.h"

// Block signals (default storage)
B_integrator_T integrator_B;

// Continuous states
X_integrator_T integrator_X;

// Block states (default storage)
DW_integrator_T integrator_DW;

// Real-time model
RT_MODEL_integrator_T integrator_M_ = RT_MODEL_integrator_T();
RT_MODEL_integrator_T *const integrator_M = &integrator_M_;

//
// This function updates continuous states using the ODE3 fixed-step
// solver algorithm
//
static void rt_ertODEUpdateContinuousStates(RTWSolverInfo *si )
{
  // Solver Matrices
  static const real_T rt_ODE3_A[3] = {
    1.0/2.0, 3.0/4.0, 1.0
  };

  static const real_T rt_ODE3_B[3][3] = {
    { 1.0/2.0, 0.0, 0.0 },

    { 0.0, 3.0/4.0, 0.0 },

    { 2.0/9.0, 1.0/3.0, 4.0/9.0 }
  };

  time_T t = rtsiGetT(si);
  time_T tnew = rtsiGetSolverStopTime(si);
  time_T h = rtsiGetStepSize(si);
  real_T *x = rtsiGetContStates(si);
  ODE3_IntgData *id = static_cast<ODE3_IntgData *>(rtsiGetSolverData(si));
  real_T *y = id->y;
  real_T *f0 = id->f[0];
  real_T *f1 = id->f[1];
  real_T *f2 = id->f[2];
  real_T hB[3];
  int_T i;
  int_T nXc = 1;
  rtsiSetSimTimeStep(si,MINOR_TIME_STEP);

  // Save the state values at time t in y, we'll use x as ynew.
  (void) memcpy(y, x,
                static_cast<uint_T>(nXc)*sizeof(real_T));

  // Assumes that rtsiSetT and ModelOutputs are up-to-date
  // f0 = f(t,y)
  rtsiSetdX(si, f0);
  integrator_derivatives();

  // f(:,2) = feval(odefile, t + hA(1), y + f*hB(:,1), args(:)(*));
  hB[0] = h * rt_ODE3_B[0][0];
  for (i = 0; i < nXc; i++) {
    x[i] = y[i] + (f0[i]*hB[0]);
  }

  rtsiSetT(si, t + h*rt_ODE3_A[0]);
  rtsiSetdX(si, f1);
  integrator_step();
  integrator_derivatives();

  // f(:,3) = feval(odefile, t + hA(2), y + f*hB(:,2), args(:)(*));
  for (i = 0; i <= 1; i++) {
    hB[i] = h * rt_ODE3_B[1][i];
  }

  for (i = 0; i < nXc; i++) {
    x[i] = y[i] + (f0[i]*hB[0] + f1[i]*hB[1]);
  }

  rtsiSetT(si, t + h*rt_ODE3_A[1]);
  rtsiSetdX(si, f2);
  integrator_step();
  integrator_derivatives();

  // tnew = t + hA(3);
  // ynew = y + f*hB(:,3);
  for (i = 0; i <= 2; i++) {
    hB[i] = h * rt_ODE3_B[2][i];
  }

  for (i = 0; i < nXc; i++) {
    x[i] = y[i] + (f0[i]*hB[0] + f1[i]*hB[1] + f2[i]*hB[2]);
  }

  rtsiSetT(si, tnew);
  rtsiSetSimTimeStep(si,MAJOR_TIME_STEP);
}

// Model step function
void integrator_step(void)
{
  SL_Bus_integrator_geometry_msgs_Twist rtb_BusAssignment;
  SL_Bus_integrator_std_msgs_Float64 b_varargout_2;
  boolean_T b_varargout_1;
  if (rtmIsMajorTimeStep(integrator_M)) {
    // set solver stop time
    rtsiSetSolverStopTime(&integrator_M->solverInfo,
                          ((integrator_M->Timing.clockTick0+1)*
      integrator_M->Timing.stepSize0));
  }                                    // end MajorTimeStep

  // Update absolute time of base rate at minor time step
  if (rtmIsMinorTimeStep(integrator_M)) {
    integrator_M->Timing.t[0] = rtsiGetT(&integrator_M->solverInfo);
  }

  // BusAssignment: '<Root>/Bus Assignment' incorporates:
  //   Constant: '<S1>/Constant'
  //   Integrator: '<Root>/Integrator'

  rtb_BusAssignment = integrator_P.Constant_Value;
  rtb_BusAssignment.Linear.X = integrator_X.Integrator_CSTATE;

  // Outputs for Atomic SubSystem: '<Root>/Publish'
  // MATLABSystem: '<S2>/SinkBlock'
  Pub_integrator_19.publish(&rtb_BusAssignment);

  // End of Outputs for SubSystem: '<Root>/Publish'
  if (rtmIsMajorTimeStep(integrator_M)) {
    // Outputs for Atomic SubSystem: '<Root>/Subscribe1'
    // MATLABSystem: '<S3>/SourceBlock' incorporates:
    //   Inport: '<S4>/In1'

    b_varargout_1 = Sub_integrator_14.getLatestMessage(&b_varargout_2);

    // Outputs for Enabled SubSystem: '<S3>/Enabled Subsystem' incorporates:
    //   EnablePort: '<S4>/Enable'

    if (b_varargout_1) {
      integrator_B.In1 = b_varargout_2;
    }

    // End of MATLABSystem: '<S3>/SourceBlock'
    // End of Outputs for SubSystem: '<S3>/Enabled Subsystem'
    // End of Outputs for SubSystem: '<Root>/Subscribe1'
  }

  if (rtmIsMajorTimeStep(integrator_M)) {
    rt_ertODEUpdateContinuousStates(&integrator_M->solverInfo);

    // Update absolute time for base rate
    // The "clockTick0" counts the number of times the code of this task has
    //  been executed. The absolute time is the multiplication of "clockTick0"
    //  and "Timing.stepSize0". Size of "clockTick0" ensures timer will not
    //  overflow during the application lifespan selected.

    ++integrator_M->Timing.clockTick0;
    integrator_M->Timing.t[0] = rtsiGetSolverStopTime(&integrator_M->solverInfo);

    {
      // Update absolute timer for sample time: [0.05s, 0.0s]
      // The "clockTick1" counts the number of times the code of this task has
      //  been executed. The resolution of this integer timer is 0.05, which is the step size
      //  of the task. Size of "clockTick1" ensures timer will not overflow during the
      //  application lifespan selected.

      integrator_M->Timing.clockTick1++;
    }
  }                                    // end MajorTimeStep
}

// Derivatives for root system: '<Root>'
void integrator_derivatives(void)
{
  XDot_integrator_T *_rtXdot;
  _rtXdot = ((XDot_integrator_T *) integrator_M->derivs);

  // Derivatives for Integrator: '<Root>/Integrator'
  _rtXdot->Integrator_CSTATE = integrator_B.In1.Data;
}

// Model initialize function
void integrator_initialize(void)
{
  // Registration code
  {
    // Setup solver object
    rtsiSetSimTimeStepPtr(&integrator_M->solverInfo,
                          &integrator_M->Timing.simTimeStep);
    rtsiSetTPtr(&integrator_M->solverInfo, &rtmGetTPtr(integrator_M));
    rtsiSetStepSizePtr(&integrator_M->solverInfo,
                       &integrator_M->Timing.stepSize0);
    rtsiSetdXPtr(&integrator_M->solverInfo, &integrator_M->derivs);
    rtsiSetContStatesPtr(&integrator_M->solverInfo, (real_T **)
                         &integrator_M->contStates);
    rtsiSetNumContStatesPtr(&integrator_M->solverInfo,
      &integrator_M->Sizes.numContStates);
    rtsiSetNumPeriodicContStatesPtr(&integrator_M->solverInfo,
      &integrator_M->Sizes.numPeriodicContStates);
    rtsiSetPeriodicContStateIndicesPtr(&integrator_M->solverInfo,
      &integrator_M->periodicContStateIndices);
    rtsiSetPeriodicContStateRangesPtr(&integrator_M->solverInfo,
      &integrator_M->periodicContStateRanges);
    rtsiSetErrorStatusPtr(&integrator_M->solverInfo, (&rtmGetErrorStatus
      (integrator_M)));
    rtsiSetRTModelPtr(&integrator_M->solverInfo, integrator_M);
  }

  rtsiSetSimTimeStep(&integrator_M->solverInfo, MAJOR_TIME_STEP);
  integrator_M->intgData.y = integrator_M->odeY;
  integrator_M->intgData.f[0] = integrator_M->odeF[0];
  integrator_M->intgData.f[1] = integrator_M->odeF[1];
  integrator_M->intgData.f[2] = integrator_M->odeF[2];
  integrator_M->contStates = ((X_integrator_T *) &integrator_X);
  rtsiSetSolverData(&integrator_M->solverInfo, static_cast<void *>
                    (&integrator_M->intgData));
  rtsiSetSolverName(&integrator_M->solverInfo,"ode3");
  rtmSetTPtr(integrator_M, &integrator_M->Timing.tArray[0]);
  integrator_M->Timing.stepSize0 = 0.05;

  {
    int32_T i;
    char_T b_zeroDelimTopic[8];
    char_T b_zeroDelimTopic_0[6];
    static const char_T tmp[7] = { 'c', 'm', 'd', '_', 'v', 'e', 'l' };

    static const char_T tmp_0[5] = { 'a', 'c', 'c', 'e', 'l' };

    // InitializeConditions for Integrator: '<Root>/Integrator'
    integrator_X.Integrator_CSTATE = integrator_P.Integrator_IC;

    // SystemInitialize for Atomic SubSystem: '<Root>/Publish'
    // Start for MATLABSystem: '<S2>/SinkBlock'
    integrator_DW.obj.matlabCodegenIsDeleted = false;
    integrator_DW.obj.isInitialized = 1;
    for (i = 0; i < 7; i++) {
      b_zeroDelimTopic[i] = tmp[i];
    }

    b_zeroDelimTopic[7] = '\x00';
    Pub_integrator_19.createPublisher(&b_zeroDelimTopic[0], 1);
    integrator_DW.obj.isSetupComplete = true;

    // End of Start for MATLABSystem: '<S2>/SinkBlock'
    // End of SystemInitialize for SubSystem: '<Root>/Publish'

    // SystemInitialize for Atomic SubSystem: '<Root>/Subscribe1'
    // SystemInitialize for Enabled SubSystem: '<S3>/Enabled Subsystem'
    // SystemInitialize for Outport: '<S4>/Out1' incorporates:
    //   Inport: '<S4>/In1'

    integrator_B.In1 = integrator_P.Out1_Y0;

    // End of SystemInitialize for SubSystem: '<S3>/Enabled Subsystem'

    // Start for MATLABSystem: '<S3>/SourceBlock'
    integrator_DW.obj_p.matlabCodegenIsDeleted = false;
    integrator_DW.obj_p.isInitialized = 1;
    for (i = 0; i < 5; i++) {
      b_zeroDelimTopic_0[i] = tmp_0[i];
    }

    b_zeroDelimTopic_0[5] = '\x00';
    Sub_integrator_14.createSubscriber(&b_zeroDelimTopic_0[0], 1);
    integrator_DW.obj_p.isSetupComplete = true;

    // End of Start for MATLABSystem: '<S3>/SourceBlock'
    // End of SystemInitialize for SubSystem: '<Root>/Subscribe1'
  }
}

// Model terminate function
void integrator_terminate(void)
{
  // Terminate for Atomic SubSystem: '<Root>/Publish'
  // Terminate for MATLABSystem: '<S2>/SinkBlock'
  if (!integrator_DW.obj.matlabCodegenIsDeleted) {
    integrator_DW.obj.matlabCodegenIsDeleted = true;
  }

  // End of Terminate for MATLABSystem: '<S2>/SinkBlock'
  // End of Terminate for SubSystem: '<Root>/Publish'

  // Terminate for Atomic SubSystem: '<Root>/Subscribe1'
  // Terminate for MATLABSystem: '<S3>/SourceBlock'
  if (!integrator_DW.obj_p.matlabCodegenIsDeleted) {
    integrator_DW.obj_p.matlabCodegenIsDeleted = true;
  }

  // End of Terminate for MATLABSystem: '<S3>/SourceBlock'
  // End of Terminate for SubSystem: '<Root>/Subscribe1'
}

//
// File trailer for generated code.
//
// [EOF]
//
