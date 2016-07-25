#ifndef _SvtIndexMap_h
#define _SvtIndexMap_h
struct svtmap_t {
  Char_t *name;
  Int_t  Id;
  Int_t  Index;
  Int_t  N;
  Int_t  elementID, Barrel, Ladder, Wafer; // select * from WafersIDs;
};
svtmap_t SvtMap[] = {
  //             Id Index elem     N  B  L  W
  {"L01B1W01", 2101,    0,   1,    0, 1, 1,1},
  {"L01B1W02", 2201,    1,   2,    1, 1, 1,2},
  {"L01B1W03", 2301,    2,   3,    2, 1, 1,3},
  {"L01B1W04", 2401,    3,   4,    3, 1, 1,4},
  {"L02B1W01", 1102,    4,   5,    4, 1, 2,1},
  {"L02B1W02", 1202,    5,   6,    5, 1, 2,2},
  {"L02B1W03", 1302,    6,   7,    6, 1, 2,3},
  {"L02B1W04", 1402,    7,   8,    7, 1, 2,4},
  {"L03B1W01", 2103,    8,   9,    8, 1, 3,1},
  {"L03B1W02", 2203,    9,  10,    9, 1, 3,2},
  {"L03B1W03", 2303,   10,  11,   10, 1, 3,3},
  {"L03B1W04", 2403,   11,  12,   11, 1, 3,4},
  {"L04B1W01", 1104,   12,  13,   12, 1, 4,1},
  {"L04B1W02", 1204,   13,  14,   13, 1, 4,2},
  {"L04B1W03", 1304,   14,  15,   14, 1, 4,3},
  {"L04B1W04", 1404,   15,  16,   15, 1, 4,4},
  {"L05B1W01", 2105,   16,  17,   16, 1, 5,1},
  {"L05B1W02", 2205,   17,  18,   17, 1, 5,2},
  {"L05B1W03", 2305,   18,  19,   18, 1, 5,3},
  {"L05B1W04", 2405,   19,  20,   19, 1, 5,4},
  {"L06B1W01", 1106,   20,  21,   20, 1, 6,1},
  {"L06B1W02", 1206,   21,  22,   21, 1, 6,2},
  {"L06B1W03", 1306,   22,  23,   22, 1, 6,3},
  {"L06B1W04", 1406,   23,  24,   23, 1, 6,4},
  {"L07B1W01", 2107,   24,  25,   24, 1, 7,1},
  {"L07B1W02", 2207,   25,  26,   25, 1, 7,2},
  {"L07B1W03", 2307,   26,  27,   26, 1, 7,3},
  {"L07B1W04", 2407,   27,  28,   27, 1, 7,4},
  {"L08B1W01", 1108,   28,  29,   28, 1, 8,1},
  {"L08B1W02", 1208,   29,  30,   29, 1, 8,2},
  {"L08B1W03", 1308,   30,  31,   30, 1, 8,3},
  {"L08B1W04", 1408,   31,  32,   31, 1, 8,4},
  {"L01B2W01", 4101,   32,  33,   32, 2, 1,1},
  {"L01B2W02", 4201,   33,  34,   33, 2, 1,2},
  {"L01B2W03", 4301,   34,  35,   34, 2, 1,3},
  {"L01B2W04", 4401,   35,  36,   35, 2, 1,4},
  {"L01B2W05", 4501,   36,  37,   36, 2, 1,5},
  {"L01B2W06", 4601,   37,  38,   37, 2, 1,6},
  {"L02B2W01", 3102,   38,  39,   38, 2, 2,1},
  {"L02B2W02", 3202,   39,  40,   39, 2, 2,2},
  {"L02B2W03", 3302,   40,  41,   40, 2, 2,3},
  {"L02B2W04", 3402,   41,  42,   41, 2, 2,4},
  {"L02B2W05", 3502,   42,  43,   42, 2, 2,5},
  {"L02B2W06", 3602,   43,  44,   43, 2, 2,6},
  {"L03B2W01", 4103,   44,  45,   44, 2, 3,1},
  {"L03B2W02", 4203,   45,  46,   45, 2, 3,2},
  {"L03B2W03", 4303,   46,  47,   46, 2, 3,3},
  {"L03B2W04", 4403,   47,  48,   47, 2, 3,4},
  {"L03B2W05", 4503,   48,  49,   48, 2, 3,5},
  {"L03B2W06", 4603,   49,  50,   49, 2, 3,6},
  {"L04B2W01", 3104,   50,  51,   50, 2, 4,1},
  {"L04B2W02", 3204,   51,  52,   51, 2, 4,2},
  {"L04B2W03", 3304,   52,  53,   52, 2, 4,3},
  {"L04B2W04", 3404,   53,  54,   53, 2, 4,4},
  {"L04B2W05", 3504,   54,  55,   54, 2, 4,5},
  {"L04B2W06", 3604,   55,  56,   55, 2, 4,6},
  {"L05B2W01", 4105,   56,  57,   56, 2, 5,1},
  {"L05B2W02", 4205,   57,  58,   57, 2, 5,2},
  {"L05B2W03", 4305,   58,  59,   58, 2, 5,3},
  {"L05B2W04", 4405,   59,  60,   59, 2, 5,4},
  {"L05B2W05", 4505,   60,  61,   60, 2, 5,5},
  {"L05B2W06", 4605,   61,  62,   61, 2, 5,6},
  {"L06B2W01", 3106,   62,  63,   62, 2, 6,1},
  {"L06B2W02", 3206,   63,  64,   63, 2, 6,2},
  {"L06B2W03", 3306,   64,  65,   64, 2, 6,3},
  {"L06B2W04", 3406,   65,  66,   65, 2, 6,4},
  {"L06B2W05", 3506,   66,  67,   66, 2, 6,5},
  {"L06B2W06", 3606,   67,  68,   67, 2, 6,6},
  {"L07B2W01", 4107,   68,  69,   68, 2, 7,1},
  {"L07B2W02", 4207,   69,  70,   69, 2, 7,2},
  {"L07B2W03", 4307,   70,  71,   70, 2, 7,3},
  {"L07B2W04", 4407,   71,  72,   71, 2, 7,4},
  {"L07B2W05", 4507,   72,  73,   72, 2, 7,5},
  {"L07B2W06", 4607,   73,  74,   73, 2, 7,6},
  {"L08B2W01", 3108,   74,  75,   74, 2, 8,1},
  {"L08B2W02", 3208,   75,  76,   75, 2, 8,2},
  {"L08B2W03", 3308,   76,  77,   76, 2, 8,3},
  {"L08B2W04", 3408,   77,  78,   77, 2, 8,4},
  {"L08B2W05", 3508,   78,  79,   78, 2, 8,5},
  {"L08B2W06", 3608,   79,  80,   79, 2, 8,6},
  {"L09B2W01", 4109,   80,  81,   80, 2, 9,1},
  {"L09B2W02", 4209,   81,  82,   81, 2, 9,2},
  {"L09B2W03", 4309,   82,  83,   82, 2, 9,3},
  {"L09B2W04", 4409,   83,  84,   83, 2, 9,4},
  {"L09B2W05", 4509,   84,  85,   84, 2, 9,5},
  {"L09B2W06", 4609,   85,  86,   85, 2, 9,6},
  {"L10B2W01", 3110,   86,  87,   86, 2,10,1},
  {"L10B2W02", 3210,   87,  88,   87, 2,10,2},
  {"L10B2W03", 3310,   88,  89,   88, 2,10,3},
  {"L10B2W04", 3410,   89,  90,   89, 2,10,4},
  {"L10B2W05", 3510,   90,  91,   90, 2,10,5},
  {"L10B2W06", 3610,   91,  92,   91, 2,10,6},
  {"L11B2W01", 4111,   92,  93,   92, 2,11,1},
  {"L11B2W02", 4211,   93,  94,   93, 2,11,2},
  {"L11B2W03", 4311,   94,  95,   94, 2,11,3},
  {"L11B2W04", 4411,   95,  96,   95, 2,11,4},
  {"L11B2W05", 4511,   96,  97,   96, 2,11,5},
  {"L11B2W06", 4611,   97,  98,   97, 2,11,6},
  {"L12B2W01", 3112,   98,  99,   98, 2,12,1},
  {"L12B2W02", 3212,   99, 100,   99, 2,12,2},
  {"L12B2W03", 3312,  100, 101,  100, 2,12,3},
  {"L12B2W04", 3412,  101, 102,  101, 2,12,4},
  {"L12B2W05", 3512,  102, 103,  102, 2,12,5},
  {"L12B2W06", 3612,  103, 104,  103, 2,12,6},
  {"L01B3W01", 6101,  104, 105,  104, 3, 1,1},
  {"L01B3W02", 6201,  105, 106,  105, 3, 1,2},
  {"L01B3W03", 6301,  106, 107,  106, 3, 1,3},
  {"L01B3W04", 6401,  107, 108,  107, 3, 1,4},
  {"L01B3W05", 6501,  108, 109,  108, 3, 1,5},
  {"L01B3W06", 6601,  109, 110,  109, 3, 1,6},
  {"L01B3W07", 6701,  110, 111,  110, 3, 1,7},
  {"L02B3W01", 5102,  111, 112,  111, 3, 2,1},
  {"L02B3W02", 5202,  112, 113,  112, 3, 2,2},
  {"L02B3W03", 5302,  113, 114,  113, 3, 2,3},
  {"L02B3W04", 5402,  114, 115,  114, 3, 2,4},
  {"L02B3W05", 5502,  115, 116,  115, 3, 2,5},
  {"L02B3W06", 5602,  116, 117,  116, 3, 2,6},
  {"L02B3W07", 5702,  117, 118,  117, 3, 2,7},
  {"L03B3W01", 6103,  118, 119,  118, 3, 3,1},
  {"L03B3W02", 6203,  119, 120,  119, 3, 3,2},
  {"L03B3W03", 6303,  120, 121,  120, 3, 3,3},
  {"L03B3W04", 6403,  121, 122,  121, 3, 3,4},
  {"L03B3W05", 6503,  122, 123,  122, 3, 3,5},
  {"L03B3W06", 6603,  123, 124,  123, 3, 3,6},
  {"L03B3W07", 6703,  124, 125,  124, 3, 3,7},
  {"L04B3W01", 5104,  125, 126,  125, 3, 4,1},
  {"L04B3W02", 5204,  126, 127,  126, 3, 4,2},
  {"L04B3W03", 5304,  127, 128,  127, 3, 4,3},
  {"L04B3W04", 5404,  128, 129,  128, 3, 4,4},
  {"L04B3W05", 5504,  129, 130,  129, 3, 4,5},
  {"L04B3W06", 5604,  130, 131,  130, 3, 4,6},
  {"L04B3W07", 5704,  131, 132,  131, 3, 4,7},
  {"L05B3W01", 6105,  132, 133,  132, 3, 5,1},
  {"L05B3W02", 6205,  133, 134,  133, 3, 5,2},
  {"L05B3W03", 6305,  134, 135,  134, 3, 5,3},
  {"L05B3W04", 6405,  135, 136,  135, 3, 5,4},
  {"L05B3W05", 6505,  136, 137,  136, 3, 5,5},
  {"L05B3W06", 6605,  137, 138,  137, 3, 5,6},
  {"L05B3W07", 6705,  138, 139,  138, 3, 5,7},
  {"L06B3W01", 5106,  139, 140,  139, 3, 6,1},
  {"L06B3W02", 5206,  140, 141,  140, 3, 6,2},
  {"L06B3W03", 5306,  141, 142,  141, 3, 6,3},
  {"L06B3W04", 5406,  142, 143,  142, 3, 6,4},
  {"L06B3W05", 5506,  143, 144,  143, 3, 6,5},
  {"L06B3W06", 5606,  144, 145,  144, 3, 6,6},
  {"L06B3W07", 5706,  145, 146,  145, 3, 6,7},
  {"L07B3W01", 6107,  146, 147,  146, 3, 7,1},
  {"L07B3W02", 6207,  147, 148,  147, 3, 7,2},
  {"L07B3W03", 6307,  148, 149,  148, 3, 7,3},
  {"L07B3W04", 6407,  149, 150,  149, 3, 7,4},
  {"L07B3W05", 6507,  150, 151,  150, 3, 7,5},
  {"L07B3W06", 6607,  151, 152,  151, 3, 7,6},
  {"L07B3W07", 6707,  152, 153,  152, 3, 7,7},
  {"L08B3W01", 5108,  153, 154,  153, 3, 8,1},
  {"L08B3W02", 5208,  154, 155,  154, 3, 8,2},
  {"L08B3W03", 5308,  155, 156,  155, 3, 8,3},
  {"L08B3W04", 5408,  156, 157,  156, 3, 8,4},
  {"L08B3W05", 5508,  157, 158,  157, 3, 8,5},
  {"L08B3W06", 5608,  158, 159,  158, 3, 8,6},
  {"L08B3W07", 5708,  159, 160,  159, 3, 8,7},
  {"L09B3W01", 6109,  160, 161,  160, 3, 9,1},
  {"L09B3W02", 6209,  161, 162,  161, 3, 9,2},
  {"L09B3W03", 6309,  162, 163,  162, 3, 9,3},
  {"L09B3W04", 6409,  163, 164,  163, 3, 9,4},
  {"L09B3W05", 6509,  164, 165,  164, 3, 9,5},
  {"L09B3W06", 6609,  165, 166,  165, 3, 9,6},
  {"L09B3W07", 6709,  166, 167,  166, 3, 9,7},
  {"L10B3W01", 5110,  167, 168,  167, 3,10,1},
  {"L10B3W02", 5210,  168, 169,  168, 3,10,2},
  {"L10B3W03", 5310,  169, 170,  169, 3,10,3},
  {"L10B3W04", 5410,  170, 171,  170, 3,10,4},
  {"L10B3W05", 5510,  171, 172,  171, 3,10,5},
  {"L10B3W06", 5610,  172, 173,  172, 3,10,6},
  {"L10B3W07", 5710,  173, 174,  173, 3,10,7},
  {"L11B3W01", 6111,  174, 175,  174, 3,11,1},
  {"L11B3W02", 6211,  175, 176,  175, 3,11,2},
  {"L11B3W03", 6311,  176, 177,  176, 3,11,3},
  {"L11B3W04", 6411,  177, 178,  177, 3,11,4},
  {"L11B3W05", 6511,  178, 179,  178, 3,11,5},
  {"L11B3W06", 6611,  179, 180,  179, 3,11,6},
  {"L11B3W07", 6711,  180, 181,  180, 3,11,7},
  {"L12B3W01", 5112,  181, 182,  181, 3,12,1},
  {"L12B3W02", 5212,  182, 183,  182, 3,12,2},
  {"L12B3W03", 5312,  183, 184,  183, 3,12,3},
  {"L12B3W04", 5412,  184, 185,  184, 3,12,4},
  {"L12B3W05", 5512,  185, 186,  185, 3,12,5},
  {"L12B3W06", 5612,  186, 187,  186, 3,12,6},
  {"L12B3W07", 5712,  187, 188,  187, 3,12,7},
  {"L13B3W01", 6113,  188, 189,  188, 3,13,1},
  {"L13B3W02", 6213,  189, 190,  189, 3,13,2},
  {"L13B3W03", 6313,  190, 191,  190, 3,13,3},
  {"L13B3W04", 6413,  191, 192,  191, 3,13,4},
  {"L13B3W05", 6513,  192, 193,  192, 3,13,5},
  {"L13B3W06", 6613,  193, 194,  193, 3,13,6},
  {"L13B3W07", 6713,  194, 195,  194, 3,13,7},
  {"L14B3W01", 5114,  195, 196,  195, 3,14,1},
  {"L14B3W02", 5214,  196, 197,  196, 3,14,2},
  {"L14B3W03", 5314,  197, 198,  197, 3,14,3},
  {"L14B3W04", 5414,  198, 199,  198, 3,14,4},
  {"L14B3W05", 5514,  199, 200,  199, 3,14,5},
  {"L14B3W06", 5614,  200, 201,  200, 3,14,6},
  {"L14B3W07", 5714,  201, 202,  201, 3,14,7},
  {"L15B3W01", 6115,  202, 203,  202, 3,15,1},
  {"L15B3W02", 6215,  203, 204,  203, 3,15,2},
  {"L15B3W03", 6315,  204, 205,  204, 3,15,3},
  {"L15B3W04", 6415,  205, 206,  205, 3,15,4},
  {"L15B3W05", 6515,  206, 207,  206, 3,15,5},
  {"L15B3W06", 6615,  207, 208,  207, 3,15,6},
  {"L15B3W07", 6715,  208, 209,  208, 3,15,7},
  {"L16B3W01", 5116,  209, 210,  209, 3,16,1},
  {"L16B3W02", 5216,  210, 211,  210, 3,16,2},
  {"L16B3W03", 5316,  211, 212,  211, 3,16,3},
  {"L16B3W04", 5416,  212, 213,  212, 3,16,4},
  {"L16B3W05", 5516,  213, 214,  213, 3,16,5},
  {"L16B3W06", 5616,  214, 215,  214, 3,16,6},
  {"L16B3W07", 5716,  215, 216,  215, 3,16,7}
};
#endif
