cfg={}
cfg.ssid="MadRemote";
cfg.pwd="yanouriou1992"
wifi.ap.config(cfg)

cfg={}
cfg.ip="192.168.3.1";
cfg.netmask="255.255.255.0";
cfg.gateway="192.168.6.1";
wifi.ap.setip(cfg);
wifi.setmode(wifi.SOFTAP)

srv=net.createServer(net.TCP)
srv:listen(5302,function(conn)
    conn:on("receive", function(client,request)
   
    print(request)
    conn:send("<h1> Hello, NodeMCU!!! </h1>")
  end)
  conn:on("sent",function(conn) conn:close() end)
end)
        --local buf = "";
        --buf = buf.."HTTP/1.1 200 OK\n\n"
        --local _, _, method, path, vars = string.find(request, "([A-Z]+) (.+)?(.+) HTTP");
        --if(method == nil)then
        --    _, _, method, path = string.find(request, "([A-Z]+) (.+) HTTP");
        --end
        --local _GET = {}
        --if (vars ~= nil)then
        --    for k, v in string.gmatch(vars, "(%w+)=(%w+)&*") do
        --        _GET[k] = v
        --    end
        --end
    
        --if(_GET.pin == "ON1")then
        --      gpio.write(led1, gpio.HIGH);
        --elseif(_GET.pin == "OFF1")then
        --      gpio.write(led1, gpio.LOW);
        --elseif(_GET.pin == "ON2")then
        --     gpio.write(led2, gpio.HIGH);
        --elseif(_GET.pin == "OFF2")then
        --      gpio.write(led2, gpio.LOW);
        --end
        --client:send(buf);
--        client:close();
--        collectgarbage();
--    end)
--end)
