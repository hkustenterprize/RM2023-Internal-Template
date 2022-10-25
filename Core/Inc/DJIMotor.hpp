/**
 * @file DJIMotor.hpp
 * @author JIANG Yicheng (EthenJ@outlook.sg)
 * @brief
 * @version 0.1
 * @date 2022-10-25
 *
 * @copyright This file is only for HKUST Enterprize RM2023 internal competition. All Rights Reserved.
 *
 */

#include "appConfig.h"

#ifdef DJIMotor_CAN
#include "main.h"

namespace DJIMotor
{

class DJIMotor
{
   public:
    DJIMotor();
    DJIMotor(const DJIMotor &) = delete;
    DJIMotor &operator=(const DJIMotor &) = delete;

    /**
     * @brief Get the raw encoder value
     *
     * @return uint16_t
     */
    uint16_t getRawEncoder() const;

    /**
     * @brief Get the current position of the motor in radian
     * @note  You may need to multiply the reduction ratio of the motor to get the actual position.
     *
     * @return uint16_t
     */
    virtual float getPosition() const;

    /**
     * @brief Set the Current Position object in radian
     * @note This just set the current reference position of the motor, it does not change the actual position of the motor.
     *
     * @param position
     */
    virtual void setPosition(float position);

    /**
     * @brief Get the current speed of the motor in revolutions per minute
     *
     * @return uint16_t
     */
    virtual int16_t getRPM() const;

    /**
     * @brief Get the actual output current(or voltage) of the motor
     *
     * @return int16_t
     */
    virtual int16_t getActualCurrent() const;

    /**
     * @brief Get the set output current(or voltage) of the motor
     *
     * @return int16_t
     */
    int16_t getOutputCurrent() const;

    /**
     * @brief Set the output current(or voltage) of the motor
     * @note  This function will limit the current(or voltage) to the current limit of the motor.
     *        Please call sendMotorGroup() to send the command to the motor.
     *
     * @param current
     */
    virtual void setOutputCurrent(int32_t current);

    /**
     * @brief Set the Current(or voltage) Limit of the motor
     * @note  To avoid overflow,
     *          the maximum current limit for M3508 is 16384,
     *          and the maximum voltage limit for GM6020 is 30000.
     *
     * @param current
     */
    void setCurrentLimit(uint16_t current);

    /**
     * @brief Get the temperature of the motor
     *
     * @return uint8_t
     */
    uint8_t getTemperature() const;

    /**
     * @brief Get the Reveice Count of the motor, this can be used to estimate the receive frequency of the motor
     *
     * @return uint32_t
     */
    virtual uint32_t getReveiceCount() const;

    /**
     * @brief Check if the motor is connected
     *
     * @return true
     * @return false
     */
    bool isConnected() const;

    static void decodeFeedback(CAN_HandleTypeDef *);

   protected:
    volatile uint16_t rawEncoder = 0;
    volatile uint16_t lastRawEncoder = 0;
    volatile float position = 0.0f;
    volatile int16_t rpm = 0;
    volatile int16_t actualCurrent = 0;
    volatile int16_t setCurrent = 0;
    volatile uint16_t currentLimit = 10000;

    volatile uint8_t temperature = 0;

    volatile int32_t rotaryCnt = 0;
    volatile int16_t positionOffset = 0;

    volatile uint32_t disconnectCnt = 0;
    volatile uint32_t receiveCnt = 0;
    volatile bool connected = false;

    friend void motorUpdate(void *);
    friend void sendMotorGroup(uint32_t group);
};

/**
 * @brief Get the Motor object
 *
 *
 * @param canid (eg. 0x205)
 * @return DJIMotor&
 */
DJIMotor &getMotor(uint32_t canid);

/**
 * @brief Send the command to the motor by group
 */
void sendMotorGroup(uint32_t group);

void init();

}  // namespace DJIMotor
#endif