sk=net.createConnection(net.TCP, 0)

uart.setup(0, 115200, 8, uart.PARITY_NONE, uart.STOPBITS_1, 1)

uart.on("data", ".",
  function(data)
    if string.find(data, "quit") then
      tmr.unregister(0)
      uart.on("data"," ",function()end,1) -- unregister callback function
    else
         sk:connect(2000,"192.168.1.40")
            sk:on("connection", function(sck,c)
                -- Wait for connection before sending.
                print("connected")
                sk:send(data:sub(1,-2))
         end)
    
         sk:on("sent", function(sck)
                print("close")
                sk:close()
                sk=net.createConnection(net.TCP, 0)
         end)
     end
end, 0)
