#ifndef COLORREF_H
#define COLORREF_H

#include "RGB.h"

class ColorRef
{
    public:
        ColorRef() { parent = 0; }
        virtual ~ColorRef() {}
        ColorRef* getRoot() {
            if (parent == 0)
                return this;
            else {
                parent = parent->getRoot();
                return parent;
            }
        }
        void setNewRef(ColorRef* newRef) {
            parent = newRef;
        }
        ColorRef* parent;
        RGB rgb;
    protected:
    private:
};

#endif // COLORREF_H
