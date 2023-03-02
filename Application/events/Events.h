#pragma once
#include "../core/BaseTypes.h"
//class Event
//{
//    //https://stackoverflow.com/questions/25903643/bind-class-function-observer-c11
//    //Same thing as this because it simply binds without using std::bind by using lambdas
//    //#define HZ_BIND_EVENT_FN(fn) [this](auto&&... args) -> decltype(auto) { return this->fn(std::forward<decltype(args)>(args)...); }
//
//
//};
//
////This is a good oportunity to look at the intermediate compiler and write down an example fo what this ends up being
//#define LL_EVENTS_DEFINE_FUNC(_class,fn ) inline void notify_single_t(_class* o) {o->fn(this);}
//class Observer
//{
//
//};
//
////Each class here is responsible for an interaction on how to handle the event and can DI the parent transform or script object / just pass in the entity 
//class DeathObserver : Observer
//{
//public:
//    void OnDie(Subject* subj);
//};
//
//class Subject
//{
//public:
//    //Here you put in every possible class that can subscribe to this Subject
//    LL_EVENTS_DEFINE_FUNC(DeathObserver, OnDie);
//};

//Subject and Observer cant be made into components if they are shared pointers
//Because Components might die, its easier for Subjects and Observers to just be managed with sharedPointers and I dont imagine there would be many anyway

//https://refactoring.guru/design-patterns/observer/cpp/example#:~:text=Observer%20in%20C%2B%2B,that%20implements%20a%20subscriber%20interface.
//This uses cpp inheritance which exposes modifiers for the class itself
class Observer
{
    // Check if nullptr before unsubbing if using raw pointers,
    // this would have to be assigned to nullptr (from the subjects death and would be a form of manual reference counting)
    //Subject* _subject = nullptr;
    Ref<Subject> _subject;
public:

    Observer(Ref<Subject> subject) : _subject(subject)
    {
        subject->Sub(this);
    }
    // Calling unsub in the destructor (kind of a must) means Subject cant "own" Observer (store with sharedPointer) because sharedPointer is in fact the caller of the desctructor
    ~Observer()
    {
        _subject->Unsub(this);
    };
};

class Subject
{
public:
    void Unsub(Observer* observer)
    {
        //_observers.remove_if([](Ref<Observer> o) { o == o; });
        _observers.remove(observer);
    };
    void Sub(Observer* observer)
    {
        _observers.push_back(observer);

    };

private:
    //Could probably use weakRefs instead but I like pointers Lul
    std::list<Observer*> _observers;
};

class EnemyDeathObserver : Observer
{
public: 
    void OnDie(Subject* ded)
    {
        delete this;
    };

};
