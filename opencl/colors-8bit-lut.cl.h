static const char* colors_8bit_lut_cl_source =
"/* This file is part of GEGL                                                  \n"
" *                                                                            \n"
" * GEGL is free software; you can redistribute it and/or                      \n"
" * modify it under the terms of the GNU Lesser General Public                 \n"
" * License as published by the Free Software Foundation; either               \n"
" * version 3 of the License, or (at your option) any later version.           \n"
" *                                                                            \n"
" * GEGL is distributed in the hope that it will be useful,                    \n"
" * but WITHOUT ANY WARRANTY; without even the implied warranty of             \n"
" * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU          \n"
" * Lesser General Public License for more details.                            \n"
" *                                                                            \n"
" * You should have received a copy of the GNU Lesser General Public           \n"
" * License along with GEGL; if not, see <http://www.gnu.org/licenses/>.       \n"
" *                                                                            \n"
" * Copyright 2012 Victor Oliveira (victormatheus@gmail.com)                   \n"
" *           2013 Daniel Sabo                                                 \n"
" */                                                                           \n"
"                                                                              \n"
"__constant float u8_gamma_to_linear_lut[] = {                                 \n"
"0.0000000000000000f,                                                          \n"
"0.0003035269835488f,                                                          \n"
"0.0006070539670977f,                                                          \n"
"0.0009105809506465f,                                                          \n"
"0.0012141079341954f,                                                          \n"
"0.0015176349177442f,                                                          \n"
"0.0018211619012930f,                                                          \n"
"0.0021246888848419f,                                                          \n"
"0.0024282158683907f,                                                          \n"
"0.0027317428519395f,                                                          \n"
"0.0030352698354884f,                                                          \n"
"0.0033465357638992f,                                                          \n"
"0.0036765073240474f,                                                          \n"
"0.0040247170184963f,                                                          \n"
"0.0043914420374103f,                                                          \n"
"0.0047769534806937f,                                                          \n"
"0.0051815167023384f,                                                          \n"
"0.0056053916242027f,                                                          \n"
"0.0060488330228571f,                                                          \n"
"0.0065120907925945f,                                                          \n"
"0.0069954101872654f,                                                          \n"
"0.0074990320432262f,                                                          \n"
"0.0080231929853850f,                                                          \n"
"0.0085681256180693f,                                                          \n"
"0.0091340587022208f,                                                          \n"
"0.0097212173202378f,                                                          \n"
"0.0103298230296269f,                                                          \n"
"0.0109600940064882f,                                                          \n"
"0.0116122451797439f,                                                          \n"
"0.0122864883569159f,                                                          \n"
"0.0129830323421730f,                                                          \n"
"0.0137020830472897f,                                                          \n"
"0.0144438435960925f,                                                          \n"
"0.0152085144229127f,                                                          \n"
"0.0159962933655096f,                                                          \n"
"0.0168073757528874f,                                                          \n"
"0.0176419544883841f,                                                          \n"
"0.0185002201283797f,                                                          \n"
"0.0193823609569357f,                                                          \n"
"0.0202885630566524f,                                                          \n"
"0.0212190103760036f,                                                          \n"
"0.0221738847933874f,                                                          \n"
"0.0231533661781104f,                                                          \n"
"0.0241576324485048f,                                                          \n"
"0.0251868596273616f,                                                          \n"
"0.0262412218948499f,                                                          \n"
"0.0273208916390749f,                                                          \n"
"0.0284260395044208f,                                                          \n"
"0.0295568344378088f,                                                          \n"
"0.0307134437329936f,                                                          \n"
"0.0318960330730115f,                                                          \n"
"0.0331047665708851f,                                                          \n"
"0.0343398068086822f,                                                          \n"
"0.0356013148750203f,                                                          \n"
"0.0368894504011000f,                                                          \n"
"0.0382043715953465f,                                                          \n"
"0.0395462352767328f,                                                          \n"
"0.0409151969068532f,                                                          \n"
"0.0423114106208097f,                                                          \n"
"0.0437350292569735f,                                                          \n"
"0.0451862043856755f,                                                          \n"
"0.0466650863368801f,                                                          \n"
"0.0481718242268894f,                                                          \n"
"0.0497065659841272f,                                                          \n"
"0.0512694583740432f,                                                          \n"
"0.0528606470231802f,                                                          \n"
"0.0544802764424424f,                                                          \n"
"0.0561284900496001f,                                                          \n"
"0.0578054301910672f,                                                          \n"
"0.0595112381629812f,                                                          \n"
"0.0612460542316176f,                                                          \n"
"0.0630100176531677f,                                                          \n"
"0.0648032666929058f,                                                          \n"
"0.0666259386437729f,                                                          \n"
"0.0684781698444002f,                                                          \n"
"0.0703600956965959f,                                                          \n"
"0.0722718506823175f,                                                          \n"
"0.0742135683801496f,                                                          \n"
"0.0761853814813079f,                                                          \n"
"0.0781874218051863f,                                                          \n"
"0.0802198203144683f,                                                          \n"
"0.0822827071298148f,                                                          \n"
"0.0843762115441488f,                                                          \n"
"0.0865004620365498f,                                                          \n"
"0.0886555862857729f,                                                          \n"
"0.0908417111834077f,                                                          \n"
"0.0930589628466875f,                                                          \n"
"0.0953074666309647f,                                                          \n"
"0.0975873471418625f,                                                          \n"
"0.0998987282471139f,                                                          \n"
"0.1022417330881013f,                                                          \n"
"0.1046164840911042f,                                                          \n"
"0.1070231029782676f,                                                          \n"
"0.1094617107782993f,                                                          \n"
"0.1119324278369056f,                                                          \n"
"0.1144353738269737f,                                                          \n"
"0.1169706677585108f,                                                          \n"
"0.1195384279883456f,                                                          \n"
"0.1221387722296019f,                                                          \n"
"0.1247718175609505f,                                                          \n"
"0.1274376804356474f,                                                          \n"
"0.1301364766903643f,                                                          \n"
"0.1328683215538180f,                                                          \n"
"0.1356333296552057f,                                                          \n"
"0.1384316150324518f,                                                          \n"
"0.1412632911402716f,                                                          \n"
"0.1441284708580578f,                                                          \n"
"0.1470272664975950f,                                                          \n"
"0.1499597898106086f,                                                          \n"
"0.1529261519961502f,                                                          \n"
"0.1559264637078274f,                                                          \n"
"0.1589608350608804f,                                                          \n"
"0.1620293756391110f,                                                          \n"
"0.1651321945016676f,                                                          \n"
"0.1682694001896907f,                                                          \n"
"0.1714411007328226f,                                                          \n"
"0.1746474036555850f,                                                          \n"
"0.1778884159836291f,                                                          \n"
"0.1811642442498602f,                                                          \n"
"0.1844749945004410f,                                                          \n"
"0.1878207723006779f,                                                          \n"
"0.1912016827407914f,                                                          \n"
"0.1946178304415758f,                                                          \n"
"0.1980693195599489f,                                                          \n"
"0.2015562537943971f,                                                          \n"
"0.2050787363903169f,                                                          \n"
"0.2086368701452558f,                                                          \n"
"0.2122307574140552f,                                                          \n"
"0.2158605001138993f,                                                          \n"
"0.2195261997292692f,                                                          \n"
"0.2232279573168085f,                                                          \n"
"0.2269658735100984f,                                                          \n"
"0.2307400485243492f,                                                          \n"
"0.2345505821610052f,                                                          \n"
"0.2383975738122710f,                                                          \n"
"0.2422811224655549f,                                                          \n"
"0.2462013267078355f,                                                          \n"
"0.2501582847299534f,                                                          \n"
"0.2541520943308267f,                                                          \n"
"0.2581828529215958f,                                                          \n"
"0.2622506575296962f,                                                          \n"
"0.2663556048028625f,                                                          \n"
"0.2704977910130658f,                                                          \n"
"0.2746773120603846f,                                                          \n"
"0.2788942634768104f,                                                          \n"
"0.2831487404299921f,                                                          \n"
"0.2874408377269175f,                                                          \n"
"0.2917706498175359f,                                                          \n"
"0.2961382707983211f,                                                          \n"
"0.3005437944157765f,                                                          \n"
"0.3049873140698863f,                                                          \n"
"0.3094689228175085f,                                                          \n"
"0.3139887133757175f,                                                          \n"
"0.3185467781250919f,                                                          \n"
"0.3231432091129507f,                                                          \n"
"0.3277780980565422f,                                                          \n"
"0.3324515363461794f,                                                          \n"
"0.3371636150483304f,                                                          \n"
"0.3419144249086609f,                                                          \n"
"0.3467040563550296f,                                                          \n"
"0.3515325995004394f,                                                          \n"
"0.3564001441459435f,                                                          \n"
"0.3613067797835095f,                                                          \n"
"0.3662525955988395f,                                                          \n"
"0.3712376804741491f,                                                          \n"
"0.3762621229909065f,                                                          \n"
"0.3813260114325301f,                                                          \n"
"0.3864294337870490f,                                                          \n"
"0.3915724777497233f,                                                          \n"
"0.3967552307256269f,                                                          \n"
"0.4019777798321958f,                                                          \n"
"0.4072402119017367f,                                                          \n"
"0.4125426134839038f,                                                          \n"
"0.4178850708481375f,                                                          \n"
"0.4232676699860717f,                                                          \n"
"0.4286904966139066f,                                                          \n"
"0.4341536361747489f,                                                          \n"
"0.4396571738409188f,                                                          \n"
"0.4452011945162279f,                                                          \n"
"0.4507857828382235f,                                                          \n"
"0.4564110231804047f,                                                          \n"
"0.4620769996544071f,                                                          \n"
"0.4677837961121590f,                                                          \n"
"0.4735314961480095f,                                                          \n"
"0.4793201831008268f,                                                          \n"
"0.4851499400560704f,                                                          \n"
"0.4910208498478356f,                                                          \n"
"0.4969329950608704f,                                                          \n"
"0.5028864580325687f,                                                          \n"
"0.5088813208549338f,                                                          \n"
"0.5149176653765214f,                                                          \n"
"0.5209955732043543f,                                                          \n"
"0.5271151257058131f,                                                          \n"
"0.5332764040105052f,                                                          \n"
"0.5394794890121072f,                                                          \n"
"0.5457244613701866f,                                                          \n"
"0.5520114015120001f,                                                          \n"
"0.5583403896342679f,                                                          \n"
"0.5647115057049292f,                                                          \n"
"0.5711248294648731f,                                                          \n"
"0.5775804404296506f,                                                          \n"
"0.5840784178911641f,                                                          \n"
"0.5906188409193369f,                                                          \n"
"0.5972017883637634f,                                                          \n"
"0.6038273388553378f,                                                          \n"
"0.6104955708078648f,                                                          \n"
"0.6172065624196511f,                                                          \n"
"0.6239603916750761f,                                                          \n"
"0.6307571363461468f,                                                          \n"
"0.6375968739940326f,                                                          \n"
"0.6444796819705821f,                                                          \n"
"0.6514056374198242f,                                                          \n"
"0.6583748172794485f,                                                          \n"
"0.6653872982822721f,                                                          \n"
"0.6724431569576875f,                                                          \n"
"0.6795424696330938f,                                                          \n"
"0.6866853124353135f,                                                          \n"
"0.6938717612919899f,                                                          \n"
"0.7011018919329731f,                                                          \n"
"0.7083757798916868f,                                                          \n"
"0.7156935005064807f,                                                          \n"
"0.7230551289219693f,                                                          \n"
"0.7304607400903537f,                                                          \n"
"0.7379104087727308f,                                                          \n"
"0.7454042095403874f,                                                          \n"
"0.7529422167760779f,                                                          \n"
"0.7605245046752924f,                                                          \n"
"0.7681511472475070f,                                                          \n"
"0.7758222183174236f,                                                          \n"
"0.7835377915261935f,                                                          \n"
"0.7912979403326302f,                                                          \n"
"0.7991027380144090f,                                                          \n"
"0.8069522576692516f,                                                          \n"
"0.8148465722161012f,                                                          \n"
"0.8227857543962835f,                                                          \n"
"0.8307698767746546f,                                                          \n"
"0.8387990117407400f,                                                          \n"
"0.8468732315098580f,                                                          \n"
"0.8549926081242338f,                                                          \n"
"0.8631572134541023f,                                                          \n"
"0.8713671191987972f,                                                          \n"
"0.8796223968878317f,                                                          \n"
"0.8879231178819663f,                                                          \n"
"0.8962693533742664f,                                                          \n"
"0.9046611743911496f,                                                          \n"
"0.9130986517934192f,                                                          \n"
"0.9215818562772946f,                                                          \n"
"0.9301108583754237f,                                                          \n"
"0.9386857284578880f,                                                          \n"
"0.9473065367331999f,                                                          \n"
"0.9559733532492861f,                                                          \n"
"0.9646862478944651f,                                                          \n"
"0.9734452903984125f,                                                          \n"
"0.9822505503331171f,                                                          \n"
"0.9911020971138298f,                                                          \n"
"1.0000000000000000f                                                           \n"
"};                                                                            \n"
"                                                                              \n"
"/* babl reference file: babl/base/rgb-constants.h */                          \n"
"#define RGB_LUMINANCE_RED    (0.222491f)                                      \n"
"#define RGB_LUMINANCE_GREEN  (0.716888f)                                      \n"
"#define RGB_LUMINANCE_BLUE   (0.060621f)                                      \n"
"                                                                              \n"
"/* R'G'B' u8 -> RGBA float */                                                 \n"
"__kernel void rgb_gamma_u8_to_rgbaf (__global const uchar  * in,              \n"
"                                     __global       float4 * out)             \n"
"{                                                                             \n"
"  int gid = get_global_id(0);                                                 \n"
"                                                                              \n"
"  float r = u8_gamma_to_linear_lut[(int)in[3 * gid + 0]];                     \n"
"  float g = u8_gamma_to_linear_lut[(int)in[3 * gid + 1]];                     \n"
"  float b = u8_gamma_to_linear_lut[(int)in[3 * gid + 2]];                     \n"
"                                                                              \n"
"  out[gid] = (float4) (r, g, b, 1.0f);                                        \n"
"}                                                                             \n"
"                                                                              \n"
"/* R'G'B' u8 -> RaGaBaA float */                                              \n"
"__kernel void rgb_gamma_u8_to_ragabaf (__global const uchar  * in,            \n"
"                                       __global       float4 * out)           \n"
"{                                                                             \n"
"  int gid = get_global_id(0);                                                 \n"
"                                                                              \n"
"  float r = u8_gamma_to_linear_lut[(int)in[3 * gid + 0]];                     \n"
"  float g = u8_gamma_to_linear_lut[(int)in[3 * gid + 1]];                     \n"
"  float b = u8_gamma_to_linear_lut[(int)in[3 * gid + 2]];                     \n"
"                                                                              \n"
"  out[gid] = (float4) (r, g, b, 1.0f);                                        \n"
"}                                                                             \n"
"                                                                              \n"
"/* R'G'B' u8 -> YA float */                                                   \n"
"__kernel void rgb_gamma_u8_to_yaf (__global const uchar  * in,                \n"
"                                   __global       float2 * out)               \n"
"{                                                                             \n"
"  int gid = get_global_id(0);                                                 \n"
"                                                                              \n"
"  float r = u8_gamma_to_linear_lut[(int)in[3 * gid + 0]];                     \n"
"  float g = u8_gamma_to_linear_lut[(int)in[3 * gid + 1]];                     \n"
"  float b = u8_gamma_to_linear_lut[(int)in[3 * gid + 2]];                     \n"
"                                                                              \n"
"  float luminance = r * RGB_LUMINANCE_RED +                                   \n"
"                    g * RGB_LUMINANCE_GREEN +                                 \n"
"                    b * RGB_LUMINANCE_BLUE;                                   \n"
"                                                                              \n"
"  out[gid] = (float2) (luminance, 1.0f);                                      \n"
"}                                                                             \n"
"                                                                              \n"
"/* R'G'B'A u8 -> RGBA float */                                                \n"
"__kernel void rgba_gamma_u8_to_rgbaf (__global const uchar4 * in,             \n"
"                                      __global       float4 * out)            \n"
"{                                                                             \n"
"  int gid = get_global_id(0);                                                 \n"
"  uchar4 in_v = in[gid];                                                      \n"
"                                                                              \n"
"  float r = u8_gamma_to_linear_lut[(int)in_v.x];                              \n"
"  float g = u8_gamma_to_linear_lut[(int)in_v.y];                              \n"
"  float b = u8_gamma_to_linear_lut[(int)in_v.z];                              \n"
"                                                                              \n"
"  out[gid] = (float4) (r, g, b, in_v.w / 255.0f);                             \n"
"}                                                                             \n"
"                                                                              \n"
"/* R'G'B'A u8 -> RaGaBaA float */                                             \n"
"__kernel void rgba_gamma_u8_to_ragabaf (__global const uchar4 * in,           \n"
"                                        __global       float4 * out)          \n"
"{                                                                             \n"
"  int gid = get_global_id(0);                                                 \n"
"  uchar4 in_v  = in[gid];                                                     \n"
"  float4 out_v;                                                               \n"
"  float4 tmp_v;                                                               \n"
"                                                                              \n"
"  tmp_v = (float4)(u8_gamma_to_linear_lut[(int)in_v.x],                       \n"
"                   u8_gamma_to_linear_lut[(int)in_v.y],                       \n"
"                   u8_gamma_to_linear_lut[(int)in_v.z],                       \n"
"                   in_v.w / 255.0f);                                          \n"
"                                                                              \n"
"  out_v   = tmp_v * tmp_v.w;                                                  \n"
"  out_v.w = tmp_v.w;                                                          \n"
"                                                                              \n"
"  out[gid] = out_v;                                                           \n"
"}                                                                             \n"
"                                                                              \n"
"/* R'G'B'A u8 -> YA float */                                                  \n"
"__kernel void rgba_gamma_u8_to_yaf (__global const uchar4 * in,               \n"
"                                    __global       float2 * out)              \n"
"{                                                                             \n"
"  int gid = get_global_id(0);                                                 \n"
"  uchar4 in_v  = in[gid];                                                     \n"
"  float4 tmp_v;                                                               \n"
"  tmp_v = (float4)(u8_gamma_to_linear_lut[(int)in_v.x],                       \n"
"                   u8_gamma_to_linear_lut[(int)in_v.y],                       \n"
"                   u8_gamma_to_linear_lut[(int)in_v.z],                       \n"
"                   in_v.w / 255.0f);                                          \n"
"  float2 out_v;                                                               \n"
"                                                                              \n"
"  float luminance = tmp_v.x * RGB_LUMINANCE_RED +                             \n"
"                    tmp_v.y * RGB_LUMINANCE_GREEN +                           \n"
"                    tmp_v.z * RGB_LUMINANCE_BLUE;                             \n"
"                                                                              \n"
"  out_v.x = luminance;                                                        \n"
"  out_v.y = tmp_v.w;                                                          \n"
"                                                                              \n"
"  out[gid] = out_v;                                                           \n"
"}                                                                             \n"
;
