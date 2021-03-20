#include "SwarmDetection.h"

void SwarmDetection::Detector()
{
    HueValues hvalues;
    cv::SimpleBlobDetector::Params params;
    std::vector<cv::KeyPoint> keyPoints;
    cv::Mat hsvFrame;
    cv::Mat mask;

    setBlobParams(0, 256, true, 30, true, 0.1f, true, 0.5f, true, 0.5f, params);

    for (;;)
    {
        frame = readFromCamera();

        cv::Mat imWithKeypoints;
        cv::drawKeypoints(frame, keyPoints, imWithKeypoints, cv::Scalar(0, 0, 255),cv::DrawMatchesFlags::DRAW_RICH_KEYPOINTS);

        getTBarPos(hvalues);

        cv::cvtColor(frame, hsvFrame, cv::COLOR_BGR2HSV);
        cv::inRange(hsvFrame, cv::Scalar(hvalues.l_h,hvalues.l_s,hvalues.l_v),cv::Scalar(hvalues.u_h,hvalues.u_s,hvalues.u_v),mask);

        //Detect all Keypoints
        #if CV_MAJOR_VERSION < 3
            cv::SimpleBlobDetector detector(params);
            detector.detect(frame,keypoints);
        #else
            cv::Ptr <cv::SimpleBlobDetector> detector = cv::SimpleBlobDetector::create(params);
            detector->detect(mask,keyPoints);
        #endif

        showFrame("Keypoints", imWithKeypoints);
        showFrame("Picture",frame);

        //Detection
        carDetection(keyPoints);       

        if (cv::waitKey(10) == 27)
        {
            std::cout << "ESC";
            break;
        }
    }
}

void SwarmDetection::carDetection(std::vector<cv::KeyPoint> keyPoints)
{
    if (keyPoints.size() > 2)
    {
        for (size_t i = 0; i < keyPoints.size()-1; i++)
        {
            for (size_t j = 0; j < keyPoints.size(); j++)
            {
                if (getDistance(keyPoints.at(i), keyPoints.at(j)) <= 100.0f && getDistance(keyPoints.at(i), keyPoints.at(j)) >= 90.0f)
                {
                    for (size_t k = 0; k < keyPoints.size(); k++)
                    {
                        if (k != (i) &&  k!= (j))
                        {
                            if(getDistance(keyPoints.at(k), keyPoints.at(i)) && getDistance(keyPoints.at(j), keyPoints.at(k)) <=400.0f)
                                if (getDistance(keyPoints.at(k), keyPoints.at(i)) && getDistance(keyPoints.at(k), keyPoints.at(j)) >= 370.0f)
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
    cap.read(frame);
    if (frame.empty()) {
        std::cerr << "ERROR! blank frame grabbed\n";
    }
    return frame;
}

void SwarmDetection::showFrame(std::string windowName, cv::Mat xframe)
{
    cv::imshow(windowName, xframe);
}

void SwarmDetection::createTBar()
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

void SwarmDetection::getTBarPos(HueValues &hvalues)
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
    getTBarPos(hvalues);
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