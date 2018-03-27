dofile("Scripts/entities.lua")
scenes = 
{
	main = 
	{
		name = "Main",
		object_list = entities
	},
	menu = 
	{
		name = "Main Menu",
		object_list = {}
	}
}

entities = nil

function test()
	-- local tc = scenes.main.objects.beast:get_transform_comp()
	-- print(scenes.main.objects.beast.name)
	-- scenes.main.objects.beast.name = "furry"
	-- print(scenes.main.objects.beast.name)
	-- print(tc.relative_transform)
	local beast = scenes.main.objects.beast
	local comp = beast:new_component()
	comp.tick = function(dt)
		local tc = beast:get_transform_comp()
		local rt = tc.relative_transform
		local t = Transform.new(Vector.new(rt.position.x + (5 * dt), rt.position.y + (5 * dt)), rt.rotation + (25 * dt), rt.scale)
		tc.relative_transform = t
		-- print(dt)
		-- local tc = beast:get_transform_comp()
		-- local t = Transform.new(tc.relative_transform.position, tc.relative_transform.rotation + (1 * dt), tc.relative_transform.scale)
		-- tc.relative_transform = t
		-- print(tc.relative_transform.rotation)
	end
end