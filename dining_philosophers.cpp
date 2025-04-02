#include <iostream>  //for prints to the console
#include <thread>    
#include <mutex>     //for synchronizing access to forks
#include <unistd.h>  //usleep for simulating eating/thinking time

class Philosopher {
public:
    Philosopher(int id, std::mutex &left, std::mutex &right)
        : id(id), left_fork(left), right_fork(right) {}
        // id (0,1,2...) fork references 
    void dine() {  
        while (true) {
            think();     
            eat();
        }
    }

private:
    int id;
    std::mutex &left_fork;
    std::mutex &right_fork;
    static std::mutex print_mutex; //shared by all the philosophers

    void log_state(const std::string &state) {
        std::lock_guard<std::mutex> lock(print_mutex); //lockguard, gets unlocked once out of scope
        std::cout << "Philosopher " << id << " " << state << "\n"; //cout with state
    }    

    void think() {
        log_state("is thinking.");
        usleep(1500000);
    } //

    void eat() { //if no forks/1 fork only available, will wait for it to be free
        std::lock(left_fork, right_fork); //atomic lock, like in Dijkstra approach
        //std::cout<< id<< " atomic locks \n";
        std::lock_guard<std::mutex> left_lock(left_fork, std::adopt_lock);
        std::lock_guard<std::mutex> right_lock(right_fork, std::adopt_lock); 
        //lock guard unlocks, once the lock guard goes out of scope (at the end of eat function)
        log_state("is eating.");
        usleep(1500000);
    }
};

std::mutex Philosopher::print_mutex;

int main(int argc, char *argv[]) {
    if (argc != 2) {
        std::cerr << "Usage: " << argv[0] << " <number_of_philosophers>\n";
        return 1;
    }
    // is there only one argument
    int num_philosophers = std::stoi(argv[1]);
    if (num_philosophers < 2) {
        std::cerr << "There must be at least two philosophers.\n";
        return 1;
    }
    // are there at least 2 philosophers

    std::mutex *forks = new std::mutex[num_philosophers];
    std::thread *threads = new std::thread[num_philosophers];
    Philosopher **philosophers = new Philosopher*[num_philosophers];
    //double * because of parametrized constructor constructor
    for (int i = 0; i < num_philosophers; ++i) {
        philosophers[i] = new Philosopher(i, forks[i], forks[(i + 1) % num_philosophers]);
        threads[i] = std::thread(&Philosopher::dine, philosophers[i]);
    }
    
    for (int i = 0; i < num_philosophers; ++i) {
        threads[i].join();      //wont happen because of no stop condition
        delete philosophers[i];  // Clean up allocated memory
    }
    
    delete[] forks;
    delete[] threads;
    delete[] philosophers; //cleans up arrays

    return 0;
} //NCOURSES do terminalowej gry ew QTEK