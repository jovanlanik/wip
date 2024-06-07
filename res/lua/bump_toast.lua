local dungeon = require('dungeon')
local act = function(self_id)
	if(math.random(10) == 1)
	then
		dungeon.sendToast('Hello Lua!')
	end
end
local action = function(self_id)
	dungeon.state.entity[self_id].type = 'ENT_SNAKE'
	dungeon.state.entity[self_id].id = 1
	return true
end
return { 'book', act, action }
