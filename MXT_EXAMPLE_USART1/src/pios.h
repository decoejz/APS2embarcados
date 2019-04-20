#define LED1_PIO          PIOC
#define LED1_PIO_ID       ID_PIOC
#define LED1_PIO_IDX      30u 
#define LED1_PIO_IDX_MASK (1u << LED1_PIO_IDX)

#define DOOR_PIO           PIOD
#define DOOR_PIO_ID        ID_PIOD
#define DOOR_PIO_IDX       28u
#define DOOR_PIO_IDX_MASK  (1u << DOOR_PIO_IDX)

#define UNLOCK_PIO           PIOB
#define UNLOCK_PIO_ID        ID_PIOB
#define UNLOCK_PIO_IDX       1u
#define UNLOCK_PIO_IDX_MASK  (1u << UNLOCK_PIO_IDX)

void init_led(void);
void init_but(void);
void door_callback();
void draw_door_open();
void draw_locked_door();