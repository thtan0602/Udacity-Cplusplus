// TrafficLight.cpp
#include <iostream>
#include <random>
#include <chrono>
#include <thread>
#include "TrafficLight.h"

template <typename T>
void MessageQueue<T>::send(T &&msg)
{
    // Acquire lock
    std::unique_lock<std::mutex> lock(_mutex);
    
    // Add message to queue
    _queue.push_back(std::move(msg));
    
    // Notify waiting thread
    _condition.notify_one();
}

template <typename T>
T MessageQueue<T>::receive()
{
    // Acquire unique lock
    std::unique_lock<std::mutex> lock(_mutex);
    
    // Wait for non-empty queue
    _condition.wait(lock, [this] { return !_queue.empty(); });
    
    // Move and remove first message
    T msg = std::move(_queue.front());
    _queue.pop_front();
    
    return msg;
}

TrafficLight::TrafficLight()
{
    _currentPhase = TrafficLightPhase::red;
}

TrafficLight::~TrafficLight() 
{
    // Ensure any running threads are joined
    for (auto& thread : threads) {
        thread.join();
    }
}

TrafficLightPhase TrafficLight::getCurrentPhase()
{
    return _currentPhase;
}

void TrafficLight::waitForGreen()
{
    while (true) {
        TrafficLightPhase phase = _messageQueue.receive();
        
        if (phase == TrafficLightPhase::green) {
            return;
        }
    }
}

void TrafficLight::simulate()
{
    // Start cycleThroughPhases in a separate thread
    threads.emplace_back(std::thread(&TrafficLight::cycleThroughPhases, this));
}

void TrafficLight::cycleThroughPhases()
{
    // Random number generator for cycle duration
    std::random_device rd;
    std::mt19937 eng(rd());
    std::uniform_int_distribution<> distr(4000, 6000);  // 4-6 seconds in milliseconds
    
    while (true) {
        // Generate random cycle duration
        int cycleDuration = distr(eng);
        
        // Record start time
        auto lastUpdate = std::chrono::system_clock::now();
        
        // Wait for cycle duration
        while (true) {
            std::this_thread::sleep_for(std::chrono::milliseconds(1));
            
            auto now = std::chrono::system_clock::now();
            auto timeSinceLastUpdate = std::chrono::duration_cast<std::chrono::milliseconds>(now - lastUpdate).count();
            
            if (timeSinceLastUpdate >= cycleDuration) {
                // Toggle traffic light phase
                _currentPhase = (_currentPhase == TrafficLightPhase::red) 
                                ? TrafficLightPhase::green 
                                : TrafficLightPhase::red;
                
                // Send new phase to message queue
                _messageQueue.send(std::move(_currentPhase));
                
                // Update last update time
                lastUpdate = std::chrono::system_clock::now();
                
                break;
            }
        }
    }
}

// Explicit template instantiation to resolve linker errors
template class MessageQueue<TrafficLightPhase>;