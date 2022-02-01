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
                        float kernels[6] = { 0 };
                        float decisions[3] = { 0 };
                        int votes[3] = { 0 };
                        kernels[0] = compute_kernel(x,   -46.0  , -37.0  , -76.0 );
                        kernels[1] = compute_kernel(x,   -50.0  , -48.0  , -74.0 );
                        kernels[2] = compute_kernel(x,   -63.0  , -23.0  , -60.0 );
                        kernels[3] = compute_kernel(x,   -63.0  , -34.0  , -73.0 );
                        kernels[4] = compute_kernel(x,   -72.0  , -57.0  , -74.0 );
                        kernels[5] = compute_kernel(x,   -71.0  , -40.0  , -60.0 );
                        decisions[0] = 3.133887638388
                        + kernels[0] * 0.004800221433
                        + kernels[1] * 0.001975795352
                        + kernels[3] * -0.006776016785
                        ;
                        decisions[1] = 4.378476520676
                        + kernels[1] * 0.003984988332
                        + kernels[4] * -0.002551372413
                        + kernels[5] * -0.001433615919
                        ;
                        decisions[2] = 2.972596236506
                        + kernels[2] * 0.003058541904
                        + kernels[3] * 0.005544184987
                        + kernels[5] * -0.008602726891
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