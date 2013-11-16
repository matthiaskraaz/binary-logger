#include "binary_logger.h"

LOG_FILE_ID( 123 );

extern void test2(void);

int main(void)
{
  LOG_VERSION("12345");

  LOG("Message without parameters");
  
  LOG("Three parameters: {0},{1},{2}", LOG_I8(11),  LOG_I8(12), LOG_I8(13));
  
  LOG( "Hexadecimal number: 0x{2:X}, decimal number: {1}, and some text: {0}", 
       LOG_S( "hi" ), 
       LOG_I16( 123 ),
       LOG_UI32( 0xabcdabcd ) );
  
  LOG( "i8: {0} {1} {2} {3}", LOG_I8( 0 ), LOG_I8( 127 ), LOG_I8( -1 ), LOG_I8( -128 ) );
  LOG( "i16: {0} {1} {2} {3}", LOG_I16( 0 ), LOG_I16( 32767 ), LOG_I16( -1 ), LOG_I16( -32768 ) );
  LOG( "i32: {0} {1} {2} {3}", LOG_I32( 0 ), LOG_I32( 2147483647L ), LOG_I32( -1 ), LOG_I32( -2147483648L ) );
  LOG( "i64: {0} {1} {2} {3}", LOG_I64( 0 ), LOG_I64( 9223372036854775807LL ), LOG_I64( -1 ), LOG_I64( -9223372036854775808LL ) );

  LOG( "ui8: {0} {1}", LOG_UI8( 0 ), LOG_UI8( 255 ) );
  LOG( "ui16: {0} {1}", LOG_UI16( 0 ), LOG_UI16( 65535 ) );
  LOG( "ui32: {0} {1}", LOG_UI32( 0 ), LOG_UI32( 4294967295UL ) );
  LOG( "ui64: {0} {1}", LOG_UI64( 0 ), LOG_UI64( 18446744073709551615ULL ) );
  
  test2( );
  
  return 0;
}
