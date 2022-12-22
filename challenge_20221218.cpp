/*
 *  CHALLENGE 2022-12-18
 *  Puzzle source: https://adventofcode.com/
 *  Solution: Danny van der Meeren <danny@illogic.nl>
 *  License: Free to use without limitations
 */

#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>
#include <chrono>

typedef struct 
{
    int x;
    int y;
    int z;
} xyz_t;

typedef std::vector <xyz_t> xyz_vect;
typedef std::vector <xyz_t>::iterator xyz_iter;

class App
{
    public:
        App() 
        { 
            dim = {0,0,0};
        }

        ~App() { }

        void run()
        {
            int all_sides, void_sides;

            load("chal_20221218.txt");
            // load("chal_20221218_test.txt");

            /* Analyze substance */
            all_sides = analyze(1);

            /* trace voids in model */
            traceVoids();

            /* analyze surface of all voids */
            void_sides = analyze(0);

            std::cout << "All sides without voids: " << (all_sides - void_sides) << std::endl;

            cleanup();
        }


        void load (const std::string fname)
        {
            std::cout << "Loading file: '" << fname << "'" << std::endl;

            xyz_vect data;                   
            int x, y, z;
            char c;

            std::ifstream ifs (fname);
            
            data.reserve(2200);

            /* get dimensions and organize input */
            while ( ifs >> x >> c >> y >> c >> z ) {
                if (x >= dim.x) dim.x = x + 1;
                if (y >= dim.y) dim.y = y + 1;
                if (z >= dim.z) dim.z = z + 1;

                data.push_back({x, y, z});
            }

            
            /* alloc */
            grid = new uint8_t**[dim.x];
            for (int i = 0; i < dim.x; i++) 
            {
                grid[i] = new uint8_t*[dim.y];
                for (int j = 0; j < dim.y; j++) 
                {
                    grid[i][j] = new uint8_t[dim.z];
                    memset((void *) grid[i][j], 0x0, sizeof(uint8_t)* dim.z);
                }
            }
            
            printf ("Grid dimensions (xyz): %d, %d, %d\n", dim.x, dim.y, dim.z);

            /* layout */
            for (xyz_iter iter = data.begin(); iter < data.end(); iter++) {
                grid[iter->x][iter->y][iter->z] = 1;
            }
        }

        void cleanup()
        {
            for (int i = 0; i < dim.x; i++) {
                for (int j = 0; j < dim.y; j++) {
                    delete grid[i][j];
                }
                delete grid[i];
            }
            delete grid;
        }

        unsigned int analyze (uint8_t marker)
        {
            unsigned int sides = 0;

            for ( int x=0; x < dim.x; x++ ) {
                for ( int y=0; y < dim.y; y++ ) {
                    for ( int z=0; z < dim.z; z++ ) {

                        /* Has a lava droplet */
                        if ( grid[x][y][z] == marker )
                        {
                            
                            if (x > 0)
                            {
                                if (x == (dim.x-1))
                                    sides++;
                                else
                                {
                                    if (grid[x+1][y][z] != marker)
                                        sides++;                                        
                                }

                                if (grid[x-1][y][z] != marker)
                                    sides++;             
                            }
                            else
                                sides++;
                            

                            if (y > 0)
                            {
                                if (y == (dim.y-1))
                                    sides++;
                                else
                                {
                                    if (grid[x][y+1][z] != marker)
                                        sides++;                                        
                                }

                                if (grid[x][y-1][z] != marker)
                                    sides++;             
                            }
                            else
                                sides++;


                            if (z > 0)
                            {
                                if (z == (dim.z-1))
                                    sides++;
                                else
                                {
                                    if (grid[x][y][z+1] != marker)
                                        sides++;                                        
                                }

                                if (grid[x][y][z-1] != marker)
                                    sides++;             
                            }
                            else
                                sides++;

                        }
                    }
                }
            }

            std::cout << "Sides found: " << sides << " for marker: " << (int) marker << std::endl;

            return sides;
        }

        void trace(int x, int y, int z, uint8_t tracert)
        {
            if (x < 0 || y < 0 || z < 0 || x >= dim.x || y >= dim.y || z >= dim.z )
                return; // OOB

            if (grid[x][y][z] != 0)
                return;
            
            grid[x][y][z] = tracert;

            trace(x-1, y, z, tracert);
            trace(x, y-1, z, tracert);
            trace(x, y, z-1, tracert);

            trace(x+1, y, z, tracert);
            trace(x, y+1, z, tracert);
            trace(x, y, z+1, tracert);
        }

        void traceVoids ()
        {
            std::cout << "Tracing voids..." << std::endl;

            /* Start trace from outsides */
            for ( int x = 0 ; x < dim.x ; x++ ) {
                for (int y=0; y < dim.y; y++) {
                    trace(x, y, 0, 2);
                    trace(x, y, dim.z-1, 2);
                }
                for (int z=0; z < dim.z; z++) {
                    trace(x, 0, z, 2);
                    trace(x, dim.y-1, z, 2);
                }
            }            
            for (int y=0; y < dim.y; y++) {
               for (int z=0; z < dim.z; z++) {
                    trace(0, y, z, 2);
                    trace(dim.x-1, y, z, 2);
                }
            }
        }

    private:
        xyz_t dim;
        uint8_t ***grid = NULL;
};



int main()
{
    App app;
    app.run();

    return 0;
}