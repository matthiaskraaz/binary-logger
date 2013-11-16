#Creates a map of all logging messages in the source code.
#Usage: find -name "*.c" | ruby scripts/bl_map.rb > bl_map.yaml or OS dependent equivalent
require 'yaml'

files = {}
files[0] = { file_name: "0", lines: [{ format: "Program version '{0}'", args: ["LOG_S"] }] }

STDIN.each_line do |file_name|
  file_name.chomp!
  begin
    contents = File.open(file_name) { |f| f.readlines }	
  rescue
    $stderr.puts "Can't read '#{file_name}'"
    raise
  end

  file_id = nil
  lines = nil
  lineno = nil
  contents.each_with_index do |line,index|
    if file_id
      if line =~ /\bLOG\b/
        lines = line
      else
        if lines
          lines += line
        end
      end
      if lines && /\bLOG\b.*?"(?<format>.*?)"(?<tail>.*?)\;/m =~ lines
        lineno = index + 1
        args = tail.scan(/\bLOG_.*?\b/)
        #TODO: Check that format string and number of args matches
        files[file_id][:lines][lineno] = { format: format, args: args }
        lines = nil			
      end
    else
      if line =~ /\bLOG_FILE_ID\s*\(\s*([0-9]+)/
        file_id = $1.to_i #TODO: handle invalid numbers
        #TODO: Check that file ids are unique
        files[file_id] = { file_name: file_name, lines: [] }
      end
    end
  end
end

puts YAML.dump( files )
