#include "DriveSim.h"
#include <stdlib.h>
#include "./inc/random.h"
#include "./inc/tm4c123gh6pm.h"

#define abs(a) ((a)<0?-(a):(a))
#define max(a,b) ((a)<(b)?(b):(a))
#define min(a,b) ((a)<(b)?(a):(b))
#define clamp(a,l,h) (min(max((a),(l)),(h)))

// static void timer1A_countdown_init(void);
// static uint32_t timer1A_read(void);
// static void timer1A_countdown_stop(void);
#include "./inc/ST7735.h"

static int32_t rpm = 700;
static int32_t throttle = 0;
static int32_t gear = 1;
static int32_t power = 512;
static int32_t shift_penalties[3] = {2500, 2500, 2500};

// static void timer1A_countdown_init(void);
// static uint32_t timer1A_read(void);
// static void timer1A_countdown_stop(void);

typedef struct {
    int32_t* buffer;
    uint32_t l2size;
    uint32_t index;
} lowpass_t;

static lowpass_t throttle_lpf;
static lowpass_t rpm_lpf;

static lowpass_t lowpass_init(uint32_t l2s, int32_t init) {
    lowpass_t lp = {
        .buffer = 0,
        .l2size = l2s,
        .index = 0
    };
    lp.buffer = (int32_t*)malloc(sizeof(int32_t) * (1<<lp.l2size));
    for (uint32_t i = 0; i < (1<<lp.l2size); i++)
        lp.buffer[i] = init;
    return lp;
}

static void lowpass_free(lowpass_t *self) {
    free(self->buffer);
}

void drive_destroy() { // i guess we'll never need this
    lowpass_free(&throttle_lpf); lowpass_free(&rpm_lpf);
}

static int32_t lowpass_step(lowpass_t *self, int32_t val) {
    self->buffer[((self->index)++) & ((1<<self->l2size)-1)] = val;
    int32_t sum = 0;
    for (uint32_t i = 0; i < (1<<self->l2size); i++) {sum+=self->buffer[i];}
    return sum>>self->l2size;
}

// power bonus from rpm, 12 bit scale given r < 8k
static int32_t power_band(int32_t r) {
    int32_t p = (1144 + (((r>>2)*1280)>>10));
    if (r>6000&&r<6500) {
        p *= (1280); p >>= 10; // * 1.25, max 4095
    }
    int32_t over = r-6500;
    if (over>0)
        p -= over*2;
    return ((1024 + (p))*819)>>10;
}

#define INT_RATIO (768)
#define MOM_STRENGTH (100)
#define MAX_ERR (1024|512)

#include "./inc/Timer4A.h"
static uint32_t position = 0;
static uint32_t ms = 0;
static int32_t err = 0;

// sim rpm and power over n clock cycles
static void _drive_step(uint32_t n) {
    int32_t thlpf = lowpass_step(&throttle_lpf, throttle);

    int32_t maintain = rpm>>1;
    int32_t mom = thlpf;
    mom = ((INT_RATIO*mom)>>10) + (((1024-INT_RATIO)*throttle)>>10);
    err = mom - maintain;
    err = (((5-gear)*256*err)>>10);
    err = clamp(err, -MAX_ERR, MAX_ERR);

    rpm += ((MOM_STRENGTH*err)>>10);
    
    rpm = clamp(rpm, 700, 8000);
    rpm = lowpass_step(&rpm_lpf, rpm);
    uint32_t lowrpm = rpm < 750;
    if (lowrpm) rpm += ((int32_t)Random32())>>27;

    switch (gear) {
        case 1:
            power = (((((gear)<<10)*1536)>>10) + power_band(rpm))>>1;
            break;
        case 2:
            power = (((((gear)<<10)*1280)>>10) + power_band(rpm))>>1;
            break;
        case 3:
            power = (((((gear)<<10)*1126)>>10) + power_band(rpm))>>1;
            break;
        case 4:
            power = (((gear)<<10) + power_band(rpm))>>1;
            break;
    }

    if (lowrpm && gear==1 && !(throttle & ~(128-1)))
        power = 0;

}

void drive_sim_dbgout() {
    ST7735_SetCursor(0,0);
    ST7735_OutChar('e');
    ST7735_SetCursor(8,0);
    ST7735_OutChar('p');
    ST7735_SetCursor(14,0);
    ST7735_OutChar('t');

    ST7735_SetCursor(2,0);
    ST7735_OutString("     ");
    ST7735_SetCursor(2,0);
    if (err<0) {ST7735_OutChar('-'); err = -err;}
    ST7735_OutUDec(err);
    
    ST7735_SetCursor(9,0);
    ST7735_OutString("     ");
    ST7735_SetCursor(9,0);
    ST7735_OutUDec(power);

    ST7735_SetCursor(15,0);
    ST7735_OutString("   ");
    char str[6] = {((ms/10000)%10)+0x30, ((ms/1000)%10)+0x30, '.', ((ms/100)%10)+0x30, ((ms/10)%10)+0x30, 0};
    ST7735_SetCursor(15,0);
    ST7735_OutString(str);

    position += power>>4;

    static int16_t p_x = 0;
    int16_t x = position >> (16-7);
    if (p_x < x)
        ST7735_DrawFastVLine(p_x, 150, 10, 0xF100);

    if (position > (1<<16)) {
        Timer4A_Stop();
    }
    p_x = x;
}

#define PF1                     (*((volatile uint32_t *)0x40025008))
#define PF2                     (*((volatile uint32_t *)0x40025010))
#define PF3                     (*((volatile uint32_t *)0x40025020))

static void tstep() {
    PF2 |= 0x04;
    if (ms) ms++;
    if (ms==0 && throttle>>5) ms++;
}

// public
void drive_step() {
    // static uint32_t p_clk = ~0;
    // uint32_t clk = timer1A_read();
    // uint32_t dt = p_clk - clk;
    // p_clk = clk;
    _drive_step(80000000/30);
}

// public
void drive_init() {
    // timer1A_countdown_init();
    Random_Init(0xF3339999);
    throttle_lpf = lowpass_init(5, 0);
    rpm_lpf = lowpass_init(1, 700);
    //Timer4A_Init(tstep, 80000000/1000, 2);
}
// public
void drive_throttle(uint32_t input) {
    throttle = input;
}
// public
void drive_upshift() {
    if (gear < 4) {
        int32_t pen = (rpm>5999&&rpm<6501)?2500:((rpm>6500)?4000:3200);
        if ((((int32_t)rpm)-pen)<700) return;
        gear++;
        shift_penalties[gear-2] = pen;
        rpm -= shift_penalties[gear-2];
        rpm_lpf.buffer[~(rpm_lpf.index&0x01)] = rpm;
    }
}
// public
void drive_downshift() {
    if (gear > 1) {
        gear--;
        rpm += shift_penalties[gear-1];
        rpm_lpf.buffer[~(rpm_lpf.index&0x01)] = rpm;
    }
}
// public
uint32_t drive_rpm() {
    return rpm;
}
// public
uint32_t drive_gear() {
    return gear;
}
// public
uint32_t drive_power() {
    return power;
}

// static void timer1A_countdown_init(void){
//   SYSCTL_RCGCTIMER_R |= 0x02;   // 0) activate TIMER1
//   TIMER1_CTL_R = 0x00000000;    // 1) disable TIMER1A during setup
//   TIMER1_CFG_R = 0x00000000;    // 2) configure for 32-bit mode
//   TIMER1_TAMR_R = 0x00000002;   // 3) configure for periodic mode, default down-count settings
//   TIMER1_TAILR_R = 0xFFFFFFFF;  // 4) reload value
//   TIMER1_TAPR_R = 0;            // 5) bus clock resolution
//   TIMER1_ICR_R = 0x00000001;    // 6) clear TIMER1A timeout flag
//   TIMER1_IMR_R = 0x00000000;    // 7) no timeout interrupt
//   NVIC_DIS0_R = 1<<21;          // 9) disable IRQ 21 in NVIC
//   TIMER1_CTL_R = 0x00000001;    // 10) enable TIMER1A
// }

// static uint32_t timer1A_read(void) {
//     return TIMER1_TAR_R;
// }

// static void timer1A_countdown_stop(void){
//   TIMER1_CTL_R = 0x00000000;  // 10) disable timer1A
// }

