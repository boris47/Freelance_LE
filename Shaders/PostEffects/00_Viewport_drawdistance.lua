-------------------------------------------------
-- Hack by shadmar to get viewport to draw > 1000
-------------------------------------------------
--Called once at start
function Script:Start()
	self.shader = Shader:Load("Shaders/PostEffects/00_shaders/_passthrough.shader")
end

--Called each time the camera is rendered
function Script:Render(camera,context,buffer,depth,diffuse,normals)
	--Enable the shader and draw the diffuse image onscreen
	if self.shader then 
		self.shader:Enable() 
		diffuse:Bind(1)
	end
	
	camera:SetRange(0.01,4096)  -- Set range here!

	context:DrawImage(diffuse,0,0,buffer:GetWidth(),buffer:GetHeight())
end

--Called when the effect is detached or the camera is deleted
function Script:Detach()
	if self.shader then
		self.shader:Release()
		self.shader = nil
	end
end
