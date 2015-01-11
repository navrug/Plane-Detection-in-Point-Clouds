#ifndef Vec3_h_
#define Vec3_h_

#include <cmath>

struct Vec3;
Vec3 operator*(double r, const Vec3& v);

struct Vec3 {
	union {
		struct {
			double x,y,z;
		};
		double D[3];
	};

    Vec3() : x(0), y(0), z(0) { }
	Vec3(double _x, double _y, double _z)
		:x(_x), y(_y), z(_z)
	{ }

	double& operator[](unsigned int i) {
		return D[i];
	}

	const double& operator[](unsigned int i) const {
		return D[i];
	}

	double maxComponent() const {
		double r = x;
		if(y>r) r = y;
		if(z>r) r = z;
		return r;
	}

	double minComponent() const {
		double r = x;
		if(y<r) r = y;
		if(z<r) r = z;
		return r;
	}

	Vec3 operator+(const Vec3& r) const {
		return Vec3(x+r.x, y+r.y, z+r.z);
	}

	Vec3 operator-(const Vec3& r) const {
		return Vec3(x-r.x, y-r.y, z-r.z);
	}

	Vec3 cmul(const Vec3& r) const {
		return Vec3(x*r.x, y*r.y, z*r.z);
	}

	Vec3 cdiv(const Vec3& r) const {
		return Vec3(x/r.x, y/r.y, z/r.z);
	}

	Vec3 operator*(double r) const {
		return Vec3(x*r,y*r,z*r);
	}


	Vec3 operator/(double r) const {
		return Vec3(x/r, y/r, z/r);
	}

	Vec3& operator+=(const Vec3& r) {
		x+=r.x;
		y+=r.y;
		z+=r.z;
		return *this;
	}

	Vec3& operator-=(const Vec3& r) {
		x-=r.x;
		y-=r.y;
		z-=r.z;
		return *this;
	}

	Vec3& operator*=(double r) {
		x*=r; y*=r; z*=r;
		return *this;
	}

	Vec3& operator/=(double r) {
		x/=r; y/=r; z/=r;
		return *this;
	}

	// Inner/dot product
	double operator*(const Vec3& r) const {
		return x*r.x + y*r.y + z*r.z;
	}

	double norm() const {
		return sqrtf(x*x+y*y+z*z);
	}

	double normSquared() const {
		return x*x + y*y + z*z;
	}

	double distance(const Vec3& r) const {
		return sqrtf((x-r.x)*(x-r.x)+(y-r.y)*(y-r.y)+(z-r.z)*(z-r.z));
	}

    double squareDistance(const Vec3& r) const {
        return (x-r.x)*(x-r.x)+(y-r.y)*(y-r.y)+(z-r.z)*(z-r.z);
    }

    Vec3 product(const Vec3& r) const {
        return Vec3(x * r.x, y * r.y, z * r.z);
    }

    Vec3 divide(const Vec3& r) const {
        return Vec3(x / r.x, y / r.y, z / r.z);
    }

	// Cross product
	Vec3 operator^(const Vec3& r) const {
		return Vec3(
				y * r.z - z * r.y,
				z * r.x - x * r.z,
				x * r.y - y * r.x
				);
	}

	Vec3 normalized() const {
		return *this / norm();
	}

    void max(const Vec3& p) {
        x = p.x > x ? p.x : x;
        y = p.y > y ? p.y : y;
        z = p.z > z ? p.z : z;
    }

    void min(const Vec3& p) {
        x = p.x < x ? p.x : x;
        y = p.y < y ? p.y : y;
        z = p.z < z ? p.z : z;
    }
};

inline Vec3 operator*(double r, const Vec3& v) {
	return Vec3(v.x*r, v.y*r, v.z*r);
}

#endif
