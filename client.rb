

require 'socket'

s = TCPSocket.new 'localhost', 5007

s.write("hello from ruby \n");

s.each_line do |line|
    puts line
end

s.close