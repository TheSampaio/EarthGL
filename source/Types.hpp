#ifndef _EARTHGL_TYPES_
#define _EARTHGL_TYPES_

using std::array;
using std::string;

typedef unsigned char uchar;
typedef unsigned short ushort;
typedef unsigned int uint;
typedef unsigned long ulong;

typedef long long llong;
typedef unsigned long long ullong;

struct Vertex
{
    Vertex(glm::vec3 position);

    glm::vec3 position;
};

#endif
