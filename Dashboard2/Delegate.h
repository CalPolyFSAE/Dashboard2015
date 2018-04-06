/*
 * Delegate.h
 *
 *  Created on: Mar 14, 2018
 *      Author:
 */

#ifndef DASHBOARD2_DELEGATE_H_
#define DASHBOARD2_DELEGATE_H_

#ifdef USE_FDELEGATE

// wrapper for type of a Member function
template<class UserClass, typename RetType, typename... VarTypes>
struct WMemFuncPrtType
{
    typedef RetType (UserClass::* Type)(VarTypes...);
};

template<typename T, typename MemFunPtrType>
class MemberFunctionCaller
{
    T* Obj;
    MemFunPtrType MemFunPtr;

public:
    MemberFunctionCaller( T* InObj, MemFunPtrType InMemFunPtr ) :
        Obj(InObj), MemFunPtr(InMemFunPtr)
    {

    }

    template <typename... ArgTypes>
    auto operator()(ArgTypes... Args) -> decltype((Obj->*MemFunPtr)(Args...))
    {
        return (Obj->*MemFunPtr)(Args...);
    }
};

template<typename T>
T&& Forward()
{

}


template<class UserClass, typename RetType, typename... VarTypes>
class Fdelegate
{
public:
    typedef WMemFuncPtrType<UserClass, RetType, VarTypes> MemFuncPtr;

    Fdelegate(UserClass* InObj, MemFuncPtr InFunc)
    {

    }

    RetType operator()()// TODO forward

};

template<typename RetType, typename... VarTypes>
class LDelegate
{
public:
    LDelegate()

    template<class UserClass>
    static d Create(Userclass Object, WMemFuncPtrType<UserClass, RetType, VarTypes...> InFunc)
    {
        ...
    }
};

#endif // USE_FDELEGATE

/* Use
 *     A a {
 *         foo(int, int)
 *     };
 *     MemberFunctionCaller<A, void (A::*)(int, int)> CC(&a, &A::foo);
    CC(1, 2);
 * */

//from
// https://www.codeproject.com/Articles/13287/Fast-C-Delegate
// TODO: Switch over to member function caller^^^
class delegate
{
public:
    delegate() :
            object_ptr (0), stub_ptr (0) {
    }

    template<class T, void (T::*TMethod)( uint8_t )>
        static delegate from_method( T* object_ptr ) {
            delegate d;
            d.object_ptr = object_ptr;
            d.stub_ptr = &method_stub<T, TMethod>; // #1

            return d;
        }

    inline void operator()( uint8_t a1 ) const {
        return (*stub_ptr) (object_ptr, a1);
    }

    static inline bool isValid(const delegate& d)
    {
        return (bool)(d.object_ptr);
    }

private:
    typedef void (*stub_type)( void* object_ptr, uint8_t );

    void* object_ptr;
    stub_type stub_ptr;

    template<class T, void (T::* TMethod)( uint8_t )>
        static void method_stub( void* object_ptr, uint8_t a1 ) {
            T* p = static_cast<T*> (object_ptr);
            return (p->*TMethod) (a1); // #2

        }
};

#endif /* DASHBOARD2_DELEGATE_H_ */
