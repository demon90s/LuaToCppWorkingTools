local CCommon = require "common"
local Object = require "object"

local script_dir = arg[0]

dofile(script_dir .. "/colorprint.lua")
dofile(script_dir .. "/xmlparser.lua")

function PrintTable(o, level)
	level = level or 1
	local t = type(o)
    if t == "number" then
    	io.write(string.format("%g", o))
    elseif
        t == "string" or 
        t == "boolean" or 
        t == "nil" then
        io.write(string.format("%q", o))    -- lua5.3 %q可用于数值,nil,boolean,并且能够保存浮点数的精度
    elseif t == "table" then
        io.write("{\n")
        for k,v in pairs(o) do
        	if type(k) == "number" then
        		k = string.format("[\"%q\"]", k)
        	end
            io.write(string.rep("    ", level), k, " = ")
            PrintTable(v, level + 1)
            io.write(",\n")
        end
        io.write(string.rep("    ", level - 1), "}")
    end
end

function main()
    ColorPrint.Error("This is an Error")
    ColorPrint.Warn("This is a Warning")
    ColorPrint.Log("This is a Log")
    ColorPrint.Special("This is Special")

    local t, error = XmlParser.Parse("??")
    assert(t == nil)

    t = XmlParser.Parse("test.xml")
    assert(t)
    
    --PrintTable(t)
    --io.write("\n")
    assert(t.value == 42)
    assert(t.Person.name == "diwen")
    assert(t.number.power[1] == 100)
    assert(t.id.data[2].date == 2000)

    local obj = Object.New()
    assert(obj:GetValue() == 0)
    obj:SetValue(42)
    assert(obj:GetValue() == 42)

    obj:Free()
end

main()

print "ALL TEST PASS"
