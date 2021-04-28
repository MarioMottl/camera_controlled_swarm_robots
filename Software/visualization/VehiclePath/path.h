#ifndef __path_h__
#define __path_h__

#include <map>

namespace Path
{
    /*
    *   Class: PathMatrix
    *   Saves booleans in form of a 3x3 matrix.
    *   This class is used in order to save data whether a pixel is part of the path or not.
    */
    class PathMatrix
    {
    private:
        bool mat[3][3];  // (3*3) matrix

    public:
        
        
        // The default constructor sets every boolean to 'false',
        PathMatrix(void) : PathMatrix(0,0,0, 0,0,0, 0,0,0) {}

        // Every variable of the matrix can be set via this constructor.
        PathMatrix(bool m0n0, bool m1n0, bool m2n0, bool m0n1, bool m1n1, bool m2n1, bool m0n2, bool m1n2, bool m2n2)
        {
            this->mat[0][0] = m0n0;
            this->mat[0][1] = m1n0;
            this->mat[0][2] = m2n0;
            this->mat[1][0] = m0n1;
            this->mat[1][1] = m1n1;
            this->mat[1][2] = m2n1;
            this->mat[2][0] = m0n2;
            this->mat[2][1] = m1n2;
            this->mat[2][2] = m2n2;
        }

        // Destructor is not used in this class.
        virtual ~PathMatrix(void)
        {
            /* dtor */
        }

        /*
        *   Access a single value of the matrix.
        *   Parameters:
        *       size_t m -> Row of the matrix (Y).
        *       size_t n -> Column of the matrix (X).
        *   Return:
        *       A non-const reference to the element (m, n).
        */
        bool& at(size_t m, size_t n)
        {
            return this->mat[n][m];
        }

        /*
        *   This is the same method like the previous one.
        *   The difference is that this method can be called
        *   when the object is constant.
        *   Return:
        *       The value of the element (m, n).
        */

        bool at(size_t m, size_t n) const
        {
            return this->mat[n][m];
        }

        /*
        *   Compares 2 objects of unequality.
        *   Parameter:
        *       PathMatrix other -> The object that should be compared with the own object.
        *   Return:
        *       True if the objects are unequal.
        *       False if the objects are equal.
        */

        bool operator!= (const PathMatrix& other) noexcept
        {
            // Iterate through the matrix.
            for(size_t n = 0; n < 3; n++)
            {
                for(size_t m = 0; m < 3; m++)
                {
                    // Return 'true' if one value is not equal.
                    if(this->mat[n][m] != other.mat[n][m])
                        return true;
                }
            }
            return false;
        }

        /*
        *   Campares 2 objects of quality.
        *   Parameter:
        *       PathMatrix other -> The object that should be compared with the own object.
        *   Return:
        *       True if the objects are equal.
        *       False if the objects are unequal.
        */ 

        bool operator== (const PathMatrix& other) noexcept
        {
            return !(*this != other);
        }

    };

    /*
    *   Class: PathDirection
    *   Saves a X and Y direction for the path.
    *   Possible values are for the direction:
    *       -1 (backwards)
    *       0 (no changes)
    *       1 (foreward)
    */

    class PathDirection
    {
    private:
        int dir_x, dir_y;

    public:
        // Default constructor initializes to default values (0).
        PathDirection(void) : PathDirection(0, 0) {}

        // Initializes the direction with custom values.
        PathDirection(int dir_x, int dir_y)
        {
            this->dir_x = dir_x;
            this->dir_y = dir_y;
        }

        // Destructor is not used.
        virtual ~PathDirection(void)
        {
            /* dtor */
        }

        /*
        *   Returns a non-const reference to the X-direction.
        *   Read and write access to that value.
        */
        int& direction_x(void) noexcept
        {
            return this->dir_x;
        }

        /*
        *   Returs the X-direction value.
        *   Used for constant objects.
        *   Readonly access to that value.
        */

        int direction_x(void) const noexcept
        {
            return this->dir_x;
        }

        /*
        *   Returns a non-const reference to the Y-direction.
        *   Read and write access to that value.
        */

        int& direction_y(void) noexcept
        {
            return this->dir_y;
        }

        /*
        *   Returs the Y-direction value.
        *   Used for constant objects.
        *   Readonly access to that value.
        */

        int direction_y(void) const noexcept
        {
            return this->dir_y;
        }
    };

    /*
    *   Initializes a matrix-to-direction-map with all possible
    *   matrices that are valid for the path and saves the
    *   corresponding directions.
    *   Parameters:
    *       std::map<PathMatrix*, PathDirection>& mat2dir -> Matrix-to-direction-map where the data should be saved to.
    */

    void setup(std::map<PathMatrix*, PathDirection>& m2d)
    {
        PathMatrix* p = new PathMatrix(0,0,0, 0,1,1, 0,1,1);
        m2d[p] = PathDirection(1, 0);
        p = new PathMatrix(0,0,1, 0,1,1, 0,1,1);
        m2d[p] = PathDirection(1, 0);
        p = new PathMatrix(1,1,1, 0,1,1, 0,1,1);
        m2d[p] = PathDirection(1, 0);
        p = new PathMatrix(0,1,1, 0,1,1, 0,1,1);
        m2d[p] = PathDirection(1, 0);
        p = new PathMatrix(0,1,1, 0,1,1, 0,0,1);
        m2d[p] = PathDirection(1, 1);
        p = new PathMatrix(1,1,1, 0,1,1, 0,0,1);
        m2d[p] = PathDirection(1, 1);
        p = new PathMatrix(1,1,1, 1,1,1, 0,0,1);
        m2d[p] = PathDirection(1, 1);
        p = new PathMatrix(0,1,1, 0,1,1, 0,0,0);
        m2d[p] = PathDirection(0, 1);
        p = new PathMatrix(1,1,1, 0,1,1, 0,0,0);
        m2d[p] = PathDirection(0, 1);
        p = new PathMatrix(1,1,1, 1,1,1, 1,0,0);
        m2d[p] = PathDirection(0, 1);
        p = new PathMatrix(1,1,1, 1,1,1, 0,0,0);
        m2d[p] = PathDirection(0, 1);
        p = new PathMatrix(1,1,1, 1,1,0, 0,0,0);
        m2d[p] = PathDirection(-1, 1);
        p = new PathMatrix(1,1,1, 1,1,0, 1,0,0);
        m2d[p] = PathDirection(-1, 1);
        p = new PathMatrix(1,1,1, 1,1,0, 1,1,0);
        m2d[p] = PathDirection(-1, 1);
        p = new PathMatrix(1,1,0, 1,1,0, 0,0,0);
        m2d[p] = PathDirection(-1, 0);
        p = new PathMatrix(1,1,0, 1,1,0, 1,0,0);
        m2d[p] = PathDirection(-1, 0);
        p = new PathMatrix(1,1,0, 1,1,0, 1,1,1);
        m2d[p] = PathDirection(-1, 0);
        p = new PathMatrix(1,1,0, 1,1,0, 1,1,0);
        m2d[p] = PathDirection(-1, 0);
        p = new PathMatrix(1,0,0, 1,1,0, 1,1,0);
        m2d[p] = PathDirection(-1, -1);
        p = new PathMatrix(1,0,0, 1,1,0, 1,1,1);
        m2d[p] = PathDirection(-1, -1);
        p = new PathMatrix(1,0,0, 1,1,1, 1,1,1);
        m2d[p] = PathDirection(-1, -1);
        p = new PathMatrix(0,0,0, 1,1,0, 1,1,0);
        m2d[p] = PathDirection(0, -1);
        p = new PathMatrix(0,0,1, 1,1,1, 1,1,1);
        m2d[p] = PathDirection(0, -1);
        p = new PathMatrix(0,0,0, 1,1,0, 1,1,1);
        m2d[p] = PathDirection(0, -1);
        p = new PathMatrix(0,0,0, 1,1,1, 1,1,1);
        m2d[p] = PathDirection(0, -1);
        p = new PathMatrix(0,0,0, 0,1,1, 1,1,1);
        m2d[p] = PathDirection(1, -1);
        p = new PathMatrix(0,0,1, 0,1,1, 1,1,1);
        m2d[p] = PathDirection(1, -1);
        p = new PathMatrix(0,1,1, 0,1,1, 1,1,1);
        m2d[p] = PathDirection(1, -1);

        p = new PathMatrix(0,0,1, 0,1,1, 0,0,1);
        m2d[p] = PathDirection(1, 1);
        p = new PathMatrix(1,1,1, 0,1,0, 0,0,0);
        m2d[p] = PathDirection(-1, 1);
        p = new PathMatrix(1,0,0, 1,1,0, 1,0,0);
        m2d[p] = PathDirection(-1, -1);
        p = new PathMatrix(0,0,0, 0,1,0, 1,1,1);
        m2d[p] = PathDirection(1, -1);

        p = new PathMatrix(1,1,1, 0,1,1, 1,1,1);
        m2d[p] = PathDirection(1, -1);
        p = new PathMatrix(1,1,1, 1,1,1, 1,0,1);
        m2d[p] = PathDirection(1, 1);
        p = new PathMatrix(1,1,1, 1,1,0, 1,1,1);
        m2d[p] = PathDirection(-1, 1);
        p = new PathMatrix(1,0,1, 1,1,1, 1,1,1);
        m2d[p] = PathDirection(-1, -1);
    }

    /*
    *   Cleanes up the matrix: deletes dynamic memory.
    *   Parameters:
    *       std::map<PathMatrix*, PathDirection> mat2dir -> The map that should be cleaned up.
    */

    void cleanup(const std::map<PathMatrix*, PathDirection>& m2d)
    {
        for(auto iter = m2d.begin(); iter != m2d.end(); iter++)
        {
            delete(iter->first);
        }
    }
};

/* DEFINE PATH-MATRICES */

#endif // __path_h__