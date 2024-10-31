require 'lightfork'

LightFork.set_mem_file_name("light_fork_ruby.mem")

pid = Process.fork

puts "Pid is #{Process.pid}, child pid is #{pid}."