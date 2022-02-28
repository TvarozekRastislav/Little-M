#pragma once
#include <stdarg.h>
namespace Eloquent {
    namespace ML {
        namespace Port {
            class SVM {
                public:
                    /**
                    * Predict class for features vector
                    */
                    int predict(float *x) {
                        float kernels[10] = { 0 };
                        float decisions[3] = { 0 };
                        int votes[3] = { 0 };
                        kernels[0] = compute_kernel(x,   -52.0  , -37.0  , -30.0 );
                        kernels[1] = compute_kernel(x,   -55.0  , -57.0  , -35.0 );
                        kernels[2] = compute_kernel(x,   -63.0  , -59.0  , -28.0 );
                        kernels[3] = compute_kernel(x,   -70.0  , -44.0  , -52.0 );
                        kernels[4] = compute_kernel(x,   -64.0  , -52.0  , -30.0 );
                        kernels[5] = compute_kernel(x,   -51.0  , -55.0  , -32.0 );
                        kernels[6] = compute_kernel(x,   -63.0  , -65.0  , -31.0 );
                        kernels[7] = compute_kernel(x,   -56.0  , -61.0  , -31.0 );
                        kernels[8] = compute_kernel(x,   -67.0  , -64.0  , -34.0 );
                        kernels[9] = compute_kernel(x,   -52.0  , -72.0  , -40.0 );
                        decisions[0] = -10.346027391553
                        + kernels[0] * 0.351892240823
                        + kernels[1] * 0.939217767376
                        + kernels[2]
                        - kernels[4]
                        - kernels[5]
                        + kernels[6] * -0.227120931645
                        + kernels[7] * -0.063989076554
                        ;
                        decisions[1] = 21.899877797428
                        + kernels[2] * 0.026872988726
                        + kernels[3] * 0.003461006595
                        + kernels[8] * -0.03033399532
                        ;
                        decisions[2] = 27.565913998728
                        + kernels[6] * 0.109222108608
                        + kernels[8] * -0.092418707283
                        + kernels[9] * -0.016803401324
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