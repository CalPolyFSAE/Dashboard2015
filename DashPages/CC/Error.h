/*
 * Error.h
 *
 *  Created on: May 14, 2018
 *      Author: root
 */

#ifndef DASHPAGES_CC_ERROR_H_
#define DASHPAGES_CC_ERROR_H_

#include "stdint.h"

class Error
{
public:
    enum class ErrorLevel : uint8_t
    {
            Message, Warning, ReturnToPits, Error, Danger, None
    };
    enum class ErrorType
        : uint8_t
        {
            HighEngineTemp,
        HighOilTemp,
        EngineOverheat,
        LowOilPressure,
        LowBatteryVoltage,
        InvalidValue,
        Invalid
    };

    static constexpr uint8_t NUM_ERROR_TYPES = 7;

    Error() {
        type = ErrorType::Invalid;
    }

    void SetError( ErrorType type ) {
        this->type = type;
    }

    ErrorType GetErrorType() const {
        return type;
    }

    ErrorLevel GetErrorLevel() const {
        switch(type)
        {
            case ErrorType::EngineOverheat:
                return ErrorLevel::Danger;
            case ErrorType::HighEngineTemp:
                return ErrorLevel::Warning;
            case ErrorType::HighOilTemp:
                return ErrorLevel::Warning;
            case ErrorType::Invalid:
                return ErrorLevel::None;
            case ErrorType::InvalidValue:
                return ErrorLevel::Error;
            case ErrorType::LowBatteryVoltage:
                return ErrorLevel::Warning;
            case ErrorType::LowOilPressure:
                return ErrorLevel::Danger;
            default:
                return ErrorLevel::None;
        }
    }

protected:
    ErrorType type;
};


#endif /* DASHPAGES_CC_ERROR_H_ */
