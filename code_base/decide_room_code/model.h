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
                        kernels[0] = compute_kernel(x,   -33.0  , -34.0  , -72.0 );
                        kernels[1] = compute_kernel(x,   -56.0  , -52.0  , -91.0 );
                        kernels[2] = compute_kernel(x,   -39.0  , -73.0  , -67.0 );
                        kernels[3] = compute_kernel(x,   -28.0  , -55.0  , -76.0 );
                        kernels[4] = compute_kernel(x,   -48.0  , -62.0  , -76.0 );
                        kernels[5] = compute_kernel(x,   -53.0  , -75.0  , -75.0 );
                        kernels[6] = compute_kernel(x,   -58.0  , -71.0  , -61.0 );
                        kernels[7] = compute_kernel(x,   -28.0  , -74.0  , -62.0 );
                        decisions[0] = -0.483139885926
                        + kernels[0] * 0.002496211333
                        + kernels[1] * 0.0040523613
                        + kernels[3] * -0.001943262755
                        + kernels[4] * -0.004605309878
                        ;
                        decisions[1] = -0.078085642317
                        + kernels[1] * 0.002518891688
                        + kernels[5] * -0.002518891688
                        ;
                        decisions[2] = 70.404871443699
                        + kernels[2]
                        + kernels[5] * -0.366758521776
                        + kernels[6] * -0.064615169479
                        + kernels[7] * -0.568626308744
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