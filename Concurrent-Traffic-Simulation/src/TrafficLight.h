// TrafficLight.h
#ifndef TRAFFICLIGHT_H
#define TRAFFICLIGHT_H
#include <mutex>
#include <deque>
#include <string>
#include <condition_variable>
#include <thread>
#include <future>
#include "TrafficObject.h"

class Vehicle;
enum class TrafficLightPhase { red, green };

template <class T>
class MessageQueue
{
public:
    void send(T &&message);
    T receive();

private:
    std::deque<T> _queue;
    std::mutex _mutex;
    std::condition_variable _condition;
};

class TrafficLight : public TrafficObject
{
public:
    TrafficLight();
    ~TrafficLight();
    
    TrafficLightPhase getCurrentPhase();
    void waitForGreen();
    void simulate();

private:
    void cycleThroughPhases();
    MessageQueue<TrafficLightPhase> _messageQueue;
    TrafficLightPhase _currentPhase;
};
#endif