/*
 * Subsystem.h
 *
 *  Created on: Jan 15, 2018
 *      Author: root
 */

#ifndef DASHBOARD2_SUBSYSTEM_H_
#define DASHBOARD2_SUBSYSTEM_H_

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
    struct TC
    {
        TC( const delegate& f, uint16_t Interval ) :
                func (f), Interval(Interval), ready(false), count(0){

        }

        inline void INT_Call_Tick() {
            if(count == 0) {
                count = Interval;
                ready = true;
            }else
                --count;
        }

        inline void Update()
        {
            if(ready){
                ready = false;
                func(0);
            }
        }

        delegate func;
        uint16_t Interval;

        volatile bool ready = false;
        volatile uint16_t count;
    };

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
    virtual void Update() = 0;
    virtual void Init() = 0;
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

    // has this subsystem been initialized
    bool bDidInit = false;
    // Interval that Update should run at (in ms)
    const uint16_t Interval;

    Subsystem(uint16_t Interval);

    //called to Initialize hardware
    virtual void Init();

    // Update
    // TODO make Fdelegate template to match func of type void (T::*)(void)
    virtual void Update(uint8_t);

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
    if(Interval > 0)
        SubsystemControl::StaticClass().RegisterEvent(
                delegate::from_method<Subsystem<T>, &Subsystem<T>::Update>(this), Interval);
}

template<class T>
void Subsystem<T>::Init()
{
    bDidInit = true;
}

template<class T>
void Subsystem<T>::Update(uint8_t)
{
    //do nothing
}


#endif /* DASHBOARD2_SUBSYSTEM_H_ */
