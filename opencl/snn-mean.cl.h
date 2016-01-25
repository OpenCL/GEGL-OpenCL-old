static const char* snn_mean_cl_source =
"float colordiff (float4 pixA,                                                 \n"
"                 float4 pixB)                                                 \n"
"{                                                                             \n"
"    float4 pix = pixA-pixB;                                                   \n"
"    pix *= pix;                                                               \n"
"    return pix.x+pix.y+pix.z;                                                 \n"
"}                                                                             \n"
"                                                                              \n"
"__kernel void snn_mean (__global const   float4 *src_buf,                     \n"
"                                         int src_width,                       \n"
"                                         int src_height,                      \n"
"                        __global         float4 *dst_buf,                     \n"
"                                         int radius,                          \n"
"                                         int pairs)                           \n"
"{                                                                             \n"
"    int gidx   =get_global_id(0);                                             \n"
"    int gidy   =get_global_id(1);                                             \n"
"    int offset =gidy * get_global_size(0) + gidx;                             \n"
"                                                                              \n"
"    __global const float4 *center_pix=                                        \n"
"        src_buf + ((radius+gidx) + (gidy+radius)* src_width);                 \n"
"    float4 accumulated=0;                                                     \n"
"                                                                              \n"
"    int count=0;                                                              \n"
"    if(pairs==2)                                                              \n"
"    {                                                                         \n"
"        for(int i=-radius;i<0;i++)                                            \n"
"        {                                                                     \n"
"            for(int j=-radius;j<0;j++)                                        \n"
"            {                                                                 \n"
"                __global const float4 *selected_pix = center_pix;             \n"
"                float  best_diff = 1000.0f;                                   \n"
"                                                                              \n"
"                    int xs[4]={                                               \n"
"                        gidx+j+radius, gidx-j+radius,                         \n"
"                        gidx-j+radius, gidx+j+radius                          \n"
"                    };                                                        \n"
"                    int ys[4]={                                               \n"
"                        gidy+i+radius, gidy-i+radius,                         \n"
"                        gidy+i+radius, gidy-i+radius};                        \n"
"                                                                              \n"
"                    for (int k=0;k<4;k++)                                     \n"
"                    {                                                         \n"
"                        if (xs[k] >= 0 && xs[k] < src_width &&                \n"
"                            ys[k] >= 0 && ys[k] < src_height)                 \n"
"                        {                                                     \n"
"                            __global const float4 *tpix =                     \n"
"                                src_buf + (xs[k] + ys[k] * src_width);        \n"
"                            float diff=colordiff(*tpix, *center_pix);         \n"
"                            if (diff < best_diff)                             \n"
"                            {                                                 \n"
"                                best_diff = diff;                             \n"
"                                selected_pix = tpix;                          \n"
"                            }                                                 \n"
"                        }                                                     \n"
"                    }                                                         \n"
"                                                                              \n"
"                accumulated += *selected_pix;                                 \n"
"                                                                              \n"
"                ++count;                                                      \n"
"                if (i==0 && j==0)                                             \n"
"                    break;                                                    \n"
"            }                                                                 \n"
"        }                                                                     \n"
"        dst_buf[offset] = accumulated/(float4)(count);                        \n"
"        return;                                                               \n"
"    }                                                                         \n"
"    else if(pairs==1)                                                         \n"
"    {                                                                         \n"
"        for(int i=-radius;i<=0;i++)                                           \n"
"        {                                                                     \n"
"            for(int j=-radius;j<=radius;j++)                                  \n"
"            {                                                                 \n"
"                __global const float4 *selected_pix = center_pix;             \n"
"                float  best_diff = 1000.0f;                                   \n"
"                                                                              \n"
"                /* skip computations for the center pixel */                  \n"
"                if (i != 0 && j != 0)                                         \n"
"                {                                                             \n"
"                    int xs[4]={                                               \n"
"                        gidx+i+radius, gidx-i+radius,                         \n"
"                        gidx-i+radius, gidx+i+radius                          \n"
"                    };                                                        \n"
"                    int ys[4]={                                               \n"
"                        gidy+j+radius, gidy-j+radius,                         \n"
"                        gidy+j+radius, gidy-j+radius                          \n"
"                    };                                                        \n"
"                                                                              \n"
"                    for (i=0;i<2;i++)                                         \n"
"                    {                                                         \n"
"                        if (xs[i] >= 0 && xs[i] < src_width &&                \n"
"                            ys[i] >= 0 && ys[i] < src_height)                 \n"
"                        {                                                     \n"
"                            __global const float4 *tpix =                     \n"
"                                src_buf + (xs[i] + ys[i] * src_width);        \n"
"                            float diff=colordiff (*tpix, *center_pix);        \n"
"                            if (diff < best_diff)                             \n"
"                            {                                                 \n"
"                                best_diff = diff;                             \n"
"                                selected_pix = tpix;                          \n"
"                            }                                                 \n"
"                        }                                                     \n"
"                    }                                                         \n"
"                }                                                             \n"
"                accumulated += *selected_pix;                                 \n"
"                ++count;                                                      \n"
"                if (i==0 && j==0)                                             \n"
"                    break;                                                    \n"
"            }                                                                 \n"
"        }                                                                     \n"
"        dst_buf[offset] = accumulated / (float4)(count);                      \n"
"        return;                                                               \n"
"    }                                                                         \n"
"    return;                                                                   \n"
"}                                                                             \n"
;
