/* with thanks from doctea to Beermat on forum.pjrc.com -- alternate linker script that puts functions into FLASHMEM by default, to give us lots more RAM1 to play with (possibly at expense of speed?)! */
/* modified by doctea */

MEMORY
{
    ITCM (rwx):  ORIGIN = 0x00000000, LENGTH = 512K
    DTCM (rwx):  ORIGIN = 0x20000000, LENGTH = 512K
    RAM (rwx):   ORIGIN = 0x20200000, LENGTH = 512K
    FLASH (rwx): ORIGIN = 0x60000000, LENGTH = 7936K
    ERAM (rwx):  ORIGIN = 0x70000000, LENGTH = 16384K
}

ENTRY(ImageVectorTable)

SECTIONS
{
    .text.headers : {
        KEEP(*(.flashconfig))
        FILL(0xFF)
        . = ORIGIN(FLASH) + 0x1000;
        KEEP(*(.ivt))
        KEEP(*(.bootdata))
        . = ALIGN(1024);
    } > FLASH

    .text.code : {
        KEEP(*(.startup))
        *(.flashmem*)

        *(EXCLUDE_FILE (
            /* Have to add these to exclude in .text.code, put in text.itcm */
            eeprom.c.o
            EEPROM.c.o
            imxrt.c.o
            interrupt.c.o
            memcpy-armv7m.S.o
            memset.S.o
            ILI9341_t3n*
            uClock.cpp.o
            Bounce2.cpp.o

            behaviour*.cpp.o
            serial.cpp.o
            MIDI.cpp.o
            midi.cpp.o
            USB.cpp.o
            usb.cpp.o
			usb.c.o
            
            adk.cpp.o
            antplus.cpp.o
            bluetooth.cpp.o
            BluetoothConnection.cpp.o
            digitizer.cpp.o
            ehci.cpp.o
            enumeration.cpp.o
            hid.cpp.o
            hub.cpp.o
            joystick.cpp.o
            keyboard.cpp.o
            keyboardHIDExtras.cpp.o
            MassStorageDriver.cpp.o
            memory.cpp.o
            midi.cpp.o
            mouse.cpp.o
            print.cpp.o
            rawhid.cpp.o
            SerEMU.cpp.o
            serial.cpp.o

			ILI9341_t3n.cpp.o

			usb.c.o
			usb_audio.cpp.o
			usb_desc.c.o
			usb_flightsim.cpp.o
			usb_inst.cpp.o
			usb_joystick.c.o
			usb_keyboard.c.o
			usb_midi.c.o
			usb_mouse.c.o
			usb_mtp.c.o
			usb_rawhid.c.o
			usb_seremu.c.o
			usb_serial.c.o
			usb_serial2.c.o
			usb_serial3.c.o
			usb_touch.c.o

			Display*.c*.o
            
			extmem.c.o           
			DMAChannel.cpp.o
			sm*.c.o
			
			SPI.cpp.o
			display_abstract.cpp.o

        ).text*)

        . = ALIGN(4);
        KEEP(*(.init))
        __preinit_array_start = .;
        KEEP (*(.preinit_array))
        __preinit_array_end = .;
        __init_array_start = .;
        KEEP (*(.init_array))
        __init_array_end = .;
        . = ALIGN(4);
    } > FLASH

    .text.progmem : {
        *(.progmem*)
        . = ALIGN(4);
    } > FLASH

    .ARM.exidx : {
        __exidx_start = .;
        *(.ARM.exidx* .ARM.extab.text* .gnu.linkonce.armexidx.*)
        __exidx_end = .;
        . = ALIGN(4);
    } > FLASH

    .text.itcm : {
        . = . + 32; /* MPU to trap NULL pointer deref */
        *(.fastrun)
        /* Have to add these to keep in text.itcm (excluded above from text.code) */
        *(eeprom.c.o .text*)
        *(EEPROM.c.o .text*)
        *(imxrt.c.o .text*)
        *(interrupt.c.o .text*)
        *(memcpy-armv7m.S.o .text*)
        *(memset.S.o .text*)
		*(usb.c.o .text*)

        *(Display* .text*) /* this increases FPS from 9 to 13 */
        *(ILI9341_t3n.cpp.o .text*)
        *(uClock.cpp.o .text*)
        *(Bounce2.cpp.o .text*)
        *(behaviour*.cpp.o .text*)
        *(serial.cpp.o .text*)
        *(MIDI*.cpp.o .text*)
        *(midi*.cpp.o .text*)
        *(USB*.cpp.o .text*)
        *(usb*.cpp.o .text*)
        *(adk.cpp.o .text*)
        *(antplus.cpp.o .text*)
        *(bluetooth.cpp.o .text*)
        *(BluetoothConnection.cpp.o .text*)
        *(digitizer.cpp.o .text*)
        *(ehci.cpp.o .text*)
        *(enumeration.cpp.o .text*)
        *(hid.cpp.o .text*)
        *(hub.cpp.o .text*)
        *(joystick.cpp.o .text*)
        *(keyboard.cpp.o .text*)
        *(keyboardHIDExtras.cpp.o .text*)
        *(MassStorageDriver.cpp.o .text*)
        *(memory.cpp.o .text*)
        *(midi.cpp.o .text*)
        *(mouse.cpp.o .text*)
        *(print.cpp.o .text*)
        *(rawhid.cpp.o .text*)
        *(SerEMU.cpp.o .text*)
        *(serial.cpp.o .text*)
        
		*(usb_audio.cpp.o .text*)
		*(usb_desc.c.o .text*)
		*(usb_flightsim.cpp.o .text*)
		*(usb_inst.cpp.o .text*)
		*(usb_joystick.c.o .text*)
		*(usb_keyboard.c.o .text*)
		*(usb_midi.c.o .text*)
		*(usb_mouse.c.o .text*)
		*(usb_mtp.c.o .text*)
		*(usb_rawhid.c.o .text*)
		*(usb_seremu.c.o .text*)
		*(usb_serial.c.o .text*)
		*(usb_serial2.c.o .text*)
		*(usb_serial3.c.o .text*)
		*(usb_touch.c.o .text*)

		*(extmem.c.o .text*)
		*(DMAChannel.cpp.o .text*)
		*(sm*.c.o .text*)
		*(SPI.cpp.o .text*)
		*(display_abstract.cpp.o .text*)

        /* the rest of these made no noticeable difference */
        . = ALIGN(16);
    } > ITCM  AT> FLASH

    .data : {
        *(.endpoint_queue)
        *(SORT_BY_ALIGNMENT(SORT_BY_NAME(.rodata*)))
        *(SORT_BY_ALIGNMENT(SORT_BY_NAME(.data*)))
        KEEP(*(.vectorsram))
    } > DTCM  AT> FLASH

    .bss ALIGN(4) : {
        *(SORT_BY_ALIGNMENT(SORT_BY_NAME(.bss*)))
        *(SORT_BY_ALIGNMENT(SORT_BY_NAME(COMMON)))
        . = ALIGN(32);
        . = . + 32; /* MPU to trap stack overflow */
    } > DTCM

    .bss.dma (NOLOAD) : {
        *(.hab_log)
        *(.dmabuffers)
        . = ALIGN(32);
    } > RAM

    .bss.extram (NOLOAD) : {
        *(SORT_BY_ALIGNMENT(SORT_BY_NAME(.externalram)))
        . = ALIGN(32);
    } > ERAM

    .text.csf : {
        FILL(0xFF)
        . = ALIGN(1024);
        KEEP(*(.csf))
        __text_csf_end = .;
    } > FLASH

    _stext = ADDR(.text.itcm);
    _etext = ADDR(.text.itcm) + SIZEOF(.text.itcm);    
    _stextload = LOADADDR(.text.itcm);

    _sdata = ADDR(.data);
    _edata = ADDR(.data) + SIZEOF(.data);
    _sdataload = LOADADDR(.data);

    _sbss = ADDR(.bss);
    _ebss = ADDR(.bss) + SIZEOF(.bss);

    _dma_start = ADDR(.bss.dma);
    _dma_end = ADDR(.bss.dma) + SIZEOF(.bss.dma);

    _heap_start = ADDR(.bss.dma) + SIZEOF(.bss.dma);
    _heap_end = ORIGIN(RAM) + LENGTH(RAM);

    _extram_start = ADDR(.bss.extram);
    _extram_end = ADDR(.bss.extram) + SIZEOF(.bss.extram);

    _itcm_block_count = (SIZEOF(.text.itcm) + 0x7FFF) >> 15;
    _flexram_bank_config = 0xAAAAAAAA | ((1 << (_itcm_block_count * 2)) - 1);
    _estack = ORIGIN(DTCM) + ((16 - _itcm_block_count) << 15);

    _flashimagelen = __text_csf_end - ORIGIN(FLASH);
    _teensy_model_identifier = 0x25;

    .debug_info     0 : { *(.debug_info) }
    .debug_abbrev   0 : { *(.debug_abbrev) }
    .debug_line     0 : { *(.debug_line) }
    .debug_frame    0 : { *(.debug_frame) }
    .debug_str      0 : { *(.debug_str) }
    .debug_loc      0 : { *(.debug_loc) }
}