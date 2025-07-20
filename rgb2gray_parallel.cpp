#include <iostream>
#include <fstream>
#include <chrono>

using namespace std;

int main() {
    
    ifstream fIn("photo.bmp", ios::binary);
    ofstream fOut("gray_photo_par.bmp", ios::binary);

    if (!fIn) {
        cerr << "Error: Input file does not exist.\n";
        return 1;
    }

    unsigned char header[54];
    fIn.read((char*)header, 54);
    fOut.write((char*)header, 54);

    int width = *(int*)(&header[18]);
    int height = *(int*)(&header[22]);
    int rowSize = ((width *3 +3)/4)*4;

    cout << "Width: " << width << endl;
    cout << "Height: " << height;

    unsigned char *row = new unsigned char[rowSize];

    chrono::nanoseconds duration_ns(0);

    for (int i= 0;i<height; ++i){
        fIn.read((char*) row,rowSize);

        auto start = chrono::high_resolution_clock::now();
        #pragma acc data copy(row[0:rowSize])
        {
            #pragma acc parallel loop
                    // check each pixel rgb to grayscale
                    
                    for (int j = 0; j < width; ++j) {
                        int indx = j *3;
                        unsigned char blue = row[indx]; // Blue
                        unsigned char green = row[indx+1]; // Green
                        unsigned char red = row[indx+2]; // Red

                        // Grayscale conversion
                        unsigned char gray = (0.3 * blue + 0.59 * green + 0.11 * red);

                        row[indx] = gray; 
                        row[indx+1] = gray; 
                        row[indx+2] = gray; 
                    }
                    
        }
        auto stop = chrono::high_resolution_clock::now();
        duration_ns += chrono::duration_cast<chrono::nanoseconds>(stop - start);
        fOut.write((char*)row, rowSize);
    }

    delete[] row;

    fIn.close();
    fOut.close();

    cout << "\nGrayScale Conversion time [PARALLEL]: " << duration_ns.count() << " ns\n";
    cout << "Image successfully converted to grayscale.\n";
    return 0;
}
