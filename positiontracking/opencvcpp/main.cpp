#include "SwarmDetection.h"

int main()
{
    SwarmDetection swarm;

    if (swarm.setupVideCapture(0) == -1)
    {
        return -1;
    }

    swarm.createTBar();
    swarm.Detector();

    return 0;
}