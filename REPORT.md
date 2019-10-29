# ECS 150 Simple Shell: Project Report
- By Nikhil Razdan & Linda Li
***
# Part I - Queue

***
# Part II - Uthread

***
# Part III - Preemption
- Our preemption works through the `sigaction` function found in `signal.h`,
  and the `setitimer` function found in `sys/time.h`
- We define a signal handler `timer_handler` which simply calls `uthread_yield`
- We then define our signal handler function to be called when the *SIGVTALARM*
  flag is raised, using `sigaction`
- Our timer, set using `setitimer`, is set to raise the *SIGVTALARM* at 100Hz
- Thus, every 0.01 seconds, if preemption is not disabled, we will yield the
  currently running thread
- `preempt_{disable, enable}` work simply by overwritting the current signal
  handler with *SIG_IGN* or `timer_handler`, respectively
***
# Part IV - Testing
- Our largest test case, which tests all our implementation 
  `preempt_{start, enable, disable}` and 
  `uthread_{yield, join, exit, create}`, is `test_preempt`

- This test case runs in 4 phases
1. **DEFAULT**, where the library and the main thread have been intialized, and
  two other normal threads have been intialized and are running with preemption
2. **DISABLED**, where the user has called `preempt_disable()` after running
  with the defult parameters
3. **ENABLED**, where the user has reenabled preempt by calling 
  `preempt_enable()`
4. **EXITING**, where all the threads have terminated and we begin to free their
  memory and return to the main thread

- **Overview**
  - There are two functions in this program `huge` and `hello_world`
  - `huge` 
    - Runs a very long loop and prints every time the iterator variable
      reaches 5,000,000
    - In addition, it enables and disables preemption, begining
      phases 2 and 3, at specific times in our program
    - There is only 1 thread of `huge`, but it runs for the lifetime of the
      program
  - `hello_world`
    - Simply prints its tid and creates a new thread of `hello_world` and joins
      to it
    - This means that by the end of the program, we have about 50 threads of 
      `hello_world` in our program, all of which are joined to some other thread

- **DEFAULT**
  - In this phase, preemption is enabled with a frequency of 100 Hz
  - When running the program, you can clearly see this, as threads of both 
  `huge` and `hello_world` print to the terminal

- **DISABLED**
  - In this phase, preemption is disabled in the single thread of `huge`
  - Thus, `huge` is the only thread that runs and only prints to the terminal
  
- **ENABLED** 
  - In this phase, preemption is re-enabled and the current thread of 
    `hello_world`begins running again, creating more threads that take away from
    take away from the processor time of `huge` 
  
- **EXITING**
  - In this phase, `huge` breaks its loop, allowing for all threads to begin
    exiting
  - First, `huge` exits
  - Then all the `hello_world` threads exit, from highest to lowest tid, as
    they are all joined to each other
  - Finally, main exits
  - Notice how all memory of the children is freed, as there is no segfault
    when the program terminates
  - The freeing is handled by the parent, in `uthread_join`
  
- Conclusion
  - This program adequetely tests all of the functions of our implementation
  - All the major functions mentioned above are called 50+ times, and all
    engage in the predicted behavior


***
# (i) Resources
  - http://www.informit.com/articles/article.aspx?p=23618&seqNum=14
