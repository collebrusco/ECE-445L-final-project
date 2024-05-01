#include "Display.h"
#include "../inc/ST7735.h"

// tach is inspired by 2006 - 2010 era SRT charger/magnum/challenger gauges
// example at https://images.craigslist.org/00C0C_ktKCwFVJbqo_0ak07K_600x450.jpg

// position and size
#define TACH_CENTER_X (64)
#define TACH_CENTER_Y (80)
#define TACH_RADIUS (56)
#define NEEDLE_LENGTH (48)
#define TACH_RING_INSET (24)

// colors
#define TACH_FACE_COLOR (0xFFFF)
#define TACH_NEEDLE_COLOR (0x001F)
#define TACH_TICK_COLOR (0)
#define TACH_REDLINE_COLOR (0x001C)
#define TACH_SHIFTZONE_COLOR (0x03F6)

#define adiff(a,b) ((a)-(b)<0 ? -((a)-(b)) : (a)-(b)) 
#define abs(a) ((a)<0?-(a):(a))

#define rpm_to_theta(rpm) (((983*rpm)>>15) + 60)

// these are binary 2^10, index = degrees
static const int16_t sin_table[360] = {
    0, 17, 35, 53, 71, 89, 107, 124, 142, 160, 177, 195, 212, 230, 247, 265, 282, 299, 316, 333, 
    350, 366, 383, 400, 416, 432, 448, 464, 480, 496, 511, 527, 542, 557, 572, 587, 601, 616, 630, 644, 
    658, 671, 685, 698, 711, 724, 736, 748, 760, 772, 784, 795, 806, 817, 828, 838, 848, 858, 868, 877, 
    886, 895, 904, 912, 920, 928, 935, 942, 949, 955, 962, 968, 973, 979, 984, 989, 993, 997, 1001, 1005, 
    1008, 1011, 1014, 1016, 1018, 1020, 1021, 1022, 1023, 1023, 1024, 1023, 1023, 1022, 1021, 1020, 1018, 1016, 
    1014, 1011, 1008, 1005, 1001, 997, 993, 989, 984, 979, 973, 968, 962, 955, 949, 942, 935, 928, 920, 912, 
    904, 895, 886, 877, 868, 858, 848, 838, 828, 817, 806, 795, 784, 772, 760, 748, 736, 724, 711, 698, 
    685, 671, 658, 644, 630, 616, 601, 587, 572, 557, 542, 527, 511, 496, 480, 464, 448, 432, 416, 400, 
    383, 366, 350, 333, 316, 299, 282, 265, 247, 230, 212, 195, 177, 160, 142, 124, 107, 89, 71, 53, 
    35, 17, 0, -17, -35, -53, -71, -89, -107, -124, -142, -160, -177, -195, -212, -230, -247, -265, -282, 
    -299, -316, -333, -350, -366, -383, -400, -416, -432, -448, -464, -480, -496, -511, -527, -542, -557, 
    -572, -587, -601, -616, -630, -644, -658, -671, -685, -698, -711, -724, -736, -748, -760, -772, -784, 
    -795, -806, -817, -828, -838, -848, -858, -868, -877, -886, -895, -904, -912, -920, -928, -935, -942, 
    -949, -955, -962, -968, -973, -979, -984, -989, -993, -997, -1001, -1005, -1008, -1011, -1014, -1016, 
    -1018, -1020, -1021, -1022, -1023, -1023, -1024, -1023, -1023, -1022, -1021, -1020, -1018, -1016, -1014, 
    -1011, -1008, -1005, -1001, -997, -993, -989, -984, -979, -973, -968, -962, -955, -949, -942, -935, 
    -928, -920, -912, -904, -895, -886, -877, -868, -858, -848, -838, -828, -817, -806, -795, -784, -772, 
    -760, -748, -736, -724, -711, -698, -685, -671, -658, -644, -630, -616, -601, -587, -572, -557, -542, 
    -527, -512, -496, -480, -464, -448, -432, -416, -400, -383, -366, -350, -333, -316, -299, -282, -265, 
    -247, -230, -212, -195, -177, -160, -142, -124, -107, -89, -71, -53, -35, -17
};

static const int16_t cos_table[360] = {
    1024, 1023, 1023, 1022, 1021, 1020, 1018, 1016, 1014, 1011, 1008, 1005, 1001, 997, 993, 989, 
    984, 979, 973, 968, 962, 955, 949, 942, 935, 928, 920, 912, 904, 895, 886, 877, 868, 858, 848, 
    838, 828, 817, 806, 795, 784, 772, 760, 748, 736, 724, 711, 698, 685, 671, 658, 644, 630, 616, 
    601, 587, 572, 557, 542, 527, 512, 496, 480, 464, 448, 432, 416, 400, 383, 366, 350, 333, 316, 
    299, 282, 265, 247, 230, 212, 195, 177, 160, 142, 124, 107, 89, 71, 53, 35, 17, 0, -17, -35, 
    -53, -71, -89, -107, -124, -142, -160, -177, -195, -212, -230, -247, -265, -282, -299, -316, -333, 
    -350, -366, -383, -400, -416, -432, -448, -464, -480, -496, -511, -527, -542, -557, -572, -587, 
    -601, -616, -630, -644, -658, -671, -685, -698, -711, -724, -736, -748, -760, -772, -784, -795, 
    -806, -817, -828, -838, -848, -858, -868, -877, -886, -895, -904, -912, -920, -928, -935, -942, 
    -949, -955, -962, -968, -973, -979, -984, -989, -993, -997, -1001, -1005, -1008, -1011, -1014, 
    -1016, -1018, -1020, -1021, -1022, -1023, -1023, -1024, -1023, -1023, -1022, -1021, -1020, -1018, 
    -1016, -1014, -1011, -1008, -1005, -1001, -997, -993, -989, -984, -979, -973, -968, -962, -955, 
    -949, -942, -935, -928, -920, -912, -904, -895, -886, -877, -868, -858, -848, -838, -828, -817, 
    -806, -795, -784, -772, -760, -748, -736, -724, -711, -698, -685, -671, -658, -644, -630, -616, 
    -601, -587, -572, -557, -542, -527, -512, -496, -480, -464, -448, -432, -416, -400, -383, -366, 
    -350, -333, -316, -299, -282, -265, -247, -230, -212, -195, -177, -160, -142, -124, -107, -89, 
    -71, -53, -35, -17, 0, 17, 35, 53, 71, 89, 107, 124, 142, 160, 177, 195, 212, 230, 247, 265, 
    282, 299, 316, 333, 350, 366, 383, 400, 416, 432, 448, 464, 480, 496, 511, 527, 542, 557, 572, 
    587, 601, 616, 630, 644, 658, 671, 685, 698, 711, 724, 736, 748, 760, 772, 784, 795, 806, 817, 
    828, 838, 848, 858, 868, 877, 886, 895, 904, 912, 920, 928, 935, 942, 949, 955, 962, 968, 973, 
    979, 984, 989, 993, 997, 1001, 1005, 1008, 1011, 1014, 1016, 1018, 1020, 1021, 1022, 1023, 1023
};

static int32_t theta_to_vec_x(uint16_t theta, int32_t mag) {
	return ((sin_table[theta] * -mag) >> 10);
}

static int32_t theta_to_vec_y(uint16_t theta, int32_t mag) {
	return ((cos_table[theta] * mag) >> 10);
}

// 24/17 is ~ sqrt(2)
static void draw_circle(int16_t x, int16_t y, int16_t radius, uint16_t color) {
	int16_t inner_radius;
	{															// center rectangle fill
		int16_t inner_diameter = (radius * 34) / 24;			// = radius * (2/sqrt(2))
		inner_radius = inner_diameter>>1;
		ST7735_FillRect(x-inner_radius,y-inner_radius,inner_diameter,inner_diameter,color);
	}
	for (int16_t i = (-radius); i <= (-inner_radius); i++) {	// left round
		for (int16_t j = (-radius); j <= (radius); j++) {
			if (i*i+j*j<radius*radius) ST7735_DrawPixel(x+i,y+j,color);
		}
	}
	for (int16_t i = (inner_radius); i <= (radius); i++) {		// right round
		for (int16_t j = (-radius); j <= (radius); j++) {
			if (i*i+j*j<radius*radius) ST7735_DrawPixel(x+i,y+j,color);
		}
	}
	for (int16_t j = (-radius); j <= (-inner_radius); j++) {	// bot round
		for (int16_t i = (-radius); i <= (radius); i++) {
			if (i*i+j*j<radius*radius) ST7735_DrawPixel(x+i,y+j,color);
		}
	}
	for (int16_t j = (inner_radius); j <= (radius); j++) {		// top round
		for (int16_t i = (-radius); i <= (radius); i++) {
			if (i*i+j*j<radius*radius) ST7735_DrawPixel(x+i,y+j,color);
		}
	}
}

static void draw_tach_center() {
	ST7735_DrawCircle(TACH_CENTER_X-4,TACH_CENTER_Y-4,TACH_TICK_COLOR);
}

static void draw_tach_num(int32_t rpm) {
	ST7735_DrawChar(TACH_CENTER_X+theta_to_vec_x(rpm_to_theta(rpm*1000), 40) - 2,
					TACH_CENTER_Y+theta_to_vec_y(rpm_to_theta(rpm*1000), 40) - 4,
					(rpm)+0x30,
					0,
					TACH_FACE_COLOR,
					1);
}

static void draw_tach_bg() {
	draw_circle(TACH_CENTER_X,TACH_CENTER_Y,TACH_RADIUS,TACH_FACE_COLOR);

	for (int32_t i = 0; i < 9; i++) {
		draw_tach_num(i);
	}

	// lower black 'display' region
	for (int32_t x = -TACH_RADIUS; x < TACH_RADIUS; x++)
	{
		for (int32_t y = 0; y < TACH_RADIUS; y++) 
		{
			if (x*x+y*y>(TACH_RADIUS-8)*(TACH_RADIUS-8)) continue; 
			if (!(y & 0xFFFFFFF0)) continue;
			if (y-8 < (( x*151348)>>18)) continue; 
			if (y-8 < ((-x*151348)>>18)) continue;
			ST7735_DrawPixel(x+TACH_CENTER_X,y+TACH_CENTER_Y,0);
		}
	}
	// inner black ring
	for (uint16_t theta = 0; theta < 360; theta++) {
		ST7735_FillRect(TACH_CENTER_X+theta_to_vec_x(theta,TACH_RADIUS-TACH_RING_INSET),
						TACH_CENTER_Y+theta_to_vec_y(theta,TACH_RADIUS-TACH_RING_INSET), 
						2,2, TACH_TICK_COLOR);
	}
	ST7735_DrawString(8,10,"RPMx1k", 0x001F);
	update_gear(1);
}

static void draw_tach_bezel() {
	for (uint16_t theta = 0; theta < 360; theta++) {
		ST7735_FillRect(TACH_CENTER_X+theta_to_vec_x(theta,TACH_RADIUS+1)-1,
						TACH_CENTER_Y+theta_to_vec_y(theta,TACH_RADIUS+1)-1, 
						4,4, (16<<11)|(32<<5)|16);
	}
}

static void draw_tach_shiftzone() {
	for (uint16_t i = 240; i < 255; i+=1) {
		ST7735_Line(
			TACH_CENTER_X+theta_to_vec_x(i, TACH_RADIUS-10),
			TACH_CENTER_Y+theta_to_vec_y(i, TACH_RADIUS-10),
			TACH_CENTER_X+theta_to_vec_x(i, TACH_RADIUS),
			TACH_CENTER_Y+theta_to_vec_y(i, TACH_RADIUS),
			TACH_SHIFTZONE_COLOR
		);
	}
}

static void draw_tach_redline() {
	for (uint16_t i = 255; i < 299; i+=1) {
		ST7735_Line(
			TACH_CENTER_X+theta_to_vec_x(i, TACH_RADIUS-10),
			TACH_CENTER_Y+theta_to_vec_y(i, TACH_RADIUS-10),
			TACH_CENTER_X+theta_to_vec_x(i, TACH_RADIUS),
			TACH_CENTER_Y+theta_to_vec_y(i, TACH_RADIUS),
			TACH_REDLINE_COLOR
		);
	}
}

static void draw_tach_ticks() {
	for (uint16_t i = 0; i < 9000; i+=1000) {
		uint16_t theta = rpm_to_theta(i);
		ST7735_Line(
			TACH_CENTER_X+theta_to_vec_x(theta, TACH_RADIUS-10),
			TACH_CENTER_Y+theta_to_vec_y(theta, TACH_RADIUS-10),
			TACH_CENTER_X+theta_to_vec_x(theta, TACH_RADIUS),
			TACH_CENTER_Y+theta_to_vec_y(theta, TACH_RADIUS),
			TACH_TICK_COLOR
		);
	}
	for (uint16_t i = 500; i < 8000; i+=1000) {
		uint16_t theta = rpm_to_theta(i);
		ST7735_Line(
			TACH_CENTER_X+theta_to_vec_x(theta, TACH_RADIUS-6),
			TACH_CENTER_Y+theta_to_vec_y(theta, TACH_RADIUS-6),
			TACH_CENTER_X+theta_to_vec_x(theta, TACH_RADIUS),
			TACH_CENTER_Y+theta_to_vec_y(theta, TACH_RADIUS),
			TACH_TICK_COLOR
		);
	}
}

static void draw_single_tach_shiftzone(uint16_t theta) {
	ST7735_Line(
		TACH_CENTER_X+theta_to_vec_x(theta, TACH_RADIUS-10),
		TACH_CENTER_Y+theta_to_vec_y(theta, TACH_RADIUS-10),
		TACH_CENTER_X+theta_to_vec_x(theta, TACH_RADIUS-7),
		TACH_CENTER_Y+theta_to_vec_y(theta, TACH_RADIUS-7),
		TACH_SHIFTZONE_COLOR
	);
}

static void draw_single_tach_redline(uint16_t theta) {
	ST7735_Line(
		TACH_CENTER_X+theta_to_vec_x(theta, TACH_RADIUS-10),
		TACH_CENTER_Y+theta_to_vec_y(theta, TACH_RADIUS-10),
		TACH_CENTER_X+theta_to_vec_x(theta, TACH_RADIUS-7),
		TACH_CENTER_Y+theta_to_vec_y(theta, TACH_RADIUS-7),
		TACH_REDLINE_COLOR
	);
}

static void draw_tach_tick(uint16_t theta) {
	ST7735_Line(
		TACH_CENTER_X+theta_to_vec_x(theta, TACH_RADIUS-10),
		TACH_CENTER_Y+theta_to_vec_y(theta, TACH_RADIUS-10),
		TACH_CENTER_X+theta_to_vec_x(theta, TACH_RADIUS-6),
		TACH_CENTER_Y+theta_to_vec_y(theta, TACH_RADIUS-6),
		TACH_TICK_COLOR
	);
}

static void draw_needle(uint32_t rpm) {
	uint16_t theta = rpm_to_theta(rpm);
	ST7735_Line(
		TACH_CENTER_X-theta_to_vec_x(theta, 8),
		TACH_CENTER_Y-theta_to_vec_y(theta, 8),
		TACH_CENTER_X+theta_to_vec_x(theta, NEEDLE_LENGTH),
		TACH_CENTER_Y+theta_to_vec_y(theta, NEEDLE_LENGTH),
		TACH_NEEDLE_COLOR
	);
}

// this is the one that handles what to redraw (as little as possible)
static void erase_needle(uint32_t rpm) {
	uint16_t theta = rpm_to_theta(rpm);
	ST7735_Line(
		TACH_CENTER_X-theta_to_vec_x(theta, 8),
		TACH_CENTER_Y-theta_to_vec_y(theta, 8),
		TACH_CENTER_X+theta_to_vec_x(theta, NEEDLE_LENGTH),
		TACH_CENTER_Y+theta_to_vec_y(theta, NEEDLE_LENGTH),
		TACH_FACE_COLOR
	);
	
	if (theta >= 240 && theta <= 250) {
		draw_single_tach_shiftzone(theta);
		draw_single_tach_shiftzone(theta+1);
		draw_single_tach_shiftzone(theta-1);
	} 
	else if (theta > 250 && theta <=260) { // special case, redraw tick at shift/redline boundary
		for (uint16_t i = 249; i < 255; i+=1) {
			ST7735_Line(
				TACH_CENTER_X+theta_to_vec_x(i, TACH_RADIUS-10),
				TACH_CENTER_Y+theta_to_vec_y(i, TACH_RADIUS-10),
				TACH_CENTER_X+theta_to_vec_x(i, TACH_RADIUS-7),
				TACH_CENTER_Y+theta_to_vec_y(i, TACH_RADIUS-7),
				TACH_SHIFTZONE_COLOR
			);
		}
		for (uint16_t i = 255; i < 261; i+=1) {
			ST7735_Line(
				TACH_CENTER_X+theta_to_vec_x(i, TACH_RADIUS-10),
				TACH_CENTER_Y+theta_to_vec_y(i, TACH_RADIUS-10),
				TACH_CENTER_X+theta_to_vec_x(i, TACH_RADIUS-7),
				TACH_CENTER_Y+theta_to_vec_y(i, TACH_RADIUS-7),
				TACH_REDLINE_COLOR
			);
		}
	} 
	else if (theta > 260 && theta < 299) {
		draw_single_tach_redline(theta);
		draw_single_tach_redline(theta+1);
		draw_single_tach_redline(theta-1);
	}

	ST7735_FillRect(TACH_CENTER_X+theta_to_vec_x(theta,TACH_RADIUS-TACH_RING_INSET),
					TACH_CENTER_Y+theta_to_vec_y(theta,TACH_RADIUS-TACH_RING_INSET), 
					2,2, TACH_TICK_COLOR);
	ST7735_FillRect(TACH_CENTER_X+theta_to_vec_x(theta-2,TACH_RADIUS-TACH_RING_INSET),
					TACH_CENTER_Y+theta_to_vec_y(theta-2,TACH_RADIUS-TACH_RING_INSET), 
					2,2, TACH_TICK_COLOR);
	ST7735_FillRect(TACH_CENTER_X+theta_to_vec_x(theta+2,TACH_RADIUS-TACH_RING_INSET),
					TACH_CENTER_Y+theta_to_vec_y(theta+2,TACH_RADIUS-TACH_RING_INSET), 
					2,2, TACH_TICK_COLOR);
	
	uint32_t mod = rpm%1000;
	uint32_t num = abs(mod<500?rpm/1000:(rpm/1000)+1);
	mod = (mod>499?1000-mod:mod);

	switch(num) {
		case 0:
		if (mod <= 233)
			draw_tach_num(num);
		break;

		case 1:
		case 2:
		if (mod <= 280)
			draw_tach_num(num);
		break;

		case 3:
		if (mod <= 256)
			draw_tach_num(num);
		break;

		case 4:
		if (mod <= 180)
			draw_tach_num(num);
		break;
		case 5:
		case 6:
		if (mod <= 233)
			draw_tach_num(num);
		break;
		case 7:
		if (mod <= 300)
			draw_tach_num(num);
		break;
		case 8:
		case 9:
		if (mod <= 256)
			draw_tach_num(num);
		break;
	}

	if (mod < 128) draw_tach_tick(rpm_to_theta(num*1000));
}

// public 
void draw_tach(uint32_t rpm) {
	draw_tach_bg();
	draw_tach_center();
	draw_needle(rpm);
	draw_tach_shiftzone();
	draw_tach_redline();
	draw_tach_ticks();
	draw_tach_bezel();
	update_rpm(rpm);
}

// public 
void update_rpm(uint32_t rpm) {
	static uint32_t p_rpm = 0;
	erase_needle(p_rpm);
	draw_tach_center();
	draw_needle(rpm);
	p_rpm = rpm;
}

// public
void update_gear(uint32_t g) {
	ST7735_DrawChar(64-3, 80+30, g+0x30, 0x001F, 0, 2);
}

// public 
void erase_tach() {
	// may not need this
}
