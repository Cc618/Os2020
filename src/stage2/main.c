
const char *hw = "Hello world !!!";

void main()
{
    // TODO : Change
    // Fill screen
    for (int i = 0xB8000; i < 0xB8000 + 80 * 25 * 2; i += 2)
		*((short*)i) = (int)0x0F << 8;

    const char *s = hw;
    for (short *c = (short*)0xB8000; *s != '\0'; ++c)
    {
        *c = *s | (0x9F << 8);
        ++s;
    }

    while (1);
}

