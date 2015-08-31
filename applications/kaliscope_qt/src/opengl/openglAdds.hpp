#ifndef _KALI_OPENGLADDS_HPP_
#define	_KALI_OPENGLADDS_HPP_

int glhProjectf(float objx, float objy, float objz, float *modelview, float *projection, int *viewport, float *windowCoordinate);
int glhUnProjectf(float winx, float winy, float winz, float *modelview, float *projection, int *viewport, float *objectCoordinate);

#endif
