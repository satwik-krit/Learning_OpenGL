#include <stdio.h>
#include <stdlib.h>
#include <math.h>

// TODO: Implement swizzling.

constexpr int VEC4_W_DEFAULT = 1;

typedef union
{
    struct
    {
        float x, y, z;
    };

    struct
    {
        float r, g, b;
    };
} vec3_t;

typedef union
{/* {{{ */
    struct 
    {
        union
        {
            union
            {
                vec3_t xyz;

                struct
                {
                    float x, y, z;
                };
            };
            float w;
        };
    };

    float members[4];
/* }}} */
} vec4_t;

typedef union
{
    float cont_members[16];
    float members[4][4];

    /* mij -> ith column, jth row (The way OpenGL expects) */
    struct 
    {
        float m00, m10, m20, m30;
        float m01, m11, m21, m31;
        float m02, m12, m22, m32;
        float m03, m13, m23, m33;
    };
} mat4_t;

mat4_t Mat4 (float m00, float m10, float m20, float m30,
             float m01, float m11, float m21, float m31,
             float m02, float m12, float m22, float m32,
             float m03, float m13, float m23, float m33)
{
    return (mat4_t) { m00, m10, m20, m30,
                      m01, m11, m21, m31,
                      m02, m12, m22, m32,
                      m03, m13, m23, m33, };
}/* }}} */

mat4_t
Mat4Scale (vec4_t* scale)
{/* {{{ */
    // TODO: Use matrix multiplication
    float x = scale->x, y = scale->y, z = scale->z;
    return Mat4(x, 0, 0, 0,
            0, y, 0, 0,
            0, 0, z, 0,
            0, 0, 0, 1 );
}/* }}} */

mat4_t
Mat4MultiplyScalar (mat4_t* mat, float a)
{/* {{{ */
    mat4_t new_mat;
    for (int i = 0; i<16; i++)
    {
        new_mat.cont_members[i] = a * mat->cont_members[i];
    }
    return new_mat;
}/* }}} */

// Matrices are multiplied in the order of arguments provided
mat4_t
Mat4MultiplyMat4 (mat4_t mat1, mat4_t mat2)
{/* {{{ */
    float sum;
    mat4_t mat = {};
    // Traversing column of mat
    for (int i = 0; i < 4; i++)
    {
        // Traversing row of mat
        for (int j = 0; j < 4; j++)
        {
            sum = 0;
            for (int k = 0; k < 4; k++)
            {
                sum = sum + (mat1.members[i][k] * mat2.members[k][j]);
                printf("sum:%f\n",sum);
            }
            mat.members[i][j] = sum;
            printf("%f\n ", mat.members[i][j]);
        }
        
    }

    for (int i = 0; i < 4; i++)
    {
        printf("\n");
        for (int j = 0; j < 4; j++)
            printf("%f ", mat.members[i][j]);
    };

    printf("mat1\n");
    for (int i = 0; i < 4; i++)
    {
        printf("\n");
        for (int j = 0; j < 4; j++)
            printf("%f ", mat1.members[i][j]);
    };
    printf("mat2\n");
    for (int i = 0; i < 4; i++)
    {
        printf("\n");
        for (int j = 0; j < 4; j++)
            printf("%f ", mat2.members[i][j]);
    };
}/* }}} */

mat4_t
Mat4Translate (vec4_t* offset)
{/* {{{ */
    float x = offset->x, y = offset->y, z = offset->z;
    // TODO: Use matrix multiplication
    return Mat4(1, 0, 0, x,
                0, 1, 0, y,
                0, 0, 1, z,
                0, 0, 0, 1 );
}/* }}} */

mat4_t
Mat4Identity ()
{/* {{{ */
    return Mat4(1, 0, 0, 0,
                0, 1, 0, 0,
                0, 0, 1, 0,
                0, 0, 0, 1 );
}/* }}} */

mat4_t
Mat4Null ()
{/* {{{ */
    return Mat4(0, 0, 0, 0,
                0, 0, 0, 0,
                0, 0, 0, 0,
                0, 0, 0, 0);
}/* }}} */

vec4_t 
Vec4 (float x, float y, float z, float w)
{/* {{{ */
    return (vec4_t) {x, y, z};
}/* }}} */

vec4_t
Vec4Add (vec4_t* a, vec4_t* b)
{/* {{{ */
    // Don't mess with the w component!
    return (vec4_t) {a->x + b->x, a->y + b->y, a->z + b->z};
}/* }}} */

vec4_t 
Vec4Subtract (vec4_t* a, vec4_t* b)
{/* {{{ */
    // Don't mess with the w component!
    return (vec4_t) {a->x - b->x, a->y - b->y, a->z - b->z};
}/* }}} */

vec4_t
Vec4AddScalar (vec4_t* a, float s)
{/* {{{ */
    return (vec4_t) {a->x + s, a->y + s, a->z + s};
}/* }}} */

float
Vec4Mag (vec4_t* a)
{/* {{{ */
    float mag =  (a->x)*(a->x) + (a->y)*(a->y) + (a->z)*(a->z);
    return sqrtf(mag);
}/* }}} */

vec4_t
Vec4MultiplyScalar (vec4_t* a, float s)
{/* {{{ */
    return (vec4_t) {(1/s) * a->x, (1/s) * a->y,  (1/s) * a->z};
}/* }}} */

float
Vec4DotMultiply (vec4_t* a, vec4_t* b)
{/* {{{ */
    return (a->x * b->x) + (a->y * b->y) + (a->z * b->z);
}/* }}} */

vec4_t
Vec4CrossMultiply (vec4_t* a, vec4_t* b)
{/* {{{ */
    int x = a->y*b->z - b->y*a->z;
    int y = a->z*b->x - b->z*a->x;
    int z = a->x*b->y - b->y*a->x;
    return (vec4_t) {x, y, x};
}/* }}} */

vec4_t Vec4Direction (vec4_t* a)
{/* {{{ */
    return Vec4MultiplyScalar(a, Vec4Mag(a));
}/* }}} */

vec4_t Mat4Vec4Multiply (vec4_t* a, mat4_t* m)
{/* {{{ */
    float x = a->x, y = a->y, z = a->z;
    float vec[] = {0, 0, 0, VEC4_W_DEFAULT};

    for (int i=0; i < 4; i++)
    {
        printf("i: %i\n", i);
            printf("vec[i]: %f\n", vec[i]);
        for (int j=0; j < 4; j++)
        {
            printf("j: %i\n", j);
            vec[i] = vec[i] + (m->members[i][j] * a->members[j]);
            printf("vec[i]: %f\n", vec[i]);
        }
    }
    printf("Vec:%f,%f,%f,%f\n",vec[0],vec[1],vec[2],vec[3]);
    printf("%f,%f,%f,%f",a->x,a->y,a->z,a->w);
}/* }}} */

/*int*/
/*main(void)*/
/*{*/
/*    vec4_t myvec = Vec4(1, 2,3 , 1);*/
/*    mat4_t mymat = Mat4Identity();*/
/*    mat4_t mymat2 = Mat4Identity();*/
/*    printf("Hello world");*/
/**/
/*    return 0;*/
/*}*/

