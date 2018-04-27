// Blynk Auth
char auth[] = "fa550e093d734cd890cdc84c7208825a";

//GPS Baud Rates
static const uint32_t GPSBaud = 9600;

//Definition for Geo Calculatios
#ifndef DEGTORAD
#define DEGTORAD 0.0174532925199432957f
#define RADTODEG 57.295779513082320876f
#endif


// Pin variables
#define SERVO_PIN 3

#define GPS_TX_PIN 6
#define GPS_RX_PIN 5

#define BLUETOOTH_TX_PIN 10
#define BLUETOOTH_RX_PIN 11

#define MOTOR_A_EN_PIN 5
#define MOTOR_B_EN_PIN 9
#define MOTOR_A_IN_1_PIN 7
#define MOTOR_A_IN_2_PIN 8
#define MOTOR_B_IN_1_PIN 12
#define MOTOR_B_IN_2_PIN 4

// If one motor tends to spin faster than the other, add offset
#define MOTOR_A_OFFSET 20
#define MOTOR_B_OFFSET 0

// You must then add your 'Declination Angle' to the compass, which is the 'Error' of the magnetic field in your location.
// Find yours here: http://www.magnetic-declination.com/
// Mine is: 13° 24' E (Positive), which is ~13 Degrees, or (which we need) 0.23 radians
#define DECLINATION_ANGLE -0.23f

// The offset of the mounting position to true north
// It would be best to run the /examples/magsensor sketch and compare to the compass on your smartphone
#define COMPASS_OFFSET 2.4f

// How often the GPS should update in MS
// Keep this above 1000
#define GPS_UPDATE_INTERVAL 1000

// Number of changes in movement to timeout for GPS streaming
// Keeps the cooler from driving away if there is a problem
#define GPS_STREAM_TIMEOUT 18

// Number of changes in movement to timeout for GPS waypoints
// Keeps the cooler from driving away if there is a problem
#define GPS_WAYPOINT_TIMEOUT 45

// PWM write for servo locations
#define SERVO_LID_OPEN 20
#define SERVO_LID_CLOSE 165

// Definitions (don't edit these)
struct GeoLoc {
  float lat;
  float lon;
};

enum CoolerLid {
  OPENED,
  CLOSED
};

