Script.Enabled = false; --bool "Enabled"

function Script:Start()
	self.bEnabled = self.Enabled;
end

function Script:Enable()--in
	self.bEnabled = true;
end

function Script:Disable()--in
	self.bEnabled = false;
end

function Script:Toggle()--in
	self.bEnabled = not self.bEnabled;
end

function Script:UpdateWorld()

	if ( self.bEnabled ) then
		local vColor = Level():WeatherManager():GetAmbientColor();
		vColor = Vec3( 1.0 ) - vColor;
		self.entity:SetParticleColor( vColor.r, vColor.g, vColor.b, vColor.r, 0 );
		
		if ( self.entity:Hidden() ) then
			self.entity:Show();
		end
	else
		self.entity:Hide();
	end

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