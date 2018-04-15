/*
 * DashPage.h
 *
 *  Created on: Apr 14, 2018
 *      Author: root
 */

#ifndef DASHPAGES_DASHPAGE_H_
#define DASHPAGES_DASHPAGE_H_


// what info gets drawn to the screen
class DashPage
{
public:
    virtual ~DashPage() {}

    virtual void Construct() = 0;
    virtual void Draw() = 0;
};


#endif /* DASHPAGES_DASHPAGE_H_ */
