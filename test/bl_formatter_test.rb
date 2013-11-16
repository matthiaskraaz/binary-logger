load "scripts/bl_formatter.rb"
require "test/unit"
 
class TestFormatter < Test::Unit::TestCase
 
  def test_shift_sint_bytes
    assert_equal( 0, shift_sint_bytes( [0], 1 ) )
    assert_equal( 127, shift_sint_bytes( [127], 1 ) )
    assert_equal( -128, shift_sint_bytes( [128], 1 ) )
    assert_equal( -1, shift_sint_bytes( [255], 1 ) )

    assert_equal( 0, shift_sint_bytes( [0,0], 2 ) )
    assert_equal( 32767, shift_sint_bytes( [255,127], 2 ) )
    assert_equal( -32768, shift_sint_bytes( [0,128], 2 ) )
    assert_equal( -1, shift_sint_bytes( [255,255], 2 ) )

    assert_equal( 0, shift_sint_bytes( [0,0], 4 ) )
    assert_equal( 2147483647, shift_sint_bytes( [255,255,255,127], 4 ) )
    assert_equal( -2147483648, shift_sint_bytes( [0,0,0,128], 4 ) )
    assert_equal( -1, shift_sint_bytes( [255,255,255,255], 4 ) )

    assert_equal( 0, shift_sint_bytes( [0,0,0,0,0,0,0,0], 8 ) )
    assert_equal( 9223372036854775807, shift_sint_bytes( [255,255,255,255,255,255,255,127], 8 ) )
    assert_equal( -9223372036854775808, shift_sint_bytes( [0,0,0,0,0,0,0,128], 8 ) )
    assert_equal( -1, shift_sint_bytes( [255,255,255,255,255,255,255,255], 8 ) )
  end

  def test_shift_uint_bytes
    assert_equal( 0, shift_uint_bytes( [0], 1 ) )
    assert_equal( 255, shift_uint_bytes( [255], 1 ) )

    assert_equal( 0, shift_uint_bytes( [0,0], 2 ) )
    assert_equal( 65535, shift_uint_bytes( [255,255], 2 ) )

    assert_equal( 0, shift_uint_bytes( [0,0], 4 ) )
    assert_equal( 4294967295, shift_uint_bytes( [255,255,255,255], 4 ) )

    assert_equal( 0, shift_uint_bytes( [0,0,0,0,0,0,0,0], 8 ) )
    assert_equal( 18446744073709551615, shift_uint_bytes( [255,255,255,255,255,255,255,255], 8 ) )
  end
 
end

