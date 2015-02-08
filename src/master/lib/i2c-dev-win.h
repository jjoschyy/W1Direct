#ifndef LIB_I2CDEV_H
#define LIB_I2CDEV_H

#define I2C_SLAVE	0x0703


static inline int i2c_smbus_write_byte(int fd, uint8_t byte){
	return 0;
}


static inline int i2c_smbus_write_byte_data(int fd, uint8_t byte, uint8_t data){
	return 0;
}


static inline uint8_t i2c_smbus_read_byte(int fd){
	return 0;
}


#endif
