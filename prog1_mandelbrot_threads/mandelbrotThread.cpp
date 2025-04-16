#include <stdio.h>
#include <thread>
#include "CycleTimer.h"
#include <algorithm>
using namespace std;

// the prog is passing the numbers of threads as an argument when running this prog okkkkkk

//strct for the arguments that would be passed to the threads
typedef struct {
    float x0, x1;
    float y0, y1;
    unsigned int width;
    unsigned int height;
    int maxIterations;
    int* output;
    int threadId;
    int numThreads;
} WorkerArgs;


// we are using this mandelbrotSerial() func given in the other file: mandelbrotSerial.cpp
// this 'extern' keyword is used for this purpose
extern void mandelbrotSerial(
    float x0, float y0, float x1, float y1,
    int width, int height,
    int startRow, int numRows,
    int maxIterations,
    int output[]);


// workerThreadStart --
void workerThreadStart(WorkerArgs * const args) 
{
    // TODO FOR CS149 STUDENTS: Implement the body of the worker
    // thread here. Each thread should make a call to mandelbrotSerial()
    // to compute a part of the output image.  For example, in a
    // program that uses two threads, thread 0 could compute the top
    // half of the image and thread 1 could compute the bottom half.

    
    //------------PART 1---------------
    // // calculating how many rows a single thread can handle
    // int rows = args->height / args->numThreads;

    // // starting row position for the current thread
    // int startRow = args->threadId * rows;

    // // ending row position
    // int endRow;
    // if (args->threadId == args->numThreads-1) //last thrread
    // {
    //     endRow=args->height;     //all remaining rows
    // }
    // else
    // {
    //     endRow=startRow + rows;
    // }

    // // timer for each thread - calling the func
    // double start = CycleTimer::currentSeconds();
    // mandelbrotSerial(args->x0, args->y0, args->x1, args->y1, args->width, args->height, startRow, endRow - startRow, args->maxIterations, args->output);
    // double end = CycleTimer::currentSeconds();



    //-------------PART 4-------------
    int numThreads = args->numThreads;
    int threadId = args->threadId;
    double start = CycleTimer::currentSeconds();

    // round robin technique used here
    for (unsigned int row = threadId; row < args->height; row += numThreads) 
    {
        mandelbrotSerial(args->x0, args->y0, args->x1, args->y1, args->width, args->height, row, 1, args->maxIterations, args->output);
    }
    double end = CycleTimer::currentSeconds();


    printf("Hello from thread %d, finished in [%.3f] sec\n", args->threadId, (end-start));
   
}


// Multi-threaded implementation of mandelbrot set image generation.
// Threads of execution are created by spawning std::threads.
void mandelbrotThread(
    int numThreads,
    float x0, float y0, float x1, float y1,
    int width, int height,
    int maxIterations, int output[])
{
    static constexpr int MAX_THREADS = 32;

    if (numThreads > MAX_THREADS)
    {
        fprintf(stderr, "Error: Max allowed threads is %d\n", MAX_THREADS);
        exit(1);
    }

    thread workers[MAX_THREADS];
    WorkerArgs args[MAX_THREADS];

    for (int i=0; i<numThreads; i++) {


        // TODO FOR CS149 STUDENTS: You may or may not wish to modify
        // the per-thread arguments here.  The code below copies the
        // same arguments for each thread
        args[i].x0 = x0;
        args[i].y0 = y0;
        args[i].x1 = x1;
        args[i].y1 = y1;
        args[i].width = width;
        args[i].height = height;
        args[i].maxIterations = maxIterations;
        args[i].numThreads = numThreads;
        args[i].output = output;
      
        args[i].threadId = i;
    }

    // Spawn the worker threads.  Note that only numThreads-1 std::threads
    // are created and the main application thread is used as a worker
    // as well.
    for (int i=1; i<numThreads; i++) {
        workers[i] = std::thread(workerThreadStart, &args[i]);
    }
    
    workerThreadStart(&args[0]);

    // join worker threads
    for (int i=1; i<numThreads; i++) {
        workers[i].join();
    }
}

