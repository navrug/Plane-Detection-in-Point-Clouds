#ifndef RGB_H
#define RGB_H


class RGB
{
    public:
        RGB();
        RGB(unsigned char& r, unsigned char& g, unsigned char& b);
        virtual ~RGB();

        unsigned char r;
        unsigned char g;
        unsigned char b;
    protected:
    private:
};

#endif // RGB_H
