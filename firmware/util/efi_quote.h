// efi_quote.h

#pragma once

#define Q(x) #x
#define QUOTE(x) Q(x)

#define C(x, y) x ## y
#define CONCATENATE(x, y) C(x, y)
