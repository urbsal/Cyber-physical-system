#include <avr/io.h>
#include <util/delay.h>

// ---------------------- I2C DRIVER ----------------------
void I2C_init(void) {
    TWSR = 0x00;        // prescaler = 1
    TWBR = 72;          // 100kHz SCL
}

void I2C_start(void) {
    TWCR = (1<<TWSTA)|(1<<TWEN)|(1<<TWINT);
    while (!(TWCR & (1<<TWINT)));
}

void I2C_write(uint8_t data) {
    TWCR = (1<<TWEN)|(1<<TWINT);
    TWDR = data;
    while (!(TWCR & (1<<TWINT)));
}

uint8_t I2C_readACK(void) {
    TWCR = (1<<TWEA)|(1<<TWEN)|(1<<TWINT);
    while (!(TWCR & (1<<TWINT)));
    return TWDR;
}

uint8_t I2C_readNACK(void) {
    TWCR = (1<<TWEN)|(1<<TWINT);
    while (!(TWCR & (1<<TWINT)));
    return TWDR;
}

void I2C_stop(void) {
    TWCR = (1<<TWEN)|(1<<TWSTO)|(1<<TWINT);
}

// ------------------ MPU6050 FUNCTIONS ------------------
#define MPU_ADDR 0x68

void MPU_init(void) {
    I2C_start();
    I2C_write(MPU_ADDR<<1);
    I2C_write(0x6B);    // power management register
    I2C_write(0x00);    // wake up MPU6050
    I2C_stop();
}

int16_t MPU_readGyroX(void) {
    int16_t gyro;

    I2C_start();
    I2C_write(MPU_ADDR<<1);
    I2C_write(0x43);   // GYRO_XOUT_H
    I2C_start();
    I2C_write((MPU_ADDR<<1) | 1); 

    gyro = ((int16_t)I2C_readACK() << 8);
    gyro |= I2C_readNACK();

    I2C_stop();
    return gyro / 131.0;     // convert to °/s
}

// ------------------ PWM INITIALIZATION ------------------
void PWM_init(void) {
    DDRD |= (1 << PD6);          // OC0A pin output
    TCCR0A = (1<<WGM00)|(1<<WGM01)|(1<<COM0A1); // Fast PWM, non-inverting
    TCCR0B = (1<<CS01);          // Prescaler 8
}

void set_fan_speed(uint8_t duty) {
    OCR0A = duty;           // duty 0–255
}

// ------------------ MAIN PROGRAM ------------------
int main(void) {
    I2C_init();
    MPU_init();
    PWM_init();

    int16_t gyro;

    while (1) {
        gyro = MPU_readGyroX();

        // ----- STATE MACHINE -----
        if (gyro <= 30) {
            set_fan_speed(0);        // OFF
        }
        else if (gyro <= 100) {
            set_fan_speed(64);       // LOW  (25%)
        }
        else if (gyro <= 200) {
            set_fan_speed(153);      // MEDIUM (60%)
        }
        else {
            set_fan_speed(255);      // HIGH (100%)
        }

        _delay_ms(200);
    }
}