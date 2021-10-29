# I2C Addresses!!
- Temp and Cap sensor is 0x36.
- Temp, Humidity, Pressure, etc. sensor is 0x77;

# Pins!
Yellow -> pin 15 (SCL)
Blue   -> pin 4  (SDA)

The default SCL pin is 22 and SDA is 21.
These change when we run the Heltec.begin() function because we want to use the OLed screen.
