dofile("Scripts/entities.lua")
scenes = 
{
	main = 
	{
		name = "Main",
		object_list = entities
	},
	splash_screen =
	{
		name = "Splash Screen"
	}
}

entities = nil

splash_screen = 
{
	img_path = "images/SplashScreen.png"
}

function test()
	-- local tc = scenes.main.objects.beast:get_transform_comp()
	-- print(scenes.main.objects.beast.name)
	-- scenes.main.objects.beast.name = "furry"
	-- print(scenes.main.objects.beast.name)
	-- print(tc.relative_transform)
	local beast = scenes.main.objects.beast
	local comp = beast:new_component()
	comp.tick = function(dt)
		print(dt)
		-- local tc = beast:get_transform_comp()
		-- local t = Transform.new(tc.relative_transform.position, tc.relative_transform.rotation + (1 * dt), tc.relative_transform.scale)
		-- tc.relative_transform = t
		-- print(tc.relative_transform.rotation)
	end
end