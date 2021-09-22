/* 
 * SimpleKalmanFilter - a Kalman Filter implementation for single variable models.
 * Created by Denys Sene, January, 1, 2017.
 * Released under MIT License - see LICENSE file for details.
 */

#ifndef __SIMPLEKALMANFILTER_H__
#define __SIMPLEKALMANFILTER_H__

typedef struct
{
  float err_measure;
  float err_estimate;
  float q;
  float current_estimate;
  float last_estimate;
  float kalman_gain;

} kalman;

void kalman_init(kalman *km);
float kalman_update(kalman *k, float mea);

#endif // __SIMPLEKALMANFILTER_H__
