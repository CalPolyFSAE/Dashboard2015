/*
 * Error.h
 *
 *  Created on: Feb 11, 2018
 *      Author: root
 */

#ifndef SERIALPRINT_SERIALERROR_H_
#define SERIALPRINT_SERIALERROR_H_

#include "stdint.h"

class SerialError
{
public:
    enum class ErrorType
        : uint8_t
    {
        Type00,
        Type01,
        Type02
    };

    SerialError(ErrorType);

private:
    ErrorType error;
};


#endif /* SERIALPRINT_SERIALERROR_H_ */
