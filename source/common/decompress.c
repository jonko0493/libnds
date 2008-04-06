/*---------------------------------------------------------------------------------

Copyright (C) 2005
Jason Rogers (dovoto)

This software is provided 'as-is', without any express or implied
warranty.  In no event will the authors be held liable for any
damages arising from the use of this software.

Permission is granted to anyone to use this software for any
purpose, including commercial applications, and to alter it and
redistribute it freely, subject to the following restrictions:

1.	The origin of this software must not be misrepresented; you
must not claim that you wrote the original software. If you use
this software in a product, an acknowledgment in the product
documentation would be appreciated but is not required.

2.	Altered source versions must be plainly marked as such, and
must not be misrepresented as being the original software.

3.	This notice may not be removed or altered from any source
distribution.


---------------------------------------------------------------------------------*/

#include <nds/decompress.h>
#include <nds/bios.h>

#ifdef ARM9
#include <nds/arm9/sassert.h>   
#endif

int getSize(uint8 *source, uint16 *dest, uint32 arg) {
   return *(uint32*)source;
}

uint8 readByte(uint8 *source) {
   return *source;
}
TDecompressionStream decomStream = {
   getSize,
   0,
   readByte
};

void decompress(const void* dst, const void* data, DecompressType type)
{

   switch(type)
   {
   case LZ77Vram:
      swiDecompressLZSSVram((void*)data, (void*)dst, 0, &decomStream);
      break;
   case LZ77:
      swiDecompressLZSSWram((void*)data, (void*)dst);
      break;
   case HUFF:
      swiDecompressHuffman((void*)data, (void*)dst, 0, &decomStream);
      break;
   case RLE:
      swiDecompressRLEWram((void*)data, (void*)dst);
      break;
   case RLEVram:
      swiDecompressRLEVram((void*)data, (void*)dst, 0, &decomStream);
      break;
   default:
      break;
   }
}
void decompressStream(const void* dst, const void* data, DecompressType type, getByteCallback callback)
{
#ifdef ARM9
   sassert(type != LZ77 && type != RLE, "Only LZ77 and RLE do not support streaming, use Vram versions");
#endif

   decomStream.readByte= callback;

   switch(type)
   {
   case LZ77Vram:
      swiDecompressLZSSVram((void*)data, (void*)dst, 0, &decomStream);
      break;
   case HUFF:
      swiDecompressHuffman((void*)data, (void*)dst, 0, &decomStream);
      break;
   case RLEVram:
      swiDecompressRLEVram((void*)data, (void*)dst, 0, &decomStream);
      break;
   default:
      break;
   }
}
