#include "common.h"

// default I2C address is 0x68
// specific I2C addresses may be passed as a parameter here
// AD0 low = 0x68 (default for InvenSense evaluation board)
// AD0 high = 0x69
//MPU6050 accelgyro;
MPU6050 accelgyro(0x69); // <-- use for AD0 high, connect AD0 to 3.3V

char custom_MPU6050_SCALE[custom_MPU6050_SCALE_LEN];

//Change this 3 variables if you want to fine tune the skecth to your needs.
#ifdef DEBUG
  #define buffersize 100
#else
  #define buffersize 1000 //Amount of readings used to average, make it higher to get more precision but sketch will be slower  (default:1000)
#endif
int acel_deadzone=8;     //Acelerometer error allowed, make it lower to get more precision, but sketch may not converge  (default:8)
int giro_deadzone=1;     //Giro error allowed, make it lower to get more precision, but sketch may not converge  (default:1)

int16_t ax, ay, az,gx, gy, gz;
int mean_ax,mean_ay,mean_az,mean_gx,mean_gy,mean_gz,state=0;
int ax_offset,ay_offset,az_offset,gx_offset,gy_offset,gz_offset;
float xy_vector_mag, x_vector_mag, y_vector_mag, z_vector_mag, scale_factor;


void seismometer::init(){
  accelgyro.initialize();
/*  LOW PASS FILTER
 *          |   ACCELEROMETER    |           GYROSCOPE
 * DLPF_CFG | Bandwidth | Delay  | Bandwidth | Delay  | Sample Rate
 * ---------+-----------+--------+-----------+--------+-------------
 * 0        | 260Hz     | 0ms    | 256Hz     | 0.98ms | 8kHz
 * 1        | 184Hz     | 2.0ms  | 188Hz     | 1.9ms  | 1kHz
 * 2        | 94Hz      | 3.0ms  | 98Hz      | 2.8ms  | 1kHz
 * 3        | 44Hz      | 4.9ms  | 42Hz      | 4.8ms  | 1kHz
 * 4        | 21Hz      | 8.5ms  | 20Hz      | 8.3ms  | 1kHz
 * 5        | 10Hz      | 13.8ms | 10Hz      | 13.4ms | 1kHz
 * 6        | 5Hz       | 19.0ms | 5Hz       | 18.6ms | 1kHz
 * 7        |   -- Reserved --   |   -- Reserved --   | Reserved
 */
  accelgyro.setDLPFMode (6); // MPU6050_DLPF_5HZ

/*  HIGH PASS FILTER
 * ACCEL_HPF | Filter Mode | Cut-off Frequency
 * ----------+-------------+------------------
 * 0         | Reset       | None
 * 1         | On          | 5Hz
 * 2         | On          | 2.5Hz
 * 3         | On          | 1.25Hz
 * 4         | On          | 0.63Hz
 * 7         | Hold        | None
 */
  accelgyro.setDHPFMode(4); // MPU6050_ACCEL_HPF_0_63HZ

/*  Set full-scale accelerometer range
 * 0 = +/- 2g
 * 1 = +/- 4g
 * 2 = +/- 8g
 * 3 = +/- 16g
 */
  accelgyro.setFullScaleAccelRange(0);
  accelgyro.setExternalFrameSync(0); // MPU6050_EXT_SYNC_DISABLED
  // wait for ready
  while (Serial.available() && Serial.read()); // empty buffer

  // start message
  Serial.println("\nMPU6050 Calibration Sketch");
  delay(2000);
  Serial.println("\nYour MPU6050 should be placed in horizontal position, with package letters facing up. \nDon't touch it until you see a finish message.\n");
  delay(3000);

  // verify connection
  Serial.println(accelgyro.testConnection() ? "MPU6050 connection successful" : "MPU6050 connection failed");
  delay(1000);
  // reset offsets
  accelgyro.setXAccelOffset(0);
  accelgyro.setYAccelOffset(0);
  accelgyro.setZAccelOffset(0);
  accelgyro.setXGyroOffset(0);
  accelgyro.setYGyroOffset(0);
  accelgyro.setZGyroOffset(0);

}

void seismometer::meansensors(){
  long i=0,buff_ax=0,buff_ay=0,buff_az=0,buff_gx=0,buff_gy=0,buff_gz=0;

  while (i<(buffersize+101)){
    // read raw accel/gyro measurements from device
    accelgyro.getMotion6(&ax, &ay, &az, &gx, &gy, &gz);
    
    if (i>100 && i<=(buffersize+100)){ //First 100 measures are discarded
      buff_ax=buff_ax+ax;
      buff_ay=buff_ay+ay;
      buff_az=buff_az+az;
      buff_gx=buff_gx+gx;
      buff_gy=buff_gy+gy;
      buff_gz=buff_gz+gz;
    }
    if (i==(buffersize+100)){
      mean_ax=buff_ax/buffersize;
      mean_ay=buff_ay/buffersize;
      mean_az=buff_az/buffersize;
      mean_gx=buff_gx/buffersize;
      mean_gy=buff_gy/buffersize;
      mean_gz=buff_gz/buffersize;
    }
    i++;
    delay(2); //Needed so we don't get repeated measures
  }
}

void seismometer::calibration(){
  ax_offset=-mean_ax/8;
  ay_offset=-mean_ay/8;
  az_offset=(16384-mean_az)/8;
 
  gx_offset=-mean_gx/4;
  gy_offset=-mean_gy/4;
  gz_offset=-mean_gz/4;
  while (1){
    int ready=0;
    accelgyro.setXAccelOffset(ax_offset);
    accelgyro.setYAccelOffset(ay_offset);
    accelgyro.setZAccelOffset(az_offset);
 
    accelgyro.setXGyroOffset(gx_offset);
    accelgyro.setYGyroOffset(gy_offset);
    accelgyro.setZGyroOffset(gz_offset);
 
    meansensors();
    Serial.println("...");
 
    if (abs(mean_ax)<=acel_deadzone) ready++;
    else ax_offset=ax_offset-mean_ax/acel_deadzone;
 
    if (abs(mean_ay)<=acel_deadzone) ready++;
    else ay_offset=ay_offset-mean_ay/acel_deadzone;
 
    if (abs(16384-mean_az)<=acel_deadzone) ready++;
    else az_offset=az_offset+(16384-mean_az)/acel_deadzone;
 
    if (abs(mean_gx)<=giro_deadzone) ready++;
    else gx_offset=gx_offset-mean_gx/(giro_deadzone+1);
 
    if (abs(mean_gy)<=giro_deadzone) ready++;
    else gy_offset=gy_offset-mean_gy/(giro_deadzone+1);
 
    if (abs(mean_gz)<=giro_deadzone) ready++;
    else gz_offset=gz_offset-mean_gz/(giro_deadzone+1);
 
    if (ready==6) break;
  }
}

void seismometer::calibrate(){
  if (state==0){
    Serial.println("\nReading sensors for first time...");
    meansensors();
    state++;
    delay(1000);
  }
 
  if (state==1) {
    Serial.println("\nCalculating offsets...");
    calibration();
    state++;
    delay(1000);
  }
 
  if (state==2) {
    meansensors();
    Serial.println("\nFINISHED!");
    Debug("\nSensor readings with offsets:\t");
    Debug(mean_ax); 
    Debug("\t");
    Debug(mean_ay); 
    Debug("\t");
    Debug(mean_az); 
    Debug("\t");
    Debug(mean_gx); 
    Debug("\t");
    Debug(mean_gy); 
    Debug("\t");
    Debugln(mean_gz);
    Debug("Your offsets:\t");
    Debug(ax_offset); 
    Debug("\t");
    Debug(ay_offset); 
    Debug("\t");
    Debug(az_offset); 
    Debug("\t");
    Debug(gx_offset); 
    Debug("\t");
    Debug(gy_offset); 
    Debug("\t");
    Debugln(gz_offset); 
    Debugln("\nData is printed as: acelX acelY acelZ giroX giroY giroZ");
    Debugln("Check that your sensor readings are close to 0 0 16384 0 0 0");
    Debugln("If calibration was succesful write down your offsets so you can set them in your projects using something similar to mpu.setXAccelOffset(youroffset)");
    state++;

    Debugln("ax,ay,az,gx,gy,gz");
  }
}

void seismometer::readaccel(){
  // read raw accel/gyro measurements from device
  accelgyro.getMotion6(&ax, &ay, &az, &gx, &gy, &gz);

  // these methods (and a few others) are also available
  //accelgyro.getAcceleration(&ax, &ay, &az);
  //accelgyro.getRotation(&gx, &gy, &gz);    
  scale_factor = atof(custom_MPU6050_SCALE); // scale_factor = pow(2, trimpot_val / 100.0); Between 1-1200
  x_vector_mag = ax / 16384.0;
  y_vector_mag = ay / 16384.0;
  z_vector_mag = az / 16384.0 - 1.0;
  xy_vector_mag = sqrt(x_vector_mag * x_vector_mag + y_vector_mag * y_vector_mag) * scale_factor;
  z_vector_mag = abs(z_vector_mag*scale_factor);
}
