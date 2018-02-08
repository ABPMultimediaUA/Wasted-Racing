#include "TCamera.h"

TCamera::TCamera()
{
    parallel=false;

    //Camera parameters default initialization
    left=1.f;
    right=2.f;
    top=1.f;
    bottom=2.f;
    near=1.f;
    far=2.f;
}

void TCamera::setCameraParameters(float l, float r, float t, float b, float n, float f)
{
    left=l;
    right=r;
    top=t;
    bottom=b;
    near=n;
    far=f;
}