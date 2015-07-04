/**
# DSVerifier - Digital Systems Verifier
#
#                Federal University of Amazonas - UFAM
#
# Authors:       Hussama Ismail <hussamaismail@gmail.com>
#                Iury Bessa     <iury.bessa@gmail.com>
#                Renato Abreu   <renatobabreu@yahoo.com.br>
#				 
# ------------------------------------------------------
#
# verify stability in closed loop
#
# ------------------------------------------------------
*/

#include "../core/funcsfxp.h"
#include "../core/closed-loop.h"
#include "../core/util.h"
#include <assert.h>

extern digital_system plant;
extern digital_system control;

int verify_stability_closedloop_using_dslib(void){

	/* Generating closed loop for series or feedback */
	double c_num[100] = control.b;
	int c_num_size = control.b_size;
	double c_den[100] = control.a;
	int c_den_size = control.a_size;

	/* Quantizing Control Coefficients */
	fxp32_t c_num_fxp[control.b_size];
	fxp_double_to_fxp_array(c_num, c_num_fxp, control.b_size);
	fxp32_t c_den_fxp[control.a_size];
	fxp_double_to_fxp_array(c_den, c_den_fxp, control.a_size);

	/* Getting Control Quantized Coefficients  */
	double c_num_qtz[control.b_size];
	fxp_to_double_array(c_num_qtz, c_num_fxp, control.b_size);
	double c_den_qtz[control.a_size];
	fxp_to_double_array(c_den_qtz, c_den_fxp, control.a_size);

	/* Getting Plant Coefficients */
	double p_num[100] =  plant.b;
	int p_num_size = plant.b_size;
	double p_den[100] = plant.a;
	int p_den_size = plant.a_size;

	double ans_num[100];
	int ans_num_size = control.b_size + plant.b_size - 1;
	double ans_den[100];
	int ans_den_size = control.a_size + plant.a_size - 1;

	#if (CONNECTION_MODE == SERIES)
		ft_closedloop_series(c_num_qtz, c_num_size, c_den_qtz, c_den_size, p_num, p_num_size, p_den, p_den_size, ans_num, ans_num_size, ans_den, ans_den_size);
	#elif (CONNECTION_MODE == FEEDBACK)
    	printf("Verifying stability for controller\n");
		check_stability(c_den_qtz, c_den_size);
		ft_closedloop_feedback(c_num_qtz, c_num_size, c_den_qtz, c_den_size, p_num, p_num_size, p_den, p_den_size, ans_num, ans_num_size, ans_den, ans_den_size);
	#endif

	/* Checking stability */
	printf("Verifying stability for closedloop function\n");
	assert(check_stability_closedloop(ans_den, ans_den_size, p_num, p_num_size, p_den, p_den_size));

	return 0;
}

