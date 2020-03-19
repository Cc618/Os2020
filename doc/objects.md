# Objects

This file provides details about 'objects' within the kernel.

## Details

An object is a struct and is always dynamically allocated.

The methods are defined like this : 
```
// Standard method
ObjectName_methodName(ObjectName*, ...)

// Constructor
ObjectName_new(...)

// Destructor
ObjectName_del(ObjectName*)
```

## Vector

This collection gathers linearly a sequence of data (stored as pointers).
All items are freed when removed.
