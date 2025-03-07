#include "utils.h"

#define SIN_TABLE_SIZE 512
static const float sinTable_f32[SIN_TABLE_SIZE + 1] = {
    0.00000000f, 0.01227154f, 0.02454123f, 0.03680722f, 0.04906767f, 0.06132074f,
    0.07356456f, 0.08579731f, 0.09801714f, 0.11022221f, 0.12241068f, 0.13458071f,
    0.14673047f, 0.15885814f, 0.17096189f, 0.18303989f, 0.19509032f, 0.20711138f,
    0.21910124f, 0.23105811f, 0.24298018f, 0.25486566f, 0.26671276f, 0.27851969f,
    0.29028468f, 0.30200595f, 0.31368174f, 0.32531029f, 0.33688985f, 0.34841868f,
    0.35989504f, 0.37131719f, 0.38268343f, 0.39399204f, 0.40524131f, 0.41642956f,
    0.42755509f, 0.43861624f, 0.44961133f, 0.46053871f, 0.47139674f, 0.48218377f,
    0.49289819f, 0.50353838f, 0.51410274f, 0.52458968f, 0.53499762f, 0.54532499f,
    0.55557023f, 0.56573181f, 0.57580819f, 0.58579786f, 0.59569930f, 0.60551104f,
    0.61523159f, 0.62485949f, 0.63439328f, 0.64383154f, 0.65317284f, 0.66241578f,
    0.67155895f, 0.68060100f, 0.68954054f, 0.69837625f, 0.70710678f, 0.71573083f,
    0.72424708f, 0.73265427f, 0.74095113f, 0.74913639f, 0.75720885f, 0.76516727f,
    0.77301045f, 0.78073723f, 0.78834643f, 0.79583690f, 0.80320753f, 0.81045720f,
    0.81758481f, 0.82458930f, 0.83146961f, 0.83822471f, 0.84485357f, 0.85135519f,
    0.85772861f, 0.86397286f, 0.87008699f, 0.87607009f, 0.88192126f, 0.88763962f,
    0.89322430f, 0.89867447f, 0.90398929f, 0.90916798f, 0.91420976f, 0.91911385f,
    0.92387953f, 0.92850608f, 0.93299280f, 0.93733901f, 0.94154407f, 0.94560733f,
    0.94952818f, 0.95330604f, 0.95694034f, 0.96043052f, 0.96377607f, 0.96697647f,
    0.97003125f, 0.97293995f, 0.97570213f, 0.97831737f, 0.98078528f, 0.98310549f,
    0.98527764f, 0.98730142f, 0.98917651f, 0.99090264f, 0.99247953f, 0.99390697f,
    0.99518473f, 0.99631261f, 0.99729046f, 0.99811811f, 0.99879546f, 0.99932238f,
    0.99969882f, 0.99992470f, 1.00000000f, 0.99992470f, 0.99969882f, 0.99932238f,
    0.99879546f, 0.99811811f, 0.99729046f, 0.99631261f, 0.99518473f, 0.99390697f,
    0.99247953f, 0.99090264f, 0.98917651f, 0.98730142f, 0.98527764f, 0.98310549f,
    0.98078528f, 0.97831737f, 0.97570213f, 0.97293995f, 0.97003125f, 0.96697647f,
    0.96377607f, 0.96043052f, 0.95694034f, 0.95330604f, 0.94952818f, 0.94560733f,
    0.94154407f, 0.93733901f, 0.93299280f, 0.92850608f, 0.92387953f, 0.91911385f,
    0.91420976f, 0.90916798f, 0.90398929f, 0.89867447f, 0.89322430f, 0.88763962f,
    0.88192126f, 0.87607009f, 0.87008699f, 0.86397286f, 0.85772861f, 0.85135519f,
    0.84485357f, 0.83822471f, 0.83146961f, 0.82458930f, 0.81758481f, 0.81045720f,
    0.80320753f, 0.79583690f, 0.78834643f, 0.78073723f, 0.77301045f, 0.76516727f,
    0.75720885f, 0.74913639f, 0.74095113f, 0.73265427f, 0.72424708f, 0.71573083f,
    0.70710678f, 0.69837625f, 0.68954054f, 0.68060100f, 0.67155895f, 0.66241578f,
    0.65317284f, 0.64383154f, 0.63439328f, 0.62485949f, 0.61523159f, 0.60551104f,
    0.59569930f, 0.58579786f, 0.57580819f, 0.56573181f, 0.55557023f, 0.54532499f,
    0.53499762f, 0.52458968f, 0.51410274f, 0.50353838f, 0.49289819f, 0.48218377f,
    0.47139674f, 0.46053871f, 0.44961133f, 0.43861624f, 0.42755509f, 0.41642956f,
    0.40524131f, 0.39399204f, 0.38268343f, 0.37131719f, 0.35989504f, 0.34841868f,
    0.33688985f, 0.32531029f, 0.31368174f, 0.30200595f, 0.29028468f, 0.27851969f,
    0.26671276f, 0.25486566f, 0.24298018f, 0.23105811f, 0.21910124f, 0.20711138f,
    0.19509032f, 0.18303989f, 0.17096189f, 0.15885814f, 0.14673047f, 0.13458071f,
    0.12241068f, 0.11022221f, 0.09801714f, 0.08579731f, 0.07356456f, 0.06132074f,
    0.04906767f, 0.03680722f, 0.02454123f, 0.01227154f, 0.00000000f, -0.01227154f,
    -0.02454123f, -0.03680722f, -0.04906767f, -0.06132074f, -0.07356456f,
    -0.08579731f, -0.09801714f, -0.11022221f, -0.12241068f, -0.13458071f,
    -0.14673047f, -0.15885814f, -0.17096189f, -0.18303989f, -0.19509032f,
    -0.20711138f, -0.21910124f, -0.23105811f, -0.24298018f, -0.25486566f,
    -0.26671276f, -0.27851969f, -0.29028468f, -0.30200595f, -0.31368174f,
    -0.32531029f, -0.33688985f, -0.34841868f, -0.35989504f, -0.37131719f,
    -0.38268343f, -0.39399204f, -0.40524131f, -0.41642956f, -0.42755509f,
    -0.43861624f, -0.44961133f, -0.46053871f, -0.47139674f, -0.48218377f,
    -0.49289819f, -0.50353838f, -0.51410274f, -0.52458968f, -0.53499762f,
    -0.54532499f, -0.55557023f, -0.56573181f, -0.57580819f, -0.58579786f,
    -0.59569930f, -0.60551104f, -0.61523159f, -0.62485949f, -0.63439328f,
    -0.64383154f, -0.65317284f, -0.66241578f, -0.67155895f, -0.68060100f,
    -0.68954054f, -0.69837625f, -0.70710678f, -0.71573083f, -0.72424708f,
    -0.73265427f, -0.74095113f, -0.74913639f, -0.75720885f, -0.76516727f,
    -0.77301045f, -0.78073723f, -0.78834643f, -0.79583690f, -0.80320753f,
    -0.81045720f, -0.81758481f, -0.82458930f, -0.83146961f, -0.83822471f,
    -0.84485357f, -0.85135519f, -0.85772861f, -0.86397286f, -0.87008699f,
    -0.87607009f, -0.88192126f, -0.88763962f, -0.89322430f, -0.89867447f,
    -0.90398929f, -0.90916798f, -0.91420976f, -0.91911385f, -0.92387953f,
    -0.92850608f, -0.93299280f, -0.93733901f, -0.94154407f, -0.94560733f,
    -0.94952818f, -0.95330604f, -0.95694034f, -0.96043052f, -0.96377607f,
    -0.96697647f, -0.97003125f, -0.97293995f, -0.97570213f, -0.97831737f,
    -0.98078528f, -0.98310549f, -0.98527764f, -0.98730142f, -0.98917651f,
    -0.99090264f, -0.99247953f, -0.99390697f, -0.99518473f, -0.99631261f,
    -0.99729046f, -0.99811811f, -0.99879546f, -0.99932238f, -0.99969882f,
    -0.99992470f, -1.00000000f, -0.99992470f, -0.99969882f, -0.99932238f,
    -0.99879546f, -0.99811811f, -0.99729046f, -0.99631261f, -0.99518473f,
    -0.99390697f, -0.99247953f, -0.99090264f, -0.98917651f, -0.98730142f,
    -0.98527764f, -0.98310549f, -0.98078528f, -0.97831737f, -0.97570213f,
    -0.97293995f, -0.97003125f, -0.96697647f, -0.96377607f, -0.96043052f,
    -0.95694034f, -0.95330604f, -0.94952818f, -0.94560733f, -0.94154407f,
    -0.93733901f, -0.93299280f, -0.92850608f, -0.92387953f, -0.91911385f,
    -0.91420976f, -0.90916798f, -0.90398929f, -0.89867447f, -0.89322430f,
    -0.88763962f, -0.88192126f, -0.87607009f, -0.87008699f, -0.86397286f,
    -0.85772861f, -0.85135519f, -0.84485357f, -0.83822471f, -0.83146961f,
    -0.82458930f, -0.81758481f, -0.81045720f, -0.80320753f, -0.79583690f,
    -0.78834643f, -0.78073723f, -0.77301045f, -0.76516727f, -0.75720885f,
    -0.74913639f, -0.74095113f, -0.73265427f, -0.72424708f, -0.71573083f,
    -0.70710678f, -0.69837625f, -0.68954054f, -0.68060100f, -0.67155895f,
    -0.66241578f, -0.65317284f, -0.64383154f, -0.63439328f, -0.62485949f,
    -0.61523159f, -0.60551104f, -0.59569930f, -0.58579786f, -0.57580819f,
    -0.56573181f, -0.55557023f, -0.54532499f, -0.53499762f, -0.52458968f,
    -0.51410274f, -0.50353838f, -0.49289819f, -0.48218377f, -0.47139674f,
    -0.46053871f, -0.44961133f, -0.43861624f, -0.42755509f, -0.41642956f,
    -0.40524131f, -0.39399204f, -0.38268343f, -0.37131719f, -0.35989504f,
    -0.34841868f, -0.33688985f, -0.32531029f, -0.31368174f, -0.30200595f,
    -0.29028468f, -0.27851969f, -0.26671276f, -0.25486566f, -0.24298018f,
    -0.23105811f, -0.21910124f, -0.20711138f, -0.19509032f, -0.18303989f,
    -0.17096189f, -0.15885814f, -0.14673047f, -0.13458071f, -0.12241068f,
    -0.11022221f, -0.09801714f, -0.08579731f, -0.07356456f, -0.06132074f,
    -0.04906767f, -0.03680722f, -0.02454123f, -0.01227154f, -0.00000000f};

float sat1_datf(float val, float up, float low)
{
    if (val > up)
        return up;
    else if (val < low)
        return low;
    else
        return val;
}

float fast_atan2(float y, float x)
{
    float abs_y = fabsf(y);
    float abs_x = fabsf(x);
    float a = fminf(abs_x, abs_y) / (fmaxf(abs_x, abs_y) + FLT_MIN);
    float s = a * a;
    float r = ((-0.0464964749f * s + 0.15931422f) * s - 0.327622764f) * s * a + a;
    if (abs_y > abs_x)
        r = 1.57079637f - r;
    if (x < 0.0f)
        r = 3.14159274f - r;
    if (y < 0.0f)
        r = -r;

    return r;
}

float fast_sin_f32(float x)
{
    float sinVal, fract, in; /* Temporary variables for input, output */
    uint16_t index;          /* Index variable */
    float a, b;              /* Two nearest output values */
    int32_t n;
    float findex;

    /* input x is in radians */
    /* Scale the input to [0 1] range from [0 2*PI] , divide input by 2*pi */
    in = x * 0.159154943092f;

    /* Calculation of floor value of input */
    n = (int32_t)in;

    /* Make negative values towards -infinity */
    if (x < 0.0f)
    {
        n--;
    }

    /* Map input value to [0 1] */
    in = in - (float)n;

    /* Calculation of index of the table */
    findex = (float)SIN_TABLE_SIZE * in;
    index = (uint16_t)findex;

    /* when "in" is exactly 1, we need to rotate the index down to 0 */
    if (index >= SIN_TABLE_SIZE)
    {
        index = 0;
        findex -= (float)SIN_TABLE_SIZE;
    }

    /* fractional value calculation */
    fract = findex - (float)index;

    /* Read two nearest values of input value from the sin table */
    a = sinTable_f32[index];
    b = sinTable_f32[index + 1];

    /* Linear interpolation process */
    sinVal = (1.0f - fract) * a + fract * b;

    /* Return the output value */
    return (sinVal);
}

float fast_cos_f32(float x)
{
    float cosVal, fract, in; /* Temporary variables for input, output */
    uint16_t index;          /* Index variable */
    float a, b;              /* Two nearest output values */
    int32_t n;
    float findex;

    /* input x is in radians */
    /* Scale the input to [0 1] range from [0 2*PI] , divide input by 2*pi, add 0.25 (pi/2) to read sine table */
    in = x * 0.159154943092f + 0.25f;

    /* Calculation of floor value of input */
    n = (int32_t)in;

    /* Make negative values towards -infinity */
    if (in < 0.0f)
    {
        n--;
    }

    /* Map input value to [0 1] */
    in = in - (float)n;

    /* Calculation of index of the table */
    findex = (float)SIN_TABLE_SIZE * in;
    index = (uint16_t)findex;

    /* when "in" is exactly 1, we need to rotate the index down to 0 */
    if (index >= SIN_TABLE_SIZE)
    {
        index = 0;
        findex -= (float)SIN_TABLE_SIZE;
    }

    /* fractional value calculation */
    fract = findex - (float)index;

    /* Read two nearest values of input value from the cos table */
    a = sinTable_f32[index];
    b = sinTable_f32[index + 1];

    /* Linear interpolation process */
    cosVal = (1.0f - fract) * a + fract * b;

    /* Return the output value */
    return (cosVal);
}

int mod(const int dividend, const int divisor)
{
    int r = dividend % divisor;
    if (r < 0)
        r += divisor;
    return r;
}

float wrap_pm(float x, float y)
{
#ifdef FPU_FPV4
    float intval = (float)round_int(x / y);
#else
    float intval = nearbyintf(x / y);
#endif
    return x - intval * y;
}

float fmodf_pos(float x, float y)
{
    float res = wrap_pm(x, y);
    if (res < 0)
        res += y;
    return res;
}

float wrap_pm_pi(float x)
{
    return wrap_pm(x, 2 * M_PI);
}

int svm(float alpha, float beta, float *tA, float *tB, float *tC)
{
    int Sextant;

    if (beta >= 0.0f)
    {
        if (alpha >= 0.0f)
        {
            // quadrant I
            if (ONE_BY_SQRT3 * beta > alpha)
            {
                Sextant = 2; // sextant v2-v3
            }
            else
            {
                Sextant = 1; // sextant v1-v2
            }
        }
        else
        {
            // quadrant II
            if (-ONE_BY_SQRT3 * beta > alpha)
            {
                Sextant = 3; // sextant v3-v4
            }
            else
            {
                Sextant = 2; // sextant v2-v3
            }
        }
    }
    else
    {
        if (alpha >= 0.0f)
        {
            // quadrant IV
            if (-ONE_BY_SQRT3 * beta > alpha)
            {
                Sextant = 5; // sextant v5-v6
            }
            else
            {
                Sextant = 6; // sextant v6-v1
            }
        }
        else
        {
            // quadrant III
            if (ONE_BY_SQRT3 * beta > alpha)
            {
                Sextant = 4; // sextant v4-v5
            }
            else
            {
                Sextant = 5; // sextant v5-v6
            }
        }
    }

    switch (Sextant)
    {
    // sextant v1-v2
    case 1:
    {
        // Vector on-times
        float t1 = alpha - ONE_BY_SQRT3 * beta;
        float t2 = TWO_BY_SQRT3 * beta;
        // PWM timings
        *tA = (1.0f - t1 - t2) * 0.5f;
        *tB = *tA + t1;
        *tC = *tB + t2;
    }
    break;

    // sextant v2-v3
    case 2:
    {
        // Vector on-times
        float t2 = alpha + ONE_BY_SQRT3 * beta;
        float t3 = -alpha + ONE_BY_SQRT3 * beta;
        // PWM timings
        *tB = (1.0f - t2 - t3) * 0.5f;
        *tA = *tB + t3;
        *tC = *tA + t2;
    }
    break;

    // sextant v3-v4
    case 3:
    {
        // Vector on-times
        float t3 = TWO_BY_SQRT3 * beta;
        float t4 = -alpha - ONE_BY_SQRT3 * beta;
        // PWM timings
        *tB = (1.0f - t3 - t4) * 0.5f;
        *tC = *tB + t3;
        *tA = *tC + t4;
    }
    break;

    // sextant v4-v5
    case 4:
    {
        // Vector on-times
        float t4 = -alpha + ONE_BY_SQRT3 * beta;
        float t5 = -TWO_BY_SQRT3 * beta;
        // PWM timings
        *tC = (1.0f - t4 - t5) * 0.5f;
        *tB = *tC + t5;
        *tA = *tB + t4;
    }
    break;

    // sextant v5-v6
    case 5:
    {
        // Vector on-times
        float t5 = -alpha - ONE_BY_SQRT3 * beta;
        float t6 = alpha - ONE_BY_SQRT3 * beta;
        // PWM timings
        *tC = (1.0f - t5 - t6) * 0.5f;
        *tA = *tC + t5;
        *tB = *tA + t6;
    }
    break;

    // sextant v6-v1
    case 6:
    {
        // Vector on-times
        float t6 = -TWO_BY_SQRT3 * beta;
        float t1 = alpha + ONE_BY_SQRT3 * beta;
        // PWM timings
        *tA = (1.0f - t6 - t1) * 0.5f;
        *tC = *tA + t1;
        *tB = *tC + t6;
    }
    break;
    }

    // if any of the results becomes NaN, result_valid will evaluate to false
    int result_valid = *tA >= 0.0f && *tA <= 1.0f && *tB >= 0.0f && *tB <= 1.0f && *tC >= 0.0f && *tC <= 1.0f;
    return result_valid ? 0 : -1;
}
