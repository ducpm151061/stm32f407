/*
 * SimpleKalmanFilter - a Kalman Filter implementation for single variable models.
 * Created by Denys Sene, January, 1, 2017.
 * Released under MIT License - see LICENSE file for details.
 */

#include <math.h>
#include <stdlib.h>
#include <string.h>
#include "SimpleKalmanFilter.h"

void kalman_init(kalman *km)
{
    if (km == NULL)
        return;
    memset(km, 0, sizeof(kalman));
    km->err_measure = 2;
    km->err_estimate = 2;
    km->q = 0.001;
}

float kalman_update(kalman *km, float mea)
{
    if (km == NULL)
        return mea;
    km->kalman_gain = km->err_estimate / (km->err_estimate + km->err_measure);
    km->current_estimate = km->last_estimate + km->kalman_gain * (mea - km->last_estimate);
    km->err_estimate = (1.0 - km->kalman_gain) * km->err_estimate + fabs(km->last_estimate - km->current_estimate) * km->q;
    km->last_estimate = km->current_estimate;

    return km->current_estimate;
}
