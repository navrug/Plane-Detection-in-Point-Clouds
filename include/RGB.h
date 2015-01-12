#ifndef RGB_H
#define RGB_H

// Red-Green-Blue color.
class RGB
{
public:
    inline RGB() :
        r(0), g(0), b(0) {}
    inline RGB(unsigned char red, unsigned char green, unsigned char blue) :
        r(red), g(green), b(blue) {}

    unsigned char r;
    unsigned char g;
    unsigned char b;
};

#endif // RGB_H
