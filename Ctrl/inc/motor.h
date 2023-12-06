#ifndef __MOTOR_H__
#define __MOTOR_H__

#include "stdint.h"
#include "stdbool.h"

typedef struct
{
	float phA;
	float phB;
	float phC;
}Iph_ABC_t;

typedef struct
{
	bool pre_calibrated;
	int32_t pole_pairs;
	float calibration_current;
	float resistance_calib_max_voltage;
	float phase_inductance;
	float phase_resistance;
	float torque_constant;
	float current_lim;
	float current_lim_margin;
	float torque_lim;
	float requested_current_range;
	float inverter_temp_limit_lower;
	float inverter_temp_limit_upper;

	bool R_wL_FF_enable;
	bool bEMF_FF_enable;
	
	float I_bus_hard_min;
	float I_bus_hard_max;
	float I_leak_max;
	
	float dc_calib_tau;
}MotorConfig_t;


typedef struct
{
	MotorConfig_t config_;
	bool is_armed_;
	Iph_ABC_t *current_meas_;
	Iph_ABC_t *dc_calib;
	float I_bus_;
	
}Motor_t;

extern Motor_t motor;

void motor_set_pre_calibrated(bool value);
void motor_set_phase_inductance(float value);
void motor_set_phase_resistance(float value);
void motor_set_current_control_bandwidth(float value);
void motor_init();
float motor_phase_current_from_adcval(uint32_t ADCValue);
bool motor_measure_phase_resistance(float test_current, float max_voltage);
bool motor_measure_phase_inductance(float rest_voltage);

bool motor_run_calibration();
void motor_update();
void motor_current_meas_cb(Iph_ABC_t *current);
void motor_dc_calib_cb(Iph_ABC_t *current);
void motor_pwm_update_cb();

#endif
