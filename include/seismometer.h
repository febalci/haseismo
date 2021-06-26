#ifndef __SEISMOMETER_H
#define __SEISMOMETER_H

/**
 * Initial power-on sequence and calibration (if needed)
 */

class seismometer{
  public:
    void init();
    void readaccel();
    void calibrate();

    float xy_vector_mag, z_vector_mag;

  private:
    void meansensors();
    void calibration();
};

#endif