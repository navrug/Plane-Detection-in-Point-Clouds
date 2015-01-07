#ifndef RGB_H
#define RGB_H


class RGB
{
    public:
        RGB();
        RGB(unsigned char r, unsigned char g, unsigned char b);
        //RGB(unsigned int r, unsigned int g, unsigned int b);

        unsigned char r;
        unsigned char g;
        unsigned char b;
};

#endif // RGB_H
