dofile("Scripts/entities.lua")
scenes = 
{
	main = 
	{
		name = "Main"
	}
}

splash_screen = 
{
	img_path = "images/SplashScreen.png"
}

scenes.main["entities"] = entities
entities = nil