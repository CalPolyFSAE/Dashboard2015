/*
 * Subsystem.h
 *
 *  Created on: Jan 15, 2018
 *      Author: root
 */

#ifndef DASHBOARD2_SUBSYSTEM_H_
#define DASHBOARD2_SUBSYSTEM_H_

#include <stdint.h>

//number of subsystem slots available
#define NUMSUBSYSTEMS 10

// manages the subsystems
class SubsystemControl
{
public:
    // register a subsystem for updating
    uint8_t RegisterSubsystem(class AbstractSubsystem&);

    // updates the subsystems
    void UpdateSubsystems();

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
};


// type of all subsystems
// generalization for typed subsystem class for updating
class AbstractSubsystem
{
public:
    virtual ~AbstractSubsystem() {} // virtual destructors always need definition
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
    Subsystem() {
        //keep track of all created subsystems
        SubsystemControl::StaticClass ().RegisterSubsystem (*this);
    }

    //called to Initialize hardware
    virtual void Init();

    // Update
    virtual void Update();

    //variables
public:

};

/////////////////////////

template<class T>
T& Subsystem<T>::StaticClass() {
    static T inst {};
    return inst;
}

template<class T>
void Subsystem<T>::Init()
{
    //do nothing
}

template<class T>
void Subsystem<T>::Update()
{
    //do nothing
}


#endif /* DASHBOARD2_SUBSYSTEM_H_ */
