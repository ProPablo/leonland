#pragma once
#include "../core/BaseTypes.h"
#include "../core/Log.h"
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
class Observer
{
    // Check if nullptr before unsubbing if using raw pointers,
    // this would have to be assigned to nullptr (from the subjects death and would be a form of manual reference counting)
    // However this has the possibility to keep a Subject alive after its death if the observer is not disposed of 
    //Subject* _subject = nullptr;
protected:
    Ref<Subject> _subject;
    bool ShouldDie = false;
public:

    Observer(std::shared_ptr<Subject> subject) : _subject(subject)
    {
        _subject->Sub(this);
    }
    // Calling unsub in the destructor (kind of a must) means Subject cant "own" Observer (store with sharedPointer) because sharedPointer is in fact the caller of the desctructor
    ~Observer()
    {
        if (_subject == nullptr) return;
        _subject->Unsub(this);
    };

    virtual void OnSubjectDied()
    {
        _subject = nullptr;
        ShouldDie = true;
    }

};

class Subject
{
public:
    virtual void Unsub(Observer* observer)
    {
        //_observers.remove_if([](Ref<Observer> o) { o == o; });
        _observers.remove(observer);
    };
    virtual void Sub(Observer* observer)
    {
        _observers.push_back(observer);
    };

protected:
    //Could probably use weakRefs instead but I like pointers Lul
    std::list<Observer*> _observers;
};

class EnemyDeathSubject : Subject
{
public:
    std::string Name;
    EnemyDeathSubject(const std::string& name) : Name(name)
    {

    }
    void Die()
    {
        std::list<Observer*>::iterator iterator = _observers.begin();
        while (iterator != _observers.end())
        {
            ((EnemyDeathObserver*)(*iterator))->OnDie(*this);
            ++iterator;
        }
    }

};

class EnemyDeathObserver : Observer
{
    // It can be seen that a should delete checker in a list is sometimes better than going through hoops to try and delete self from a vector due to memory locality  
    //std::vector<EnemyDeathObserver>& _parent;
public:
    bool ShouldDelete = false;
    //The static cast here is fine and will only reference count if it is saved to a variable, in this case it is just being passed around in stack 
    EnemyDeathObserver(Ref<EnemyDeathSubject>subject)
        : Observer(std::static_pointer_cast<Subject>(subject))
    {
    }

    void OnDie(const EnemyDeathSubject& subject)
    {
        //Dont do this plz as it may be in the stack OR worse is in a container
        //delete this;
        log_dbg("subject " << subject.Name << "Just died lol");
        ShouldDelete = true;
    }

};
