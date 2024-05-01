#include "Sound.h"
#include "./inc/Timer0A.h"
#include "./inc/Timer1A.h"
#include "./inc/Timer2A.h"
#include "../inc/Timer5A.h"
#include "DAC.h"

#define sin_table sin_table_10bit_256

// options, can comment out unsused ones to save rom
// static const uint16_t sin_table_12bit_2048[2048] = {2048, 2054, 2060, 2066, 2073, 2079, 2085, 2091, 2098, 2104, 2110, 2117, 2123, 2129, 2135, 2142, 2148, 2154, 2161, 2167, 2173, 2179, 2186, 2192, 2198, 2204, 2211, 2217, 2223, 2229, 2236, 2242, 2248, 2254, 2261, 2267, 2273, 2279, 2286, 2292, 2298, 2304, 2311, 2317, 2323, 2329, 2336, 2342, 2348, 2354, 2360, 2367, 2373, 2379, 2385, 2391, 2398, 2404, 2410, 2416, 2422, 2429, 2435, 2441, 2447, 2453, 2459, 2466, 2472, 2478, 2484, 2490, 2496, 2502, 2508, 2515, 2521, 2527, 2533, 2539, 2545, 2551, 2557, 2563, 2569, 2576, 2582, 2588, 2594, 2600, 2606, 2612, 2618, 2624, 2630, 2636, 2642, 2648, 2654, 2660, 2666, 2672, 2678, 2684, 2690, 2696, 2702, 2708, 2714, 2720, 2726, 2732, 2737, 2743, 2749, 2755, 2761, 2767, 2773, 2779, 2785, 2790, 2796, 2802, 2808, 2814, 2820, 2825, 2831, 2837, 2843, 2849, 2854, 2860, 2866, 2872, 2877, 2883, 2889, 2895, 2900, 2906, 2912, 2917, 2923, 2929, 2934, 2940, 2946, 2951, 2957, 2963, 2968, 2974, 2980, 2985, 2991, 2996, 3002, 3007, 3013, 3018, 3024, 3030, 3035, 3041, 3046, 3051, 3057, 3062, 3068, 3073, 3079, 3084, 3090, 3095, 3100, 3106, 3111, 3117, 3122, 3127, 3133, 3138, 3143, 3148, 3154, 3159, 3164, 3170, 3175, 3180, 3185, 3191, 3196, 3201, 3206, 3211, 3216, 3222, 3227, 3232, 3237, 3242, 3247, 3252, 3257, 3262, 3267, 3273, 3278, 3283, 3288, 3293, 3298, 3303, 3307, 3312, 3317, 3322, 3327, 3332, 3337, 3342, 3347, 3352, 3356, 3361, 3366, 3371, 3376, 3380, 3385, 3390, 3395, 3399, 3404, 3409, 3414, 3418, 3423, 3428, 3432, 3437, 3441, 3446, 3451, 3455, 3460, 3464, 3469, 3473, 3478, 3482, 3487, 3491, 3496, 3500, 3505, 3509, 3513, 3518, 3522, 3526, 3531, 3535, 3539, 3544, 3548, 3552, 3557, 3561, 3565, 3569, 3573, 3578, 3582, 3586, 3590, 3594, 3598, 3602, 3606, 3611, 3615, 3619, 3623, 3627, 3631, 3635, 3639, 3643, 3646, 3650, 3654, 3658, 3662, 3666, 3670, 3674, 3677, 3681, 3685, 3689, 3692, 3696, 3700, 3704, 3707, 3711, 3715, 3718, 3722, 3726, 3729, 3733, 3736, 3740, 3743, 3747, 3750, 3754, 3757, 3761, 3764, 3768, 3771, 3774, 3778, 3781, 3784, 3788, 3791, 3794, 3798, 3801, 3804, 3807, 3811, 3814, 3817, 3820, 3823, 3826, 3829, 3833, 3836, 3839, 3842, 3845, 3848, 3851, 3854, 3857, 3860, 3862, 3865, 3868, 3871, 3874, 3877, 3880, 3882, 3885, 3888, 3891, 3893, 3896, 3899, 3902, 3904, 3907, 3909, 3912, 3915, 3917, 3920, 3922, 3925, 3927, 3930, 3932, 3935, 3937, 3940, 3942, 3944, 3947, 3949, 3951, 3954, 3956, 3958, 3961, 3963, 3965, 3967, 3969, 3972, 3974, 3976, 3978, 3980, 3982, 3984, 3986, 3988, 3990, 3992, 3994, 3996, 3998, 4000, 4002, 4004, 4005, 4007, 4009, 4011, 4013, 4014, 4016, 4018, 4020, 4021, 4023, 4025, 4026, 4028, 4029, 4031, 4033, 4034, 4036, 4037, 4039, 4040, 4042, 4043, 4044, 4046, 4047, 4048, 4050, 4051, 4052, 4054, 4055, 4056, 4057, 4059, 4060, 4061, 4062, 4063, 4064, 4065, 4066, 4067, 4068, 4069, 4070, 4071, 4072, 4073, 4074, 4075, 4076, 4077, 4078, 4079, 4079, 4080, 4081, 4082, 4082, 4083, 4084, 4084, 4085, 4086, 4086, 4087, 4087, 4088, 4088, 4089, 4089, 4090, 4090, 4091, 4091, 4092, 4092, 4092, 4093, 4093, 4093, 4094, 4094, 4094, 4094, 4095, 4095, 4095, 4095, 4095, 4095, 4095, 4095, 4095, 4095, 4096, 4095, 4095, 4095, 4095, 4095, 4095, 4095, 4095, 4095, 4095, 4094, 4094, 4094, 4094, 4093, 4093, 4093, 4092, 4092, 4092, 4091, 4091, 4090, 4090, 4089, 4089, 4088, 4088, 4087, 4087, 4086, 4086, 4085, 4084, 4084, 4083, 4082, 4082, 4081, 4080, 4079, 4079, 4078, 4077, 4076, 4075, 4074, 4073, 4072, 4071, 4070, 4069, 4068, 4067, 4066, 4065, 4064, 4063, 4062, 4061, 4060, 4059, 4057, 4056, 4055, 4054, 4052, 4051, 4050, 4048, 4047, 4046, 4044, 4043, 4042, 4040, 4039, 4037, 4036, 4034, 4033, 4031, 4029, 4028, 4026, 4025, 4023, 4021, 4020, 4018, 4016, 4014, 4013, 4011, 4009, 4007, 4005, 4004, 4002, 4000, 3998, 3996, 3994, 3992, 3990, 3988, 3986, 3984, 3982, 3980, 3978, 3976, 3974, 3972, 3969, 3967, 3965, 3963, 3961, 3958, 3956, 3954, 3951, 3949, 3947, 3944, 3942, 3940, 3937, 3935, 3932, 3930, 3927, 3925, 3922, 3920, 3917, 3915, 3912, 3909, 3907, 3904, 3902, 3899, 3896, 3893, 3891, 3888, 3885, 3882, 3880, 3877, 3874, 3871, 3868, 3865, 3862, 3860, 3857, 3854, 3851, 3848, 3845, 3842, 3839, 3836, 3833, 3829, 3826, 3823, 3820, 3817, 3814, 3811, 3807, 3804, 3801, 3798, 3794, 3791, 3788, 3784, 3781, 3778, 3774, 3771, 3768, 3764, 3761, 3757, 3754, 3750, 3747, 3743, 3740, 3736, 3733, 3729, 3726, 3722, 3718, 3715, 3711, 3707, 3704, 3700, 3696, 3692, 3689, 3685, 3681, 3677, 3674, 3670, 3666, 3662, 3658, 3654, 3650, 3646, 3643, 3639, 3635, 3631, 3627, 3623, 3619, 3615, 3611, 3606, 3602, 3598, 3594, 3590, 3586, 3582, 3578, 3573, 3569, 3565, 3561, 3557, 3552, 3548, 3544, 3539, 3535, 3531, 3526, 3522, 3518, 3513, 3509, 3505, 3500, 3496, 3491, 3487, 3482, 3478, 3473, 3469, 3464, 3460, 3455, 3451, 3446, 3441, 3437, 3432, 3428, 3423, 3418, 3414, 3409, 3404, 3399, 3395, 3390, 3385, 3380, 3376, 3371, 3366, 3361, 3356, 3352, 3347, 3342, 3337, 3332, 3327, 3322, 3317, 3312, 3307, 3303, 3298, 3293, 3288, 3283, 3278, 3273, 3267, 3262, 3257, 3252, 3247, 3242, 3237, 3232, 3227, 3222, 3216, 3211, 3206, 3201, 3196, 3191, 3185, 3180, 3175, 3170, 3164, 3159, 3154, 3148, 3143, 3138, 3133, 3127, 3122, 3117, 3111, 3106, 3100, 3095, 3090, 3084, 3079, 3073, 3068, 3062, 3057, 3051, 3046, 3041, 3035, 3030, 3024, 3018, 3013, 3007, 3002, 2996, 2991, 2985, 2980, 2974, 2968, 2963, 2957, 2951, 2946, 2940, 2934, 2929, 2923, 2917, 2912, 2906, 2900, 2895, 2889, 2883, 2877, 2872, 2866, 2860, 2854, 2849, 2843, 2837, 2831, 2825, 2820, 2814, 2808, 2802, 2796, 2790, 2785, 2779, 2773, 2767, 2761, 2755, 2749, 2743, 2737, 2732, 2726, 2720, 2714, 2708, 2702, 2696, 2690, 2684, 2678, 2672, 2666, 2660, 2654, 2648, 2642, 2636, 2630, 2624, 2618, 2612, 2606, 2600, 2594, 2588, 2582, 2576, 2569, 2563, 2557, 2551, 2545, 2539, 2533, 2527, 2521, 2515, 2508, 2502, 2496, 2490, 2484, 2478, 2472, 2466, 2459, 2453, 2447, 2441, 2435, 2429, 2422, 2416, 2410, 2404, 2398, 2391, 2385, 2379, 2373, 2367, 2360, 2354, 2348, 2342, 2336, 2329, 2323, 2317, 2311, 2304, 2298, 2292, 2286, 2279, 2273, 2267, 2261, 2254, 2248, 2242, 2236, 2229, 2223, 2217, 2211, 2204, 2198, 2192, 2186, 2179, 2173, 2167, 2161, 2154, 2148, 2142, 2135, 2129, 2123, 2117, 2110, 2104, 2098, 2091, 2085, 2079, 2073, 2066, 2060, 2054, 2048, 2041, 2035, 2029, 2022, 2016, 2010, 2004, 1997, 1991, 1985, 1978, 1972, 1966, 1960, 1953, 1947, 1941, 1934, 1928, 1922, 1916, 1909, 1903, 1897, 1891, 1884, 1878, 1872, 1866, 1859, 1853, 1847, 1841, 1834, 1828, 1822, 1816, 1809, 1803, 1797, 1791, 1784, 1778, 1772, 1766, 1759, 1753, 1747, 1741, 1735, 1728, 1722, 1716, 1710, 1704, 1697, 1691, 1685, 1679, 1673, 1666, 1660, 1654, 1648, 1642, 1636, 1629, 1623, 1617, 1611, 1605, 1599, 1593, 1587, 1580, 1574, 1568, 1562, 1556, 1550, 1544, 1538, 1532, 1526, 1519, 1513, 1507, 1501, 1495, 1489, 1483, 1477, 1471, 1465, 1459, 1453, 1447, 1441, 1435, 1429, 1423, 1417, 1411, 1405, 1399, 1393, 1387, 1381, 1375, 1369, 1363, 1358, 1352, 1346, 1340, 1334, 1328, 1322, 1316, 1310, 1305, 1299, 1293, 1287, 1281, 1275, 1270, 1264, 1258, 1252, 1246, 1241, 1235, 1229, 1223, 1218, 1212, 1206, 1200, 1195, 1189, 1183, 1178, 1172, 1166, 1161, 1155, 1149, 1144, 1138, 1132, 1127, 1121, 1115, 1110, 1104, 1099, 1093, 1088, 1082, 1077, 1071, 1065, 1060, 1054, 1049, 1044, 1038, 1033, 1027, 1022, 1016, 1011, 1005, 1000, 995, 989, 984, 978, 973, 968, 962, 957, 952, 947, 941, 936, 931, 925, 920, 915, 910, 904, 899, 894, 889, 884, 879, 873, 868, 863, 858, 853, 848, 843, 838, 833, 828, 822, 817, 812, 807, 802, 797, 792, 788, 783, 778, 773, 768, 763, 758, 753, 748, 743, 739, 734, 729, 724, 719, 715, 710, 705, 700, 696, 691, 686, 681, 677, 672, 667, 663, 658, 654, 649, 644, 640, 635, 631, 626, 622, 617, 613, 608, 604, 599, 595, 590, 586, 582, 577, 573, 569, 564, 560, 556, 551, 547, 543, 538, 534, 530, 526, 522, 517, 513, 509, 505, 501, 497, 493, 489, 484, 480, 476, 472, 468, 464, 460, 456, 452, 449, 445, 441, 437, 433, 429, 425, 421, 418, 414, 410, 406, 403, 399, 395, 391, 388, 384, 380, 377, 373, 369, 366, 362, 359, 355, 352, 348, 345, 341, 338, 334, 331, 327, 324, 321, 317, 314, 311, 307, 304, 301, 297, 294, 291, 288, 284, 281, 278, 275, 272, 269, 266, 262, 259, 256, 253, 250, 247, 244, 241, 238, 235, 233, 230, 227, 224, 221, 218, 215, 213, 210, 207, 204, 202, 199, 196, 193, 191, 188, 186, 183, 180, 178, 175, 173, 170, 168, 165, 163, 160, 158, 155, 153, 151, 148, 146, 144, 141, 139, 137, 134, 132, 130, 128, 126, 123, 121, 119, 117, 115, 113, 111, 109, 107, 105, 103, 101, 99, 97, 95, 93, 91, 90, 88, 86, 84, 82, 81, 79, 77, 75, 74, 72, 70, 69, 67, 66, 64, 62, 61, 59, 58, 56, 55, 53, 52, 51, 49, 48, 47, 45, 44, 43, 41, 40, 39, 38, 36, 35, 34, 33, 32, 31, 30, 29, 28, 27, 26, 25, 24, 23, 22, 21, 20, 19, 18, 17, 16, 16, 15, 14, 13, 13, 12, 11, 11, 10, 9, 9, 8, 8, 7, 7, 6, 6, 5, 5, 4, 4, 3, 3, 3, 2, 2, 2, 1, 1, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 1, 1, 2, 2, 2, 3, 3, 3, 4, 4, 5, 5, 6, 6, 7, 7, 8, 8, 9, 9, 10, 11, 11, 12, 13, 13, 14, 15, 16, 16, 17, 18, 19, 20, 21, 22, 23, 24, 25, 26, 27, 28, 29, 30, 31, 32, 33, 34, 35, 36, 38, 39, 40, 41, 43, 44, 45, 47, 48, 49, 51, 52, 53, 55, 56, 58, 59, 61, 62, 64, 66, 67, 69, 70, 72, 74, 75, 77, 79, 81, 82, 84, 86, 88, 90, 91, 93, 95, 97, 99, 101, 103, 105, 107, 109, 111, 113, 115, 117, 119, 121, 123, 126, 128, 130, 132, 134, 137, 139, 141, 144, 146, 148, 151, 153, 155, 158, 160, 163, 165, 168, 170, 173, 175, 178, 180, 183, 186, 188, 191, 193, 196, 199, 202, 204, 207, 210, 213, 215, 218, 221, 224, 227, 230, 233, 235, 238, 241, 244, 247, 250, 253, 256, 259, 262, 266, 269, 272, 275, 278, 281, 284, 288, 291, 294, 297, 301, 304, 307, 311, 314, 317, 321, 324, 327, 331, 334, 338, 341, 345, 348, 352, 355, 359, 362, 366, 369, 373, 377, 380, 384, 388, 391, 395, 399, 403, 406, 410, 414, 418, 421, 425, 429, 433, 437, 441, 445, 449, 452, 456, 460, 464, 468, 472, 476, 480, 484, 489, 493, 497, 501, 505, 509, 513, 517, 522, 526, 530, 534, 538, 543, 547, 551, 556, 560, 564, 569, 573, 577, 582, 586, 590, 595, 599, 604, 608, 613, 617, 622, 626, 631, 635, 640, 644, 649, 654, 658, 663, 667, 672, 677, 681, 686, 691, 696, 700, 705, 710, 715, 719, 724, 729, 734, 739, 743, 748, 753, 758, 763, 768, 773, 778, 783, 788, 792, 797, 802, 807, 812, 817, 822, 828, 833, 838, 843, 848, 853, 858, 863, 868, 873, 879, 884, 889, 894, 899, 904, 910, 915, 920, 925, 931, 936, 941, 947, 952, 957, 962, 968, 973, 978, 984, 989, 995, 1000, 1005, 1011, 1016, 1022, 1027, 1033, 1038, 1044, 1049, 1054, 1060, 1065, 1071, 1077, 1082, 1088, 1093, 1099, 1104, 1110, 1115, 1121, 1127, 1132, 1138, 1144, 1149, 1155, 1161, 1166, 1172, 1178, 1183, 1189, 1195, 1200, 1206, 1212, 1218, 1223, 1229, 1235, 1241, 1246, 1252, 1258, 1264, 1270, 1275, 1281, 1287, 1293, 1299, 1305, 1310, 1316, 1322, 1328, 1334, 1340, 1346, 1352, 1358, 1363, 1369, 1375, 1381, 1387, 1393, 1399, 1405, 1411, 1417, 1423, 1429, 1435, 1441, 1447, 1453, 1459, 1465, 1471, 1477, 1483, 1489, 1495, 1501, 1507, 1513, 1519, 1526, 1532, 1538, 1544, 1550, 1556, 1562, 1568, 1574, 1580, 1587, 1593, 1599, 1605, 1611, 1617, 1623, 1629, 1636, 1642, 1648, 1654, 1660, 1666, 1673, 1679, 1685, 1691, 1697, 1704, 1710, 1716, 1722, 1728, 1735, 1741, 1747, 1753, 1759, 1766, 1772, 1778, 1784, 1791, 1797, 1803, 1809, 1816, 1822, 1828, 1834, 1841, 1847, 1853, 1859, 1866, 1872, 1878, 1884, 1891, 1897, 1903, 1909, 1916, 1922, 1928, 1934, 1941, 1947, 1953, 1960, 1966, 1972, 1978, 1985, 1991, 1997, 2004, 2010, 2016, 2022, 2029, 2035, 2041 };
// static const uint16_t sin_table_11bit_1024[1024] = {1024, 1030, 1036, 1042, 1049, 1055, 1061, 1067, 1074, 1080, 1086, 1093, 1099, 1105, 1111, 1118, 1124, 1130, 1136, 1143, 1149, 1155, 1161, 1168, 1174, 1180, 1186, 1192, 1199, 1205, 1211, 1217, 1223, 1229, 1236, 1242, 1248, 1254, 1260, 1266, 1272, 1278, 1284, 1291, 1297, 1303, 1309, 1315, 1321, 1327, 1333, 1339, 1345, 1351, 1357, 1363, 1368, 1374, 1380, 1386, 1392, 1398, 1404, 1410, 1415, 1421, 1427, 1433, 1438, 1444, 1450, 1456, 1461, 1467, 1473, 1478, 1484, 1490, 1495, 1501, 1506, 1512, 1517, 1523, 1528, 1534, 1539, 1545, 1550, 1555, 1561, 1566, 1571, 1577, 1582, 1587, 1592, 1598, 1603, 1608, 1613, 1618, 1623, 1628, 1633, 1639, 1644, 1649, 1653, 1658, 1663, 1668, 1673, 1678, 1683, 1688, 1692, 1697, 1702, 1707, 1711, 1716, 1720, 1725, 1730, 1734, 1739, 1743, 1748, 1752, 1756, 1761, 1765, 1769, 1774, 1778, 1782, 1786, 1791, 1795, 1799, 1803, 1807, 1811, 1815, 1819, 1823, 1827, 1831, 1835, 1838, 1842, 1846, 1850, 1853, 1857, 1861, 1864, 1868, 1871, 1875, 1878, 1882, 1885, 1889, 1892, 1895, 1899, 1902, 1905, 1908, 1911, 1914, 1918, 1921, 1924, 1927, 1930, 1932, 1935, 1938, 1941, 1944, 1946, 1949, 1952, 1954, 1957, 1960, 1962, 1965, 1967, 1970, 1972, 1974, 1977, 1979, 1981, 1983, 1986, 1988, 1990, 1992, 1994, 1996, 1998, 2000, 2002, 2003, 2005, 2007, 2009, 2010, 2012, 2014, 2015, 2017, 2018, 2020, 2021, 2023, 2024, 2025, 2027, 2028, 2029, 2030, 2031, 2032, 2033, 2034, 2035, 2036, 2037, 2038, 2039, 2040, 2041, 2041, 2042, 2043, 2043, 2044, 2044, 2045, 2045, 2046, 2046, 2046, 2047, 2047, 2047, 2047, 2047, 2047, 2047, 2048, 2047, 2047, 2047, 2047, 2047, 2047, 2047, 2046, 2046, 2046, 2045, 2045, 2044, 2044, 2043, 2043, 2042, 2041, 2041, 2040, 2039, 2038, 2037, 2036, 2035, 2034, 2033, 2032, 2031, 2030, 2029, 2028, 2027, 2025, 2024, 2023, 2021, 2020, 2018, 2017, 2015, 2014, 2012, 2010, 2009, 2007, 2005, 2003, 2002, 2000, 1998, 1996, 1994, 1992, 1990, 1988, 1986, 1983, 1981, 1979, 1977, 1974, 1972, 1970, 1967, 1965, 1962, 1960, 1957, 1954, 1952, 1949, 1946, 1944, 1941, 1938, 1935, 1932, 1930, 1927, 1924, 1921, 1918, 1914, 1911, 1908, 1905, 1902, 1899, 1895, 1892, 1889, 1885, 1882, 1878, 1875, 1871, 1868, 1864, 1861, 1857, 1853, 1850, 1846, 1842, 1838, 1835, 1831, 1827, 1823, 1819, 1815, 1811, 1807, 1803, 1799, 1795, 1791, 1786, 1782, 1778, 1774, 1769, 1765, 1761, 1756, 1752, 1748, 1743, 1739, 1734, 1730, 1725, 1720, 1716, 1711, 1707, 1702, 1697, 1692, 1688, 1683, 1678, 1673, 1668, 1663, 1658, 1653, 1649, 1644, 1639, 1633, 1628, 1623, 1618, 1613, 1608, 1603, 1598, 1592, 1587, 1582, 1577, 1571, 1566, 1561, 1555, 1550, 1545, 1539, 1534, 1528, 1523, 1517, 1512, 1506, 1501, 1495, 1490, 1484, 1478, 1473, 1467, 1461, 1456, 1450, 1444, 1438, 1433, 1427, 1421, 1415, 1410, 1404, 1398, 1392, 1386, 1380, 1374, 1368, 1363, 1357, 1351, 1345, 1339, 1333, 1327, 1321, 1315, 1309, 1303, 1297, 1291, 1284, 1278, 1272, 1266, 1260, 1254, 1248, 1242, 1236, 1229, 1223, 1217, 1211, 1205, 1199, 1192, 1186, 1180, 1174, 1168, 1161, 1155, 1149, 1143, 1136, 1130, 1124, 1118, 1111, 1105, 1099, 1093, 1086, 1080, 1074, 1067, 1061, 1055, 1049, 1042, 1036, 1030, 1023, 1017, 1011, 1005, 998, 992, 986, 980, 973, 967, 961, 954, 948, 942, 936, 929, 923, 917, 911, 904, 898, 892, 886, 879, 873, 867, 861, 855, 848, 842, 836, 830, 824, 818, 811, 805, 799, 793, 787, 781, 775, 769, 763, 756, 750, 744, 738, 732, 726, 720, 714, 708, 702, 696, 690, 684, 679, 673, 667, 661, 655, 649, 643, 637, 632, 626, 620, 614, 609, 603, 597, 591, 586, 580, 574, 569, 563, 557, 552, 546, 541, 535, 530, 524, 519, 513, 508, 502, 497, 492, 486, 481, 476, 470, 465, 460, 455, 449, 444, 439, 434, 429, 424, 419, 414, 408, 403, 398, 394, 389, 384, 379, 374, 369, 364, 359, 355, 350, 345, 340, 336, 331, 327, 322, 317, 313, 308, 304, 299, 295, 291, 286, 282, 278, 273, 269, 265, 261, 256, 252, 248, 244, 240, 236, 232, 228, 224, 220, 216, 212, 209, 205, 201, 197, 194, 190, 186, 183, 179, 176, 172, 169, 165, 162, 158, 155, 152, 148, 145, 142, 139, 136, 133, 129, 126, 123, 120, 117, 115, 112, 109, 106, 103, 101, 98, 95, 93, 90, 87, 85, 82, 80, 77, 75, 73, 70, 68, 66, 64, 61, 59, 57, 55, 53, 51, 49, 47, 45, 44, 42, 40, 38, 37, 35, 33, 32, 30, 29, 27, 26, 24, 23, 22, 20, 19, 18, 17, 16, 15, 14, 13, 12, 11, 10, 9, 8, 7, 6, 6, 5, 4, 4, 3, 3, 2, 2, 1, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 1, 2, 2, 3, 3, 4, 4, 5, 6, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16, 17, 18, 19, 20, 22, 23, 24, 26, 27, 29, 30, 32, 33, 35, 37, 38, 40, 42, 44, 45, 47, 49, 51, 53, 55, 57, 59, 61, 64, 66, 68, 70, 73, 75, 77, 80, 82, 85, 87, 90, 93, 95, 98, 101, 103, 106, 109, 112, 115, 117, 120, 123, 126, 129, 133, 136, 139, 142, 145, 148, 152, 155, 158, 162, 165, 169, 172, 176, 179, 183, 186, 190, 194, 197, 201, 205, 209, 212, 216, 220, 224, 228, 232, 236, 240, 244, 248, 252, 256, 261, 265, 269, 273, 278, 282, 286, 291, 295, 299, 304, 308, 313, 317, 322, 327, 331, 336, 340, 345, 350, 355, 359, 364, 369, 374, 379, 384, 389, 394, 398, 403, 408, 414, 419, 424, 429, 434, 439, 444, 449, 455, 460, 465, 470, 476, 481, 486, 492, 497, 502, 508, 513, 519, 524, 530, 535, 541, 546, 552, 557, 563, 569, 574, 580, 586, 591, 597, 603, 609, 614, 620, 626, 632, 637, 643, 649, 655, 661, 667, 673, 679, 684, 690, 696, 702, 708, 714, 720, 726, 732, 738, 744, 750, 756, 763, 769, 775, 781, 787, 793, 799, 805, 811, 818, 824, 830, 836, 842, 848, 855, 861, 867, 873, 879, 886, 892, 898, 904, 911, 917, 923, 929, 936, 942, 948, 954, 961, 967, 973, 980, 986, 992, 998, 1005, 1011, 1017 };
// static const uint16_t sin_table_11bit_512[512]   = {1024, 1036, 1049, 1061, 1074, 1086, 1099, 1111, 1124, 1136, 1149, 1161, 1174, 1186, 1199, 1211, 1223, 1236, 1248, 1260, 1272, 1284, 1297, 1309, 1321, 1333, 1345, 1357, 1368, 1380, 1392, 1404, 1415, 1427, 1438, 1450, 1461, 1473, 1484, 1495, 1506, 1517, 1528, 1539, 1550, 1561, 1571, 1582, 1592, 1603, 1613, 1623, 1633, 1644, 1653, 1663, 1673, 1683, 1692, 1702, 1711, 1720, 1730, 1739, 1748, 1756, 1765, 1774, 1782, 1791, 1799, 1807, 1815, 1823, 1831, 1838, 1846, 1853, 1861, 1868, 1875, 1882, 1889, 1895, 1902, 1908, 1914, 1921, 1927, 1932, 1938, 1944, 1949, 1954, 1960, 1965, 1970, 1974, 1979, 1983, 1988, 1992, 1996, 2000, 2003, 2007, 2010, 2014, 2017, 2020, 2023, 2025, 2028, 2030, 2032, 2034, 2036, 2038, 2040, 2041, 2043, 2044, 2045, 2046, 2046, 2047, 2047, 2047, 2048, 2047, 2047, 2047, 2046, 2046, 2045, 2044, 2043, 2041, 2040, 2038, 2036, 2034, 2032, 2030, 2028, 2025, 2023, 2020, 2017, 2014, 2010, 2007, 2003, 2000, 1996, 1992, 1988, 1983, 1979, 1974, 1970, 1965, 1960, 1954, 1949, 1944, 1938, 1932, 1927, 1921, 1914, 1908, 1902, 1895, 1889, 1882, 1875, 1868, 1861, 1853, 1846, 1838, 1831, 1823, 1815, 1807, 1799, 1791, 1782, 1774, 1765, 1756, 1748, 1739, 1730, 1720, 1711, 1702, 1692, 1683, 1673, 1663, 1653, 1644, 1633, 1623, 1613, 1603, 1592, 1582, 1571, 1561, 1550, 1539, 1528, 1517, 1506, 1495, 1484, 1473, 1461, 1450, 1438, 1427, 1415, 1404, 1392, 1380, 1368, 1357, 1345, 1333, 1321, 1309, 1297, 1284, 1272, 1260, 1248, 1236, 1223, 1211, 1199, 1186, 1174, 1161, 1149, 1136, 1124, 1111, 1099, 1086, 1074, 1061, 1049, 1036, 1024, 1011, 998, 986, 973, 961, 948, 936, 923, 911, 898, 886, 873, 861, 848, 836, 824, 811, 799, 787, 775, 763, 750, 738, 726, 714, 702, 690, 679, 667, 655, 643, 632, 620, 609, 597, 586, 574, 563, 552, 541, 530, 519, 508, 497, 486, 476, 465, 455, 444, 434, 424, 414, 403, 394, 384, 374, 364, 355, 345, 336, 327, 317, 308, 299, 291, 282, 273, 265, 256, 248, 240, 232, 224, 216, 209, 201, 194, 186, 179, 172, 165, 158, 152, 145, 139, 133, 126, 120, 115, 109, 103, 98, 93, 87, 82, 77, 73, 68, 64, 59, 55, 51, 47, 44, 40, 37, 33, 30, 27, 24, 22, 19, 17, 15, 13, 11, 9, 7, 6, 4, 3, 2, 1, 1, 0, 0, 0, 0, 0, 0, 0, 1, 1, 2, 3, 4, 6, 7, 9, 11, 13, 15, 17, 19, 22, 24, 27, 30, 33, 37, 40, 44, 47, 51, 55, 59, 64, 68, 73, 77, 82, 87, 93, 98, 103, 109, 115, 120, 126, 133, 139, 145, 152, 158, 165, 172, 179, 186, 194, 201, 209, 216, 224, 232, 240, 248, 256, 265, 273, 282, 291, 299, 308, 317, 327, 336, 345, 355, 364, 374, 384, 394, 403, 414, 424, 434, 444, 455, 465, 476, 486, 497, 508, 519, 530, 541, 552, 563, 574, 586, 597, 609, 620, 632, 643, 655, 667, 679, 690, 702, 714, 726, 738, 750, 763, 775, 787, 799, 811, 824, 836, 848, 861, 873, 886, 898, 911, 923, 936, 948, 961, 973, 986, 998, 1011 };
static const uint16_t sin_table_10bit_512[512]   = {512, 518, 524, 530, 537, 543, 549, 555, 562, 568, 574, 580, 587, 593, 599, 605, 611, 618, 624, 630, 636, 642, 648, 654, 660, 666, 672, 678, 684, 690, 696, 702, 707, 713, 719, 725, 730, 736, 742, 747, 753, 758, 764, 769, 775, 780, 785, 791, 796, 801, 806, 811, 816, 822, 826, 831, 836, 841, 846, 851, 855, 860, 865, 869, 874, 878, 882, 887, 891, 895, 899, 903, 907, 911, 915, 919, 923, 926, 930, 934, 937, 941, 944, 947, 951, 954, 957, 960, 963, 966, 969, 972, 974, 977, 980, 982, 985, 987, 989, 991, 994, 996, 998, 1000, 1001, 1003, 1005, 1007, 1008, 1010, 1011, 1012, 1014, 1015, 1016, 1017, 1018, 1019, 1020, 1020, 1021, 1022, 1022, 1023, 1023, 1023, 1023, 1023, 1024, 1023, 1023, 1023, 1023, 1023, 1022, 1022, 1021, 1020, 1020, 1019, 1018, 1017, 1016, 1015, 1014, 1012, 1011, 1010, 1008, 1007, 1005, 1003, 1001, 1000, 998, 996, 994, 991, 989, 987, 985, 982, 980, 977, 974, 972, 969, 966, 963, 960, 957, 954, 951, 947, 944, 941, 937, 934, 930, 926, 923, 919, 915, 911, 907, 903, 899, 895, 891, 887, 882, 878, 874, 869, 865, 860, 855, 851, 846, 841, 836, 831, 826, 822, 816, 811, 806, 801, 796, 791, 785, 780, 775, 769, 764, 758, 753, 747, 742, 736, 730, 725, 719, 713, 707, 702, 696, 690, 684, 678, 672, 666, 660, 654, 648, 642, 636, 630, 624, 618, 611, 605, 599, 593, 587, 580, 574, 568, 562, 555, 549, 543, 537, 530, 524, 518, 512, 505, 499, 493, 486, 480, 474, 468, 461, 455, 449, 443, 436, 430, 424, 418, 412, 405, 399, 393, 387, 381, 375, 369, 363, 357, 351, 345, 339, 333, 327, 321, 316, 310, 304, 298, 293, 287, 281, 276, 270, 265, 259, 254, 248, 243, 238, 232, 227, 222, 217, 212, 207, 201, 197, 192, 187, 182, 177, 172, 168, 163, 158, 154, 149, 145, 141, 136, 132, 128, 124, 120, 116, 112, 108, 104, 100, 97, 93, 89, 86, 82, 79, 76, 72, 69, 66, 63, 60, 57, 54, 51, 49, 46, 43, 41, 38, 36, 34, 32, 29, 27, 25, 23, 22, 20, 18, 16, 15, 13, 12, 11, 9, 8, 7, 6, 5, 4, 3, 3, 2, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 2, 3, 3, 4, 5, 6, 7, 8, 9, 11, 12, 13, 15, 16, 18, 20, 22, 23, 25, 27, 29, 32, 34, 36, 38, 41, 43, 46, 49, 51, 54, 57, 60, 63, 66, 69, 72, 76, 79, 82, 86, 89, 93, 97, 100, 104, 108, 112, 116, 120, 124, 128, 132, 136, 141, 145, 149, 154, 158, 163, 168, 172, 177, 182, 187, 192, 197, 201, 207, 212, 217, 222, 227, 232, 238, 243, 248, 254, 259, 265, 270, 276, 281, 287, 293, 298, 304, 310, 316, 321, 327, 333, 339, 345, 351, 357, 363, 369, 375, 381, 387, 393, 399, 405, 412, 418, 424, 430, 436, 443, 449, 455, 461, 468, 474, 480, 486, 493, 499, 505 };
static const uint16_t sin_table_10bit_256[256]   = {512, 524, 537, 549, 562, 574, 587, 599, 611, 624, 636, 648, 660, 672, 684, 696, 707, 719, 730, 742, 753, 764, 775, 785, 796, 806, 816, 826, 836, 846, 855, 865, 874, 882, 891, 899, 907, 915, 923, 930, 937, 944, 951, 957, 963, 969, 974, 980, 985, 989, 994, 998, 1001, 1005, 1008, 1011, 1014, 1016, 1018, 1020, 1021, 1022, 1023, 1023, 1024, 1023, 1023, 1022, 1021, 1020, 1018, 1016, 1014, 1011, 1008, 1005, 1001, 998, 994, 989, 985, 980, 974, 969, 963, 957, 951, 944, 937, 930, 923, 915, 907, 899, 891, 882, 874, 865, 855, 846, 836, 826, 816, 806, 796, 785, 775, 764, 753, 742, 730, 719, 707, 696, 684, 672, 660, 648, 636, 624, 611, 599, 587, 574, 562, 549, 537, 524, 512, 499, 486, 474, 461, 449, 436, 424, 412, 399, 387, 375, 363, 351, 339, 327, 316, 304, 293, 281, 270, 259, 248, 238, 227, 217, 207, 197, 187, 177, 168, 158, 149, 141, 132, 124, 116, 108, 100, 93, 86, 79, 72, 66, 60, 54, 49, 43, 38, 34, 29, 25, 22, 18, 15, 12, 9, 7, 5, 3, 2, 1, 0, 0, 0, 0, 0, 1, 2, 3, 5, 7, 9, 12, 15, 18, 22, 25, 29, 34, 38, 43, 49, 54, 60, 66, 72, 79, 86, 93, 100, 108, 116, 124, 132, 141, 149, 158, 168, 177, 187, 197, 207, 217, 227, 238, 248, 259, 270, 281, 293, 304, 316, 327, 339, 351, 363, 375, 387, 399, 412, 424, 436, 449, 461, 474, 486, 499 };
static const uint32_t env_sin_table[128] = {768, 780, 793, 805, 817, 830, 842, 854, 865, 877, 888, 899, 910, 920, 930, 939, 949, 957, 965, 973, 980, 987, 993, 999, 1004, 1009, 1012, 1016, 1019, 1021, 1022, 1023, 1024, 1023, 1022, 1021, 1019, 1016, 1012, 1009, 1004, 999, 993, 987, 980, 973, 965, 957, 949, 939, 930, 920, 910, 899, 888, 877, 865, 854, 842, 830, 817, 805, 793, 780, 768, 755, 742, 730, 718, 705, 693, 681, 670, 658, 647, 636, 625, 615, 605, 596, 586, 578, 570, 562, 555, 548, 542, 536, 531, 526, 523, 519, 516, 514, 513, 512, 512, 512, 513, 514, 516, 519, 523, 526, 531, 536, 542, 548, 555, 562, 570, 578, 586, 596, 605, 615, 625, 636, 647, 658, 670, 681, 693, 705, 718, 730, 742, 755 };
static const uint32_t env2_sin_table[256] = {512, 524, 537, 549, 562, 574, 587, 599, 611, 624, 636, 648, 660, 672, 684, 696, 707, 719, 730, 742, 753, 764, 775, 785, 796, 806, 816, 826, 836, 846, 855, 865, 874, 882, 891, 899, 907, 915, 923, 930, 937, 944, 951, 957, 963, 969, 974, 980, 985, 989, 994, 998, 1001, 1005, 1008, 1011, 1014, 1016, 1018, 1020, 1021, 1022, 1023, 1023, 1024, 1023, 1023, 1022, 1021, 1020, 1018, 1016, 1014, 1011, 1008, 1005, 1001, 998, 994, 989, 985, 980, 974, 969, 963, 957, 951, 944, 937, 930, 923, 915, 907, 899, 891, 882, 874, 865, 855, 846, 836, 826, 816, 806, 796, 785, 775, 764, 753, 742, 730, 719, 707, 696, 684, 672, 660, 648, 636, 624, 611, 599, 587, 574, 562, 549, 537, 524, 512, 499, 486, 474, 461, 449, 436, 424, 412, 399, 387, 375, 363, 351, 339, 327, 316, 304, 293, 281, 270, 259, 248, 238, 227, 217, 207, 197, 187, 177, 168, 158, 149, 141, 132, 124, 116, 108, 100, 93, 86, 79, 72, 66, 60, 54, 49, 43, 38, 34, 29, 25, 22, 18, 15, 12, 9, 7, 5, 3, 2, 1, 0, 0, 0, 0, 0, 1, 2, 3, 5, 7, 9, 12, 15, 18, 22, 25, 29, 34, 38, 43, 49, 54, 60, 66, 72, 79, 86, 93, 100, 108, 116, 124, 132, 141, 149, 158, 168, 177, 187, 197, 207, 217, 227, 238, 248, 259, 270, 281, 293, 304, 316, 327, 339, 351, 363, 375, 387, 399, 412, 424, 436, 449, 461, 474, 486, 499 };
static const uint32_t env3_sin_table[1024] = {640, 642, 644, 647, 649, 651, 654, 656, 658, 661, 663, 665, 668, 670, 672, 675, 677, 679, 682, 684, 687, 689, 691, 694, 696, 698, 701, 703, 705, 707, 710, 712, 714, 717, 719, 721, 724, 726, 728, 731, 733, 735, 737, 740, 742, 744, 746, 749, 751, 753, 755, 758, 760, 762, 764, 767, 769, 771, 773, 775, 778, 780, 782, 784, 786, 789, 791, 793, 795, 797, 799, 802, 804, 806, 808, 810, 812, 814, 816, 818, 821, 823, 825, 827, 829, 831, 833, 835, 837, 839, 841, 843, 845, 847, 849, 851, 853, 855, 857, 859, 861, 863, 864, 866, 868, 870, 872, 874, 876, 878, 879, 881, 883, 885, 887, 889, 890, 892, 894, 896, 897, 899, 901, 903, 904, 906, 908, 909, 911, 913, 914, 916, 918, 919, 921, 922, 924, 926, 927, 929, 930, 932, 933, 935, 936, 938, 939, 941, 942, 944, 945, 947, 948, 949, 951, 952, 953, 955, 956, 957, 959, 960, 961, 963, 964, 965, 966, 968, 969, 970, 971, 972, 974, 975, 976, 977, 978, 979, 980, 981, 982, 984, 985, 986, 987, 988, 989, 990, 991, 992, 992, 993, 994, 995, 996, 997, 998, 999, 999, 1000, 1001, 1002, 1003, 1003, 1004, 1005, 1006, 1006, 1007, 1008, 1008, 1009, 1010, 1010, 1011, 1011, 1012, 1013, 1013, 1014, 1014, 1015, 1015, 1016, 1016, 1017, 1017, 1017, 1018, 1018, 1019, 1019, 1019, 1020, 1020, 1020, 1021, 1021, 1021, 1021, 1022, 1022, 1022, 1022, 1022, 1023, 1023, 1023, 1023, 1023, 1023, 1023, 1023, 1023, 1023, 1023, 1024, 1023, 1023, 1023, 1023, 1023, 1023, 1023, 1023, 1023, 1023, 1023, 1022, 1022, 1022, 1022, 1022, 1021, 1021, 1021, 1021, 1020, 1020, 1020, 1019, 1019, 1019, 1018, 1018, 1017, 1017, 1017, 1016, 1016, 1015, 1015, 1014, 1014, 1013, 1013, 1012, 1011, 1011, 1010, 1010, 1009, 1008, 1008, 1007, 1006, 1006, 1005, 1004, 1003, 1003, 1002, 1001, 1000, 999, 999, 998, 997, 996, 995, 994, 993, 992, 992, 991, 990, 989, 988, 987, 986, 985, 984, 982, 981, 980, 979, 978, 977, 976, 975, 974, 972, 971, 970, 969, 968, 966, 965, 964, 963, 961, 960, 959, 957, 956, 955, 953, 952, 951, 949, 948, 947, 945, 944, 942, 941, 939, 938, 936, 935, 933, 932, 930, 929, 927, 926, 924, 922, 921, 919, 918, 916, 914, 913, 911, 909, 908, 906, 904, 903, 901, 899, 897, 896, 894, 892, 890, 889, 887, 885, 883, 881, 879, 878, 876, 874, 872, 870, 868, 866, 864, 863, 861, 859, 857, 855, 853, 851, 849, 847, 845, 843, 841, 839, 837, 835, 833, 831, 829, 827, 825, 823, 821, 818, 816, 814, 812, 810, 808, 806, 804, 802, 799, 797, 795, 793, 791, 789, 786, 784, 782, 780, 778, 775, 773, 771, 769, 767, 764, 762, 760, 758, 755, 753, 751, 749, 746, 744, 742, 740, 737, 735, 733, 731, 728, 726, 724, 721, 719, 717, 714, 712, 710, 707, 705, 703, 701, 698, 696, 694, 691, 689, 687, 684, 682, 679, 677, 675, 672, 670, 668, 665, 663, 661, 658, 656, 654, 651, 649, 647, 644, 642, 639, 637, 635, 632, 630, 628, 625, 623, 621, 618, 616, 614, 611, 609, 607, 604, 602, 600, 597, 595, 592, 590, 588, 585, 583, 581, 578, 576, 574, 572, 569, 567, 565, 562, 560, 558, 555, 553, 551, 548, 546, 544, 542, 539, 537, 535, 533, 530, 528, 526, 524, 521, 519, 517, 515, 512, 510, 508, 506, 504, 501, 499, 497, 495, 493, 490, 488, 486, 484, 482, 480, 477, 475, 473, 471, 469, 467, 465, 463, 461, 458, 456, 454, 452, 450, 448, 446, 444, 442, 440, 438, 436, 434, 432, 430, 428, 426, 424, 422, 420, 418, 416, 415, 413, 411, 409, 407, 405, 403, 401, 400, 398, 396, 394, 392, 390, 389, 387, 385, 383, 382, 380, 378, 376, 375, 373, 371, 370, 368, 366, 365, 363, 361, 360, 358, 357, 355, 353, 352, 350, 349, 347, 346, 344, 343, 341, 340, 338, 337, 335, 334, 332, 331, 330, 328, 327, 326, 324, 323, 322, 320, 319, 318, 316, 315, 314, 313, 311, 310, 309, 308, 307, 305, 304, 303, 302, 301, 300, 299, 298, 297, 295, 294, 293, 292, 291, 290, 289, 288, 287, 287, 286, 285, 284, 283, 282, 281, 280, 280, 279, 278, 277, 276, 276, 275, 274, 273, 273, 272, 271, 271, 270, 269, 269, 268, 268, 267, 266, 266, 265, 265, 264, 264, 263, 263, 262, 262, 262, 261, 261, 260, 260, 260, 259, 259, 259, 258, 258, 258, 258, 257, 257, 257, 257, 257, 256, 256, 256, 256, 256, 256, 256, 256, 256, 256, 256, 256, 256, 256, 256, 256, 256, 256, 256, 256, 256, 256, 256, 257, 257, 257, 257, 257, 258, 258, 258, 258, 259, 259, 259, 260, 260, 260, 261, 261, 262, 262, 262, 263, 263, 264, 264, 265, 265, 266, 266, 267, 268, 268, 269, 269, 270, 271, 271, 272, 273, 273, 274, 275, 276, 276, 277, 278, 279, 280, 280, 281, 282, 283, 284, 285, 286, 287, 287, 288, 289, 290, 291, 292, 293, 294, 295, 297, 298, 299, 300, 301, 302, 303, 304, 305, 307, 308, 309, 310, 311, 313, 314, 315, 316, 318, 319, 320, 322, 323, 324, 326, 327, 328, 330, 331, 332, 334, 335, 337, 338, 340, 341, 343, 344, 346, 347, 349, 350, 352, 353, 355, 357, 358, 360, 361, 363, 365, 366, 368, 370, 371, 373, 375, 376, 378, 380, 382, 383, 385, 387, 389, 390, 392, 394, 396, 398, 400, 401, 403, 405, 407, 409, 411, 413, 415, 416, 418, 420, 422, 424, 426, 428, 430, 432, 434, 436, 438, 440, 442, 444, 446, 448, 450, 452, 454, 456, 458, 461, 463, 465, 467, 469, 471, 473, 475, 477, 480, 482, 484, 486, 488, 490, 493, 495, 497, 499, 501, 504, 506, 508, 510, 512, 515, 517, 519, 521, 524, 526, 528, 530, 533, 535, 537, 539, 542, 544, 546, 548, 551, 553, 555, 558, 560, 562, 565, 567, 569, 572, 574, 576, 578, 581, 583, 585, 588, 590, 592, 595, 597, 600, 602, 604, 607, 609, 611, 614, 616, 618, 621, 623, 625, 628, 630, 632, 635, 637 };

static void toneA_ISR(void);
static void toneB_ISR(void);
static void toneC_ISR(void);

static const uint16_t *tableA = sin_table, *tableB = sin_table, *tableC = sin_table;
static uint32_t tableAsize = 256, tableBsize = 256, tableCsize = 256;
static enum ToneGenerator tones;

void sound_init(enum ToneGenerator tns) {
    tones = tns;
    if (tones&TONE_A) Timer0A_Init(toneA_ISR, 0, 0);
    if (tones&TONE_B) Timer1A_Init(toneB_ISR, 0, 0);
    if (tones&TONE_C) Timer2A_Init(toneC_ISR, 0, 0);
}

void EnableInterrupts();
void DisableInterrupts();

void tone_start(enum ToneGenerator tone, uint16_t frequency) {
    switch (tone) {
    case TONE_A:
        uint32_t n = 80000000 / ((uint32_t)frequency * tableAsize);
        TIMER0_TAILR_R = n;
        if (!(TIMER0_CTL_R&0x00000001)){
            TIMER0_CTL_R |= 0x00000001;
        }
        break;
    case TONE_B:
        uint32_t n = 80000000 / ((uint32_t)frequency * tableBsize);
        TIMER1_TAILR_R = n;
        if (!(TIMER1_CTL_R&0x00000001)){
            TIMER1_CTL_R |= 0x00000001;
        }
        break;
    case TONE_C:
        uint32_t n = 80000000 / ((uint32_t)frequency * tableCsize);
        TIMER2_TAILR_R = n;
        if (!(TIMER2_CTL_R&0x00000001)){
            TIMER2_CTL_R |= 0x00000001;
        }
        break; 
    default: break;
    }
}

void tone_stop(enum ToneGenerator tone) {
    switch (tone) {
    case TONE_A:
        TIMER0_CTL_R = 0;
        break;
    case TONE_B:
        TIMER1_CTL_R = 0;
        break;
    case TONE_C:
        TIMER2_CTL_R = 0;
        break;
    default: break;
    }
}

void tone_set_table(enum ToneGenerator tone, const uint16_t* table, uint32_t size) {
    switch (tone) {
    case TONE_A:
        tableA = table; tableAsize = size;
        break;
    case TONE_B:
        tableA = table; tableAsize = size;
        break;
    case TONE_C:
        tableA = table; tableAsize = size;
        break;
    default: break;
    }
}

void sound_stop() {
    if (tones&TONE_A) TIMER0_CTL_R = 0;
    if (tones&TONE_B) TIMER1_CTL_R = 0;
    if (tones&TONE_C) TIMER2_CTL_R = 0;
    DAC_out(0);
}

static uint32_t toneAidx = 0;
static uint32_t toneBidx = 0;
static uint32_t toneCidx = 0;
static uint32_t eidx = 0;

#define PF1   (*((volatile uint32_t *)0x40025008)) // RED LED
#define PF2   (*((volatile uint32_t *)0x40025010)) // BLUE LED
#define PF3   (*((volatile uint32_t *)0x40025020)) // GREEN LED

#define DAC_out(val) SSI2_DR_R = val;

static void toneA_ISR(void) {
    // PF1 ^= 0x02;
    // PF1 ^= 0x02;
    DAC_out((tableA[(++toneAidx)&(tableAsize-1)] +
            tableB[toneBidx&(tableBsize-1)] +
            tableC[toneCidx&(tableCsize-1)]));
    // PF1 ^= 0x02;
}

static void toneB_ISR(void) {
    // PF2 ^= 0x04;
    // PF2 ^= 0x04;
    DAC_out((tableA[toneAidx&(tableAsize-1)] +
            tableB[(++toneBidx)&(tableBsize-1)] +
            tableC[toneCidx&(tableCsize-1)]));
    // PF2 ^= 0x04;
}

static void toneC_ISR(void) {
    // PF3 ^= 0x08;
    // PF3 ^= 0x08;
    DAC_out((tableA[toneAidx&(tableAsize-1)] +
            tableB[toneBidx&(tableBsize-1)] +
            tableC[(++toneCidx)&(tableCsize-1)]));
    // PF3 ^= 0x08;
}
