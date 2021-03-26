#pragma once
#include "opencv2/opencv.hpp"
#include <iostream>
#include <cmath>
#include <thread>

#define N_CARS 1        //max amount of cars in frame

/**
* @brief Hue values for the detection of keypoints
*/
struct HueValues
{
    int l_h;
    int l_s;
    int l_v;
    int u_h;
    int u_s;
    int u_v;
};

/**
* @brief Car implementation
*/
struct Car
{
    float x, y;
    float rotation;
};

class SwarmDetection
{
private:
    cv::Mat frame;
    cv::VideoCapture cap;
    std::vector <Car> cars[N_CARS];

public:
    SwarmDetection() = default;
    ~SwarmDetection() = default;

    /**
     * @brief Sets up the VideoCapture
     * @param deviceID -> Hardware ID of the camera thats going to be used.
     * @return -> returns 0 for success -1 if an error occoured
     */
    int setupVideCapture(int deviceID);

    /**
    * @brief Reads out one frame and returns it
    * @return -> returns one frame
    */
    cv::Mat readFromCamera();

    /**
    * @brief Displays the frame provided with the provided windowname
    * @param windowName -> name of the Window thats being displayed
    * @param frame -> frame thats going to be displayed
    */
    static void showFrame(std::string windowName, cv::Mat frame);

    /**
    * @brief Creates a Trackbar for the lower and upper values of the pixels that need to be tracked
    */
    void createTBar();

    /**
    * @brief Reads out the Trackbar positions and returns it.
    * @return @param hvalues -> struct of values of Trackbar
    */
    void getTBarPos(HueValues& hvalues);

    /**
    * @brief Debug Output: Prints out the Hue values of the Trackbar
    */
    void printHueValues(HueValues& hvalues);

    /**
    * @brief Sets all the parametes for the Blobdetection that is needed to extract all cars from the frame.
    * @param minThreshold -> minimum Threshhold for conversion into binary file
    * @param maxThreshold -> maximum Threshhold for conversion into binary file
    * @param filterByArea -> only by area filtered pixels
    * @param minArea -> minimum area that should be considered
    * @param filterByCircularity -> only circular shapes should be considered
    * @param minCircularity -> minimum circularity that should be considered
    * @param filterByConvexity -> extracted blobs have to have convecity (convex hull algorithm)
    * @param minConvexity -> miminum convexity
    * @param filterByInertia -> extracted blob have to have inertioa between min / max inertia
    * @param minInertiaRatio -> minimum inertia
    * @return @param params -> Returns the parameter values at once for easier setup
    */
    void setBlobParams(float minThreshhold, float maxThreshhold, bool filterByArea, float minArea, bool filterByCircularity, float minCircularity, bool filterByConvexity, float minConvexity, bool filterByInertia, float minInertiaRatio, cv::SimpleBlobDetector::Params& params);
    
    /**
    * @brief Looks for valid cars indication points in the keyPoints vector
    * @param keyPoints -> keypoints that have been etracted from the frame
    */
    static void carDetection(std::vector <cv::KeyPoint> *keyPoints);
    
    /**
    * @brief Returns the distance between to KeyPoints
    * @param p1 -> Keypoint 1
    * @param p2 -> Keypoint 2
    * @return returns the distance between the two points
    */
    static float getDistance(cv::KeyPoint p1, cv::KeyPoint p2);

    /**
    * @brief Loops the video stream. Starts Trackbar. Detects all Keypoints.
    */
    void Detector();

    /**
    * 
    * 
    */
    static void drawKeyPoints(cv::Mat xframe, std::vector <cv::KeyPoint>* keyPoints,SwarmDetection *p);
};