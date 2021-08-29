
#ifndef NN_CORE_HEADER
#define NN_CORE_HEADER

#include <gsl/gsl_vector.h>
#include <gsl/gsl_matrix.h>
#include <gsl/gsl_blas.h>

#include <neuralnet_activations.h>
#include <QDebug>
inline gsl_vector* forwardPropagate(gsl_vector* inputVector, gsl_matrix** weightPtrs, gsl_vector** a, gsl_vector** z, int* nodeCountArray, int layers, bool training, int activation){
        void (*activation_V[ACTIVATION_TABLE_SIZE])(gsl_vector* ) = {sigmoid_V, RELU_V, tanh_V};

        gsl_vector* v1 = NULL;
        gsl_vector* v2 = gsl_vector_calloc(nodeCountArray[0]);
        gsl_vector_memcpy(v2, inputVector);

        z[0] = gsl_vector_calloc(nodeCountArray[0]);
        gsl_vector_memcpy(z[0], inputVector);
        // Foward Propagate via Matrix Vector Multiplication using GSL BLAS
        for (int i = 1; i <= layers; i++){
                v1 = gsl_vector_calloc(nodeCountArray[i]);
                gsl_blas_dgemv(CblasNoTrans,  1.0, weightPtrs[i], v2, 0.0, v1);

                a[i] = gsl_vector_calloc(nodeCountArray[i]);
                gsl_vector_memcpy(a[i], v1);

                activation_V[activation](v1);

                z[i] = gsl_vector_calloc(nodeCountArray[i]);
                gsl_vector_memcpy(z[i], v1);

                gsl_vector_free(v2);
                v2 = gsl_vector_calloc(nodeCountArray[i]);

                gsl_vector_memcpy(v2, v1);
                gsl_vector_free(v1);
        }
        return v2;
}


inline double backPropagate(gsl_vector* inputVector, gsl_vector* outputVector ,gsl_matrix** weightPtrs, gsl_vector** a,gsl_vector** z,int* nodeCountArray,double learningRate, int regularizationMask, double alpha ,double beta,int layer, int activation){
// Backpropagate first layer
        double (*activation_derivatives[ACTIVATION_TABLE_SIZE])(double) = {sigmoid_derivative, RELU_derivative, tanh_derivative};

        gsl_vector *derivLayer = gsl_vector_calloc(nodeCountArray[layer-1]);
        gsl_vector* prevDerivLayer = gsl_vector_calloc(nodeCountArray[layer]);

        gsl_matrix* offsetMatrix[layer+1];

        gsl_vector* differenceLayer = gsl_vector_calloc(nodeCountArray[layer]);
        gsl_vector_memcpy(differenceLayer, inputVector);
        gsl_vector_sub(differenceLayer, outputVector);

        double MSE = 0.0;
        for (int i = 0; i < nodeCountArray[layer]; i++){
                double t = gsl_vector_get(differenceLayer,i);
                MSE += t*t;
        }
        MSE /= nodeCountArray[layer];
        for (int i = 0; i < nodeCountArray[layer]; i++){
                double t = activation_derivatives[activation](gsl_vector_get(a[layer], i)) * (gsl_vector_get(differenceLayer,i));
                gsl_vector_set(prevDerivLayer, i,t);
        }

        offsetMatrix[layer] = gsl_matrix_calloc(nodeCountArray[layer],  nodeCountArray[layer-1]);
        gsl_blas_dger(learningRate, prevDerivLayer,z[layer-1], offsetMatrix[layer]);

        for (int l = layer-1; l >= 1; l--){


                for (int i = 0; i < nodeCountArray[l]; i++){
                        double k = 0.0;
                        for (int j = 0; j < nodeCountArray[l+1]; j++){
                                k += gsl_vector_get(prevDerivLayer, j) * activation_derivatives[activation](gsl_vector_get(a[l],i)) * gsl_matrix_get(weightPtrs[l+1],j,i);
                        }
                        gsl_vector_set(derivLayer,i,k);
                }

                /*
                 maybe ill implement a proper vectorized version of the backprop algorithm later
                gsl_vector *calc1 = gsl_vector_calloc(nodeCountArray[l]);
                gsl_blas_dgemv(CblasTrans,1.0,weightPtrs[l+1],prevDerivLayer,0.0,calc21);
                */

                // Create offset Matrix
                offsetMatrix[l] = gsl_matrix_calloc(nodeCountArray[l],  nodeCountArray[l-1]);
                gsl_blas_dger(learningRate, derivLayer, z[l-1], offsetMatrix[l]);

                gsl_vector_free(prevDerivLayer);
                prevDerivLayer = gsl_vector_calloc(nodeCountArray[l]);

                gsl_vector_memcpy(prevDerivLayer, derivLayer);

                gsl_vector_free(derivLayer);
                derivLayer = gsl_vector_calloc(nodeCountArray[l-1]);
        }

        gsl_vector_free(prevDerivLayer);
        gsl_vector_free(derivLayer);
        gsl_vector_free(differenceLayer);

        gsl_vector_free(z[0]);

        // Compute new weights

        for (int l = 1; l <= layer; l++){
                if (regularizationMask & 1){
                        gsl_matrix* L1 = gsl_matrix_calloc(weightPtrs[l]->size1, weightPtrs[l]->size2);
                        gsl_matrix_set_all(L1,alpha);

                        gsl_matrix_sub(weightPtrs[l], L1);
                        gsl_matrix_free(L1);
                }
                if (regularizationMask & 2){
                        gsl_matrix* L2 = gsl_matrix_calloc(weightPtrs[l]->size1, weightPtrs[l]->size2);
                        gsl_matrix_memcpy(L2, weightPtrs[l]);
                        gsl_matrix_scale(L2, beta);

                        gsl_matrix_sub(weightPtrs[l], L2);
                        gsl_matrix_free(L2);
                }
                               gsl_matrix_sub(weightPtrs[l],offsetMatrix[l]);
                               gsl_matrix_free(offsetMatrix[l]);
                               gsl_vector_free(a[l]);
                               gsl_vector_free(z[l]);
                       }

                       return MSE;
               }


#endif
