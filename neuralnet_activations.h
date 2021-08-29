#include <math.h>
#include <gsl/gsl_vector.h>


#ifndef NN_ACTIVATIONS_HEADER
#define NN_ACTIVATIONS_HEADER

#define SIGMOID 0
#define RELU 1
#define TANH 2

#define ACTIVATION_TABLE_SIZE 3

// Derivatives of activation functions

inline double sigmoid_derivative(double x){
	double t = exp(x);
	double y = t/(1+t);

	return y*(1-y);
} 

inline double RELU_derivative(double x){
	if (x <= 0.0){
		return 0.0;
	}
	else{
		return 1.0;
	}
}

inline double tanh_derivative(double x){
	double t= 1/cosh(x);
	return t*t;
}

// Activation functions ( for vectors )


inline void sigmoid_V(gsl_vector* a){
        for (int i = 0; i < a->size; i++){
                double x = exp(gsl_vector_get(a,i));
                gsl_vector_set(a,i,x/(1.0+x));
        }
}


inline void RELU_V(gsl_vector* a){
        for (int i = 0; i < a->size; i++){
                double x = fmax(x,0.0);
        }
}

inline void tanh_V(gsl_vector* a){
	for (int i =0; i < a->size; i++){
		gsl_vector_set(a,i,tanh(gsl_vector_get(a,i)));
	}	
}




#endif
