#pragma once
class Event
{
    //https://stackoverflow.com/questions/25903643/bind-class-function-observer-c11
    //Same thing as this because it simply binds without using std::bind by using lambdas
    //#define HZ_BIND_EVENT_FN(fn) [this](auto&&... args) -> decltype(auto) { return this->fn(std::forward<decltype(args)>(args)...); }


};

//This is a good oportunity to look at the intermediate compiler and write down an example fo what this ends up being
#define LL_EVENTS_DEFINE_FUNC(_class,fn ) inline void notify_single_t(_class* o) {o->fn(this);}
class Observer
{

};
class DeathObserver : Observer
{
public:
    void OnDie(Subject* subj);
};

class Subject
{
public:
    LL_EVENTS_DEFINE_FUNC(DeathObserver, OnDie);
};