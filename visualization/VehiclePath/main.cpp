/******************************************************************************************************************************************
* Title:        Path generator for "Camera controlled shwarm robots"
* Programtitle: pathgenerator(.exe)
* Author:       Michael Reim
* Date:         24.11.2020
* Description:
*   Program that generates goals from a drawn path on an image file.
*   The programm supports most of the image filetypes (e.g. .png, .jpg,...).
*   Debug images are always .png images.
*
*   Command syntax:
*       pathgenerator(.exe) <path to input file> <path to output file> <number of goals> [<flags>]
*   Command flags:
*       -nodebug -> No debug images will be generated.
*       -log -> Debug messages will be printed an a log file.
*       -invert -> The image gets invertet to be able to use a white background.
*
*   Return values:
*       0 -> Success!
*       -1 -> There is something wrong with the command.
*       -2 -> Faild reading the image.
*       -3 -> Failed to convert to grayscale.
*       -4 -> Failed to generate goals (too many goals).
*       -5 -> Failed to print goals to a file.
*       If something is wrong with the command, a message will be printed.
*       If another error occurs, the error message will be in the current log file if log is enabled.
*
*   Note: in the code are sometimes for-loops that look like this:
*       int i;
*       for(i=0; i < end; i++)
*   and some are like this:
*       for(int i=0; i < end; i++)
*   According to the second example, 'i' has to be pushed to the stack and popped from the stack
*   every iteration of the loop. The first example may seem unnecessary but since you ended up 
*   with a lot of loop iterations, like gothrough a 4k picture, this trick can save you some runtime.
*   In case of a radixsort algotithm, with 10 000 000 elements to sort, the algorithm ran about
*   10% faster.
*
*   In my case, everytime when I ended up with a lot of loop-cycles I chose the first method and
*   if I ended up with just a few loop-cycles I chose the second method.
*   You can also go everytime for the first method!
*
* @version release 1.0.0
* @copyright (C) Michael Reim, distribution without my consent is prohibited.
*
* If there are any bugs, contact me!
******************************************************************************************************************************************/

#ifndef STB_IMAGE_IMPLEMENTATION
    #define STB_IMAGE_IMPLEMENTATION
#endif //STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>

#ifndef STB_IMAGE_WRITE_IMPLEMENTATION
    #define STB_IMAGE_WRITE_IMPLEMENTATION
#endif //STB_IMAGE_WRITE_IMPLEMENTATION
#include <stb_image_write.h>

#include <cstdio>   // for in- and output
#include <cstring>  // for sveral string-operation-functions
#include <vector>   
#include <chrono>   // for time measurement
#include <direct.h> // to create directories
#include <dirent.h> // to check if directory exists
#include <ctime>
#include "path.h"

#define PATH_THRESHOLD 10   // treshold value to determine if a pixel is path or not
#define ARG_MIN_LENGTH 4    // minimal length for argc (command length)

/*
*   Contains information for one image.
*   Members:
*       width -> width of the image in pixels
*       height -> height of the image in pixels
*       channels -> number of channels that the image contains (e.g. RGB, RGBA)
*/
struct image_info_t
{
    int width{0}, height{0};
    int channels{0};
};

/*
*   Contains a X and Y coordinate for an image.
*   Members:
*       x -> X-Coordinate of the pixel.
*       y -> Y-Coordinate of the pixel.
*/

struct img_coord_t
{
    int x, y;
};

/*
*   Is used to save the pixel-coordinates of the generated goals.
*   Contains also X and Y coordinate values.
*/

using goal_coord_t = img_coord_t;

/*
*   Is an enumeration for several flags.
*   The flags are set at the start of the program where the input arguments
*   (argc, argv) get decoded.
*   Each flag represents one bit to be able to concatinate multiple flags with a logic OR.
*   Flags:
*       PATH_NONE -> Is the default value for initialization and means that no flags are set.
*       PATH_NO_DEBUG_IMAGES -> If this flag is set no debug images will be generated.
*       PATH_LOG -> If this flag is set, debug messages will be printed to a log file.
*       PATH_INVERT -> If this flag is set, color values will be inverted to be able to use a white background.
*/

enum path_flag_type : int
{
    PATH_NONE               = 0x0,
    PATH_NO_DEBUG_IMAGES    = 0x1,
    PATH_LOG                = 0x2,
    PATH_INVERT             = 0x4
};

using path_flag_t = int;

/* ---------- PROTOTYPES ---------- */

/*
*   Converts a 2-dimensional input (X, Y) to a 1-dimensional output.
*   Used to access 1-dimensional arrays or vectors with a 2-dimensional (X, Y) input.
*   Parameters:
*       size_t x -> X value of the 2-dimensional input.
*       size_t y -> Y value of the 2-dimensional input.
*       image_into_t image_info -> image_info_t struct of the image that should be accessed.
*   Return:
*       1-dimensional array index.
*/

size_t img_at(size_t, size_t, const image_info_t&);

/*
*   Searches the given path-matrix of the given map and returns the
*   direction of that matrix.
*   Parameters:
*       std::map<Path::PathMatrix*, Path::PathDirection> mat2dir -> Map where the matrices and the directions are located.
*       Path::PathMatrix mat -> The matrix that should be searched for.
*   Return:
*       A pointer to the found matrix or.
*       'nullptr' if the matrix couldn't be found.
*/
const Path::PathDirection* get_dirp(const std::map<Path::PathMatrix*, Path::PathDirection>&, const Path::PathMatrix&);

/*
*   Determines if a pixel is a path or not.
*   Parameters:
*       uint8_t value -> Value of the current (grayscale) pixel.
*       uint8_t threshold -> Value of the treshold.
*   Return:
*       True if the pixel value is a path.
*       False if the pixel value is not a path.
*/

bool is_path(uint8_t, uint8_t);

/*
*   Generates a matrix for a given image position.
*   The given position is the center pixel.
*   If the given coordinate is the edge or the corner of the image, the surrounding
*   pixels that would be outside of the image will always be 0.
*   Parameters:
*       uint8_t* data -> Pointer to the image (pixel) data.
*       int cx -> Current X value of the center pixel.
*       int cy -> Current Y value of the center pixel.
*       image_info_t image_info -> image_info_t struct of the image that should be accessed.
*   Return:
*       Returns a 3x3 matrix for the center pixel + 8 surrounding pixels.
*   Note: The matrix contains only information whether the pixel is a path (1)
*         or it is not a path (0).
*/

Path::PathMatrix gen_pathmatrix(uint8_t*, int, int, const image_info_t&);

/*
*   Parameters:
*       path_flag_t flag -> Flag-value of the path_flag_t enum.
*   Return:
*       True if the PATH_NO_DEBUG_IMAGES is set.
*       False if the PATH_NO_DEBUG_IMAGES is not set.
*/

bool should_print_img(path_flag_t);

/*
*   Parameters:
*       path_flag_t flag -> Flag-value of the path_flag_t enum.
*   Return:
*       True if the PATH_LOG is set.
*       False if the PATH_LOG is not set.
*/

bool should_log(path_flag_t);

/*
*   Parameters:
*       path_flag_t flag -> Flag-value of the path_flag_t enum.
*   Return:
*       True if the PATH_INVERT is set.
*       False if the PATH_INVERT is not set.
*/

bool should_invert(path_flag_t flag);

/*
*   Analyzes a atring of it is a valid decimal number.
*   Parameter:
*       char* string -> Any string that should be analyzed.
*   Return:
*       True if the given string is a valid number.
*       False if the given string is not a valid number.
*/

bool is_number(const char* const);

/*
*   Reads the pixels of any given image file.
*   Parameters:
*       char* image_path -> Path to the image that should be read.
*       image_info_t& image_info -> Reference to a image_info_t struct where the corresponding image information gets written to.
*   Return:
*       Pixel data of the image.
*/

uint8_t* read_image(const char* const, image_info_t&);

/*
*   Outputs image (pixel) data.
*   Only .png filetype is supported.
*   Parameters:
*       char* image_path -> Path where the image should be saved.
*       image_info_t image_info -> Image information of the corresponding image.
*/

void write_image(const char* const, const image_info_t&);

/*
*   Converts any image to a grayscale image.
*   Undependend of the given number of channels.
*   Parameter:
*       uint8_t** data -> Pointer to the buffer where the image data is located.
*                         Furthermore, the new data gets written back into the buffer.
*                         (new buffer will be allocated and the old one gets deleted)
*       image_info_t image_info -> Struct where the new image information (of the grayscale image)
*                                  gets written to.
*/

void to_grayscale(uint8_t**, image_info_t&);

/*
*   Inverts the image to be able to use a white background instead of a white background.
*   Parameter:
*       uint8_t* data -> Data of the image.
*       image_info_t image_info -> Struct of the corresponding image information.
*/

void invert_image(uint8_t*, image_info_t&);

/*
*   Generated the path of any given image.
*   Parameters:
*       uint8_t* data -> Data of the image.
*       std::vector<img_coord_t>& path_pixels -> Vector where all the coordinates of the generated path will be saved to.
*       std::map<Path::PathMatrix*, Path::PathDirection> mat2dir -> Map where the matrices and the directions are located.
*       image_info_t image_info -> Struct of the corresponding image information.
*/

void gen_path(uint8_t*, std::vector<img_coord_t>&, const std::map<Path::PathMatrix*, Path::PathDirection>&, const image_info_t&);

/*
*   Generates goals form any given path.
*   Parameters:
*       std::vector<img_coord_t> path_pixels -> Vector with all the coordinates of the given path.
*       std::vector<goal_coord_t>& goals -> Vector where the coordinates of the generated goals will be saved to.
*       unsigned int n -> Number of goals that should be generated from the given path.
*                         Is limited to the number of coordinates the path itself has.
*   Return:
*       True if everything worked well.
*       False if there was an invalid number of goals given.
*/

bool gen_goals(const std::vector<img_coord_t>&, std::vector<goal_coord_t>&, unsigned int);

/*
*   Prints the goals into a file.
*   Parameters:
*       std::vector<goal_coord_t> goals -> Vector with all the goals that should be printed.
*       char* path -> Path to the file where the goals get printed to.
*       image_info_t image_info -> Struct of the corresponding image information.
*/

bool print_goals(const std::vector<goal_coord_t>&, const char* const, const image_info_t&);

/* ---------- FUNCTIONS ---------- */

/*
*   Note: The functions are not described twice.
*         Only the prototypes of all those functions are described via a header.
*/

size_t img_at(size_t x, size_t y, const image_info_t& ii)
{
    /*
    *   Example data with 3*3 pixels and 4 channels for each pixel (RGBA format).
    *   The data is stored in a 1-dimensional array.
    *   To get the index of a simple X-Y gid there is the following equation:
    *       index = Y * (number of elements in X-direction) + X
    * 
    *                   X ... (n pixels) * (n channels) -> elements in X
    *   ---------------------------------->
    *   R1 G1 B1 A1 R2 G2 B2 A2 R3 G3 B3 A3 |
    *   R4 G4 B4 A4 R5 G5 B5 A5 R6 R6 R6 A6 |   Y
    *   R7 G7 B7 A7 R8 G8 B8 A8 R9 G9 B9 A9 \/
    *   
    *   Resulting equation: Y * (n pixels in X) * (n channels) + X * (n channels)
    *   To get the index of the pixel 5 ('R5') the formula is like this:
    *       1 * 3 * 4 + 1 * 4 = 16 -> which is correct
    *   Note that the indices start at 0 and NOT at 1!
    */
    return y * ii.width * ii.channels + x * ii.channels;
}

const Path::PathDirection* get_dirp(const std::map<Path::PathMatrix*, Path::PathDirection>& m2d, const Path::PathMatrix& mat)
{
    // Gothrough every element of the map.
    for(auto iter = m2d.begin(); iter != m2d.end(); iter++)
    {
        // If there was found a matching element in the map, return a pointer to the second element
        // of the pair which is the direction.
        if(*iter->first == mat)
            return &iter->second;
    }
    // If there was found nothing return 'nullptr'.
    return nullptr;
}

bool is_path(uint8_t value, uint8_t threshold)
{
    // Self-explaining, otherwise see the description at the prorotypes.
    return(value < threshold) ? false : true;
}

Path::PathMatrix gen_pathmatrix(uint8_t* data, int cx, int cy, const image_info_t& ii)
{
    Path::PathMatrix mat;
    // 2-dimensional loop for a 3*3 matrix
    int x, y;   // a optimization for speed
    for(y = cy - 1; y <= cy + 1; y++)
    {
        for(x = cx - 1; x <= cx + 1; x++)
        {
            // If the x or y would be outside of the image set the value in the matrix to 'false'.
            // If the pixel is part of the path set the value in the matrix to 'true' (comes from is_path(...)).
            // If the pixel is part of the background set the value in the matrix to 'false' (comes from is_path(...)).
            mat.at(y + 1 - cy, x + 1 - cx) = (y < 0 || x < 0 || y >= ii.height || x >= ii.width) ? false : is_path(data[img_at(x, y, ii)], PATH_THRESHOLD);
        }
    }
    return mat;
}

bool should_print_img(path_flag_t flag)
{
    // fetch the corresponding bit
    return !(flag & path_flag_type::PATH_NO_DEBUG_IMAGES);
}

bool should_log(path_flag_t flag)
{
    // fetch the corresponding bit
    return flag & path_flag_type::PATH_LOG;
}

bool should_invert(path_flag_t flag)
{
    // fetch the corresponding bit
    return flag & path_flag_type::PATH_INVERT;
}

bool is_number(const char* const str)
{
    // Gothrough every character of the string and check if it's any character ranging from 0 to 9.
    // If there is not other character the string is considered as a valid number.
    for(size_t i = 0; i < strlen(str); i++)
    {
        if(str[i] < '0' || str[i] > '9')
            return false;   // Teturn 'false' if there is a character that is not in the range of 0 to 9.
    }
    return true;    // Otherwise return 'true'.
}

uint8_t* read_image(const char* const path, image_info_t& ii)
{
    // Load image with stbi's library function "stbi_load(...)".
    // Prototype: stbi_load(const char* path, int* width, int* height, int* n_channels, int channels_to_read)
    return stbi_load(path, &ii.width, &ii.height, &ii.channels, 0);
}

void write_image(uint8_t* data, const char* const path, const image_info_t& ii)
{
    // The image stride is, when you look at line 319, exactly one line of the example data.
    const size_t IMG_STRIDE = ii.width * ii.channels;
    // Write .png image with stbi's library function "stbi_write_png(...)".
    // Prototype: stbi_write_png(const char* path, int width, int height, int n_channels, const uint8_t* data, size_t image_stride)
    stbi_write_png(path, ii.width, ii.height, ii.channels, data, IMG_STRIDE);
}

void to_grayscale(uint8_t** data, image_info_t& ii)
{
    /* Grayscale: 1 color-channel that means a multiplication with the number of channels is useless.
    *                                               |  
    *                                               \/  here
    */
    uint8_t* img_gray_data = new uint8_t[ii.width * ii.height]; // Allocating the new image buffer for the grayscale image.
    image_info_t gray_ii{ii.width, ii.height, 1};               // And also declare a new image_info_t struct for it.

    // 2-dimensional loop that iterates every pixel of the image.
    int x, y;   // a optimization for speed
    for(y = 0; y < ii.height; y++)
    {
        for(x = 0; x < ii.width; x++)
        {
            unsigned int sum = 0;
            // Calculate the average value of all color components except the alpha channel if the image has one.
            // The average value of the 3 colors (RGB) corresponds to the grayscale value.
            for(int c = 0; c < ((ii.channels < 4) ? ii.channels : 3); c++)
            {
                sum += (*data)[img_at(x, y, ii) + c];
            }
            // Write the grayscale value to the new generated buffer.
            img_gray_data[img_at(x, y, gray_ii)] = sum / ii.channels;
        }
    }
    ii = gray_ii;               // update the image information
    stbi_image_free(*data);     // free the old data
    *data = img_gray_data;      // let the data-pointer point to the new data (grayscale image data)
}

void invert_image(uint8_t* data, image_info_t& ii)
{
    int x, y;
    int index;
    // 2-dimensional loop to iterate through the image
    for(y = 0; y < ii.height; y++)
    {
        for(x = 0; x < ii.width; x++)
        {
            index = img_at(x, y, ii);           // get index
            data[index] = 255 - data[index];    // invert operation
        }
    }
}

void gen_path(uint8_t* data, std::vector<img_coord_t>& path, const std::map<Path::PathMatrix*, Path::PathDirection>& m2d, const image_info_t& ii)
{
    // If the image has no pixels, there is no need to generate a path.
    if(ii.width == 0 || ii.height == 0)
        return;

    int bx = 0, by = 0;                         // Not only a optimization for speed, the variables are also used later in the code!!!
    Path::PathMatrix begin_mat;                 // Matrix of the begin pixel.
    const Path::PathDirection* dir = nullptr;   // Direction of the matrix initialized to 'nullptr'.

    // Iterate through the pixels of the image.
    // The loop breaks if the image was iterated through if no path was found or, which is more likely,
    // the loop breaks at the first pixel that has a valid matrix.
    for(by = 0; by < ii.height && dir == nullptr; by++)
    {
        for(bx = 0; bx < ii.width && dir == nullptr; bx++)
        {
            begin_mat = gen_pathmatrix(data, bx, by, ii);   // Generate matrix of current pixel.
            dir = get_dirp(m2d, begin_mat);                 // Get the direction.    
        }
    }
    
    // Subtract 1 because the loop iterates through one time too much.
    int x = --bx;
    int y = --by;

    // Current matrix.
    // Initialize it to the begin matrix because this is the starting point.
    Path::PathMatrix cur_mat = begin_mat;
    do
    {
        // Get the direction of the current matrix.
        dir = get_dirp(m2d, cur_mat);
        // Maybe the direction is invalid which means the pointer points to 'nullptr'.
        if(dir != nullptr)
        {
            path.push_back({x, y});     // Push the current x and y value into the vector because this is a valid path-coordinate.
            x += dir->direction_x();    // Add the direction to the x and y value which is returned by the direction_x() and direction_y() method.
            y += dir->direction_y();
        }
        cur_mat = gen_pathmatrix(data, x, y, ii);   // Generate the next matrix out of the new x and y value.
    } 
    while(!(x == bx && y == by) && dir != nullptr); // Break if the begin matrix has been reached again or if the direction has become invalid.
}

bool gen_goals(const std::vector<img_coord_t>& path, std::vector<goal_coord_t>& goals, unsigned int num_goals)
{
    // Invalid number of goals:
    //  If the number of goals is to big or if 'path' does not contain any coordinates.
    if(num_goals > path.size() || path.size() == 0)
        return false;

    // Calculate the width between the goals.
    float g2g_width = (float)path.size() / (float)num_goals;
    // Split the path into a certain number of goals.
    for(float i = 0; i < path.size(); i += g2g_width)
    {
        goals.push_back(path.at((size_t)i));
    }
    // Because the vehicle should drive back to the begin whre it started,
    // the first goal has to be pushed at the end of the goal-vector.
    // Only if the vector contains at least one element.
    if(goals.size() > 0)
        goals.push_back(goals.at(0));

    return true;
}

bool print_goals(const std::vector<goal_coord_t>& goals, const char* const path, const image_info_t& ii)
{
    FILE* file = fopen(path, "w");
    if(file == nullptr)
        return false;

    /* Iterate through every goal and write the position (x, y) to the file.
    *  The image coordinates are represented as NTC (Normalized Texture Coordinates)
    *  or NIC (Normalized Image Coordinates).
    *  That means the coordinates are, undependend from the size of the image, clamped
    *  to 0.0 and 1.0.
    *  This is done with following equation: NTC = NIC = (pos in px) / (size in px)
    */
    float ntc_x, ntc_y;
    for(const goal_coord_t& pos : goals)
    {
        ntc_x = (float)pos.x / (float)ii.width;
        ntc_y = (float)pos.y / (float)ii.height;
        fprintf(file, "%f %f\n", ntc_x, ntc_y);
    }
    
    fclose(file);
    return true;
}

// ITS SHOWTIME
// For command (program) syntax see header.
int main(const int argc, const char* const * const argv)
{
    /* GET CURRENT TIME */
    int64_t timenow;
    _time64(&timenow);                          // Get time in seconds (GMT).
    tm* local_time = _localtime64(&timenow);    // Get time in years, months, days, hours, minutes, seconds (local time).

    // Convert time values to strings with a 0 at the begin if the number is smaller than 10.
    char mon_str[8], day_str[8], hour_str[8], min_str[8], sec_str[8];
    sprintf(mon_str,    ((local_time->tm_mon < 10)  ? "0%hd": "%hd"), (int16_t)local_time->tm_mon + 1);
    sprintf(day_str,    ((local_time->tm_mday < 10) ? "0%hd": "%hd"), (int16_t)local_time->tm_mday);
    sprintf(hour_str,   ((local_time->tm_hour < 10) ? "0%hd": "%hd"), (int16_t)local_time->tm_hour);
    sprintf(min_str,    ((local_time->tm_min < 10)  ? "0%hd": "%hd"), (int16_t)local_time->tm_min);
    sprintf(sec_str,    ((local_time->tm_sec < 10)  ? "0%hd": "%hd"), (int16_t)local_time->tm_sec);

    // Convert date to string
    char date_str[48];
    sprintf(date_str, "%d-%s-%s", local_time->tm_year + 1900, mon_str, day_str);

    // Convert time to string
    char time_str[48], time_prefix[48];
    sprintf(time_str, "%s-%s-%s", hour_str, min_str, sec_str);
    sprintf(time_prefix, "[%s:%s:%s]", hour_str, min_str, sec_str);
  
    /* CREATE DIRECTORIES */
    // Path to the directories.
    constexpr char DEBUG_IMAGE_DIR[] = "./debug";
    constexpr char LOG_DIRECTORY[] = "./logs";

    // Try to open the directories.
    DIR* debug_dir = opendir(DEBUG_IMAGE_DIR);
    DIR* log_dir = opendir(LOG_DIRECTORY);

    // Check if the directories exist and if not create them.
    if(debug_dir == nullptr)
        mkdir(DEBUG_IMAGE_DIR);
    if(log_dir == nullptr)
        mkdir(LOG_DIRECTORY);

    // Close the directories if they are open.
    closedir(debug_dir);
    closedir(log_dir);

    /* CREATE LOG FILE */
    char filepath[256];
    sprintf(filepath, "%s/%s_log.txt", LOG_DIRECTORY, date_str);
    FILE* logfile = fopen(filepath, "a");
    if(logfile == nullptr)
    {
        printf("[ERROR] Could not open or create file: \"%s\"\n", filepath);
        return -1;
    }

    /* DECODE INPUT COMMAND */
    if(argc < ARG_MIN_LENGTH)
    {
        printf("[ERROR] Too few arguments given, required at lest 3.\n");
        return -1;
    }

    path_flag_t flags = path_flag_type::PATH_NONE;
    // Iterate through every argument that will be a flag.
    for(int i = ARG_MIN_LENGTH; i < argc; i++)
    {
        // Set flags if they should be set via the command.
        if(strcmp(argv[i], "-nodebug") == 0)
            flags |= path_flag_type::PATH_NO_DEBUG_IMAGES;
        else if(strcmp(argv[i], "-log") == 0)
            flags |= path_flag_type::PATH_LOG;
        else if(strcmp(argv[i], "-invert") == 0)
            flags |= path_flag_type::PATH_INVERT;
        else
        {
            printf("[ERROR] Invalid flag: \"%s\"\n", argv[i]);
            return -1;
        }
    }

    // Check if 4th string (3rd argument) argument is actually a number.
    // For more information see the syntax in the header.
    if(!is_number(argv[3]))
    {
        printf("[ERROR] 3rd \"%s\" argument is not a number.\n", argv[3]);
        return -1;
    }
    unsigned int num_goals;
    sscanf(argv[3], "%u", &num_goals);  // Conver the 3rd argument to a number (unsigned int).

    if(should_log(flags))
        fprintf(logfile, "\n-----------------------------------------------\n");

    /* CREATE DEBUG IMAGE FILEPATHs */
    char OUT_GRAYSCALE_PATH[256], OUT_INVERTED_PATH[256], OUT_PATHIMG_PATH[256], OUT_GOALSIMG_PATH[256];
    sprintf(OUT_GRAYSCALE_PATH, "%s/%s-%s_grayscale.png", DEBUG_IMAGE_DIR, date_str, time_str);
    sprintf(OUT_INVERTED_PATH, "%s/%s-%s_inverted.png", DEBUG_IMAGE_DIR, date_str, time_str);
    sprintf(OUT_PATHIMG_PATH, "%s/%s-%s_path.png", DEBUG_IMAGE_DIR, date_str, time_str);
    sprintf(OUT_GOALSIMG_PATH, "%s/%s-%s_goals.png", DEBUG_IMAGE_DIR, date_str, time_str);

    // Values for time measurement.
    std::chrono::microseconds t_read, t_convert, t_path, t_goals, t_print, t_exec;
    // Makte timepoints.
    std::chrono::time_point t0      = std::chrono::steady_clock::now();
    std::chrono::time_point t0_exec = std::chrono::steady_clock::now();
    // Declare image info.
    image_info_t img_info;

    /* READ IMAGE */
    if(should_log(flags))
        fprintf(logfile, "%s [INFO] Reading image: %s...\n", time_prefix, argv[1]);
    t0 = std::chrono::steady_clock::now(); // Get current time.
    uint8_t* data = read_image(argv[1], img_info);  // Read image.
    t_read = std::chrono::duration_cast<std::chrono::microseconds>(std::chrono::steady_clock::now() - t0); // Save the time difference.
    if(data == NULL)
    {
        // Exiting with -2 if loading image has failed (file not found).
        if(should_log(flags))
            fprintf(logfile, "%s [ERROR] Failed to read image: %s\n", time_prefix, argv[1]);
        return -2;
    }
    if(should_log(flags))
        fprintf(logfile, "%s [INFO] Successfully loaded image: %s\n", time_prefix, argv[1]);

    /* CONVERT TO GRAYSCALE */
    if(should_log(flags))
        fprintf(logfile, "%s [INFO] Converting to grayscale...\n", time_prefix);
    t0 = std::chrono::steady_clock::now(); // Get current time
    to_grayscale(&data, img_info);                  // Convert to grayscale.
    t_convert = std::chrono::duration_cast<std::chrono::microseconds>(std::chrono::steady_clock::now() - t0); // Save the time difference.
    if(data == NULL)
    {
        // Exit with -3 if converting to grayscale has failed.
        if(should_log(flags))
            fprintf(logfile, "%s [ERROR] Failed to convert to grayscale.\n", time_prefix);
        return -3;
    }
    // If debug is enabled, write the grayscale image.
    if(should_print_img(flags))
        write_image(data, OUT_GRAYSCALE_PATH, img_info);
    if(should_log(flags))
        fprintf(logfile, "%s [INFO] Successfully converted to grayscale.\n", time_prefix);

    /* INVERT IMAGE */
    if(should_invert(flags))
    {
        if(should_log(flags))
            fprintf(logfile, "%s [INFO] Invert image...\n", time_prefix);
        invert_image(data, img_info);   // Invert the image
        if(should_print_img(flags))
            write_image(data, OUT_INVERTED_PATH, img_info);
        if(should_log(flags))
            fprintf(logfile, "%s [INFO] Successfully inverted the image.\n", time_prefix);
    }

    /* GENERATE PATH */
    std::map<Path::PathMatrix*, Path::PathDirection> mat2dir;
    std::vector<img_coord_t> path_pixels;

    // Read all the valid matrices and their directions.
    Path::setup(mat2dir);
    if(should_log(flags))
        fprintf(logfile, "%s [INFO] Generating path...\n", time_prefix);
    t0 = std::chrono::steady_clock::now(); // Get current time.
    gen_path(data, path_pixels, mat2dir, img_info); // Generate path.
    t_path = std::chrono::duration_cast<std::chrono::microseconds>(std::chrono::steady_clock::now() - t0); // Save the time difference.
    if(should_log(flags))
        fprintf(logfile, "%s [INFO] Generated path.\n", time_prefix);

    /* GENERATE GOALS */
    std::vector<goal_coord_t> goals;
    if(should_log(flags))
        fprintf(logfile, "%s [INFO] Generating goals...\n", time_prefix);
    t0 = std::chrono::steady_clock::now(); // Get current time.
    if(!gen_goals(path_pixels, goals, num_goals))   // Generate goals.
    {
        // Exit with -4 if goal generation has failed.
        if(should_log(flags))
            fprintf(logfile, "%s [ERROR] Failed to generate goals: too many goals given.\n", time_prefix);
        return -4;
    }
    t_goals = std::chrono::duration_cast<std::chrono::microseconds>(std::chrono::steady_clock::now() - t0); // Save the time difference.
    if(should_log(flags))
        fprintf(logfile, "%s [INFO] Generated goals.\n", time_prefix);

    // If debug is enabled, write the image where the path is shown and the image where the goals are shown.
    if(should_print_img(flags))
    {
        for(const goal_coord_t& pos : goals)
        {
            data[img_at(pos.x, pos.y, img_info)] = 255;
        }
        write_image(data, OUT_GOALSIMG_PATH, img_info);

        for(const img_coord_t& pos : path_pixels)
        {
            data[img_at(pos.x, pos.y, img_info)] = 255;
        }
        write_image(data, OUT_PATHIMG_PATH, img_info);
    }

    /* PRINT GOALS TO FILE */
    if(should_log(flags))
        fprintf(logfile, "%s [INFO] Printing goals to: %s...\n", time_prefix, argv[2]);
    t0 = std::chrono::steady_clock::now(); // Get current time.
    if(!print_goals(goals, argv[2], img_info))      // Print goals.
    {
        // Exit with -5 if printing goals has failed.
        if(should_log(flags))
            fprintf(logfile, "%s [ERROR] Failed to print goals.\n", time_prefix);
        return -5;
    }
    t_print = std::chrono::duration_cast<std::chrono::microseconds>(std::chrono::steady_clock::now() - t0); // Save the time difference.
    if(should_log(flags))
        fprintf(logfile, "%s [INFO] Successfully printed goals to: %s\n", time_prefix, argv[2]);

    stbi_image_free(data);  // Free the data of the image.
    Path::cleanup(mat2dir); // Cleanup the map for the matrices.
    t_exec = std::chrono::duration_cast<std::chrono::microseconds>(std::chrono::steady_clock::now() - t0_exec); // Get execution time.
    
    // If program is not silent, print debug messages.
    if(should_log(flags))
    {
        fprintf(logfile, "\n");
        fprintf(logfile, "%s [INFO] Reading image time: %lfms\n", time_prefix, t_read.count() / 1000.0);
        fprintf(logfile, "%s [INFO] Converting to grayscale time: %lfms\n", time_prefix, t_convert.count() / 1000.0);
        fprintf(logfile, "%s [INFO] Gnerating path time: %lfms\n", time_prefix, t_path.count() / 1000.0);
        fprintf(logfile, "%s [INFO] Generating goals time: %lfms\n", time_prefix, t_goals.count() / 1000.0);
        fprintf(logfile, "%s [INFO] Printing goals time: %lfms\n", time_prefix, t_print.count() / 1000.0);
        fprintf(logfile, "%s [INFO] Execution time: %lfms\n", time_prefix, t_exec.count() / 1000.0);
    }
    fclose(logfile);

    return 0;   // you have been terminated
}