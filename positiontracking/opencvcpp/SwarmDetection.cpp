#include "SwarmDetection.h"

void SwarmDetection::Detector()
{
    HueValues hvalues;
    cv::SimpleBlobDetector::Params params;
    std::vector<cv::KeyPoint> keyPoints;
    cv::Mat hsvFrame;
    cv::Mat mask;

    //std::thread t1;
    std::thread server;

    server = std::thread(startServer,this);
    server.detach();

    setBlobParams(0, 256, true, 100, true, 0.1f, true, 0.5f, true, 0.5f, params);

    for (;;)
    {
        cv::Mat xframe = readFromCamera();

        cv::Mat imWithKeypoints;

        getTBarPosHV(hvalues);

        cv::cvtColor(xframe, hsvFrame, cv::COLOR_BGR2HSV);
        cv::inRange(hsvFrame, cv::Scalar(hvalues.l_h,hvalues.l_s,hvalues.l_v),cv::Scalar(hvalues.u_h,hvalues.u_s,hvalues.u_v),mask);

        //Detect all Keypoints
        #if CV_MAJOR_VERSION < 3
            cv::SimpleBlobDetector detector(params);
            detector.detect(frame,keypoints);
        #else
            cv::Ptr <cv::SimpleBlobDetector> detector = cv::SimpleBlobDetector::create(params);
            detector->detect(mask,keyPoints);
        #endif

        simpleCarDetection(keyPoints);

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
    cv::drawKeypoints(xframe, *keyPoints, xframe, cv::Scalar(0, 0, 255), cv::DrawMatchesFlags::DRAW_RICH_KEYPOINTS);
    p->pic.frame = xframe;
}

void SwarmDetection::simpleCarDetection(std::vector<cv::KeyPoint> keyPoints)
{
    if (keyPoints.size() > 2)
    {
        float x = 0, y = 0;
        getDimensions();
        std::vector<cv::KeyPoint>::const_iterator it = keyPoints.begin(), end = keyPoints.end();
        std::cout << "[DEBUG]Number of keypoints: " << keyPoints.size() << std::endl;
        if (keyPoints.size() == 3)
        {
            std::cout << "[DEBUG]ABv = " << getDistance(keyPoints[0], keyPoints[1]) << "Pixel" << std::endl;
            std::cout << "[DEBUG]BCv = " << getDistance(keyPoints[1], keyPoints[2]) << "Pixel" << std::endl;
            std::cout << "[DEBUG]ACv = " << getDistance(keyPoints[0], keyPoints[2]) << "Pixel" << std::endl;
        }
        for (; it != end; ++it)
        {
            x += it->pt.x;
            y += it->pt.y;
        }
        x = (x / keyPoints.size()) / pic.width;
        y = (y / keyPoints.size()) / pic.height;
        std::cout << "[DEBUG]Median Xrel: " << x << " Median Yrel: " << y << std::endl;
        pkgReady = true;
        makePacket(x, y);
    }
}

void SwarmDetection::getDimensions()
{
    pic.width = pic.frame.size().width;
    pic.height = pic.frame.size().height;
}

void SwarmDetection::printDimensions()
{
    getDimensions();
    std::cout << " Width: " << pic.width << "Height: " << pic.height  <<std::endl;
}

void SwarmDetection::makePacket(float x, float y)
{
    packet.set_goal(x, y);
    packet.set_vehicle_id(0);
    packet.allocate(packet.min_size());
    packet.encode();
}

void SwarmDetection::startServer(SwarmDetection* p)
{
    bool running = true;
    cppsock::utility_error_t err = p->listener.setup(cppsock::make_any<cppsock::IPv4>(PORT), 1);
    std::cout << "[SERVER]server status: " << cppsock::utility_strerror(err) << ", server address: " << p->listener.sock().getsockname() << std::endl;
    if(err < 0)
    {
        throw std::logic_error("unable to setup server");
    }

    for(;;)
    {
        p->listener.accept(p->connection);

        std::cout << "[SERVER] Client connected" << std::endl;

        while(running)
        {
            running = p->sendPacket(p);
        }
        std::cout << "[SERVER] Client disconnected" << std::endl;
    }
}

bool SwarmDetection::sendPacket(SwarmDetection* p)
{
    if (p->pkgReady)
    {
        if(p->connection.send(p->packet.rawdata(), p->packet.min_size(), 0) < 0)
            return false;
        std::this_thread::sleep_for(std::chrono::milliseconds(10));
    }
    return true;
}

void SwarmDetection::carDetection(std::vector<cv::KeyPoint> *keyPoints)
{
    if (keyPoints->size() > 2)
    {
        for (size_t i = 0; i < keyPoints->size()-1; i++)
        {
            for (size_t j = 0; j < keyPoints->size(); j++)
            {
                if (getDistance(keyPoints->at(i), keyPoints->at(j)) <= 100.0f && getDistance(keyPoints->at(i), keyPoints->at(j)) >= 90.0f)
                {
                    for (size_t k = 0; k < keyPoints->size(); k++)
                    {
                        if (k != (i) &&  k!= (j))
                        {
                            if(getDistance(keyPoints->at(k), keyPoints->at(i)) && getDistance(keyPoints->at(j), keyPoints->at(k)) <=400.0f)
                                if (getDistance(keyPoints->at(k), keyPoints->at(i)) && getDistance(keyPoints->at(k), keyPoints->at(j)) >= 370.0f)
                                {
                                    std::cout << "Car Found";
                                }
                        }
                    }
                }
            }
        }
    }
}

float SwarmDetection::getDistance(cv::KeyPoint p1, cv::KeyPoint p2)
{
    float a, b;
    a = p2.pt.x - p1.pt.x;
    b = p2.pt.y - p1.pt.y;
    return sqrt(a*a+b*b);
}

int SwarmDetection::setupVideCapture(int deviceID)
{
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
    cap.read(pic.frame);
    if (pic.frame.empty()) {
        std::cerr << "ERROR! blank frame grabbed\n";
    }
    return pic.frame;
}

void SwarmDetection::showFrame(std::string windowName, cv::Mat xframe)
{
    cv::imshow(windowName, xframe);
}

void SwarmDetection::createTBarHV()
{
    cv::namedWindow("Tracking");
    cv::createTrackbar("LH", "Tracking", 0, 255);
    cv::createTrackbar("LS", "Tracking", 0, 255);
    cv::createTrackbar("LV", "Tracking", 0, 255);
    cv::createTrackbar("UH", "Tracking", 0, 255);
    cv::createTrackbar("US", "Tracking", 0, 255);
    cv::createTrackbar("UV", "Tracking", 0, 255);
    cv::moveWindow("Tracking", 20, 20);
}

void SwarmDetection::getTBarPosHV(HueValues &hvalues)
{
    hvalues.l_h = cv::getTrackbarPos("LH", "Tracking");
    hvalues.l_s = cv::getTrackbarPos("LS", "Tracking");
    hvalues.l_v = cv::getTrackbarPos("LV", "Tracking");
    hvalues.u_h = cv::getTrackbarPos("UH", "Tracking");
    hvalues.u_s = cv::getTrackbarPos("US", "Tracking");
    hvalues.u_v = cv::getTrackbarPos("UV", "Tracking");
}

void SwarmDetection::printHueValues(HueValues& hvalues)
{
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
