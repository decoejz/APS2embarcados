 typedef struct {
	 const uint8_t *data;
	 uint16_t width;
	 uint16_t height;
	 uint8_t dataSize;
 } tImage;
 
  typedef struct {
	  long int code;
	  const tImage *image;
  } tChar;
  
  typedef struct {
	  int length;
	  const tChar *chars;
	  char start_char;
	  char end_char;
  } tFont;
 
#include "icones/diario.h"
#include "icones/right_arrow.h"
#include "icones/left_arrow.h"
#include "icones/unlock.h"
#include "icones/lock.h"
#include "icones/pesado.h"
#include "icones/rapido.h"
#include "icones/play_pause.h"
#include "icones/home.h"
#include "icones/OK.h"
#include "fontes/sourcecodepro_28.h"
#include "fontes/calibri_36.h"
#include "fontes/arial_72.h"
