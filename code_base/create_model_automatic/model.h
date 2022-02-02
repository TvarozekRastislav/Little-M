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
                        float kernels[11] = { 0 };
                        float decisions[3] = { 0 };
                        int votes[3] = { 0 };
                        kernels[0] = compute_kernel(x,   -54.0  , -30.0  , -75.0 );
                        kernels[1] = compute_kernel(x,   -61.0  , -55.0  , -88.0 );
                        kernels[2] = compute_kernel(x,   -68.0  , -23.0  , -64.0 );
                        kernels[3] = compute_kernel(x,   -58.0  , -41.0  , -72.0 );
                        kernels[4] = compute_kernel(x,   -60.0  , -16.0  , -75.0 );
                        kernels[5] = compute_kernel(x,   -64.0  , -38.0  , -71.0 );
                        kernels[6] = compute_kernel(x,   -56.0  , -31.0  , -70.0 );
                        kernels[7] = compute_kernel(x,   -77.0  , -18.0  , -74.0 );
                        kernels[8] = compute_kernel(x,   -70.0  , -27.0  , -64.0 );
                        kernels[9] = compute_kernel(x,   -64.0  , -47.0  , -62.0 );
                        kernels[10] = compute_kernel(x,   -68.0  , -28.0  , -73.0 );
                        decisions[0] = -2.914606925503
                        + kernels[0] * 0.087591753052
                        + kernels[3] * -0.024459128047
                        + kernels[4] * -0.022672095078
                        + kernels[6] * -0.040460529926
                        ;
                        decisions[1] = 3.542539235923
                        + kernels[0] * 0.009619075065
                        + kernels[1] * 0.00107370446
                        + kernels[9] * -0.002202369232
                        + kernels[10] * -0.008490410293
                        ;
                        decisions[2] = 20.410007720796
                        + kernels[2] * 0.717829713284
                        + kernels[5] * 0.692058022268
                        + kernels[7] * 0.444974883969
                        + kernels[8] * -0.854862619521
                        - kernels[10]
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