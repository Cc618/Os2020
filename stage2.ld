/* Entry point physical address */
offset = 0x8000;
sector_size = 512;

OUTPUT_FORMAT("elf32-i386")
OUTPUT_ARCH(i386)
ENTRY(entry)

SECTIONS
{
	/* --- Entry --- */
	.entry offset :
	{
		*(.entry)
	}

	/* --- Stage 2 --- */
	.text :
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

	/* --- End Of Stage 2 (magic number) --- */
	. = ALIGN(sector_size);

	.end :
	{
		*(.end)
	}

	. = ALIGN(sector_size);
}
