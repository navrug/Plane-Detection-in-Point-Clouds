#ifndef MATRIX_H
#define MATRIX_H

template<unsigned int N, unsigned int M>
class Matrix
{
public:
    // Return a vector of the kernel. The result is zero only if the kernel is trivial.
    static std::array<double, M> getKernel(const std::array<std::array<double, M>, N>& mat);
};

template<unsigned int N>
class Matrix<N, 0>
{
public:
    static std::array<double, 0> getKernel(const std::array<std::array<double, 0>, N>& mat);
};


template<unsigned int N, unsigned int M>
std::array<double, M> Matrix<N, M>::getKernel(const std::array<std::array<double, M>, N>& mat)
{
    // We select a line with a non-zero coefficient in the first column
    unsigned int line = 0;
    for (; line < N ; ++line)
        if (mat[line][0] != 0)
            break;
    
    // Only zeros in the first column => (1 0 ... 0) is in kernel
    if (line == N)
    {
        std::array<double, M> result;
        result[0] = 1;
        for (unsigned int i = 1 ; i < M ; ++i)
            result[i] = 0;
        return result;
    }
    
    // Recursion (Gauss pivot)
    std::array<double, M-1> tmp;
    // Only one line => trivial solution
    if (N == 1)
    {
        tmp[0] = 1;
        for (unsigned int i = 1 ; i + 1 < M ; ++i)
            tmp[i] = 0;
    }
    // Only one column => trivial solution
    // Several lines and columns => recursion
    else if (M != 1)
    {
        // We erase the first column and the selected line
        std::array<std::array<double, M-1>, N-1> _mat;
        unsigned int ii = 0;
        for (unsigned int i = 0 ; i + 1 < N ; ++i, ++ii)
        {
            if (ii == line)
                ++ii;
            double ratio = mat[ii][0] / mat[line][0];
            
            // Gauss pivot formula
            for (unsigned int j = 0 ; j + 1 < M ; ++j)
                _mat[i][j] = mat[ii][j + 1] - ratio * mat[line][j + 1];
        }
        
        // Kernel of the submatrix
        tmp = Matrix<N-1, M-1>::getKernel(_mat);
    }
    
    // We extend the kernel to the first column according to the kernel of the last N-1 columns
    std::array<double, M> result;
    result[0] = 0;
    for (unsigned int j = 0 ; j + 1 < M ; ++j)
    {
        result[j + 1] = tmp[j];
        result[0] -= tmp[j] * mat[line][j + 1] / mat[line][0];
    }
    return result;
}


template<unsigned int N>
std::array<double, 0> Matrix<N, 0>::getKernel(const std::array<std::array<double, 0>, N>& mat)
{
    return std::array<double, 0>();
}

#endif // MATRIX_H
