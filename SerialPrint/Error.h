/*
 * Error.h
 *
 *  Created on: Feb 11, 2018
 *      Author: root
 */

#ifndef SERIALPRINT_ERROR_H_
#define SERIALPRINT_ERROR_H_

#include "stdint.h"

class Error
{
public:
    enum class ErrorType
        : uint8_t
    {
        Type00,
        Type01,
        Type02
    };

    Error(ErrorType);

private:
    ErrorType error;
};


#endif /* SERIALPRINT_ERROR_H_ */
