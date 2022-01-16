#pragma once
#include <cstdarg>
namespace Eloquent {
    namespace ML {
        namespace Port {
            class SVM {
                public:
                    /**
                    * Predict class for features vector
                    */
                    int predict(float *x) {
                        float kernels[8] = { 0 };
                        float decisions[3] = { 0 };
                        int votes[3] = { 0 };
                        kernels[0] = compute_kernel(x,   -47.0  , -50.0  , -77.0 );
                        kernels[1] = compute_kernel(x,   -22.0  , -56.0  , -75.0 );
                        kernels[2] = compute_kernel(x,   -29.0  , -73.0  , -71.0 );
                        kernels[3] = compute_kernel(x,   -57.0  , -59.0  , -67.0 );
                        kernels[4] = compute_kernel(x,   -45.0  , -62.0  , -67.0 );
                        kernels[5] = compute_kernel(x,   -52.0  , -65.0  , -70.0 );
                        kernels[6] = compute_kernel(x,   -55.0  , -64.0  , -80.0 );
                        kernels[7] = compute_kernel(x,   -39.0  , -64.0  , -60.0 );
                        decisions[0] = -3.414607657558
                        + kernels[0] * 0.016649712724
                        + kernels[1] * -0.00586248447
                        + kernels[3] * -0.010787228254
                        ;
                        decisions[1] = 7.04131386954
                        + kernels[0] * 0.009293107183
                        + kernels[6] * -0.006629838661
                        + kernels[7] * -0.002663268522
                        ;
                        decisions[2] = 24.217125778397
                        + kernels[2] * 0.615017252248
                        + kernels[3]
                        + kernels[4] * 0.288326890889
                        + kernels[5] * -0.231038481431
                        + kernels[6] * -0.672305661706
                        - kernels[7]
                        ;
                        votes[decisions[0] > 0 ? 0 : 1] += 1;
                        votes[decisions[1] > 0 ? 0 : 2] += 1;
                        votes[decisions[2] > 0 ? 1 : 2] += 1;
                        int val = votes[0];
                        int idx = 0;

                        for (int i = 1; i < 3; i++) {
                            if (votes[i] > val) {
                                val = votes[i];
                                idx = i;
                            }
                        }

                        return idx;
                    }

                    /**
                    * Predict readable class name
                    */
                    const char* predictLabel(float *x) {
                        return idxToLabel(predict(x));
                    }

                    /**
                    * Convert class idx to readable name
                    */
                    const char* idxToLabel(uint8_t classIdx) {
                        switch (classIdx) {
                            case 0:
                            return "1";
                            case 1:
                            return "2";
                            case 2:
                            return "3";
                            default:
                            return "Houston we have a problem";
                        }
                    }

                protected:
                    /**
                    * Compute kernel between feature vector and support vector.
                    * Kernel type: linear
                    */
                    float compute_kernel(float *x, ...) {
                        va_list w;
                        va_start(w, 3);
                        float kernel = 0.0;

                        for (uint16_t i = 0; i < 3; i++) {
                            kernel += x[i] * va_arg(w, double);
                        }

                        return kernel;
                    }
                };
            }
        }
    }