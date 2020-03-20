# Some conventions

## 16 bits calling convention

When booting, the OS is in 16 bits, here is the calling convention :

| Register | Function                      |
| -------- | ----------------------------- |
| ax       | Return value, can be modified |
| bx       | First arg, can be modified    |
| cx       | Second arg, can be modified   |
| si       | Can be modified               |

Other registers must be preserved.
