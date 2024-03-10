#include "utils.h"

bool SVM(float alpha, float beta, Iph_ABC_t *t)
{
    int Sextant;

    if (beta >= 0.0f) {
        if (alpha >= 0.0f) {
            //quadrant I
            if (one_by_sqrt3 * beta > alpha)
                Sextant = 2; //sextant v2-v3
            else
                Sextant = 1; //sextant v1-v2
        } else {
            //quadrant II
            if (-one_by_sqrt3 * beta > alpha)
                Sextant = 3; //sextant v3-v4
            else
                Sextant = 2; //sextant v2-v3
        }
    } else {
        if (alpha >= 0.0f) {
            //quadrant IV
            if (-one_by_sqrt3 * beta > alpha)
                Sextant = 5; //sextant v5-v6
            else
                Sextant = 6; //sextant v6-v1
        } else {
            //quadrant III
            if (one_by_sqrt3 * beta > alpha)
                Sextant = 4; //sextant v4-v5
            else
                Sextant = 5; //sextant v5-v6
        }
    }

    switch (Sextant) {
        // sextant v1-v2
        case 1: {
            // Vector on-times
            float t1 = alpha - one_by_sqrt3 * beta;
            float t2 = two_by_sqrt3 * beta;

            // PWM timings
            t->phA = (1.0f - t1 - t2) * 0.5f;
            t->phB = t->phA + t1;
            t->phC = t->phB + t2;
        } break;

            // sextant v2-v3
        case 2: {
            // Vector on-times
            float t2 = alpha + one_by_sqrt3 * beta;
            float t3 = -alpha + one_by_sqrt3 * beta;

            // PWM timings
            t->phB = (1.0f - t2 - t3) * 0.5f;
            t->phA = t->phB + t3;
            t->phC = t->phA + t2;
        } break;

            // sextant v3-v4
        case 3: {
            // Vector on-times
            float t3 = two_by_sqrt3 * beta;
            float t4 = -alpha - one_by_sqrt3 * beta;

            // PWM timings
            t->phB = (1.0f - t3 - t4) * 0.5f;
            t->phC = t->phB + t3;
            t->phA = t->phC + t4;
        } break;

            // sextant v4-v5
        case 4: {
            // Vector on-times
            float t4 = -alpha + one_by_sqrt3 * beta;
            float t5 = -two_by_sqrt3 * beta;

            // PWM timings
            t->phC = (1.0f - t4 - t5) * 0.5f;
            t->phB = t->phC + t5;
            t->phA = t->phB + t4;
        } break;

            // sextant v5-v6
        case 5: {
            // Vector on-times
            float t5 = -alpha - one_by_sqrt3 * beta;
            float t6 = alpha - one_by_sqrt3 * beta;

            // PWM timings
            t->phC = (1.0f - t5 - t6) * 0.5f;
            t->phA = t->phC + t5;
            t->phB = t->phA + t6;
        } break;

            // sextant v6-v1
        case 6: {
            // Vector on-times
            float t6 = -two_by_sqrt3 * beta;
            float t1 = alpha + one_by_sqrt3 * beta;

            // PWM timings
            t->phA = (1.0f - t6 - t1) * 0.5f;
            t->phC = t->phA + t1;
            t->phB = t->phC + t6;
        } break;
    }

    bool result_valid =
            t->phA >= 0.0f && t->phA <= 1.0f
            && t->phB >= 0.0f && t->phB <= 1.0f
            && t->phC >= 0.0f && t->phC <= 1.0f;
    return result_valid;
}