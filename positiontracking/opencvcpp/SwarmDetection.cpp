#include "SwarmDetection.h"

void SwarmDetection::Detector()
{
    cv::SimpleBlobDetector::Params params;
    std::vector<cv::KeyPoint> keyPoints;
    cv::Mat hsvFrame;
    cv::Mat mask;

    //Sets up an thread for the server
    std::thread server;

    server = std::thread(startServer,this);
    server.detach();

    //Sets the Parameters for the Detection WIP
    setBlobParams(0, 256, true, 100, true, 0.1f, true, 0.5f, true, 0.5f, params);

    for (;;)
    {
        cv::Mat xframe = readFromCamera();

        cv::Mat imWithKeypoints;
        //Get the Trackbar positions of the sliders
        getTBarPosHV(hvalues);

        //Transform the fram into the HSV colour spectrum
        cv::cvtColor(xframe, hsvFrame, cv::COLOR_BGR2HSV);
        
        //Applies a mask onto the frame with the values the user selected
        //Applies the mask in steps 
        cv::inRange(hsvFrame, cv::Scalar(hvalues.l_h,hvalues.l_s,hvalues.l_v),cv::Scalar(hvalues.u_h,hvalues.u_s,hvalues.u_v),mask);

        //Detect all Keypoints on different versions of openCV
        #if CV_MAJOR_VERSION < 3
            cv::SimpleBlobDetector detector(params);
            detector.detect(frame,keypoints);
        #else
            cv::Ptr <cv::SimpleBlobDetector> detector = cv::SimpleBlobDetector::create(params);
            detector->detect(mask,keyPoints);
        #endif

        //Advanced Car Detection
        //setCarDimensions(100.0, 150.0, 150.0);
        //carDetection(keyPoints);
    	
        //Extract keypoints
        simpleCarDetection(keyPoints);

        //Draw the Keypoints on a canvas
        drawKeyPoints(pic.frame, &keyPoints, this);
        showFrame("Keypoints", pic.frame);

        if (cv::waitKey(10) == 27)
        {
            std::cout << "ESC";
            break;
        }
    }
    std::cout << "[SERVER] Shutdown" << std::endl;
}

void SwarmDetection::drawKeyPoints(cv::Mat xframe, std::vector<cv::KeyPoint>* keyPoints,SwarmDetection *p)
{
    //Draws the keypoints onto an given frame in red
    cv::drawKeypoints(xframe, *keyPoints, xframe, cv::Scalar(0, 0, 255), cv::DrawMatchesFlags::DRAW_RICH_KEYPOINTS);
    p->pic.frame = xframe;
}

void SwarmDetection::simpleCarDetection(std::vector<cv::KeyPoint> keyPoints)
{
    auto x = 0.00f, y = 0.00f;
    if (keyPoints.size() > 2)
    {
        //gets the dimensions of the frame
        getDimensions();
        std::vector<cv::KeyPoint>::const_iterator it = keyPoints.begin(), end = keyPoints.end();
        std::cout << "[DEBUG]Number of keypoints: " << keyPoints.size() << std::endl;
        if (keyPoints.size() >= 3)
        {
            std::cout << "[DEBUG]ABv = " << getDistance(keyPoints[0], keyPoints[1]) << "Pixel" << std::endl;
            std::cout << "[DEBUG]BCv = " << getDistance(keyPoints[1], keyPoints[2]) << "Pixel" << std::endl;
            std::cout << "[DEBUG]ACv = " << getDistance(keyPoints[0], keyPoints[2]) << "Pixel" << std::endl;
            //Calculates the norm coords of the car
            for (; it != end; ++it)
            {
                x += it->pt.x;
                y += it->pt.y;
            }
        x = (x / keyPoints.size()) / pic.width;
        y = (y / keyPoints.size()) / pic.height;
        std::cout << "[DEBUG]Median Xrel: " << x << " Median Yrel: " << y << std::endl;
        pkgReady = true;
        makePacket(x, y, 0);
        pkgReady = false;
        }
    }
    //Signifies that a packed is ready and packs it
}

void SwarmDetection::getDimensions()
{
    //Returns width and height of the frame that is shown
    pic.width = pic.frame.size().width;
    pic.height = pic.frame.size().height;
}

void SwarmDetection::printDimensions()
{
    //[DEBUG] Output to print the Dimensions
    getDimensions();
    std::cout << " Width: " << pic.width << "Height: " << pic.height  <<std::endl;
}

void SwarmDetection::makePacket(float x, float y, int id)
{
    //Uses the SchwarmPacket to make a packet for easier encoding and decoding
    packet.set_goal(x, y);
    packet.set_vehicle_id(id);
    packet.allocate(packet.min_size());
    packet.encode();
}

void SwarmDetection::startServer(SwarmDetection* p)
{
    //Starts the server on PORT 10001
    bool running = true;
    cppsock::utility_error_t err = p->listener.setup(cppsock::make_any<cppsock::IPv4>(PORT), 1);
    std::cout << "[SERVER]server status: " << cppsock::utility_strerror(err) << ", server address: " << p->listener.sock().getsockname() << std::endl;
    if(err < 0)
    {
        throw std::logic_error("unable to setup server");
    }

    for(;;)
    {
        //Listens for an connection
        p->listener.accept(p->connection);

        std::cout << "[SERVER] Client connected" << std::endl;

        while(running)
        {
            //Sends the client the packet
            running = p->sendPacket(p);
        }
        std::cout << "[SERVER] Client disconnected" << std::endl;
        p->listener.close();
        exit(-1);
    }
}

bool SwarmDetection::sendPacket(SwarmDetection* p)
{
    //Sends the client the packet
    if (p->pkgReady)
    {
        if(p->connection.send(p->packet.rawdata(), p->packet.min_size(), 0) <= 0)
            return false;
        std::this_thread::sleep_for(std::chrono::milliseconds(10));
    }
    return true;
}

void SwarmDetection::setCarDimensions(float vAB, float vAC, float vBC)
{
    cdim.vAB = vAB;
    cdim.vAC = vAB;
    cdim.vBC = vBC;
}

void SwarmDetection::getCarMidPoint(int id)
{
    getDimensions();
    cars.at(id).x = ((cars.at(id).apos[0] + cars.at(id).bpos[0] + cars.at(id).cpos[0]) / 3) / pic.width;
    cars.at(id).y = ((cars.at(id).apos[1] + cars.at(id).bpos[1] + cars.at(id).cpos[1]) / 3) / pic.height;
}

void SwarmDetection::carDetection(std::vector<cv::KeyPoint> keyPoints)
{
    int cnumber = 0;
    for (size_t i = 0; i < keyPoints.size()-2; i++)
    {
        for (size_t j = 0; j < keyPoints.size()-1; j++)
        {
        	//Search for the A & B Points of the Triangle
            if (getDistance(keyPoints.at(i), keyPoints.at(j)) <= cdim.vAB + 2 && getDistance(keyPoints.at(i), keyPoints.at(j)) >= cdim.vAB - 2)
            {
                cars.at(cnumber).apos[0] = keyPoints.at(i).pt.x;
                cars.at(cnumber).apos[1] = keyPoints.at(i).pt.y;
                cars.at(cnumber).bpos[0] = keyPoints.at(j).pt.x;
                cars.at(cnumber).bpos[1] = keyPoints.at(j).pt.y;
            	for (size_t k = 0; k < keyPoints.size(); k++)
            	{
            		//Search for the C Point
            		if(getDistance(keyPoints.at(i), keyPoints.at(k)) <= cdim.vAC + 2 && getDistance(keyPoints.at(i), keyPoints.at(k)) >= cdim.vAC - 2)
            		{
                        if (getDistance(keyPoints.at(j), keyPoints.at(k)) <= cdim.vAC + 2 && getDistance(keyPoints.at(j), keyPoints.at(k)) >= cdim.vAC - 2)
                        {
                            cars.at(cnumber).cpos[0] = keyPoints.at(k).pt.x;
                            cars.at(cnumber).cpos[1] = keyPoints.at(k).pt.y;
                            getCarMidPoint(cnumber);
                            std::cout << "Car Found @ " << cars.at(cnumber).x << " " << cars.at(cnumber).y <<  std::endl;
                            if (cnumber < N_CARS-1)
                            {
                                cnumber++;
                            }
                        	//If a car is found the loop can stop and the function can return
                            pkgReady = true;
                            makePacket(cars.at(cnumber).x, cars.at(cnumber).y, cnumber);
                            pkgReady = false;
                            return;
                        }
            		}
            	}
            }
        }
    }
}

float SwarmDetection::getDistance(cv::KeyPoint p1, cv::KeyPoint p2)
{
    //Returns the distance between the two provided keypoints
    const auto a =  p2.pt.x - p1.pt.x;
    const auto b = p2.pt.y - p1.pt.y;
    return sqrt(a*a+b*b);
}

SwarmDetection::SwarmDetection()
{
    //reads out the last values the user used
    cars.reserve(N_CARS);
    std::ifstream f("settings.cfg");
    if (f.is_open())
    {
        f >> hvalues.l_h >> hvalues.l_s >> hvalues.l_v >> hvalues.u_h >> hvalues.u_s >> hvalues.u_v;
        f.close();
    }
    //if none are found sets them to zero
    else
    {
        hvalues.l_h = hvalues.l_s = hvalues.l_v = hvalues.u_h = hvalues.u_s = hvalues.u_v = 0;
    }
}

SwarmDetection::~SwarmDetection()
{
    //saves the last settings the user put in
    std::ofstream f;
    f.open("settings.cfg");
    f << hvalues.l_h << "\n" << hvalues.l_s << "\n" <<hvalues.l_v << "\n" <<hvalues.u_h << "\n" << hvalues.u_s << "\n" << hvalues.u_v << "\n";
    f.close();
}

int SwarmDetection::setupVideoCapture(int deviceID)
{
    //Setups the videocapture with the provided deviceID
    int apiID = cv::CAP_ANY;

    cap.open(deviceID, apiID);

    if (!cap.isOpened()) {
        std::cerr << "ERROR! Unable to open camera\n";
        return -1;
    }
    return 0;
}

cv::Mat SwarmDetection::readFromCamera()
{
    //Reads one frame from the camera and returns it
    cap.read(pic.frame);
    if (pic.frame.empty()) {
        std::cerr << "ERROR! blank frame grabbed\n";
    }
    return pic.frame;
}

void SwarmDetection::showFrame(std::string windowName, cv::Mat xframe)
{
    //Displays the frame that is provided
    cv::imshow(windowName, xframe);
}

void SwarmDetection::createTBarHV()
{
    //Creates an simple trackbar so the user can adjust the settings while the
    //programm is running
    cv::namedWindow("Tracking");
    cv::createTrackbar("LH", "Tracking", &hvalues.l_h, 255);
    cv::createTrackbar("LS", "Tracking", &hvalues.l_s, 255);
    cv::createTrackbar("LV", "Tracking", &hvalues.l_v, 255);
    cv::createTrackbar("UH", "Tracking", &hvalues.u_h, 255);
    cv::createTrackbar("US", "Tracking", &hvalues.u_s, 255);
    cv::createTrackbar("UV", "Tracking", &hvalues.u_v, 255);
    cv::moveWindow("Tracking", 20, 20);
}

void SwarmDetection::getTBarPosHV(HueValues &hvalues)
{
    //sets the hvalues to the corresponding trackbar values
    hvalues.l_h = cv::getTrackbarPos("LH", "Tracking");
    hvalues.l_s = cv::getTrackbarPos("LS", "Tracking");
    hvalues.l_v = cv::getTrackbarPos("LV", "Tracking");
    hvalues.u_h = cv::getTrackbarPos("UH", "Tracking");
    hvalues.u_s = cv::getTrackbarPos("US", "Tracking");
    hvalues.u_v = cv::getTrackbarPos("UV", "Tracking");
}

void SwarmDetection::printHueValues(HueValues& hvalues)
{
    //[DEBUG] prints the hvalues into the console
    getTBarPosHV(hvalues);
    std::cout << "LH" << hvalues.l_h << std::endl
              << "LS" << hvalues.l_s << std::endl
              << "LV" << hvalues.l_v << std::endl
              << "UH" << hvalues.u_h << std::endl
              << "US" << hvalues.u_s << std::endl
              << "UV" << hvalues.u_v;
}

void SwarmDetection::setBlobParams(float minThreshhold, float maxThreshhold, bool filterByArea, float minArea, bool filterByCircularity, float minCircularity, bool filterByConvexity, float minConvexity, bool filterByInertia, float minInertiaRatio, cv::SimpleBlobDetector::Params &params)
{
    //sets the Blob parameters for the detection of the keypoints
    params.minThreshold = minThreshhold;
    params.maxThreshold = maxThreshhold;
    params.filterByArea = filterByArea;
    params.minArea = minArea;
    params.filterByCircularity = filterByCircularity;
    params.minCircularity = minCircularity;
    params.filterByConvexity = filterByConvexity;
    params.minConvexity = minConvexity;
    params.filterByInertia = filterByInertia;
    params.minInertiaRatio = minInertiaRatio;
}
