const char* getSystemLuaScript() {
	return R"LUASCRIPT(

function getConfig(x)
	return x
end

primeTimer = Timer.new()
primeTimer:reset();

function pumpLogic()
	local prime = primeTimer:getElapsedSeconds() > getConfig(0)
	local spinning = getTimeSinceTrigger() < 1

	return prime or spinning
end

	)LUASCRIPT";
}