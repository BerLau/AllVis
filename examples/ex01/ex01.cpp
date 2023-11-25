#include <stdio.h>
#include "math/base.h"
#include "vector.h"
int main()
{
    int a = 2;
    printf("a = %d, ", a);
    if (Core::Math::sign(a) == 1)
    {
        printf("which is positive\n");
    }
    else
    {
        printf("which is negative\n");
    }

    Core::Vector v1 = Core::Vector::ones(3);
    Core::Vector v2 = Core::Vector::ones(3);
    Core::Vector v3 = v1 + v2;
    printf("v3 = [%f, %f, %f]\n", v3[0], v3[1], v3[2]);

    return 0;
}