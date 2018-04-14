/*
 * Subsystem.h
 *
 *  Created on: Jan 15, 2018
 *      Author: root
 */

#ifndef DASHBOARD2_SUBSYSTEM_H_
#define DASHBOARD2_SUBSYSTEM_H_

// TODO TESTING
#include "AVRLibrary/arduino/Arduino.h"
///////////////

#include <stdint.h>
#include "Delegate.h"
#include "Dashboard2.h"

//number of subsystem slots available
#define NUMSUBSYSTEMS 10

// manages the subsystems
class SubsystemControl
{
public:
    // register a subsystem for updating
    uint8_t RegisterSubsystem(class AbstractSubsystem&);

    void MainLoop();

    // 1000 Hz timing tick
    void INT_CALL_TickAllTC();

    // registers an Event to run every Interval ms
    // there is no task scheduling, so no gaurentee that event will fire
    // at specified interval
    int8_t RegisterEvent(delegate const& Event, uint16_t Interval);

    //bool UnregisterEvent(int8_t Eventslot)

    // inits the subsystems
    void InitSubsystems();

    //get instance (for other classes that want to use the SubsystemControl)
    static SubsystemControl& StaticClass();

    // disallow other instances
    SubsystemControl(SubsystemControl const&) = delete;
    void operator=(SubsystemControl const&) = delete;

private:
    SubsystemControl();

    // all subsystems
    class AbstractSubsystem* systems[NUMSUBSYSTEMS];

    // have the subsystems been initialized
    bool bHaveSubInit = false;

    // private class for controlling timing
    struct TC;

    TC* TimeControl[CONFIG::RSCMAXJOBS];

    int8_t getNextFreeTC();
};


// type of all subsystems
// generalization for typed subsystem class for updating
class AbstractSubsystem
{
public:
    friend class SubsystemControl;

    virtual ~AbstractSubsystem() {} // virtual destructors always need definition

protected:
    virtual void Init() = 0;

    // has this subsystem been initialized
    bool bDidInit = false;
};

// per subsystem Types
// templating allows StaticClass to return correct type
// allows for overriding of static methods
// this is Curiously Recursive Template Pattern (CRTP)
// https://stackoverflow.com/questions/34222703/how-to-override-static-method-of-template-class-in-derived-class
template<class T>
class Subsystem : public AbstractSubsystem
{
public:
    // this class needs to be able to call Update() and init
    friend class SubsystemControl;

    virtual ~Subsystem() {}

    // get ref to singleton class of type T
    inline static T& StaticClass();

protected:
    // Interval that Update should run at (in ms)
    const uint16_t Interval;

    Subsystem(uint16_t Interval);

    //called to Initialize hardware
    virtual void Init() override;

private:
    inline void UpdateDelegateFunc( uint8_t ) {
        static_cast<T*>(this)->Update(0);
    }

};

/////////////////////////

template<class T>
T& Subsystem<T>::StaticClass() {
    static T inst {};
    return inst;
}

template<class T>
Subsystem<T>::Subsystem(uint16_t Interval) :
    Interval(Interval)
{
    //keep track of all created subsystems
    SubsystemControl::StaticClass ().RegisterSubsystem (*this);
    // create an event for Update if Interval is not zero
    // this allows the update function to run on the same event structure that other timed events are using
    if(Interval > 0)
        if(SubsystemControl::StaticClass().RegisterEvent(
                delegate::from_method<Subsystem<T>, &Subsystem<T>::UpdateDelegateFunc>(this), Interval) < 0)
        {
#ifdef DEBUG_PRINT
            Serial.print(FSTR("[ERROR]: "));
            Serial.print(__FILE__);
            Serial.print(FSTR(" at "));
            Serial.print(__LINE__);
            Serial.print(FSTR(": RegisterEvent returned a negative number"));
#endif
        }
}

template<class T>
void Subsystem<T>::Init()
{
    bDidInit = true;
#ifdef DEBUG_PRINT
    Serial.print(FSTR("[INFO]: Subsystem Init: Interval "));
    Serial.print(Interval);
    Serial.println(FSTR(" ms."));
#endif // DEBUG_PRINT
}


#endif /* DASHBOARD2_SUBSYSTEM_H_ */
