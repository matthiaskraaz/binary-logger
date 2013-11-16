# -*- coding: utf-8 -*-
require 'yaml'

#Main routine of script
def main
  #TODO: open in binary mode for Windows
  map_file = File.open(ARGV[0])
  map = load_map_file(map_file)
  log_file = File.new(ARGV[1])
  translate_log_file(map, log_file)
end

#Loads map from file.
#Returns loaded map data structure.
def load_map_file map_file
  return YAML.load(map_file.read)
end

#Translates the messages in log_file to human readable format using
#the format strings from the specified map and prints them to stdout.
#
#TODO: handle not found and async between data and format
#TODO: handle incomplete and corrupted messages, including check of checksum
def translate_log_file map, log_file
  state = :rs
  file_id = nil
  file = nil
  line = nil
  lineno = nil
  parameters_length = nil
  parameter_bytes = nil

  log_file.each_byte do |b|
    case state
    when :rs
      parameter_bytes = []
      state = :file_id_lo
    when :file_id_lo
      file_id = b
      state = :file_id_hi
    when :file_id_hi
      file_id += b * 256
      file = map[file_id]
      if file
        state = :lineno_lo
      else
        $stderr.puts "File ID #{file_id} not found in map"
        state = :lingering
      end
    when :lineno_lo
      lineno = b
      state = :lineno_hi
    when :lineno_hi
      lineno += b * 256
      line = file[:lines][lineno]
      state = :parameters_length
    when :parameters_length
      parameters_length = b % 128
      if parameters_length > 0
        state = :parameters
      else
        state = :checksum
      end
    when :parameters
      parameter_bytes << b
      parameters_length -= 1
      if parameters_length == 0
        state = :checksum
      end
    when :checksum
      if line
        parameters = split_parameter_bytes file, line, parameter_bytes
        message = format_log_message(file, lineno, line, parameters)
        puts message
      else
        $stderr.puts "#{file[:file_name]}@#{lineno}: Line not found in map"
      end
      state = :rs
    end
  end
end

#Splits parameters bytes into chunks according to information from map file.
def split_parameter_bytes file, line, parameter_bytes
  parameters = []
  line[:args].each do |arg|
    case arg
    when 'LOG_I8'
      parameters << shift_sint_bytes(parameter_bytes, 1)
    when 'LOG_UI8'
      parameters << shift_uint_bytes(parameter_bytes, 1)
    when 'LOG_I16'
      parameters << shift_sint_bytes(parameter_bytes, 2)
    when 'LOG_UI16'
      parameters << shift_uint_bytes(parameter_bytes, 2)
    when 'LOG_I32'
      parameters << shift_sint_bytes(parameter_bytes, 4)
    when 'LOG_UI32'
      parameters << shift_uint_bytes(parameter_bytes, 4)
    when 'LOG_I64'
      parameters << shift_sint_bytes(parameter_bytes, 8)
    when 'LOG_UI64'
      parameters << shift_uint_bytes(parameter_bytes, 8)
    when 'LOG_S'
      parameters << shift_string_bytes(parameter_bytes)
    else
      $stder.puts "#{file[:file_name]}@#{lineno}: #{line[:format]} - unknown argument type #{arg}"
      break
    end
  end
  return parameters
end

#Shifts bytes from parameter_bytes until a null byte is found.
#Returns the removed bytes converted to a string.
def shift_string_bytes parameter_bytes
  parameter = []
  while parameter_bytes.length > 0
    b = parameter_bytes.shift
    if b == 0
      break
    end
    parameter << b
  end
  return parameter.pack('c*')
end

#Shifts count bytes from parameter_bytes.
#Returns the removed bytes converted to a signed little endian number.
def shift_sint_bytes parameter_bytes, count
  parameter = shift_uint_bytes(parameter_bytes, count)
  if parameter >= 2**(count * 8 - 1)
    parameter = parameter - 2**(count * 8)
  end
  return parameter
end

#Shifts count bytes from parameter_bytes.
#Returns the removed bytes converted to an unsigned little endian number.
def shift_uint_bytes parameter_bytes, count
  parameter = 0
  value = 1
  while parameter_bytes.length > 0 && count > 0
    parameter += parameter_bytes.shift * value
    value *= 0x100
    count -= 1
  end
  return parameter
end

#Formats a single log message and returns the result.
def format_log_message file, lineno, line, parameters
  message = insert_parameters(line, parameters)
  return "#{file[:file_name]}@#{lineno}: #{message}"
end

#Inserts parameters into format string from map file
#and returns the result.
def insert_parameters line, parameters
  return line[:format].gsub(/{.*?}/) do |match| 
    #TODO: lots of error handling
    if /{(?<index>[0-9]*):(?<format>.*)}/ =~ match
      index = index.to_i
      parameter = parameters[index]
      "%#{format}" % parameter
    elsif /{(?<index>[0-9]*)}/ =~ match
      index = index.to_i
      parameter = parameters[index]
      parameter.to_s
    else
      match
    end
  end
end

if ($0 == __FILE__)
  main
end
