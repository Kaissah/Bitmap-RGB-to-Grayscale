#include <iostream>
#include <fstream>
#include <chrono>

using namespace std;

int main() {
    chrono::nanoseconds duration_ns(0);
    ifstream fIn("photo.bmp", ios::binary);
    ofstream fOut("gray_photo_seq.bmp", ios::binary);

    if (!fIn) {
        cerr << "Error: Input file does not exist.\n";
        return 1;
    }

    unsigned char header[54];
    fIn.read((char*)header, 54);
    fOut.write((char*)header, 54);

    // Extract width, height, and bit depth
    int width = *(int*)(&header[18]);
    int height = *(int*)(&header[22]);

    cout << "Width: " << width << endl;
    cout << "Height: " << height;
    
    int size = width * height;
    
    auto start = chrono::high_resolution_clock::now();
    // Process each pixel: RGB → Grayscale
    for (int i = 0; i < size; ++i) {
        unsigned char blue = fIn.get(); // Blue
        unsigned char green = fIn.get(); // Green
        unsigned char red = fIn.get(); // Red

        // Grayscale conversion
        unsigned char gray = (0.3 * blue + 0.59 * green + 0.11 * red);

        // Write gray pixel (R=G=B=gray)
        fOut.put(gray);
        fOut.put(gray);
        fOut.put(gray);
        
    }
    auto stop = chrono::high_resolution_clock::now();
    duration_ns = chrono::duration_cast<chrono::nanoseconds>(stop - start);

    fIn.close();
    fOut.close();

    cout << "\nGrayScale Conversion time [Sequential]:" << duration_ns.count() << " ns\n";
    cout << "Image successfully converted to grayscale.\n";
    return 0;
}
