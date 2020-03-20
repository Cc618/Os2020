/* Entry point */
/* TODO : Boot / Real / Protected */
kernel_entry = 1M;
sector_size = 512;

OUTPUT_FORMAT("elf32-i386")
OUTPUT_ARCH(i386)
ENTRY(entry)

SECTIONS
{
	.text kernel_entry :
	{
		*(.text)
	}

	.rodata :
	{
		*(.rodata*)
	}
	
	/*
		obj/boot/*(.rodata*)
		obj/kernel/*(.rodata*)
	*/

	.bss :
	{
		*(.bss)
	}

	.data :
	{
		*(.data)
	}


	/* --- Real Mode (boot) --- */

	/*
	.text real_entry :
	{
		*(.text)
	}

	.rodata :
	{
		*(.rodata*)
	}

	.bss :
	{
		*(.bss)
	}

	.data :
	{
		*(.data)
	}
	*/

	/* --- Protected Mode (kernel) --- */

	/* --- End Of Kernel (magic number) --- */
	/*
	. = ALIGN(sector_size);

	.custom : {
		*(.custom)
	}

	. = ALIGN(sector_size);
	*/
}
