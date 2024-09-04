/*
 * Mod7.cpp
 *
 *  Created on: Aug 29, 2024
 *      Author: robwilliams
 */

#include <iostream>
#include <thread>
#include <mutex>
#include <condition_variable>

// Mutex for synchronizing shared resources
std::mutex m;

// Condition variable
std::condition_variable conVar;

// boolean for letting conditional variable know when thread one has completed
bool threadOne_completed = false;

// threadOne counts from 0 to 20
void countIncrease() {
	std::cout << "Counting up from 0 to 20: " << std::endl;

	// for loop increments from 0 to 20
    for (int i = 0; i <= 20; ++i) {
        // m locked so only threadOne prints
        std::unique_lock<std::mutex> lock(m);
        std::cout << i << std::endl;
    }

    //  m locked again to update boolean and signal to conVar
    std::unique_lock<std::mutex> lock(m);

    // Let program know that threadOne has completed
    threadOne_completed = true;

    // Use built in notify function to let program know threadTwo may start
    conVar.notify_one();
    std::cout << "\nCounting down from 20 to 0: " << std::endl;
}

// threadTwo counts from 20 to 0
void countDecrease() {
    std::unique_lock<std::mutex> lock(m);

    // Wait until the first thread has completed counting up
    while (!threadOne_completed) {

    	// Unlock m
        conVar.wait(lock);
    }

    // for loop decrements from 20 to 0
    for (int i = 20; i >= 0; --i) {
        std::cout << i << std::endl;
    }
}

int main() {

    // Initialize threadOne and countIncrease
    std::thread threadOne(countIncrease);

    // Initialize threadTwo and countDecrease
    std::thread threadTwo(countDecrease);

    // Main won't finish until threadOne and threadTwo have finished
    threadOne.join();
    threadTwo.join();
    std::cout << "\nBoth threads completed. " << std::endl;

    return 0;
}
