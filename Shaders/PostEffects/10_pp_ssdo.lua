------------------------------------------------------------
-- SSDO Shader by Josh Klint, some modifications by Shadmar
------------------------------------------------------------

--Called once at start
function Script:Start()
	self.shader = Shader:Load("Shaders/PostEffects/00_shaders/_ssdo.shader")
	self.shader_pass = Shader:Load("Shaders/PostEffects/00_shaders/_passthrough.shader")
end

--Called each time the camera is rendered
function Script:Render(camera,context,buffer,depth,diffuse,normals)
	--Enable the shader and draw the diffuse image onscreen
	
	if self.shader 
	and camera:GetScale().y==1
	and self.shader_pass
	then 
		self.shader:Enable() 
		diffuse:Bind(1)
		depth:Bind(2)
		context:DrawImage(diffuse,0,0,buffer:GetWidth(),buffer:GetHeight())
	else
		if self.shader_pass then self.shader_pass:Enable() end
		context:DrawRect(0,0,buffer:GetWidth(),buffer:GetHeight())
	end
end

--Called when the effect is detached or the camera is deleted
function Script:Detach()
	if self.shader then
		self.shader:Release()
		self.shader = nil
	end
	if self.shader_pass then
		self.shader_pass:Release()
		self.shader_pass = nil
	end
end