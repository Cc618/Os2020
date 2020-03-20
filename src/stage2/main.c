
void main()
{
    *((short*)0xB8000) = 0xF041;

    while (1);
}

