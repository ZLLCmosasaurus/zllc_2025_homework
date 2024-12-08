#include "main.h"



#include "stm32f1xx.h"

#include "tim.h"

#ifndef ENCODE_H

#define ENCODE_H

#ifdef __cplusplus
extern "C" {
#endif
	
	extern int32_t encoder_count[4];
	
	extern int32_t prev_encoder_count[4];

	typedef struct {
    int asdf;
    float bsdf;
	}my_struct;
	
	void process_data(my_struct *data);


#ifdef __cplusplus
}
#endif
#endif

