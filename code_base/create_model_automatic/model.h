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
                        float kernels[16] = { 0 };
                        float decisions[3] = { 0 };
                        int votes[3] = { 0 };
                        kernels[0] = compute_kernel(x,   -61.0  , -55.0  , -88.0 );
                        kernels[1] = compute_kernel(x,   -54.0  , -30.0  , -75.0 );
                        kernels[2] = compute_kernel(x,   -60.0  , -16.0  , -75.0 );
                        kernels[3] = compute_kernel(x,   -56.0  , -31.0  , -70.0 );
                        kernels[4] = compute_kernel(x,   -77.0  , -18.0  , -74.0 );
                        kernels[5] = compute_kernel(x,   -58.0  , -41.0  , -72.0 );
                        kernels[6] = compute_kernel(x,   -75.0  , -19.0  , -74.0 );
                        kernels[7] = compute_kernel(x,   -64.0  , -38.0  , -71.0 );
                        kernels[8] = compute_kernel(x,   -68.0  , -28.0  , -69.0 );
                        kernels[9] = compute_kernel(x,   -68.0  , -23.0  , -64.0 );
                        kernels[10] = compute_kernel(x,   -67.0  , -46.0  , -66.0 );
                        kernels[11] = compute_kernel(x,   -66.0  , -44.0  , -66.0 );
                        kernels[12] = compute_kernel(x,   -64.0  , -47.0  , -62.0 );
                        kernels[13] = compute_kernel(x,   -77.0  , -22.0  , -67.0 );
                        kernels[14] = compute_kernel(x,   -67.0  , -37.0  , -67.0 );
                        kernels[15] = compute_kernel(x,   -68.0  , -28.0  , -73.0 );
                        decisions[0] = -2.914306173706
                        + kernels[1] * 0.087594890594
                        + kernels[2] * -0.022673268808
                        + kernels[3] * -0.040461017935
                        + kernels[5] * -0.024460603851
                        ;
                        decisions[1] = 3.545248120479
                        + kernels[0] * 0.001073631696
                        + kernels[1] * 0.009623424084
                        + kernels[12] * -0.002202967485
                        + kernels[15] * -0.008494088295
                        ;
                        decisions[2] = 13.672855815228
                        + kernels[4]
                        + kernels[6] * 9.3279572e-05
                        + kernels[7] * 0.193538892715
                        + kernels[8] * 0.456880140572
                        + kernels[9] * 0.602845139663
                        + kernels[10]
                        + kernels[11] * -0.253357452522
                        - kernels[13]
                        - kernels[14]
                        - kernels[15]
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