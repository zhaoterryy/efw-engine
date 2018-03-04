dofile("Scripts/entities.lua")
scenes = 
{
	main = 
	{
		name = "Main",
		entity_list = entities
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

comp = SceneObject.new():new_component()
function comp:tick()
	print "hello"
end
comp:internal_tick()