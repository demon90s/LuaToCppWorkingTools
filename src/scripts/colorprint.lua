local colorprint = require "common".ColorPrintf

ColorPrint = ColorPrint or {}

function ColorPrint.Error(msg, ...)
	local out = string.format(msg, ...)
	colorprint(0, out)
end

function ColorPrint.Log(msg, ...)
	local out = string.format(msg, ...)
	colorprint(1, out)
end

function ColorPrint.Warn(msg, ...)
	local out = string.format(msg, ...)
	colorprint(2, out)
end

function ColorPrint.Special(msg, ...)
	local out = string.format(msg, ...)
	colorprint(3, out)
end