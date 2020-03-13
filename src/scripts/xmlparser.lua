local parsexml = require "common".ParseXml

XmlParser = XmlParser or {}

function XmlParser.Parse(path)
	local t, error = parsexml(path)
	if nil == t then
		return nil, error
	end

	return t
end
