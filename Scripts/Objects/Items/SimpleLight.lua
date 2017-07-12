
function Script:Start()

	self.On = false;
	
end

function Script:UpdateWorld()

	local iMorning = Level():WeatherManager():TansformTime( "08:30:00" );
	local iEvening = Level():WeatherManager():TansformTime( "20:30:00" );
	
	if ( Level():WeatherManager():GetTime() > iMorning ) then
		if ( self.On == true ) then self.On = false; end
	end

	if ( Level():WeatherManager():GetTime() > iEvening ) then
		if ( self.On == false ) then self.On = true; end
	end
	if ( self.On == true ) then
		self.entity:Show();
	else
		self.entity:Hide();
	end
	
--	Gamedata\Scripts\MyScript	
--	sononaclasse:Ciao();
--	naltraclasse:Ciao();
--	MyScript.ThisPrint( "ciao" );

end


--[[
function Script:UpdatePhysics()
	
end
]]

--[[
--This can be used to select which objects an entity collides with.  This overrides collision types completely.
function Script:Overlap(e)
	return Collision:Collide
end
]]

--[[
function Script:Collision(entity, position, normal, speed)
	
end
]]

--[[
function Script:Draw()
	
end
]]

--[[
function Script:DrawEach(camera)
	
end
]]

--[[
--This function will be called after the world is rendered, before the screen is refreshed.
--Use this to perform any 2D drawing you want the entity to display.
function Script:PostRender(context)
	
end
]]

--[[
--This function will be called when the entity is deleted.
function Script:Detach()
	
end
]]

--[[
--This function will be called when the last instance of this script is deleted.
function Script:Cleanup()
	
end
]]