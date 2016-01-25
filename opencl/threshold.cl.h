static const char* threshold_cl_source =
"__kernel void gegl_threshold (__global const float2     *in,                  \n"
"                              __global const float      *aux,                 \n"
"                              __global       float2     *out,                 \n"
"                             float value)                                     \n"
"{                                                                             \n"
"  int gid = get_global_id(0);                                                 \n"
"  float2 in_v  = in [gid];                                                    \n"
"  float  aux_v = (aux)? aux[gid] : value;                                     \n"
"  float2 out_v;                                                               \n"
"  out_v.x = (in_v.x > aux_v)? 1.0f : 0.0f;                                    \n"
"  out_v.y = in_v.y;                                                           \n"
"  out[gid]  =  out_v;                                                         \n"
"}                                                                             \n"
;
