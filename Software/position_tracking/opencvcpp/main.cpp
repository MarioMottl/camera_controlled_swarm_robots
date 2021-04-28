#include "SwarmDetection.h"

int main()
{
    SwarmDetection swarm;

    if (swarm.setupVideoCapture(0) == -1)
    {
        return -1;
    }

    swarm.createTBarHV();
    swarm.Detector();

    return 0;
}
