#include <Wire.h>


// Adresses I2C des deux capteurs
#define I2C_A31301_ADDRESS 0x60


// X-axis register
#define REGISTER_MSB_X 0x1E
#define REGISTER_LSB_X 0x1F

// Y-axis register
#define REGISTER_MSB_Y 0x20
#define REGISTER_LSB_Y 0x21

// Z-axis register
#define REGISTER_MSB_Z 0x22
#define REGISTER_LSB_Z 0x23




void setup() {
  // Initialize serial communication
  Serial.begin(115200);
  while (!Serial);

  // I²C initialization
  Wire.begin();
}

void loop() {
  // Send data on serial
  Serial.print(getX(I2C_A31301_ADDRESS));
  Serial.print(",");
  Serial.print(getY(I2C_A31301_ADDRESS));
  Serial.print(",");
  Serial.println(getZ(I2C_A31301_ADDRESS));  

  // 10ms => 100Hz frame rate
  delay(10);
}

/**
 
 * \brief Read and return the X-axis magnitude
 *
 * \param address A31301 sensor I²C address
 * \return signed X-axis magnitude on 16 bits
 */
int16_t getX(byte address) {
    
  // Read MSB and LSB for X-axis
  uint8_t msb = Request_info(address, REGISTER_MSB_X);
  uint8_t lsb = Request_info(address, REGISTER_LSB_X);

  // Merge MSB and LSB on 15 bits
  int16_t combined = ((msb & 0x7F) << 8) | lsb;

  // Check the 15th bit to duplicate the sign on the 16 bit (0=positive / 1=negative)
  if (combined & 0x4000) combined |= 0x8000; 

  return combined;
}





/** 
 * \brief Read and return the Y-axis magnitude
 *
 * \param address A31301 sensor I²C address
 * \return signed Y-axis magnitude on 16 bits
 */
int16_t getY(byte address) {
    
  // Read MSB and LSB for Y-axis
  uint8_t msb = Request_info(address, REGISTER_MSB_Y);
  uint8_t lsb = Request_info(address, REGISTER_LSB_Y);

  // Merge MSB and LSB on 15 bits
  int16_t combined = ((msb & 0x7F) << 8) | lsb;

  // Check the 15th bit to duplicate the sign on the 16 bit (0=positive / 1=negative)
  if (combined & 0x4000) combined |= 0x8000; 

  return combined;
}




/** 
 * \brief Read and return the Z-axis magnitude
 *
 * \param address A31301 sensor I²C address
 * \return signed Z-axis magnitude on 16 bits
 */
int16_t getZ(byte address) {
    
  // Read MSB and LSB for Z-axis
  uint8_t msb = Request_info(address, REGISTER_MSB_Z);
  uint8_t lsb = Request_info(address, REGISTER_LSB_Z);

  // Merge MSB and LSB on 15 bits
  int16_t combined = ((msb & 0x7F) << 8) | lsb;

  // Check the 15th bit to duplicate the sign on the 16 bit (0=positive / 1=negative)
  if (combined & 0x4000) combined |= 0x8000; 

  return combined;
}



/** 
 * \brief Read and return a byte from the given register 
 *
 * \param address A31301 sensor I²C address
 * \param registerAddress Register to read address
 * \return The byte read in the register
 */
uint8_t Request_info(uint8_t address, uint8_t registerAddress) {
  // Configure the I²C write/read
  // 1. Write the register address
  // 2. Read the register content
  Wire.beginTransmission(address);
  // Set register address
  Wire.write(registerAddress);
  Wire.endTransmission(false);  
  // Read register content
  Wire.requestFrom(address, 1);

  // Return the byte read at the register address
  return Wire.read();
}