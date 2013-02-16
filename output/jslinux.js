/* 
   Linux launcher

   Copyright (c) 2011 Fabrice Bellard

   Redistribution or commercial use is prohibited without the author's
   permission.

   Mods for avrjslinux are (C) 2013 Charles Lohr.
   I requested permission a while ago and got it.
   ...more or less, I just bolted some junk on the
   side of Fabrice Bellard's ridiculous thing.
*/
"use strict";

var term, pc, boot_start_time;

function term_start()
{
    term = new Term(80, 30, term_handler);

    term.open();
}

/* send chars to the serial port */
function term_handler(str)
{
    pc.serial.send_chars(str);
}

function clipboard_set(val)
{
    var el;
    el = document.getElementById("text_clipboard");
    el.value = val;
}

function clipboard_get()
{
    var el;
    el = document.getElementById("text_clipboard");
    return el.value;
}

function clear_clipboard()
{
    var el;
    el = document.getElementById("text_clipboard");
    el.value = "";
}

/* just used to display the boot time in the VM */
function get_boot_time()
{
    return (+new Date()) - boot_start_time;
}

function start()
{
    var start_addr, initrd_size, params, cmdline_addr;
    
    params = new Object();

    /* serial output chars */
    params.serial_write = term.write.bind(term);

//XXX CNLOHR
//    params.com2_write = cnlohrdatain;

    /* memory size (in bytes) */
    params.mem_size = 32 * 1024 * 1024;

    /* clipboard I/O */
    params.clipboard_get = clipboard_get;
    params.clipboard_set = clipboard_set;

    params.get_boot_time = get_boot_time;

    pc = new PCEmulator(params);

//Map the AVR's IO ports.
	pc.register_ioport_write( 0x400, 0xff, 1, ioport_write8 );
	pc.register_ioport_read( 0x400, 0xff, 1, ioport_read8 );

	pc.register_ioport_write( 0x400, 0xff, 2, ioport_write16 );
	pc.register_ioport_read( 0x400, 0xff, 2, ioport_read16 );

//    pc.load_binary("vmlinux-3.7.8.bin", 0x00100000);
//    initrd_size = pc.load_binary("root.bin", 0x00400000);

//    pc.load_binary("vmlinux-2.6.34.14.bin", 0x00100000);
//    initrd_size = pc.load_binary("root.bin", 0x00400000);

    pc.load_binary("vmlinux-2.6.20.bin", 0x00100000);
    initrd_size = pc.load_binary("root.bin", 0x00400000);

    start_addr = 0x10000;
    pc.load_binary("linuxstart.bin", start_addr);


    /* set the Linux kernel command line */
    /* Note: we don't use initramfs because it is not possible to
       disable gzip decompression in this case, which would be too
       slow. */
    cmdline_addr = 0xf800;
    pc.cpu.write_string(cmdline_addr, "console=ttyS0 root=/dev/ram0 rw init=/sbin/init notsc=1");

    pc.cpu.eip = start_addr;
    pc.cpu.regs[0] = params.mem_size; /* eax */
    pc.cpu.regs[3] = initrd_size; /* ebx */
    pc.cpu.regs[1] = cmdline_addr; /* ecx */

    boot_start_time = (+new Date());

    pc.start();
}

term_start();

