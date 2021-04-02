#pragma once
#include "opencv2/opencv.hpp"
#include <iostream>
#include <cmath>
#include <thread>
#include <chrono>
#include <atomic>
#include <packet.h>
#include <exception>
#include "../../library/cppsock/cppsock.hpp"
#include <fstream>

#define PORT 10001
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

struct PicData
{
    cv::Mat frame;
    cv::Mat rFrame;
    size_t width;
    size_t height;
};


//cppsock::tcp::socket_collection sc(Swarmserver::on_insert, Swarmserver::on_recv, Swarmserver::on_disconnect);


class SwarmDetection
{
private:
    PicData pic;
    cv::VideoCapture cap;
    std::vector <Car> cars[N_CARS];
    Schwarm::GoalPacket packet;
    cppsock::tcp::listener listener;
    cppsock::tcp::socket connection;
    std::atomic_bool pkgReady = false;
    HueValues hvalues;

public:
    SwarmDetection();
    ~SwarmDetection();

    /**
     * @brief Sets up the VideoCapture
     * @param deviceID -> Hardware ID of the camera thats going to be used.
     * @return -> returns 0 for success -1 if an error occurred
     */
    int setupVideCapture(int deviceID);

    /**
    * @brief Reads out one frame and returns it
    * @return -> returns one frame
    */
    cv::Mat readFromCamera();

    /**
    * @brief Displays the frame provided with the provided window name
    * @param windowName -> name of the Window thats being displayed
    * @param frame -> frame thats going to be displayed
    */
    static void showFrame(std::string windowName, cv::Mat frame);

    /**
    * @brief Creates a Track bar for the lower and upper values of the pixels that need to be tracked
    */
    void createTBarHV();


    /**
    * @brief Reads out the Track bar positions and returns it.
    * @return @param hvalues -> struct of values of Track bar
    */
    void getTBarPosHV(HueValues& hvalues);


    /**
    * @brief Debug Output: Prints out the Hue values of the Track bar
    */
    void printHueValues(HueValues& hvalues);

    /**
    * @brief Sets all the parameters for the Blob detection that is needed to extract all cars from the frame.
    * @param minThreshold -> minimum Threshold for conversion into binary file
    * @param maxThreshold -> maximum Threshold for conversion into binary file
    * @param filterByArea -> only by area filtered pixels
    * @param minArea -> minimum area that should be considered
    * @param filterByCircularity -> only circular shapes should be considered
    * @param minCircularity -> minimum circularity that should be considered
    * @param filterByConvexity -> extracted blobs have to have convexity (convex hull algorithm)
    * @param minConvexity -> minimum convexity
    * @param filterByInertia -> extracted blob have to have inertia between min / max inertia
    * @param minInertiaRatio -> minimum inertia
    * @return @param params -> Returns the parameter values at once for easier setup
    */
    void setBlobParams(float minThreshhold, float maxThreshhold, bool filterByArea, float minArea, bool filterByCircularity, float minCircularity, bool filterByConvexity, float minConvexity, bool filterByInertia, float minInertiaRatio, cv::SimpleBlobDetector::Params& params);
    
    /**
    * @brief Looks for valid cars indication points in the keyPoints vector
    * @param keyPoints -> keypoints that have been retracted from the frame
    */
    static void carDetection(std::vector <cv::KeyPoint> *keyPoints);
    
    /**
    * @brief Returns the distance between to KeyPoints
    * @param p1 -> Keypoints 1
    * @param p2 -> Keypoints 2
    * @return returns the distance between the two points
    */
    static float getDistance(cv::KeyPoint p1, cv::KeyPoint p2);

    /**
    * @brief Loops the video stream. Starts Track bar. Detects all Keypoints.
    */
    void Detector();

    /**
    * @brief Draws Keypoints onto an frame.
    * @param xframe -> frame where Keypoints are gonna be drawn on
    * @param keyPoints -> Pointer to the Keypoint Vector 
    * @param p -> this pointer
    */
    static void drawKeyPoints(cv::Mat xframe, std::vector <cv::KeyPoint>* keyPoints,SwarmDetection *p);

    /**
    * @brief A simpler much faster version of the carDetection 
    *        currently only works with one car
    * @param keyPoints -> Pointer to the keypoint vector
    */
    void simpleCarDetection(std::vector<cv::KeyPoint> keyPoints);


    /**
    * @brief Gets the dimensions of the frame
    */
    void getDimensions();

    /**
    * @brief Prints the dimensions of the frame into the console
    */
    void printDimensions();

    void makePacket(float x, float y);

    static void startServer(SwarmDetection *p);

    static bool sendPacket(SwarmDetection *p);
};