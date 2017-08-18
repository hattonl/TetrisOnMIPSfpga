#ifndef MFP_MEMORY_MAPPED_REGISTERS_H
#define MFP_MEMORY_MAPPED_REGISTERS_H

#define MFP_LEDS_ADDR           0xBF800000
#define MFP_SWITCHES_ADDR       0xBF800004
#define MFP_BUTTONS_ADDR        0xBF800008
#define MFP_SEGS_ADDR			0xBF80000C

#define MFP_VGA1_ADDR			0xBF800010
#define MFP_VGA2_ADDR			0xBF800014
#define MFP_VGA3_ADDR			0xBF800018
#define MFP_VGA4_ADDR			0xBF80001C
#define MFP_VGA5_ADDR			0xBF800020
#define MFP_VGA6_ADDR			0xBF800024
#define MFP_VGA7_ADDR			0xBF800028

#define MFP_TIME_ADDR           0XBF80002C


#define MFP_LEDS                (* (volatile unsigned *) MFP_LEDS_ADDR      )
#define MFP_SWITCHES            (* (volatile unsigned *) MFP_SWITCHES_ADDR      )
#define MFP_BUTTONS             (* (volatile unsigned *) MFP_BUTTONS_ADDR       )
#define MFP_SEGS				(* (volatile unsigned *) MFP_SEGS_ADDR		)

#define MFP_VGA1				(* (volatile unsigned *) MFP_VGA1_ADDR		)
#define MFP_VGA2				(* (volatile unsigned *) MFP_VGA2_ADDR		)
#define MFP_VGA3				(* (volatile unsigned *) MFP_VGA3_ADDR		)
#define MFP_VGA4				(* (volatile unsigned *) MFP_VGA4_ADDR		)
#define MFP_VGA5				(* (volatile unsigned *) MFP_VGA5_ADDR		)
#define MFP_VGA6				(* (volatile unsigned *) MFP_VGA6_ADDR		)
#define MFP_VGA7				(* (volatile unsigned *) MFP_VGA7_ADDR		)
#define MFP_TIME                (* (volatile unsigned *) MFP_TIME_ADDR      )

// This define is used in boot.S code

#define BOARD_16_LEDS_ADDR      MFP_LEDS_ADDR

#endif
