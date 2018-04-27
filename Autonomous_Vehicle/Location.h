#include <HardwareSerial.h>
#include <TinyGPS++.h>

// The TinyGPS++ object
TinyGPSPlus gps;

// The serial connection to the GPS device
HardwareSerial ss(0);
HardwareSerial rs(2);

// Structure for the gps from location information
struct GeoLocFrom {
  unsigned long utime;
  float lat;
  float lon;
};

// Structure for the gps TO location information
struct GeoLocTo {
  unsigned long utime;
  float lat;
  float lon;
};

// Display the LOCAL location information
void displayLInfo()
{
  Serial.print("LTIME: "); Serial.println(gps.time.value());
  Serial.print(F("Location: "));
  if (gps.location.isValid())
  {
    Serial.print(gps.location.lat(), 6);
    Serial.print(F(","));
    Serial.print(gps.location.lng(), 6);
  }
  else
  {
    Serial.print(F("INVALID"));
  }

  Serial.print(F("  Date/Time: "));
  if (gps.date.isValid())
  {
    Serial.print(gps.date.month());
    Serial.print(F("/"));
    Serial.print(gps.date.day());
    Serial.print(F("/"));
    Serial.print(gps.date.year());
  }
  else
  {
    Serial.print(F("INVALID"));
  }

  Serial.print(F(" "));
  if (gps.time.isValid())
  {
    if (gps.time.hour() < 10) Serial.print(F("0"));
    Serial.print(gps.time.hour());
    Serial.print(F(":"));
    if (gps.time.minute() < 10) Serial.print(F("0"));
    Serial.print(gps.time.minute());
    Serial.print(F(":"));
    if (gps.time.second() < 10) Serial.print(F("0"));
    Serial.print(gps.time.second());
    Serial.print(F("."));
    if (gps.time.centisecond() < 10) Serial.print(F("0"));
    Serial.print(gps.time.centisecond());
  }
  else
  {
    Serial.print(F("INVALID"));
  }

  Serial.println();
}

// Display the REMOTE location information
void displayRInfo()
{
  Serial.print("RTIME: "); Serial.println(gps.time.value());

  Serial.print(F("RLocation: "));
  if (gps.location.isValid())
  {
    Serial.print(gps.location.lat(), 6);
    Serial.print(F(","));
    Serial.print(gps.location.lng(), 6);
  }
  else
  {
    Serial.print(F("INVALID"));
  }

  Serial.print(F("  RDate/Time: "));
  if (gps.date.isValid())
  {
    Serial.print(gps.date.month());
    Serial.print(F("/"));
    Serial.print(gps.date.day());
    Serial.print(F("/"));
    Serial.print(gps.date.year());
  }
  else
  {
    Serial.print(F("INVALID"));
  }

  Serial.print(F(" "));
  if (gps.time.isValid())
  {
    if (gps.time.hour() < 10) Serial.print(F("0"));
    Serial.print(gps.time.hour());
    Serial.print(F(":"));
    if (gps.time.minute() < 10) Serial.print(F("0"));
    Serial.print(gps.time.minute());
    Serial.print(F(":"));
    if (gps.time.second() < 10) Serial.print(F("0"));
    Serial.print(gps.time.second());
    Serial.print(F("."));
    if (gps.time.centisecond() < 10) Serial.print(F("0"));
    Serial.print(gps.time.centisecond());
  }
  else
  {
    Serial.print(F("INVALID"));
  }

  Serial.println();
}

// Configure the Location information
void configLocation() {
  ss.begin(GPSBaud);
  rs.begin(GPSBaud);
  Serial.println(F("DeviceExample.ino"));
  Serial.println(F("A simple demonstration of TinyGPS++ with an attached GPS module"));
  Serial.print(F("Testing TinyGPS++ library v. ")); Serial.println(TinyGPSPlus::libraryVersion());
  Serial.println(F("by Mikal Hart"));
  Serial.println();
}

//Distance in meters
float geoDistance(struct GeoLocFrom & a, struct GeoLocTo & b) {
  const float R = 6371000; // km
  float p1 = a.lat * DEGTORAD;
  float p2 = b.lat * DEGTORAD;
  float dp = (b.lat - a.lat) * DEGTORAD;
  float dl = (b.lon - a.lon) * DEGTORAD;

  float x = sin(dp / 2) * sin(dp / 2) + cos(p1) * cos(p2) * sin(dl / 2) * sin(dl / 2);
  float y = 2 * atan2(sqrt(x), sqrt(1 - x));

  return R * y;
}


//Bearing in degrees
float geoBearing(struct GeoLocFrom & a, struct GeoLocTo & b) {
  float y = sin(b.lon - a.lon) * cos(b.lat);
  float x = cos(a.lat) * sin(b.lat) - sin(a.lat) * cos(b.lat) * cos(b.lon - a.lon);
  return atan2(y, x) * RADTODEG;
}

// Get the current location readings
void getLocationReadings() {
  GeoLocFrom geoFrom;
  GeoLocTo geoTo;
  bool newFromData = false;
  bool newToData = false;

  //From Data
  for (unsigned long start = millis(); millis() - start < 1000;) {
    // This sketch displays information every time a new sentence is correctly encoded.
    while (ss.available())
      if (gps.encode(ss.read())) {
        if (gps.location.isValid())
        {
          newFromData = true;
        }
      }
  }
  if (newFromData) {
    displayLInfo();
    geoFrom.utime = gps.time.value();
    geoFrom.lon = gps.location.lng();
    geoFrom.lat = gps.location.lat();
  }

  //To Data
  for (unsigned long start = millis(); millis() - start < 1000;) {
    // This sketch displays information every time a new sentence is correctly encoded.
    while (rs.available())
      if (gps.encode(rs.read())) {
        if (gps.location.isValid())
        {
          newToData = true;
        }
      }
  }
  if (newToData) {
    displayRInfo();
    geoTo.utime = gps.time.value();
    geoTo.lon = gps.location.lng();
    geoTo.lat = gps.location.lat();
  }
  // Get Bearings
  double bearingTo = geoBearing(geoFrom, geoTo);
  Serial.print("Bearing: ");
  Serial.println(bearingTo);

  //  Get Distance
  float distanceTo = geoDistance(geoFrom, geoTo);
  Serial.print("Distance To: ");
  Serial.println(distanceTo);

}


