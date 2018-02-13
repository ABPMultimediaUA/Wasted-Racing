#include "TCamera.h"

TCamera::TCamera(float z)
{
    parallel=false;

    //Camera parameters default initialization
    left=1.f;
    right=2.f;
    top=1.f;
    bottom=2.f;
    near=1.f;
    far=2.f;
    zoom = z;

    projectionMatrix() = glm::perspective(zoom, (float)1280 / (float)720, near, far);
}

void TCamera::setCameraParameters(float l, float r, float t, float b, float n, float f)
{
    left=l;
    right=r;
    top=t;
    bottom=b;
    near=n;
    far=f;

    projectionMatrix() = glm::perspective(zoom, (float)1280 / (float)720, near, far);
}

void TCamera::beginDraw()
{
    viewMatrix() = modelMatrix();
}